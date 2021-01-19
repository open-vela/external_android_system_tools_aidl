// Copyright (C) 2021 The Android Open Source Project
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
	"android/soong/android"
	"android/soong/cc"
	"android/soong/java"
	"android/soong/rust"

	"path/filepath"
	"strconv"
	"strings"

	"github.com/google/blueprint/proptools"
)

func addCppLibrary(mctx android.LoadHookContext, i *aidlInterface, versionForModuleName string, lang string) string {
	cppSourceGen := i.versionedName(versionForModuleName) + "-" + lang + "-source"
	cppModuleGen := i.versionedName(versionForModuleName) + "-" + lang
	cppOutputGen := i.cppOutputName(versionForModuleName) + "-" + lang
	version := i.normalizeVersion(versionForModuleName)
	srcs, aidlRoot := i.srcsForVersion(mctx, version)
	if len(srcs) == 0 {
		// This can happen when the version is about to be frozen; the version
		// directory is created but API dump hasn't been copied there.
		// Don't create a library for the yet-to-be-frozen version.
		return ""
	}

	var overrideVndkProperties cc.VndkProperties

	if i.moduleVersionForVndk() != versionForModuleName {
		// We only want the VNDK to include the latest interface. For interfaces in
		// development, they will be frozen, so we put their latest version in the
		// VNDK. For interfaces which are already frozen, we put their latest version
		// in the VNDK, and when that version is frozen, the version in the VNDK can
		// be updated. Otherwise, we remove this library from the VNDK, to avoid adding
		// multiple versions of the same library to the VNDK.
		overrideVndkProperties.Vndk.Enabled = proptools.BoolPtr(false)
		overrideVndkProperties.Vndk.Support_system_process = proptools.BoolPtr(false)
	}

	var commonProperties *CommonNativeBackendProperties
	if lang == langCpp {
		commonProperties = &i.properties.Backend.Cpp.CommonNativeBackendProperties
	} else if lang == langNdk || lang == langNdkPlatform {
		commonProperties = &i.properties.Backend.Ndk.CommonNativeBackendProperties
	}

	genLog := proptools.Bool(commonProperties.Gen_log)
	genTrace := proptools.Bool(i.properties.Gen_trace)

	mctx.CreateModule(aidlGenFactory, &nameProperties{
		Name: proptools.StringPtr(cppSourceGen),
	}, &aidlGenProperties{
		Srcs:       srcs,
		AidlRoot:   aidlRoot,
		Imports:    concat(i.properties.Imports, []string{i.ModuleBase.Name()}),
		Stability:  i.properties.Stability,
		Lang:       lang,
		BaseName:   i.ModuleBase.Name(),
		GenLog:     genLog,
		Version:    version,
		GenTrace:   genTrace,
		Unstable:   i.properties.Unstable,
		Visibility: srcsVisibility(mctx, lang),
		Flags:      i.properties.Flags,
	})
	importPostfix := i.getImportPostfix(mctx, version, lang)

	importExportDependencies := wrap("", i.properties.Imports, importPostfix)
	var sharedLibDependency []string
	var headerLibs []string
	var sdkVersion *string
	var minSdkVersion *string
	var stl *string
	var cpp_std *string
	var hostSupported *bool
	var addCflags []string

	if lang == langCpp {
		importExportDependencies = append(importExportDependencies, "libbinder", "libutils")
		if genTrace {
			sharedLibDependency = append(sharedLibDependency, "libcutils")
		}
		hostSupported = i.properties.Host_supported
		minSdkVersion = i.properties.Backend.Cpp.Min_sdk_version
	} else if lang == langNdk {
		importExportDependencies = append(importExportDependencies, "libbinder_ndk")
		if genTrace {
			sharedLibDependency = append(sharedLibDependency, "libandroid")
		}
		sdkVersion = proptools.StringPtr("current")
		stl = proptools.StringPtr("c++_shared")
		minSdkVersion = i.properties.Backend.Ndk.Min_sdk_version
	} else if lang == langNdkPlatform {
		importExportDependencies = append(importExportDependencies, "libbinder_ndk")
		if genTrace {
			headerLibs = append(headerLibs, "libandroid_aidltrace")
			sharedLibDependency = append(sharedLibDependency, "libcutils")
		}
		hostSupported = i.properties.Host_supported
		addCflags = append(addCflags, "-DBINDER_STABILITY_SUPPORT")
		minSdkVersion = i.properties.Backend.Ndk.Min_sdk_version
	} else {
		panic("Unrecognized language: " + lang)
	}

	vendorAvailable := i.properties.Vendor_available
	productAvailable := i.properties.Product_available
	if lang == langCpp {
		// Vendor and product modules cannot use the libbinder (cpp) backend of AIDL in a
		// way that is stable. So, in order to prevent accidental usage of these library by
		// vendor and product forcibly disabling this version of the library.
		//
		// It may be the case in the future that we will want to enable this (if some generic
		// helper should be used by both libbinder vendor things using /dev/vndbinder as well
		// as those things using /dev/binder + libbinder_ndk to talk to stable interfaces).
		if "vintf" == proptools.String(i.properties.Stability) {
			overrideVndkProperties.Vndk.Private = proptools.BoolPtr(true)
		}
		// As libbinder is not available for the product processes, we must not create
		// product variant for the aidl_interface
		productAvailable = nil
	}

	if lang == langNdk {
		// TODO(b/121157555): when the NDK variant is its own variant, these wouldn't interact,
		// but we can't create a vendor or product version of an NDK variant
		//
		// nil (unspecified) is used instead of false so that this can't conflict with
		// 'vendor: true', for instance.
		vendorAvailable = nil
		productAvailable = nil
		overrideVndkProperties.Vndk.Enabled = proptools.BoolPtr(false)
		overrideVndkProperties.Vndk.Support_system_process = proptools.BoolPtr(false)
	}

	mctx.CreateModule(cc.LibraryFactory, &ccProperties{
		Name:                      proptools.StringPtr(cppModuleGen),
		Vendor_available:          vendorAvailable,
		Product_available:         productAvailable,
		Host_supported:            hostSupported,
		Defaults:                  []string{"aidl-cpp-module-defaults"},
		Double_loadable:           i.properties.Double_loadable,
		Generated_sources:         []string{cppSourceGen},
		Generated_headers:         []string{cppSourceGen},
		Export_generated_headers:  []string{cppSourceGen},
		Shared_libs:               append(importExportDependencies, sharedLibDependency...),
		Header_libs:               headerLibs,
		Export_shared_lib_headers: importExportDependencies,
		Sdk_version:               sdkVersion,
		Stl:                       stl,
		Cpp_std:                   cpp_std,
		Cflags:                    append(addCflags, "-Wextra", "-Wall", "-Werror"),
		Stem:                      proptools.StringPtr(cppOutputGen),
		Apex_available:            commonProperties.Apex_available,
		Min_sdk_version:           minSdkVersion,
		UseApexNameMacro:          true,
		Target:                    targetProperties{Darwin: perTargetProperties{Enabled: proptools.BoolPtr(false)}},
		Tidy:                      proptools.BoolPtr(true),
		// Do the tidy check only for the generated headers
		Tidy_flags:            []string{"--header-filter=" + android.PathForOutput(mctx).String() + ".*"},
		Tidy_checks_as_errors: []string{"*"},
	}, &i.properties.VndkProperties, &commonProperties.VndkProperties, &overrideVndkProperties)

	return cppModuleGen
}

