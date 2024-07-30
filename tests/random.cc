// Copyright 2024 GlitchyByte
// SPDX-License-Identifier: Apache-2.0

#include <gb.h>
#include <gtest/gtest.h>

TEST(Random, randomInt) {
    // This is random, so this test can pass randomly! But unlikely.
    gb::Random<std::minstd_rand> random;
    auto const generator { random.createIntGenerator<uint32_t>(10, 15) };
    for (int i = 0; i < 100; ++i) {
        uint32_t const value = generator();
        EXPECT_GE(value, 10);
        EXPECT_LE(value, 15);
    }
}

TEST(Random, randomFloat) {
    // This is random, so this test can pass randomly! But unlikely.
    gb::Random<std::minstd_rand> random;
    auto const generator { random.createFloatGenerator<double_t>(15.0, 20.0) };
    for (int i = 0; i < 100; ++i) {
        double_t const value = generator();
        EXPECT_GE(value, 15.0);
        EXPECT_LT(value, 20.0);
    }
}

TEST(Random, randomMtInt) {
    // This is random, so this test can pass randomly! But unlikely.
    gb::RandomMT random;
    auto const generator { random.createIntGenerator<uint32_t>(10, 15) };
    for (int i = 0; i < 100; ++i) {
        uint32_t const value = generator();
        EXPECT_GE(value, 10);
        EXPECT_LE(value, 15);
    }
}

TEST(Random, randomMtFloat) {
    // This is random, so this test can pass randomly! But unlikely.
    gb::RandomMT random;
    auto const generator { random.createFloatGenerator<double_t>(15.0, 20.0) };
    for (int i = 0; i < 100; ++i) {
        double_t const value = generator();
        EXPECT_GE(value, 15.0);
        EXPECT_LT(value, 20.0);
    }
}

TEST(Random, randomMt64Int) {
    // This is random, so this test can pass randomly! But unlikely.
    gb::RandomMT64 random;
    auto const generator { random.createIntGenerator<uint32_t>(10, 15) };
    for (int i = 0; i < 100; ++i) {
        uint32_t const value = generator();
        EXPECT_GE(value, 10);
        EXPECT_LE(value, 15);
    }
}

TEST(Random, randomMt64Float) {
    // This is random, so this test can pass randomly! But unlikely.
    gb::RandomMT64 random;
    auto const generator { random.createFloatGenerator<double_t>(15.0, 20.0) };
    for (int i = 0; i < 100; ++i) {
        double_t const value = generator();
        EXPECT_GE(value, 15.0);
        EXPECT_LT(value, 20.0);
    }
}
