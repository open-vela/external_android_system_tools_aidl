// Copyright (C) 2019 The Android Open Source Project
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

package aidl

import (
	"fmt"
	"io/ioutil"
	"os"
	"path/filepath"
	"strings"
	"testing"

	"github.com/google/blueprint"
	"github.com/google/blueprint/proptools"

	"android/soong/android"
	"android/soong/apex"
	"android/soong/cc"
	"android/soong/java"
	"android/soong/rust"
)

var buildDir string

func setUp() {
	var err error
	buildDir, err = ioutil.TempDir("", "soong_aidl_test")
	if err != nil {
		panic(err)
	}
}

func tearDown() {
	os.RemoveAll(buildDir)
}

func TestMain(m *testing.M) {
	run := func() int {
		setUp()
		defer tearDown()

		return m.Run()
	}

	os.Exit(run())
}

type testCustomizer func(fs map[string][]byte, config android.Config)

func withFiles(files map[string][]byte) testCustomizer {
	return func(fs map[string][]byte, config android.Config) {
		for k, v := range files {
			fs[k] = v
		}
	}
}

func setReleaseEnv() testCustomizer {
	return func(_ map[string][]byte, config android.Config) {
		config.TestProductVariables.Platform_sdk_codename = proptools.StringPtr("REL")
		config.TestProductVariables.Platform_sdk_final = proptools.BoolPtr(true)
	}
}