func addJavaLibrary(mctx android.LoadHookContext, i *aidlInterface, versionForModuleName string) string {
	javaSourceGen := i.versionedName(versionForModuleName) + "-java-source"
	javaModuleGen := i.versionedName(versionForModuleName) + "-java"
	version := i.normalizeVersion(versionForModuleName)
	srcs, aidlRoot := i.srcsForVersion(mctx, version)
	if len(srcs) == 0 {
		// This can happen when the version is about to be frozen; the version
		// directory is created but API dump hasn't been copied there.
		// Don't create a library for the yet-to-be-frozen version.
		return ""
	}

	sdkVersion := i.properties.Backend.Java.Sdk_version
	if !proptools.Bool(i.properties.Backend.Java.Platform_apis) && sdkVersion == nil {
		// platform apis requires no default
		sdkVersion = proptools.StringPtr("system_current")
	}

	mctx.CreateModule(aidlGenFactory, &nameProperties{
		Name: proptools.StringPtr(javaSourceGen),
	}, &aidlGenProperties{
		Srcs:       srcs,
		AidlRoot:   aidlRoot,
		Imports:    concat(i.properties.Imports, []string{i.ModuleBase.Name()}),
		Stability:  i.properties.Stability,
		Lang:       langJava,
		BaseName:   i.ModuleBase.Name(),
		Version:    version,
		GenTrace:   proptools.Bool(i.properties.Gen_trace),
		Unstable:   i.properties.Unstable,
		Visibility: srcsVisibility(mctx, langJava),
		Flags:      i.properties.Flags,
	})

	importPostfix := i.getImportPostfix(mctx, version, langJava)
	mctx.CreateModule(java.LibraryFactory, &javaProperties{
		Name:            proptools.StringPtr(javaModuleGen),
		Installable:     proptools.BoolPtr(true),
		Defaults:        []string{"aidl-java-module-defaults"},
		Sdk_version:     sdkVersion,
		Platform_apis:   i.properties.Backend.Java.Platform_apis,
		Static_libs:     wrap("", i.properties.Imports, importPostfix),
		Srcs:            []string{":" + javaSourceGen},
		Apex_available:  i.properties.Backend.Java.Apex_available,
		Min_sdk_version: i.properties.Backend.Java.Min_sdk_version,
	})

	return javaModuleGen
}

