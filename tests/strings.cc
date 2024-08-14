// Copyright 2024 GlitchyByte
// SPDX-License-Identifier: Apache-2.0

#include <gb.h>
#include <gtest/gtest.h>

TEST(Strings, createVectorStringViewFromCArray) {
    char const* argv[] { "one", "two", "three" };
    int const argc = 3;
    auto const v = gb::strings::createVectorStringViewFromCArray(argc, argv);
    ASSERT_EQ(v[0], "one");
    ASSERT_EQ(v[1], "two");
    ASSERT_EQ(v[2], "three");
}

TEST(Strings, createVectorStringFromCArray) {
    char const* argv[] { "one", "two", "three" };
    int const argc = 3;
    auto const v = gb::strings::createVectorStringFromCArray(argc, argv);
    ASSERT_EQ(v[0], "one");
    ASSERT_EQ(v[1], "two");
    ASSERT_EQ(v[2], "three");
}

TEST(Strings, replace) {
    std::string str { "Hello world!" };
    auto replaced { gb::strings::replace(str, "world", "universe") };
    ASSERT_EQ(replaced, "Hello universe!");
}

TEST(Strings, replace_nothingToReplace) {
    std::string const str { "Hello world!" };
    auto replaced { gb::strings::replace(str, "darling", "universe") };
    ASSERT_EQ(replaced, "Hello world!");
}

TEST(Strings, replaceInPlace) {
    std::string str { "Hello world!" };
    auto& replaced { gb::strings::replaceInPlace(str, "world", "universe") };
    ASSERT_EQ(replaced, "Hello universe!");
    ASSERT_EQ(str, "Hello universe!");
    gb::strings::replaceInPlace(str, "Hello", "Goodbye");
    ASSERT_EQ(replaced, "Goodbye universe!");
    ASSERT_EQ(str, "Goodbye universe!");
}

TEST(Strings, replaceInPlace_nothingToReplace) {
    std::string str { "Hello world!" };
    auto replaced { gb::strings::replaceInPlace(str, "darling", "universe") };
    ASSERT_EQ(replaced, "Hello world!");
    ASSERT_EQ(str, "Hello world!");
}

TEST(Strings, splitWeak) {
    std::string const str { "one:two:three" };
    auto const parts = gb::strings::splitWeak(str, ":");
    ASSERT_EQ(parts.size(), 3);
    ASSERT_EQ(parts[0], "one");
    ASSERT_EQ(parts[1], "two");
    ASSERT_EQ(parts[2], "three");
}

TEST(Strings, splitWeak_tokenNotFound) {
    std::string const str { "one:two:three" };
    auto const parts = gb::strings::splitWeak(str, "|");
    ASSERT_EQ(parts.size(), 1);
    ASSERT_EQ(parts[0], "one:two:three");
}

TEST(Strings, split) {
    std::string const str { "one:two:three" };
    auto const parts = gb::strings::split(str, ":");
    ASSERT_EQ(parts.size(), 3);
    ASSERT_EQ(parts[0], "one");
    ASSERT_EQ(parts[1], "two");
    ASSERT_EQ(parts[2], "three");
}

TEST(Strings, split_tokenNotFound) {
    std::string const str { "one:two:three" };
    auto const parts = gb::strings::split(str, "|");
    ASSERT_EQ(parts.size(), 1);
    ASSERT_EQ(parts[0], "one:two:three");
}