func _testAidl(t *testing.T, bp string, customizers ...testCustomizer) (*android.TestContext, android.Config) {
	t.Helper()

	bp = bp + java.GatherRequiredDepsForTest()
	bp = bp + cc.GatherRequiredDepsForTest(android.Android)
	bp = bp + `
		package {
			default_visibility: ["//visibility:public"],
		}
		java_defaults {
			name: "aidl-java-module-defaults",
		}
		cc_defaults {
			name: "aidl-cpp-module-defaults",
		}
		rust_defaults {
			name: "aidl-rust-module-defaults",
		}
		cc_library {
			name: "libbinder",
		}
		cc_library {
			name: "libutils",
		}
		cc_library {
			name: "libcutils",
		}
		cc_library {
			name: "libbinder_ndk",
			stubs: {
				versions: ["29"],
			}
		}
		ndk_library {
			name: "libbinder_ndk",
			symbol_file: "libbinder_ndk.map.txt",
			first_version: "29",
		}
		cc_library {
			name: "liblog",
			no_libcrt: true,
			nocrt: true,
			system_shared_libs: [],
		}
		rust_library {
			name: "libstd",
			crate_name: "std",
			srcs: [""],
			no_stdlibs: true,
			sysroot: true,
		}
		rust_library {
			name: "libtest",
			crate_name: "test",
			srcs: [""],
			no_stdlibs: true,
			sysroot: true,
		}
		rust_library {
			name: "liblazy_static",
			crate_name: "lazy_static",
			srcs: [""],
		}
		rust_library {
			name: "libbinder_rs",
			crate_name: "binder",
			srcs: [""],
		}
		aidl_interfaces_metadata {
			name: "aidl_metadata_json",
		}
	`
	fs := map[string][]byte{}

	cc.GatherRequiredFilesForTest(fs)

	for _, c := range customizers {
		// The fs now needs to be populated before creating the config, call customizers twice
		// for now, once to get any fs changes, and later after the config was created to
		// set product variables or targets.
		tempConfig := android.TestArchConfig(buildDir, nil, bp, fs)
		c(fs, tempConfig)
	}

	config := android.TestArchConfig(buildDir, nil, bp, fs)

	// To keep tests stable, fix Platform_sdk_codename and Platform_sdk_final
	// Use setReleaseEnv() to test release version
	config.TestProductVariables.Platform_sdk_codename = proptools.StringPtr("Q")
	config.TestProductVariables.Platform_version_active_codenames = []string{"Q"}
	config.TestProductVariables.Platform_sdk_final = proptools.BoolPtr(false)

	for _, c := range customizers {
		// The fs now needs to be populated before creating the config, call customizers twice
		// for now, earlier to get any fs changes, and now after the config was created to
		// set product variables or targets.
		tempFS := map[string][]byte{}
		c(tempFS, config)
	}

	ctx := android.NewTestArchContext(config)
	android.RegisterPackageBuildComponents(ctx)
	ctx.PreArchMutators(android.RegisterVisibilityRuleChecker)

	cc.RegisterRequiredBuildComponentsForTest(ctx)
	java.RegisterRequiredBuildComponentsForTest(ctx)
	ctx.RegisterModuleType("aidl_interface", aidlInterfaceFactory)
	ctx.RegisterModuleType("aidl_interfaces_metadata", aidlInterfacesMetadataSingletonFactory)
	ctx.RegisterModuleType("rust_defaults", func() android.Module {
		return rust.DefaultsFactory()
	})
	ctx.RegisterModuleType("rust_library", rust.RustLibraryFactory)

	ctx.RegisterModuleType("apex", apex.BundleFactory)
	ctx.RegisterModuleType("apex_key", apex.ApexKeyFactory)

	ctx.PreArchMutators(android.RegisterDefaultsPreArchMutators)
	ctx.PreArchMutators(android.RegisterVisibilityRuleGatherer)

	ctx.PreArchMutators(func(ctx android.RegisterMutatorsContext) {
		ctx.BottomUp("checkImports", checkImports)
		ctx.TopDown("createAidlInterface", createAidlInterfaceMutator)
	})

	ctx.PostDepsMutators(android.RegisterVisibilityRuleEnforcer)

	ctx.PreDepsMutators(func(ctx android.RegisterMutatorsContext) {
		ctx.BottomUp("rust_libraries", rust.LibraryMutator).Parallel()
		ctx.BottomUp("rust_stdlinkage", rust.LibstdMutator).Parallel()
		ctx.BottomUp("rust_begin", rust.BeginMutator).Parallel()
	})
	ctx.PostDepsMutators(android.RegisterOverridePostDepsMutators)
	ctx.PreDepsMutators(apex.RegisterPreDepsMutators)
	ctx.PostDepsMutators(apex.RegisterPostDepsMutators)
	ctx.PostDepsMutators(func(ctx android.RegisterMutatorsContext) {
		ctx.BottomUp("checkUnstableModule", checkUnstableModuleMutator).Parallel()
		ctx.BottomUp("recordVersions", recordVersions).Parallel()
		ctx.BottomUp("checkDuplicatedVersions", checkDuplicatedVersions).Parallel()
	})
	ctx.Register()

	return ctx, config
}

func testAidl(t *testing.T, bp string, customizers ...testCustomizer) (*android.TestContext, android.Config) {
	t.Helper()
	ctx, config := _testAidl(t, bp, customizers...)
	_, errs := ctx.ParseBlueprintsFiles("Android.bp")
	android.FailIfErrored(t, errs)
	_, errs = ctx.PrepareBuildActions(config)
	android.FailIfErrored(t, errs)
	return ctx, config
}

func testAidlError(t *testing.T, pattern, bp string, customizers ...testCustomizer) {
	t.Helper()
	ctx, config := _testAidl(t, bp, customizers...)
	_, errs := ctx.ParseBlueprintsFiles("Android.bp")
	if len(errs) > 0 {
		android.FailIfNoMatchingErrors(t, pattern, errs)
		return
	}
	_, errs = ctx.PrepareBuildActions(config)
	if len(errs) > 0 {
		android.FailIfNoMatchingErrors(t, pattern, errs)
		return
	}
	t.Fatalf("missing expected error %q (0 errors are returned)", pattern)
}