func addRustLibrary(mctx android.LoadHookContext, i *aidlInterface, versionForModuleName string) string {
	rustSourceGen := i.versionedName(versionForModuleName) + "-rust-source"
	rustModuleGen := i.versionedName(versionForModuleName) + "-rust"
	version := i.normalizeVersion(versionForModuleName)
	srcs, aidlRoot := i.srcsForVersion(mctx, version)
	if len(srcs) == 0 {
		// This can happen when the version is about to be frozen; the version
		// directory is created but API dump hasn't been copied there.
		// Don't create a library for the yet-to-be-frozen version.
		return ""
	}

	mctx.CreateModule(aidlGenFactory, &nameProperties{
		Name: proptools.StringPtr(rustSourceGen),
	}, &aidlGenProperties{
		Srcs:       srcs,
		AidlRoot:   aidlRoot,
		Imports:    concat(i.properties.Imports, []string{i.ModuleBase.Name()}),
		Stability:  i.properties.Stability,
		Lang:       langRust,
		BaseName:   i.ModuleBase.Name(),
		Version:    version,
		Unstable:   i.properties.Unstable,
		Visibility: srcsVisibility(mctx, langRust),
		Flags:      i.properties.Flags,
	})

	versionedRustName := fixRustName(i.versionedName(versionForModuleName))

	mctx.CreateModule(aidlRustLibraryFactory, &rustProperties{
		Name:           proptools.StringPtr(rustModuleGen),
		Crate_name:     versionedRustName,
		Stem:           proptools.StringPtr("lib" + versionedRustName),
		Defaults:       []string{"aidl-rust-module-defaults"},
		Host_supported: i.properties.Host_supported,
		Target:         targetProperties{Darwin: perTargetProperties{Enabled: proptools.BoolPtr(false)}},
	}, &rust.SourceProviderProperties{
		Source_stem: proptools.StringPtr(versionedRustName),
	}, &aidlRustSourceProviderProperties{
		SourceGen: rustSourceGen,
		Imports:   i.properties.Imports,
	})

	return rustModuleGen
}

// This function returns module name with version. Assume that there is foo of which latest version is 2
// Version -> Module name
// "1"->foo-V1
// "2"->foo-V2
// "3"(unfrozen)->foo-unstable
// ""-> foo
// "unstable" -> foo-unstable
func (i *aidlInterface) versionedName(version string) string {
	name := i.ModuleBase.Name()
	if version == "" {
		return name
	}
	if version == i.currentVersion() || version == unstableVersion {
		return name + "-" + unstableVersion
	}
	return name + "-V" + version
}

// This function returns C++ artifact's name. Mostly, it returns same as versionedName(),
// But, it returns different value only if it is the case below.
// Assume that there is foo of which latest version is 2
// foo-unstable -> foo-V3
// foo -> foo-V2 (latest frozen version)
// Assume that there is bar of which version hasn't been defined yet.
// bar -> bar
// bar-unstable -> bar-V1
func (i *aidlInterface) cppOutputName(version string) string {
	name := i.ModuleBase.Name()
	if i.hasVersion() && version == unstableVersion {
		panic("A versioned module's output name in C++ must not contain 'unstable'")
	}
	// If the module doesn't have version, it returns with version(-V1) only if 'version' is unstable,
	// otherwise, it returns the name without version.
	if !i.hasVersion() {
		// TODO(b/150578172): Use "-V1" as 'unstable' when the build system supports it, or remove it altogether later.
		if version == "" {
			return name
		}
		// latestVersion() always returns "0"
		i, err := strconv.Atoi(i.latestVersion())
		if err != nil {
			panic(err)
		}
		return name + "-V" + strconv.Itoa(i+1)
	}
	if version == "" {
		version = i.latestVersion()
	}
	return name + "-V" + version
}

func (i *aidlInterface) srcsForVersion(mctx android.LoadHookContext, version string) (srcs []string, aidlRoot string) {
	if version == i.currentVersion() {
		return i.properties.Srcs, i.properties.Local_include_dir
	} else {
		aidlRoot = filepath.Join(aidlApiDir, i.ModuleBase.Name(), version)
		full_paths, err := mctx.GlobWithDeps(filepath.Join(mctx.ModuleDir(), aidlRoot, "**/*.aidl"), nil)
		if err != nil {
			panic(err)
		}
		for _, path := range full_paths {
			// Here, we need path local to the module
			srcs = append(srcs, strings.TrimPrefix(path, mctx.ModuleDir()+"/"))
		}
		return srcs, aidlRoot
	}
}
