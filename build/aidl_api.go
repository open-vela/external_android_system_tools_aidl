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

	"fmt"
	"io"
	"path/filepath"
	"strconv"
	"strings"

	"github.com/google/blueprint"
	"github.com/google/blueprint/proptools"
)

var (
	aidlDumpApiRule = pctx.StaticRule("aidlDumpApiRule", blueprint.RuleParams{
		Command: `rm -rf "${outDir}" && mkdir -p "${outDir}" && ` +
			`${aidlCmd} --dumpapi --structured ${imports} ${optionalFlags} --out ${outDir} ${in} && ` +
			`(cd ${outDir} && find ./ -name "*.aidl" -print0 | LC_ALL=C sort -z | xargs -0 sha1sum && echo ${latestVersion}) | sha1sum | cut -d " " -f 1 > ${hashFile} `,
		CommandDeps: []string{"${aidlCmd}"},
	}, "optionalFlags", "imports", "outDir", "hashFile", "latestVersion")

	aidlCheckApiRule = pctx.StaticRule("aidlCheckApiRule", blueprint.RuleParams{
		Command: `(${aidlCmd} ${optionalFlags} --checkapi=${checkApiLevel} ${old} ${new} && touch ${out}) || ` +
			`(cat ${messageFile} && exit 1)`,
		CommandDeps: []string{"${aidlCmd}"},
		Description: "AIDL CHECK API: ${new} against ${old}",
	}, "optionalFlags", "old", "new", "messageFile", "checkApiLevel")

	aidlVerifyHashRule = pctx.StaticRule("aidlVerifyHashRule", blueprint.RuleParams{
		Command: `if [ $$(cd '${apiDir}' && { find ./ -name "*.aidl" -print0 | LC_ALL=C sort -z | xargs -0 sha1sum && echo ${version}; } | sha1sum | cut -d " " -f 1) = $$(read -r <'${hashFile}' hash extra; printf %s $$hash) ]; then ` +
			`touch ${out}; else cat '${messageFile}' && exit 1; fi`,
		Description: "Verify ${apiDir} files have not been modified",
	}, "apiDir", "version", "messageFile", "hashFile")
)

type aidlApiProperties struct {
	BaseName  string
	Srcs      []string `android:"path"`
	AidlRoot  string   // base directory for the input aidl file
	Stability *string
	Imports   []string
	Versions  []string
}

type aidlApi struct {
	android.ModuleBase

	properties aidlApiProperties

	// for triggering api check for version X against version X-1
	checkApiTimestamps android.WritablePaths

	// for triggering updating current API
	updateApiTimestamp android.WritablePath

	// for triggering check that files have not been modified
	checkHashTimestamps android.WritablePaths

	// for triggering freezing API as the new version
	freezeApiTimestamp android.WritablePath
}

func (m *aidlApi) apiDir() string {
	return filepath.Join(aidlApiDir, m.properties.BaseName)
}

// `m <iface>-freeze-api` will freeze ToT as this version
func (m *aidlApi) nextVersion() string {
	if len(m.properties.Versions) == 0 {
		return "1"
	} else {
		latestVersion := m.properties.Versions[len(m.properties.Versions)-1]

		i, err := strconv.Atoi(latestVersion)
		if err != nil {
			panic(err)
		}

		return strconv.Itoa(i + 1)
	}
}

type apiDump struct {
	dir      android.Path
	files    android.Paths
	hashFile android.OptionalPath
}

