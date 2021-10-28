/*
 * Copyright (C) 2021, The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <string>

#include <android-base/result-gmock.h>
#include <gtest/gtest.h>

#include "parser.h"

using ::android::base::testing::Ok;
using ::testing::Not;

TEST(PermParser, ParseSimpleExpression) {
  auto ret = perm::Parser::Parse("permission = READ_PHONE_STATE");
  EXPECT_THAT(ret, Ok());
  ret = perm::Parser::Parse("uid = SYSTEM_UID");
  EXPECT_THAT(ret, Ok());
}

TEST(PermParser, ParseQuantifiedExpression) {
  auto ret =
      perm::Parser::Parse("permission = READ_PHONE_STATE || permission = ACCESS_FINE_LOCATION");
  EXPECT_THAT(ret, Ok());
  ret = perm::Parser::Parse("permission = READ_PHONE_STATE || uid = SYSTEM_UID");
  EXPECT_THAT(ret, Ok());
  ret = perm::Parser::Parse("permission = READ_PHONE_STATE && uid = SYSTEM_UID");
  EXPECT_THAT(ret, Ok());
};

TEST(PermParser, ParseIncorrectExpression) {
  auto ret = perm::Parser::Parse("permission =");
  EXPECT_THAT(ret, Not(Ok()));
  ret = perm::Parser::Parse("uid =");
  EXPECT_THAT(ret, Not(Ok()));
  ret = perm::Parser::Parse("||uid= SYSTEM_UID");
  EXPECT_THAT(ret, Not(Ok()));
  ret = perm::Parser::Parse("");
  EXPECT_THAT(ret, Not(Ok()));
  ret = perm::Parser::Parse("permission=m=");
  EXPECT_THAT(ret, Not(Ok()));
}
