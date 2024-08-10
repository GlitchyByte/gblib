// Copyright 2024 GlitchyByte
// SPDX-License-Identifier: Apache-2.0

#include <gb.h>
#include <gtest/gtest.h>

TEST(Random, randomInt) {
    // This is random, so this test can give a false positive! But unlikely.
    // It will never be a false negative though.
    gb::Random<std::minstd_rand> random;
    auto const generator { random.createIntGenerator<uint32_t>(10, 15) };
    for (int i = 0; i < 100; ++i) {
        uint32_t const value = generator();
        ASSERT_GE(value, 10);
        ASSERT_LE(value, 15);
    }
}

TEST(Random, randomFloat_canonical) {
    // This is random, so this test can give a false positive! But unlikely.
    // It will never be a false negative though.
    gb::Random<std::minstd_rand> random;
    auto const generator { random.createCanonicalGenerator<double_t>() };
    for (int i = 0; i < 100; ++i) {
        double_t const value = generator();
        ASSERT_GE(value, 0.0);
        ASSERT_LT(value, 1.0);
    }
}

TEST(Random, randomFloat_range) {
    // This is random, so this test can give a false positive! But unlikely.
    // It will never be a false negative though.
    gb::Random<std::minstd_rand> random;
    auto const generator { random.createFloatGenerator<double_t>(15.0, 20.0) };
    for (int i = 0; i < 100; ++i) {
        double_t const value = generator();
        ASSERT_GE(value, 15.0);
        ASSERT_LT(value, 20.0);
    }
}

TEST(Random, randomMtInt) {
    // This is random, so this test can give a false positive! But unlikely.
    // It will never be a false negative though.
    gb::RandomMT random;
    auto const generator { random.createIntGenerator<uint32_t>(10, 15) };
    for (int i = 0; i < 100; ++i) {
        uint32_t const value = generator();
        ASSERT_GE(value, 10);
        ASSERT_LE(value, 15);
    }
}

TEST(Random, randomMtFloat_canonical) {
    // This is random, so this test can give a false positive! But unlikely.
    // It will never be a false negative though.
    gb::RandomMT random;
    auto const generator { random.createCanonicalGenerator<double_t>() };
    for (int i = 0; i < 100; ++i) {
        double_t const value = generator();
        ASSERT_GE(value, 0.0);
        ASSERT_LT(value, 1.0);
    }
}

TEST(Random, randomMtFloat_range) {
    // This is random, so this test can give a false positive! But unlikely.
    // It will never be a false negative though.
    gb::RandomMT random;
    auto const generator { random.createFloatGenerator<double_t>(15.0, 20.0) };
    for (int i = 0; i < 100; ++i) {
        double_t const value = generator();
        ASSERT_GE(value, 15.0);
        ASSERT_LT(value, 20.0);
    }
}

TEST(Random, randomMt64Int) {
    // This is random, so this test can give a false positive! But unlikely.
    // It will never be a false negative though.
    gb::RandomMT64 random;
    auto const generator { random.createIntGenerator<uint32_t>(10, 15) };
    for (int i = 0; i < 100; ++i) {
        uint32_t const value = generator();
        ASSERT_GE(value, 10);
        ASSERT_LE(value, 15);
    }
}

TEST(Random, randomMt64Float_canonical) {
    // This is random, so this test can give a false positive! But unlikely.
    // It will never be a false negative though.
    gb::RandomMT64 random;
    auto const generator { random.createCanonicalGenerator<double_t>() };
    for (int i = 0; i < 100; ++i) {
        double_t const value = generator();
        ASSERT_GE(value, 0.0);
        ASSERT_LT(value, 1.0);
    }
}

TEST(Random, randomMt64Float_range) {
    // This is random, so this test can give a false positive! But unlikely.
    // It will never be a false negative though.
    gb::RandomMT64 random;
    auto const generator { random.createFloatGenerator<double_t>(15.0, 20.0) };
    for (int i = 0; i < 100; ++i) {
        double_t const value = generator();
        ASSERT_GE(value, 15.0);
        ASSERT_LT(value, 20.0);
    }
}