// asserts that there are expected module regardless of variants
func assertModulesExists(t *testing.T, ctx *android.TestContext, names ...string) {
	t.Helper()
	missing := []string{}
	for _, name := range names {
		variants := ctx.ModuleVariantsForTests(name)
		if len(variants) == 0 {
			missing = append(missing, name)
		}
	}
	if len(missing) > 0 {
		// find all the modules that do exist
		allModuleNames := make(map[string]bool)
		ctx.VisitAllModules(func(m blueprint.Module) {
			allModuleNames[ctx.ModuleName(m)] = true
		})
		t.Errorf("expected modules(%v) not found. all modules: %v", missing, android.SortedStringKeys(allModuleNames))
	}
}

func assertContains(t *testing.T, actual, expected string) {
	t.Helper()
	if !strings.Contains(actual, expected) {
		t.Errorf("%q is not found in %q.", expected, actual)
	}
}

func assertListContains(t *testing.T, actual []string, expected string) {
	t.Helper()
	for _, a := range actual {
		if strings.Contains(a, expected) {
			return
		}
	}
	t.Errorf("%q is not found in %v.", expected, actual)
}

// Vintf module must have versions in release version
func TestVintfWithoutVersionInRelease(t *testing.T) {
	vintfWithoutVersionBp := `
	aidl_interface {
		name: "foo",
		stability: "vintf",
		srcs: [
			"IFoo.aidl",
		],
		backend: {
			rust: {
				enabled: true,
			},
		},
	}`
	expectedError := `module "foo_interface": versions: must be set \(need to be frozen\) when "unstable" is false, PLATFORM_VERSION_CODENAME is REL, and "owner" property is missing.`
	testAidlError(t, expectedError, vintfWithoutVersionBp, setReleaseEnv())

	ctx, _ := testAidl(t, vintfWithoutVersionBp)
	assertModulesExists(t, ctx, "foo-V1-java", "foo-V1-rust", "foo-V1-cpp", "foo-V1-ndk", "foo-V1-ndk_platform")
}

// Check if using unstable version in release cause an error.
func TestUnstableVersionUsageInRelease(t *testing.T) {
	unstableVersionUsageInJavaBp := `
	aidl_interface {
		name: "foo",
		versions: [
			"1",
		],
		srcs: [
			"IFoo.aidl",
		],
	}
	java_library {
		name: "bar",
		libs: ["foo-V2-java"],
	}`

	expectedError := `foo-V2-java is disallowed in release version because it is unstable.`
	testAidlError(t, expectedError, unstableVersionUsageInJavaBp, setReleaseEnv(), withFiles(map[string][]byte{
		"aidl_api/foo/1/foo.1.aidl": nil,
	}))

	testAidl(t, unstableVersionUsageInJavaBp, withFiles(map[string][]byte{
		"aidl_api/foo/1/foo.1.aidl": nil,
	}))

	// A stable version can be used in release version
	stableVersionUsageInJavaBp := `
	aidl_interface {
		name: "foo",
		versions: [
			"1",
		],
		srcs: [
			"IFoo.aidl",
		],
	}
	java_library {
		name: "bar",
		libs: ["foo-V1-java"],
	}`

	testAidl(t, stableVersionUsageInJavaBp, setReleaseEnv(), withFiles(map[string][]byte{
		"aidl_api/foo/1/foo.1.aidl": nil,
	}))

	testAidl(t, stableVersionUsageInJavaBp, withFiles(map[string][]byte{
		"aidl_api/foo/1/foo.1.aidl": nil,
	}))
}

// The module which has never been frozen and is not "unstable" is not allowed in release version.
func TestNonVersionedModuleUsageInRelease(t *testing.T) {
	nonVersionedModuleUsageInJavaBp := `
	aidl_interface {
		name: "foo",
		srcs: [
			"IFoo.aidl",
		],
	}

	java_library {
		name: "bar",
		libs: ["foo-V1-java"],
	}`

	expectedError := `"foo_interface": versions: must be set \(need to be frozen\) when "unstable" is false, PLATFORM_VERSION_CODENAME is REL, and "owner" property is missing.`
	testAidlError(t, expectedError, nonVersionedModuleUsageInJavaBp, setReleaseEnv())
	testAidl(t, nonVersionedModuleUsageInJavaBp)

	nonVersionedUnstableModuleUsageInJavaBp := `
	aidl_interface {
		name: "foo",
		srcs: [
			"IFoo.aidl",
		],
		unstable: true,
	}

	java_library {
		name: "bar",
		libs: ["foo-java"],
	}`

	testAidl(t, nonVersionedUnstableModuleUsageInJavaBp, setReleaseEnv())
	testAidl(t, nonVersionedUnstableModuleUsageInJavaBp)
}