func (m *aidlApi) createApiDumpFromSource(ctx android.ModuleContext) apiDump {
	srcs, imports := getPaths(ctx, m.properties.Srcs)

	if ctx.Failed() {
		return apiDump{}
	}

	var importPaths []string
	importPaths = append(importPaths, imports...)
	ctx.VisitDirectDeps(func(dep android.Module) {
		if importedAidl, ok := dep.(*aidlInterface); ok {
			importPaths = append(importPaths, importedAidl.properties.Full_import_paths...)
		}
	})

	var apiDir android.WritablePath
	var apiFiles android.WritablePaths
	var hashFile android.WritablePath

	apiDir = android.PathForModuleOut(ctx, "dump")
	aidlRoot := android.PathForModuleSrc(ctx, m.properties.AidlRoot)
	for _, src := range srcs {
		baseDir := getBaseDir(ctx, src, aidlRoot)
		relPath, _ := filepath.Rel(baseDir, src.String())
		outFile := android.PathForModuleOut(ctx, "dump", relPath)
		apiFiles = append(apiFiles, outFile)
	}
	hashFile = android.PathForModuleOut(ctx, "dump", ".hash")
	latestVersion := "latest-version"
	if len(m.properties.Versions) >= 1 {
		latestVersion = m.properties.Versions[len(m.properties.Versions)-1]
	}

	var optionalFlags []string
	if m.properties.Stability != nil {
		optionalFlags = append(optionalFlags, "--stability", *m.properties.Stability)
	}

	ctx.Build(pctx, android.BuildParams{
		Rule:    aidlDumpApiRule,
		Outputs: append(apiFiles, hashFile),
		Inputs:  srcs,
		Args: map[string]string{
			"optionalFlags": strings.Join(optionalFlags, " "),
			"imports":       strings.Join(wrap("-I", importPaths, ""), " "),
			"outDir":        apiDir.String(),
			"hashFile":      hashFile.String(),
			"latestVersion": latestVersion,
		},
	})
	return apiDump{apiDir, apiFiles.Paths(), android.OptionalPathForPath(hashFile)}
}

func (m *aidlApi) makeApiDumpAsVersion(ctx android.ModuleContext, dump apiDump, version string) android.WritablePath {
	timestampFile := android.PathForModuleOut(ctx, "updateapi_"+version+".timestamp")

	modulePath := android.PathForModuleSrc(ctx).String()

	targetDir := filepath.Join(modulePath, m.apiDir(), version)
	rb := android.NewRuleBuilder(pctx, ctx)
	// Wipe the target directory and then copy the API dump into the directory
	rb.Command().Text("mkdir -p " + targetDir)
	rb.Command().Text("rm -rf " + targetDir + "/*")
	if version != currentVersion {
		rb.Command().Text("cp -rf " + dump.dir.String() + "/. " + targetDir).Implicits(dump.files)
		// If this is making a new frozen (i.e. non-current) version of the interface,
		// modify Android.bp file to add the new version to the 'versions' property.
		rb.Command().BuiltTool("bpmodify").
			Text("-w -m " + m.properties.BaseName).
			Text("-parameter versions -a " + version).
			Text(android.PathForModuleSrc(ctx, "Android.bp").String())
	} else {
		// In this case (unfrozen interface), don't copy .hash
		rb.Command().Text("cp -rf " + dump.dir.String() + "/* " + targetDir).Implicits(dump.files)
	}
	rb.Command().Text("touch").Output(timestampFile)

	rb.Build("dump_aidl_api"+m.properties.BaseName+"_"+version,
		"Making AIDL API of "+m.properties.BaseName+" as version "+version)
	return timestampFile
}

func (m *aidlApi) checkApi(ctx android.ModuleContext, oldDump, newDump apiDump, checkApiLevel string, messageFile android.Path) android.WritablePath {
	newVersion := newDump.dir.Base()
	timestampFile := android.PathForModuleOut(ctx, "checkapi_"+newVersion+".timestamp")

	var optionalFlags []string
	if m.properties.Stability != nil {
		optionalFlags = append(optionalFlags, "--stability", *m.properties.Stability)
	}

	var implicits android.Paths
	implicits = append(implicits, oldDump.files...)
	implicits = append(implicits, newDump.files...)
	implicits = append(implicits, messageFile)
	ctx.Build(pctx, android.BuildParams{
		Rule:      aidlCheckApiRule,
		Implicits: implicits,
		Output:    timestampFile,
		Args: map[string]string{
			"optionalFlags": strings.Join(optionalFlags, " "),
			"old":           oldDump.dir.String(),
			"new":           newDump.dir.String(),
			"messageFile":   messageFile.String(),
			"checkApiLevel": checkApiLevel,
		},
	})
	return timestampFile
}

func (m *aidlApi) checkCompatibility(ctx android.ModuleContext, oldDump, newDump apiDump) android.WritablePath {
	messageFile := android.PathForSource(ctx, "system/tools/aidl/build/message_check_compatibility.txt")
	return m.checkApi(ctx, oldDump, newDump, "compatible", messageFile)
}

