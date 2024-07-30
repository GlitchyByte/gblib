// Copyright 2024 GlitchyByte
// SPDX-License-Identifier: Apache-2.0

#include <gb.h>
#include <gtest/gtest.h>

TEST(Common, randomInt) {
    // This is random, so this test can pass randomly! But unlikely.
    gb::Random random;
    auto const generator { random.createIntGenerator<uint32_t>(10, 15) };
    for (int i = 0; i < 100; ++i) {
        uint32_t const value = generator();
        EXPECT_GE(value, 10);
        EXPECT_LE(value, 15);
    }
}

TEST(Common, randomFloat) {
    // This is random, so this test can pass randomly! But unlikely.
    gb::Random random;
    auto const generator { random.createFloatGenerator<double_t>(15.0, 20.0) };
    for (int i = 0; i < 100; ++i) {
        double_t const value = generator();
        EXPECT_GE(value, 15.0);
        EXPECT_LT(value, 20.0);
    }
}