func TestUnstableVersionedModuleUsageInRelease(t *testing.T) {
	nonVersionedModuleUsageInJavaBp := `
	aidl_interface {
		name: "foo",
		srcs: [
			"IFoo.aidl",
		],
		versions: ["1"],
	}

	java_library {
		name: "bar",
		libs: ["foo-V2-java"],
	}`

	expectedError := `Android.bp:10:2: module \"bar\" variant \"android_common\": foo-V2-java is disallowed in release version because it is unstable, and its \"owner\" property is missing.`
	testAidlError(t, expectedError, nonVersionedModuleUsageInJavaBp, setReleaseEnv())
	testAidl(t, nonVersionedModuleUsageInJavaBp, withFiles(map[string][]byte{
		"aidl_api/foo/1/foo.1.aidl": nil,
	}))
}

func TestUnstableModules(t *testing.T) {
	testAidlError(t, `module "foo_interface": stability: must be empty when "unstable" is true`, `
		aidl_interface {
			name: "foo",
			stability: "vintf",
			unstable: true,
			srcs: [
				"IFoo.aidl",
			],
			backend: {
				rust: {
					enabled: true,
				},
			},
		}
	`)

	testAidlError(t, `module "foo_interface": versions: cannot have versions for an unstable interface`, `
		aidl_interface {
			name: "foo",
			versions: [
				"1",
			],
			unstable: true,
			srcs: [
				"IFoo.aidl",
			],
			backend: {
				rust: {
					enabled: true,
				},
			},
		}
	`)

	ctx, _ := testAidl(t, `
		aidl_interface {
			name: "foo",
			unstable: true,
			srcs: [
				"IFoo.aidl",
			],
			backend: {
				rust: {
					enabled: true,
				},
			},
		}
	`)

	assertModulesExists(t, ctx, "foo-java", "foo-rust", "foo-cpp", "foo-ndk", "foo-ndk_platform")
}

func TestCreatesModulesWithNoVersions(t *testing.T) {
	ctx, _ := testAidl(t, `
		aidl_interface {
			name: "foo",
			srcs: [
				"IFoo.aidl",
			],
			backend: {
				rust: {
					enabled: true,
				},
			},
		}
	`)

	assertModulesExists(t, ctx, "foo-V1-java", "foo-V1-rust", "foo-V1-cpp", "foo-V1-ndk", "foo-V1-ndk_platform")
}

func TestCreatesModulesWithFrozenVersions(t *testing.T) {
	// Each version should be under aidl_api/<name>/<ver>
	testAidlError(t, `aidl_api/foo/1`, `
		aidl_interface {
			name: "foo",
			srcs: [
				"IFoo.aidl",
			],
			versions: [
				"1",
			],
			backend: {
				rust: {
					enabled: true,
				},
			},
		}
	`)

	ctx, _ := testAidl(t, `
		aidl_interface {
			name: "foo",
			srcs: [
				"IFoo.aidl",
			],
			versions: [
				"1",
			],
			backend: {
				rust: {
					enabled: true,
				},
			},
		}
	`, withFiles(map[string][]byte{
		"aidl_api/foo/1/foo.1.aidl": nil,
	}))

	// For frozen version "1"
	assertModulesExists(t, ctx, "foo-V1-java", "foo-V1-rust", "foo-V1-cpp", "foo-V1-ndk", "foo-V1-ndk_platform")

	// For ToT (current)
	assertModulesExists(t, ctx, "foo-V2-java", "foo-V2-rust", "foo-V2-cpp", "foo-V2-ndk", "foo-V2-ndk_platform")
}

