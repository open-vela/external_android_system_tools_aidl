#
# Copyright (C) 2023 Xiaomi Corporation
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
#

.PRECIOUS: %.cpp

SYSTEM = $(shell uname | tr '[:upper:]' '[:lower:]')
SYS_ARCH = $(shell uname -m | sed 's/arm64/aarch64/')

ROOTDIR = $(CURDIR)/../../../../..

LIBCUTILSDIR = $(ROOTDIR)/external/android/system/core/libcutils
LIBBASEDIR = $(ROOTDIR)/external/android/system/libbase
LIBLOGDIR = $(ROOTDIR)/external/android/system/logging
LIBFMTDIR = $(ROOTDIR)/external/fmtlib/fmtlib
LIBGTESTDIR = $(ROOTDIR)/external/googletest/googletest

# Please install these tools before building.
# sudo apt install bison flex clang libc++-dev libc++abi-dev
# In Ubuntu 20.04.6 LTS, the tools' version:
# bison=3.5.1
# clang=10.0.0-4ubuntu1
# flex=2.6.4
# You can use these tools on macOS too.
BISON = bison
CC = clang++
ifeq ($(SYSTEM), darwin)
CC += -arch $(shell uname -m)
endif
FLEX = flex

CXXFLAGS += -stdlib=libc++
CXXFLAGS += -std=gnu++17
CXXFLAGS += -Wall -Wextra
CXXFLAGS += -O2
CXXFLAGS += -pthread

LDFLAGS = -static-libstdc++ -lc++abi

CXXFLAGS += -Iaidl
CXXFLAGS += -I$(LIBBASEDIR)/include
CXXFLAGS += -I$(LIBFMTDIR)/include
CXXFLAGS += -I$(LIBGTESTDIR)/googletest
CXXFLAGS += -I$(LIBGTESTDIR)/googletest/include
CXXFLAGS += -I$(LIBLOGDIR)/include
CXXFLAGS += -I$(LIBCUTILSDIR)/include

PROGNAME = $(ROOTDIR)/prebuilts/tools/aidl/$(SYSTEM)/$(SYS_ARCH)/aidl

# main
CXXSRCS += main.cpp

# libaidl-common
CXXSRCS += aidl_checkapi.cpp \
	aidl_dumpapi.cpp \
	aidl_language.cpp \
	aidl_to_cpp_common.cpp \
	aidl_to_cpp.cpp \
	aidl_to_java.cpp \
	aidl_to_ndk.cpp \
	aidl_to_rust.cpp \
	aidl_typenames.cpp \
	aidl.cpp \
	ast_java.cpp \
	check_valid.cpp \
	code_writer.cpp \
	comments.cpp \
	diagnostics.cpp \
	generate_aidl_mappings.cpp \
	generate_cpp.cpp \
	generate_java_binder.cpp \
	generate_java.cpp \
	generate_ndk.cpp \
	generate_rust.cpp \
	import_resolver.cpp \
	io_delegate.cpp \
	location.cpp \
	logging.cpp \
	options.cpp \
	parser.cpp \
	permission.cpp \
	preprocess.cpp \
	aidl_const_expressions.cpp \
	aidl_language_y.cpp \
	aidl_language_l.cpp

# libbase
CXXSRCS += $(LIBBASEDIR)/strings.cpp \
	$(LIBBASEDIR)/stringprintf.cpp \
	$(LIBBASEDIR)/logging.cpp \
	$(LIBBASEDIR)/file.cpp \
	$(LIBBASEDIR)/threads.cpp \
	$(LIBBASEDIR)/posix_strerror_r.cpp

# libfmt
CXXSRCS += $(LIBFMTDIR)/src/format.cc

# liblog
CXXSRCS += $(LIBLOGDIR)/log_event_list.cpp \
	$(LIBLOGDIR)/log_event_write.cpp \
	$(LIBLOGDIR)/logger_name.cpp \
	$(LIBLOGDIR)/logger_read.cpp \
	$(LIBLOGDIR)/logger_write.cpp \
	$(LIBLOGDIR)/logprint.cpp \
	$(LIBLOGDIR)/properties.cpp

# libcutils
CXXSRCS += $(LIBCUTILSDIR)/config_utils.cpp \
	$(LIBCUTILSDIR)/iosched_policy.cpp \
	$(LIBCUTILSDIR)/load_file.cpp \
	$(LIBCUTILSDIR)/native_handle.cpp \
	$(LIBCUTILSDIR)/record_stream.cpp \
	$(LIBCUTILSDIR)/threads.cpp

# libgtest
CXXSRCS += $(LIBGTESTDIR)/googletest/src/gtest.cc \
	$(LIBGTESTDIR)/googletest/src/gtest-assertion-result.cc \
	$(LIBGTESTDIR)/googletest/src/gtest-port.cc \
	$(LIBGTESTDIR)/googletest/src/gtest-printers.cc \
	$(LIBGTESTDIR)/googletest/src/gtest-filepath.cc \
	$(LIBGTESTDIR)/googletest/src/gtest-test-part.cc \
	$(LIBGTESTDIR)/googletest/src/gtest-death-test.cc

OBJS = $(patsubst %.cpp,%.o,$(patsubst %.cc,%.o,$(CXXSRCS)))

all: $(PROGNAME)

$(PROGNAME): $(OBJS)
	$(CC) $(CXXFLAGS) $(LDFLAGS) -o $(PROGNAME) $(OBJS)
%.o: %.cpp
	$(CC) $(CXXFLAGS) -c $< -o $@

%.o: %.cc
	$(CC) $(CXXFLAGS) -c $< -o $@

%.cpp %.h: %.yy
	@echo BISON $@
	$(BISON) -Lc++ --defines=$*.h -o $*.cpp $<

%.cpp: %.ll
	@echo FLEX $@
	$(FLEX) -o $@ $<

aidl_language.cpp: aidl_language_y.h

clean:
	rm $(OBJS) $(PROGNAME) aidl_language_l.cpp aidl_language_y.cpp aidl_language_y.h *.hh