func (m *aidlApi) checkEquality(ctx android.ModuleContext, oldDump apiDump, newDump apiDump) android.WritablePath {
	// Use different messages depending on whether platform SDK is finalized or not.
	// In case when it is finalized, we should never allow updating the already frozen API.
	// If it's not finalized, we let users to update the current version by invoking
	// `m <name>-update-api`.
	messageFile := android.PathForSource(ctx, "system/tools/aidl/build/message_check_equality.txt")
	sdkIsFinal := !ctx.Config().DefaultAppTargetSdk(ctx).IsPreview()
	if sdkIsFinal {
		messageFile = android.PathForSource(ctx, "system/tools/aidl/build/message_check_equality_release.txt")
	}
	formattedMessageFile := android.PathForModuleOut(ctx, "message_check_equality.txt")
	rb := android.NewRuleBuilder(pctx, ctx)
	rb.Command().Text("sed").Flag(" s/%s/" + m.properties.BaseName + "/g ").Input(messageFile).Text(" > ").Output(formattedMessageFile)
	rb.Build("format_message_"+m.properties.BaseName, "")

	var implicits android.Paths
	implicits = append(implicits, oldDump.files...)
	implicits = append(implicits, newDump.files...)
	implicits = append(implicits, formattedMessageFile)
	return m.checkApi(ctx, oldDump, newDump, "equal", formattedMessageFile)
}

func (m *aidlApi) checkIntegrity(ctx android.ModuleContext, dump apiDump) android.WritablePath {
	version := dump.dir.Base()
	timestampFile := android.PathForModuleOut(ctx, "checkhash_"+version+".timestamp")
	messageFile := android.PathForSource(ctx, "system/tools/aidl/build/message_check_integrity.txt")

	i, _ := strconv.Atoi(version)
	if i == 1 {
		version = "latest-version"
	} else {
		version = strconv.Itoa(i - 1)
	}

	var implicits android.Paths
	implicits = append(implicits, dump.files...)
	implicits = append(implicits, dump.hashFile.Path())
	implicits = append(implicits, messageFile)
	ctx.Build(pctx, android.BuildParams{
		Rule:      aidlVerifyHashRule,
		Implicits: implicits,
		Output:    timestampFile,
		Args: map[string]string{
			"apiDir":      dump.dir.String(),
			"version":     version,
			"hashFile":    dump.hashFile.Path().String(),
			"messageFile": messageFile.String(),
		},
	})
	return timestampFile
}