func TestErrorsWithUnsortedVersions(t *testing.T) {
	testAidlError(t, `versions: should be sorted`, `
		aidl_interface {
			name: "foo",
			srcs: [
				"IFoo.aidl",
			],
			versions: [
				"2",
				"1",
			],
			backend: {
				rust: {
					enabled: true,
				},
			},
		}
	`)
}

func TestErrorsWithDuplicateVersions(t *testing.T) {
	testAidlError(t, `versions: duplicate`, `
		aidl_interface {
			name: "foo",
			srcs: [
				"IFoo.aidl",
			],
			versions: [
				"1",
				"1",
			],
		}
	`)
}

func TestErrorsWithNonPositiveVersions(t *testing.T) {
	testAidlError(t, `versions: should be > 0`, `
		aidl_interface {
			name: "foo",
			srcs: [
				"IFoo.aidl",
			],
			versions: [
				"-1",
				"1",
			],
		}
	`)
}

func TestErrorsWithNonIntegerVersions(t *testing.T) {
	testAidlError(t, `versions: "first" is not an integer`, `
		aidl_interface {
			name: "foo",
			srcs: [
				"IFoo.aidl",
			],
			versions: [
				"first",
			],
		}
	`)
}

const (
	androidVariant    = "android_common"
	nativeVariant     = "android_arm_armv7-a-neon_shared"
	nativeRustVariant = "android_arm_armv7-a-neon_dylib"
)

func TestNativeOutputIsAlwaysVersioned(t *testing.T) {
	var ctx *android.TestContext
	assertOutput := func(moduleName, variant, outputFilename string) {
		t.Helper()
		producer, ok := ctx.ModuleForTests(moduleName, variant).Module().(android.OutputFileProducer)
		if !ok {
			t.Errorf("%s(%s): should be OutputFileProducer.", moduleName, variant)
		}
		paths, err := producer.OutputFiles("")
		if err != nil {
			t.Errorf("%s(%s): failed to get OutputFiles: %v", moduleName, variant, err)
		}
		if len(paths) != 1 || paths[0].Base() != outputFilename {
			t.Errorf("%s(%s): expected output %q, but got %v", moduleName, variant, outputFilename, paths)
		}
	}

	// No versions
	ctx, _ = testAidl(t, `
		aidl_interface {
			name: "foo",
			srcs: [
				"IFoo.aidl",
			],
			backend: {
				rust: {
					enabled: true,
				},
			},
		}
	`)
	// Even though there is no version, generated modules have version(V1) unless it isn't an unstable interface.
	assertOutput("foo-V1-java", androidVariant, "foo-V1-java.jar")

	assertOutput("foo-V1-cpp", nativeVariant, "foo-V1-cpp.so")
	assertOutput("foo-V1-rust", nativeRustVariant, "libfoo_V1.dylib.so")

	// With versions: "1", "2"
	ctx, _ = testAidl(t, `
		aidl_interface {
			name: "foo",
			srcs: [
				"IFoo.aidl",
			],
			versions: [
				"1", "2",
			],
			backend: {
				rust: {
					enabled: true,
				},
			},
		}
	`, withFiles(map[string][]byte{
		"aidl_api/foo/1/foo.1.aidl": nil,
		"aidl_api/foo/2/foo.2.aidl": nil,
	}))

	// alias for the latest frozen version (=2)
	assertOutput("foo-V2-java", androidVariant, "foo-V2-java.jar")
	assertOutput("foo-V2-cpp", nativeVariant, "foo-V2-cpp.so")
	assertOutput("foo-V2-rust", nativeRustVariant, "libfoo_V2.dylib.so")

	// frozen "1"
	assertOutput("foo-V1-java", androidVariant, "foo-V1-java.jar")
	assertOutput("foo-V1-cpp", nativeVariant, "foo-V1-cpp.so")
	assertOutput("foo-V1-rust", nativeRustVariant, "libfoo_V1.dylib.so")

	// tot
	assertOutput("foo-V3-java", androidVariant, "foo-V3-java.jar")
	assertOutput("foo-V3-cpp", nativeVariant, "foo-V3-cpp.so")
	assertOutput("foo-V3-rust", nativeRustVariant, "libfoo_V3.dylib.so")

	// skip ndk/ndk_platform since they follow the same rule with cpp
}

