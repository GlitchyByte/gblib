// Copyright 2024 GlitchyByte
// SPDX-License-Identifier: Apache-2.0

#include <glitchybyte/gb.hpp>
#include <gtest/gtest.h>

TEST(StringInterpolationVars, interpolate) {
    std::string const str { "${greeting} ${subject}!" };
    gb::StringInterpolationVars vars;
    vars.set("greeting", "Hello")
        .set("subject", "world");
    std::string const result { vars.interpolate(str) };
    ASSERT_EQ(result, "Hello world!");
}

TEST(StringInterpolationVars, interpolate_repeated) {
    std::string const str { "${x} + ${y} + ${x} = ${r}" };
    gb::StringInterpolationVars vars;
    vars.set("x", "1")
        .set("y", "2")
        .set("z", "3")
        .set("r", "4");
    std::string const result { vars.interpolate(str) };
    ASSERT_EQ(result, "1 + 2 + 1 = 4");
}

TEST(StringInterpolationVars, interpolate_nothingToReplace) {
    std::string const str { "${x} + ${y} + ${x} = ${r}" };
    gb::StringInterpolationVars vars;
    vars.set("other", "7");
    std::string const result { vars.interpolate(str) };
    ASSERT_EQ(result, "${x} + ${y} + ${x} = ${r}");
}

TEST(StringInterpolationVars, interpolate_emptyTemplate) {
    std::string const str;
    gb::StringInterpolationVars vars;
    vars.set("other", "7");
    std::string const result { vars.interpolate(str) };
    ASSERT_EQ(result, "");
}

TEST(StringInterpolationVars, interpolate_reuse) {
    std::string const str { "${x} + ${y} + ${x} = ${r}" };
    gb::StringInterpolationVars vars;
    vars.set("x", "1")
        .set("y", "2")
        .set("r", "4");
    std::string result { vars.interpolate(str) };
    ASSERT_EQ(result, "1 + 2 + 1 = 4");
    vars.set("x", "2")
        .set("y", "5")
        .set("r", "9");
    result = vars.interpolate(str);
    ASSERT_EQ(result, "2 + 5 + 2 = 9");
}