func (m *aidlApi) GenerateAndroidBuildActions(ctx android.ModuleContext) {
	// An API dump is created from source and it is compared against the API dump of the
	// 'current' (yet-to-be-finalized) version. By checking this we enforce that any change in
	// the AIDL interface is gated by the AIDL API review even before the interface is frozen as
	// a new version.
	totApiDump := m.createApiDumpFromSource(ctx)
	currentApiDir := android.ExistentPathForSource(ctx, ctx.ModuleDir(), m.apiDir(), currentVersion)
	var currentApiDump apiDump
	if currentApiDir.Valid() {
		currentApiDump = apiDump{
			dir:      currentApiDir.Path(),
			files:    ctx.Glob(filepath.Join(currentApiDir.Path().String(), "**/*.aidl"), nil),
			hashFile: android.ExistentPathForSource(ctx, ctx.ModuleDir(), m.apiDir(), currentVersion, ".hash"),
		}
		checked := m.checkEquality(ctx, currentApiDump, totApiDump)
		m.checkApiTimestamps = append(m.checkApiTimestamps, checked)
	} else {
		// The "current" directory might not exist, in case when the interface is first created.
		// Instruct user to create one by executing `m <name>-update-api`.
		rb := android.NewRuleBuilder(pctx, ctx)
		ifaceName := m.properties.BaseName
		rb.Command().Text(fmt.Sprintf(`echo "API dump for the current version of AIDL interface %s does not exist."`, ifaceName))
		rb.Command().Text(fmt.Sprintf(`echo Run "m %s-update-api", or add "unstable: true" to the build rule `+
			`for the interface if it does not need to be versioned`, ifaceName))
		// This file will never be created. Otherwise, the build will pass simply by running 'm; m'.
		alwaysChecked := android.PathForModuleOut(ctx, "checkapi_current.timestamp")
		rb.Command().Text("false").ImplicitOutput(alwaysChecked)
		rb.Build("check_current_aidl_api", "")
		m.checkApiTimestamps = append(m.checkApiTimestamps, alwaysChecked)
	}

	// Also check that version X is backwards compatible with version X-1.
	// "current" is checked against the latest version.
	var dumps []apiDump
	for _, ver := range m.properties.Versions {
		apiDir := filepath.Join(ctx.ModuleDir(), m.apiDir(), ver)
		apiDirPath := android.ExistentPathForSource(ctx, apiDir)
		if apiDirPath.Valid() {
			dumps = append(dumps, apiDump{
				dir:      apiDirPath.Path(),
				files:    ctx.Glob(filepath.Join(apiDirPath.String(), "**/*.aidl"), nil),
				hashFile: android.ExistentPathForSource(ctx, ctx.ModuleDir(), m.apiDir(), ver, ".hash"),
			})
		} else if ctx.Config().AllowMissingDependencies() {
			ctx.AddMissingDependencies([]string{apiDir})
		} else {
			ctx.ModuleErrorf("API version %s path %s does not exist", ver, apiDir)
		}
	}
	if currentApiDir.Valid() {
		dumps = append(dumps, currentApiDump)
	}
	for i, _ := range dumps {
		if dumps[i].hashFile.Valid() {
			checkHashTimestamp := m.checkIntegrity(ctx, dumps[i])
			m.checkHashTimestamps = append(m.checkHashTimestamps, checkHashTimestamp)
		}

		if i == 0 {
			continue
		}
		checked := m.checkCompatibility(ctx, dumps[i-1], dumps[i])
		m.checkApiTimestamps = append(m.checkApiTimestamps, checked)
	}

	// API dump from source is updated to the 'current' version. Triggered by `m <name>-update-api`
	m.updateApiTimestamp = m.makeApiDumpAsVersion(ctx, totApiDump, currentVersion)

	// API dump from source is frozen as the next stable version. Triggered by `m <name>-freeze-api`
	nextVersion := m.nextVersion()
	m.freezeApiTimestamp = m.makeApiDumpAsVersion(ctx, totApiDump, nextVersion)
}

func (m *aidlApi) AndroidMk() android.AndroidMkData {
	return android.AndroidMkData{
		Custom: func(w io.Writer, name, prefix, moduleDir string, data android.AndroidMkData) {
			android.WriteAndroidMkData(w, data)
			targetName := m.properties.BaseName + "-freeze-api"
			fmt.Fprintln(w, ".PHONY:", targetName)
			fmt.Fprintln(w, targetName+":", m.freezeApiTimestamp.String())

			targetName = m.properties.BaseName + "-update-api"
			fmt.Fprintln(w, ".PHONY:", targetName)
			fmt.Fprintln(w, targetName+":", m.updateApiTimestamp.String())
		},
	}
}

func (m *aidlApi) DepsMutator(ctx android.BottomUpMutatorContext) {
	ctx.AddDependency(ctx.Module(), nil, wrap("", m.properties.Imports, aidlInterfaceSuffix)...)
}

func aidlApiFactory() android.Module {
	m := &aidlApi{}
	m.AddProperties(&m.properties)
	android.InitAndroidModule(m)
	return m
}

func addApiModule(mctx android.LoadHookContext, i *aidlInterface) string {
	apiModule := i.ModuleBase.Name() + aidlApiSuffix
	srcs, aidlRoot := i.srcsForVersion(mctx, i.currentVersion())
	mctx.CreateModule(aidlApiFactory, &nameProperties{
		Name: proptools.StringPtr(apiModule),
	}, &aidlApiProperties{
		BaseName:  i.ModuleBase.Name(),
		Srcs:      srcs,
		AidlRoot:  aidlRoot,
		Stability: i.properties.Stability,
		Imports:   concat(i.properties.Imports, []string{i.ModuleBase.Name()}),
		Versions:  i.properties.Versions,
	})
	return apiModule
}