func TestImports(t *testing.T) {
	testAidlError(t, `Import does not exist:`, `
		aidl_interface {
			name: "foo",
			srcs: [
				"IFoo.aidl",
			],
			imports: [
				"bar",
			]
		}
	`)

	testAidlError(t, `backend.java.enabled: Java backend not enabled in the imported AIDL interface "bar"`, `
		aidl_interface {
			name: "foo",
			srcs: [
				"IFoo.aidl",
			],
			imports: [
				"bar",
			]
		}
		aidl_interface {
			name: "bar",
			srcs: [
				"IBar.aidl",
			],
			backend: {
				java: {
					enabled: false,
				},
			},
		}
	`)

	testAidlError(t, `backend.cpp.enabled: C\+\+ backend not enabled in the imported AIDL interface "bar"`, `
		aidl_interface {
			name: "foo",
			srcs: [
				"IFoo.aidl",
			],
			imports: [
				"bar",
			]
		}
		aidl_interface {
			name: "bar",
			srcs: [
				"IBar.aidl",
			],
			backend: {
				cpp: {
					enabled: false,
				},
			},
		}
	`)

	ctx, _ := testAidl(t, `
		aidl_interface {
			name: "foo",
			srcs: [
				"IFoo.aidl",
			],
			backend: {
				rust: {
					enabled: true,
				},
			},
			imports: [
				"bar.1",
			]
		}
		aidl_interface {
			name: "bar.1",
			srcs: [
				"IBar.aidl",
			],
			backend: {
				rust: {
					enabled: true,
				},
			},
		}
	`)

	ldRule := ctx.ModuleForTests("foo-V1-cpp", nativeVariant).Rule("ld")
	libFlags := ldRule.Args["libFlags"]
	libBar := filepath.Join("bar.1-V1-cpp", nativeVariant, "bar.1-V1-cpp.so")
	if !strings.Contains(libFlags, libBar) {
		t.Errorf("%q is not found in %q", libBar, libFlags)
	}

	rustcRule := ctx.ModuleForTests("foo-V1-rust", nativeRustVariant).Rule("rustc")
	libFlags = rustcRule.Args["libFlags"]
	libBar = filepath.Join(buildDir, ".intermediates", "bar.1-V1-rust", nativeRustVariant, "libbar_1_V1.dylib.so")
	libBarFlag := "--extern bar_1=" + libBar
	if !strings.Contains(libFlags, libBarFlag) {
		t.Errorf("%q is not found in %q", libBarFlag, libFlags)
	}
}

