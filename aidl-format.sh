#!/usr/bin/env bash

# Copyright (C) 2020 The Android Open Source Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

set -e

# future considerations:
# - could we make this work with git-clang-format instead?
# - should we have our own formatter?

function _aidl-format() (
    if [ $# != 1 ]; then
      echo "Usage: $0 <file/directory>"
      exit 1
    fi

    local loc="$1"

    local tmpfile="$tmpfile"

    # Do a "reversible" conversion of the input file so that it is more friendly
    # to clang-format. For example 'oneway interface Foo{}' is not recognized as
    # an interface. Convert it to 'interface __aidl_oneway__ Foo{}'.
    function prepare() {
      # oneway interface Foo {} is not correctly recognized as an interface by
      # clang-format. Change it to interface __aidl_oneway__ Foo {}.
      sed -i -E 's/oneway[[:space:]]+interface/interface\ __aidl_oneway__/g' "$1"

      # When a declaration becomes too long, clang-format splits the declaration
      # into multiple lines. In doing so, annotations that are at the front of
      # the declaration are always split. i.e.
      #
      # @utf8InCpp @nullable void foo(int looooooo....ong, int looo....ong);
      #
      # becomes
      #
      # @utf8InCpp
      # @nullable
      # void foo(int loooooo...ong,
      #     int looo.....ong);
      #
      # This isn't desirable for utf8InCpp and nullable annotations which are
      # semantically tagged to the type, not the member (field/method). We want
      # to have the annotations in the same line as the type that they actually
      # annotate. i.e.
      #
      # @utf8InCpp @nullable void foo(int looo....ong,
      #     int looo.....ong);
      #
      # To do so, the annotations are temporarily replaced with tokens that are
      # not annotations.
      sed -i -E 's/@utf8InCpp/__aidl_utf8inCpp__/g' "$1"
      sed -i -E 's/@nullable/__aidl_nullable__/g' "$1"
    }

    function apply-clang-format() {
      local input="$1"
      local temp="$(mktemp)"
      cat "$input" | clang-format \
        --style='{BasedOnStyle: Google, BreakAfterJavaFieldAnnotations: false, ColumnLimit: 100}' \
        --assume-filename=${input%.*}.java \
        > "$temp"
      mv "$temp" "$input"
    }

    # clang-format is good, but doesn't perfectly fit to our needs. Fix the
    # minor mismatches manually.
    function fixup() {
      # Revert the changes done during the prepare call. Notice that the
      # original tokens (@utf8InCpp, etc.) are shorter than the temporary tokens
      # (__aidl_utf8InCpp, etc.). This can make the output text length shorter
      # than the specified column limit. We can try to reduce the undesirable
      # effect by keeping the tokens to have similar lengths, but that seems to
      # be an overkill at this moment. We can revisit this when this becomes a
      # real problem.
      sed -i -E 's/interface\ __aidl_oneway__/oneway\ interface/g' "$1"
      sed -i -E 's/__aidl_utf8inCpp__/@utf8InCpp/g' "$1"
      sed -i -E 's/__aidl_nullable__/@nullable/g' "$1"

      # clang-format adds space around "=" in annotation parameters. e.g.
      # @Anno(a = 100). The following awk script removes the spaces back.
      # @Anno(a = 1, b = 2) @Anno(c = 3, d = 4) int foo = 3; becomes
      # @Anno(a=1, b=2) @Anno(c=3, d=4) int foo = 3;
      # [^@,=] ensures that the match doesn't cross the characters, otherwise
      # "a = 1, b = 2" would match only once and will become "a = 1, b=2".
      gawk -i inplace \
        '/@[^@]+\(.*=.*\)/ { # matches a line having @anno(param = val) \
              print(gensub(/([^@,=]+) = ([^@,=]+)/, "\\1=\\2", "g", $0)); \
              done=1;\
        } \
        {if (!done) {print($0);} done=0;}' "$1"
    }

    function format-one() {
        local input="$1"
        local output="$(mktemp)"

        cp "$input" "$output"
        prepare "$output"
        apply-clang-format "$output"
        fixup "$output"

        if diff -q "$output" "$input" >/dev/null; then
            rm "$output"
        else
            mv "$output" "$input"
        fi
    }

    while read -d '' aidl; do
        format-one "$aidl"
    done < <(find "$loc" -not -path "*/aidl_api/*" -type f -name '*.aidl' -print0)
)

_aidl-format "$@"