TEST(Strings, unindent) {
    std::string const text { gb::strings::unindent(R"===(
        The 1st line.
            The 2nd line.
            The 3rd line.

                The 4th line.
    )===") };
    auto const lines = gb::strings::split(text, "\n");
    ASSERT_EQ(lines.size(), 6);
    ASSERT_EQ(lines[0], "The 1st line.");
    ASSERT_EQ(lines[2], "    The 3rd line.");
    ASSERT_EQ(lines[3], "");
    ASSERT_EQ(lines[4], "        The 4th line.");
    ASSERT_EQ(lines[5], "");
}

TEST(Strings, unindent_singleLine) {
    std::string const text { gb::strings::unindent("  The 1st line.") };
    auto const lines = gb::strings::split(text, "\n");
    ASSERT_EQ(lines.size(), 1);
    ASSERT_EQ(lines[0], "The 1st line.");
}

TEST(Strings, fromVector) {
    std::vector const v { "one", "two", "three" };
    auto const str = gb::strings::fromVector(v);
    ASSERT_EQ(str, "one, two, three");
}

TEST(Strings, fromVector_oneItem) {
    std::vector const v { "one" };
    auto const str = gb::strings::fromVector(v);
    ASSERT_EQ(str, "one");
}

TEST(Strings, addThousandSeparators) {
    std::string const num { "1234567" };
    auto const str = gb::strings::addThousandSeparators(num);
    ASSERT_EQ(str, "1,234,567");
}

TEST(Strings, addThousandSeparators_negative) {
    std::string const num { "-1234567" };
    auto const str = gb::strings::addThousandSeparators(num);
    ASSERT_EQ(str, "-1,234,567");
}

TEST(Strings, addThousandSeparators_under1000) {
    std::string const num { "123" };
    auto const str = gb::strings::addThousandSeparators(num);
    ASSERT_EQ(str, "123");
}

TEST(Strings, addThousandSeparators_negativeUnder1000) {
    std::string const num { "-123" };
    auto const str = gb::strings::addThousandSeparators(num);
    ASSERT_EQ(str, "-123");
}

TEST(Strings, addThousandSeparators_float) {
    std::string const num { "1234567.8901" };
    auto const str = gb::strings::addThousandSeparators(num);
    ASSERT_EQ(str, "1,234,567.8901");
}

TEST(Strings, addThousandSeparators_negativeFloat) {
    std::string const num { "-1234567.8901" };
    auto const str = gb::strings::addThousandSeparators(num);
    ASSERT_EQ(str, "-1,234,567.8901");
}

TEST(Strings, fromTime) {
    std::time_t const now { std::time(nullptr) };
    std::tm tm { *std::localtime(&now) };
    tm.tm_hour = 21;
    tm.tm_min = 17;
    tm.tm_sec = 8;
    std::time_t const time = std::mktime(&tm);
    std::string const str = gb::strings::fromTime(time, "%l:%M:%S %p");
    ASSERT_EQ(str, " 9:17:08 PM");
}

TEST(Strings, fromNumber_integral) {
    int64_t const value = 1234567;
    auto const str = gb::strings::fromNumber(value);
    ASSERT_EQ(str, "1234567");
}

TEST(Strings, fromNumber_integralNegative) {
    int64_t const value = -1234567;
    auto const str = gb::strings::fromNumber(value);
    ASSERT_EQ(str, "-1234567");
}

TEST(Strings, fromNumber_toNumber_integral) {
    int64_t const value = 9871234567;
    auto const str = gb::strings::fromNumber(value);
    ASSERT_EQ(str, "9871234567");
    auto const back = gb::strings::toNumber<int64_t>(str);
    ASSERT_EQ(back, 9871234567);
}

TEST(Strings, fromNumber_floatingPoint) {
    double_t const value = 1234567.8901;
    auto const str = gb::strings::fromNumber(value);
    ASSERT_EQ(str, "1234567.8901");
}

TEST(Strings, fromNumber_toNumber_floatingPoint) {
    double_t const value = 1234567.8901;
    auto const str = gb::strings::fromNumber(value);
    ASSERT_EQ(str, "1234567.8901");
    auto const back = gb::strings::toNumber<double_t>(str);
    ASSERT_EQ(back, 1234567.8901);
}

TEST(Strings, toNumber_stringView) {
    std::string_view const viewf { "1234567.8901" };
    auto const valuef = gb::strings::toNumber<double_t>(viewf);
    ASSERT_EQ(valuef, 1234567.8901);
    std::string_view const viewi { "9871234567" };
    auto const valuei = gb::strings::toNumber<int64_t>(viewi);
    ASSERT_EQ(valuei, 9871234567);
}

TEST(Strings, hexFromNumber) {
    int32_t const value = 1234567;
    auto const str1 = gb::strings::hexFromNumber(value, false, false);
    ASSERT_EQ(str1, "12d687");
    auto const str2 = gb::strings::hexFromNumber(value, false, true);
    ASSERT_EQ(str2, "12D687");
    auto const str3 = gb::strings::hexFromNumber(value, true, false);
    ASSERT_EQ(str3, "0012d687");
    auto const str4 = gb::strings::hexFromNumber(value, true, true);
    ASSERT_EQ(str4, "0012D687");
}

TEST(Strings, hexToNumber) {
    auto const value1 = gb::strings::hexToNumber<int32_t>("12d687");
    ASSERT_EQ(value1, 1234567);
    auto const value2 = gb::strings::hexToNumber<int32_t>("12D687");
    ASSERT_EQ(value2, 1234567);
    auto const value3 = gb::strings::hexToNumber<int32_t>("0012d687");
    ASSERT_EQ(value3, 1234567);
    auto const value4 = gb::strings::hexToNumber<int32_t>("0012D687");
    ASSERT_EQ(value4, 1234567);
}