func TestDuplicatedVersions(t *testing.T) {
	// foo depends on myiface-V2-ndk via direct dep and also on
	// myiface-V1-ndk via indirect dep. This should be prohibited.
	testAidlError(t, `depends on multiple versions of the same aidl_interface: myiface-V1-ndk, myiface-V2-ndk`, `
		aidl_interface {
			name: "myiface",
			srcs: ["IFoo.aidl"],
			versions: ["1", "2"],
		}

		cc_library {
			name: "foo",
			shared_libs: ["myiface-V2-ndk", "bar"],
		}

		cc_library {
			name: "bar",
			shared_libs: ["myiface-V1-ndk"],
		}

	`, withFiles(map[string][]byte{
		"aidl_api/myiface/1/myiface.1.aidl": nil,
		"aidl_api/myiface/1/.hash":          nil,
		"aidl_api/myiface/2/myiface.2.aidl": nil,
		"aidl_api/myiface/2/.hash":          nil,
	}))
	testAidlError(t, `depends on multiple versions of the same aidl_interface: myiface-V1-ndk, myiface-V2-ndk`, `
		aidl_interface {
			name: "myiface",
			srcs: ["IFoo.aidl"],
			versions: ["1"],
		}

		aidl_interface {
			name: "myiface2",
			srcs: ["IBar.aidl"],
			imports: ["myiface"]
		}

		cc_library {
			name: "foobar",
			shared_libs: ["myiface-V1-ndk", "myiface2-V1-ndk"],
		}

	`, withFiles(map[string][]byte{
		"aidl_api/myiface/1/myiface.1.aidl": nil,
		"aidl_api/myiface/1/.hash":          nil,
	}))
	testAidlError(t, `depends on multiple versions of the same aidl_interface: myiface-V1-ndk-source, myiface-V2-ndk`, `
		aidl_interface {
			name: "myiface",
			srcs: ["IFoo.aidl"],
			versions: ["1"],
			backend: {
				ndk: {
					srcs_available: true,
				},
			},
		}

		aidl_interface {
			name: "myiface2",
			srcs: ["IBar.aidl"],
			imports: ["myiface"]
		}

		cc_library {
			name: "foobar",
			srcs: [":myiface-V1-ndk-source"],
			shared_libs: ["myiface2-V1-ndk"],
		}

	`, withFiles(map[string][]byte{
		"aidl_api/myiface/1/myiface.1.aidl": nil,
		"aidl_api/myiface/1/.hash":          nil,
	}))
	testAidl(t, `
		aidl_interface {
			name: "myiface",
			srcs: ["IFoo.aidl"],
			versions: ["1"],
			backend: {
				ndk: {
					srcs_available: true,
				},
			},
		}

		aidl_interface {
			name: "myiface2",
			srcs: ["IBar.aidl"],
			imports: ["myiface"]
		}

		cc_library {
			name: "foobar",
			srcs: [":myiface-V2-ndk-source"],
			shared_libs: ["myiface2-V1-ndk"],
		}

	`, withFiles(map[string][]byte{
		"aidl_api/myiface/1/myiface.1.aidl": nil,
		"aidl_api/myiface/1/.hash":          nil,
	}))
	testAidl(t, `
		aidl_interface {
			name: "myiface",
			srcs: ["IFoo.aidl"],
			versions: ["1"],
		}

		aidl_interface {
			name: "myiface2",
			srcs: ["IBar.aidl"],
			imports: ["myiface"]
		}

		cc_library {
			name: "foobar",
			shared_libs: ["myiface-V2-ndk", "myiface2-V1-ndk"],
		}

	`, withFiles(map[string][]byte{
		"aidl_api/myiface/1/myiface.1.aidl": nil,
		"aidl_api/myiface/1/.hash":          nil,
	}))
}

func TestUnstableVndkModule(t *testing.T) {
	testAidlError(t, `module "myiface_interface": stability: must be "vintf" if the module is for VNDK.`, `
		aidl_interface {
			name: "myiface",
			srcs: ["IFoo.aidl"],
			vendor_available: true,
			product_available: true,
			unstable: true,
			vndk: {
				enabled: true,
			},
		}
	`)
	testAidlError(t, `module "myiface_interface": stability: must be "vintf" if the module is for VNDK.`, `
		aidl_interface {
			name: "myiface",
			vendor_available: true,
			product_available: true,
			srcs: ["IFoo.aidl"],
			vndk: {
				enabled: true,
			},
		}
	`)
	testAidl(t, `
		aidl_interface {
			name: "myiface",
			vendor_available: true,
			product_available: true,
			srcs: ["IFoo.aidl"],
			stability: "vintf",
			vndk: {
				enabled: true,
			},
		}
	`)
}

func TestCcModuleWithApexNameMacro(t *testing.T) {
	ctx, _ := testAidl(t, `
		aidl_interface {
			name: "myiface",
			srcs: ["IFoo.aidl"],
			backend: {
				ndk: {
					apex_available: ["myapex"],
				},
			},
		}
		apex {
			name: "myapex",
			key: "myapex.key",
			native_shared_libs: ["myiface-V1-ndk_platform"],
		}
		apex_key {
			name: "myapex.key",
			public_key: "testkey.avbpubkey",
			private_key: "testkey.pem",
		}
	`, withFiles(map[string][]byte{
		"system/sepolicy/apex/myapex-file_contexts": nil,
	}))

	ccRule := ctx.ModuleForTests("myiface-V1-ndk_platform", "android_arm64_armv8-a_static_myapex").Rule("cc")
	assertContains(t, ccRule.Args["cFlags"], "-D__ANDROID_APEX__")
	assertContains(t, ccRule.Args["cFlags"], "-D__ANDROID_APEX_NAME__='\"myapex\"'")
	assertContains(t, ccRule.Args["cFlags"], "-D__ANDROID_APEX_MYAPEX__")
}

func TestSrcsAvailable(t *testing.T) {
	bp := `
		aidl_interface {
			name: "myiface",
			srcs: ["IFoo.aidl"],
			backend: {
				java: {
					srcs_available: %s,
				},
				cpp: {
					srcs_available: %s,
				},
			},
		}
	`
	customizer := withFiles(map[string][]byte{
		"otherpackage/Android.bp": []byte(`
			java_library {
				name: "javalib",
				srcs: [":myiface-V1-java-source"],
			}
			cc_library_shared {
				name: "cclib",
				srcs: [":myiface-V1-cpp-source"],
			}
		`),
	})
	ctx, _ := testAidl(t, fmt.Sprintf(bp, "true", "true"), customizer)
	javaInputs := ctx.ModuleForTests("javalib", "android_common").Rule("javac").Inputs.Strings()
	assertListContains(t, javaInputs, "myiface-V1-java-source/gen/IFoo.java")
	ccInput := ctx.ModuleForTests("cclib", "android_arm64_armv8-a_shared").Rule("cc").Input.String()
	assertContains(t, ccInput, "myiface-V1-cpp-source/gen/IFoo.cpp")

	testAidlError(t, `depends on //.:myiface-V1-java-source which is not visible to this module`,
		fmt.Sprintf(bp, "false", "true"), customizer)
	testAidlError(t, `depends on //.:myiface-V1-cpp-source which is not visible to this module`,
		fmt.Sprintf(bp, "true", "false"), customizer)
}

func TestRustDuplicateNames(t *testing.T) {
	testAidl(t, `
		aidl_interface {
			name: "myiface",
			srcs: ["dir/a/Foo.aidl", "dir/b/Foo.aidl"],
			backend: {
				rust: {
					enabled: true,
				},
			},
		}
	`)
}

func TestAidlFlags(t *testing.T) {
	ctx, _ := testAidl(t, `
		aidl_interface {
			name: "myiface",
			srcs: ["a/Foo.aidl", "b/Bar.aidl"],
			flags: ["-Weverything", "-Werror"],
			backend: { rust: { enabled: true }}
		}
	`)
	for module, outputs := range map[string][]string{
		"myiface-V1-cpp-source":  {"a/Foo.h", "b/Bar.h"},
		"myiface-V1-java-source": {"a/Foo.java", "b/Bar.java"},
		"myiface-V1-ndk-source":  {"aidl/a/Foo.h", "aidl/b/Bar.h"},
		"myiface-V1-rust-source": {"a/Foo.rs", "b/Bar.rs"},
	} {
		for _, output := range outputs {
			t.Run(module+"/"+output, func(t *testing.T) {
				params := ctx.ModuleForTests(module, "").Output(output)
				assertContains(t, params.Args["optionalFlags"], "-Weverything")
				assertContains(t, params.Args["optionalFlags"], "-Werror")
			})
		}
	}
}
