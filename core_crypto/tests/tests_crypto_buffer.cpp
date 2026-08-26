#include <gtest/gtest.h>
#include "crypto_buffer.h"

using namespace security;

TEST(CrypBufferTest, Initialization) {
    CryptoBuffer buf(16, 0xAA);

    EXPECT_EQ(buf.size(), 16);
    EXPECT_FALSE(buf.empty());
    EXPECT_EQ(buf.at(0), 0xAA);
    EXPECT_EQ(buf.at(15), 0xAA);

}

TEST(CryptoBufferTest, OutOfBoundsThrowException) {
    CryptoBuffer buf(8, 0x00);

    EXPECT_THROW(buf.at(8), std::out_of_range);
}

TEST(CryptoBufferTest, XorTransformation) {
    const uint8_t raw_data[] = {0x10, 0x20, 0x30, 0x40};
    CryptoBuffer buf(raw_data, 4);

    uint8_t key = 0xFF;
    buf.xor_transform(key);

    EXPECT_EQ(buf.at(0), 0x10 ^ key);

    buf.xor_transform(key);
    EXPECT_EQ(buf.at(0), 0x10);
    EXPECT_EQ(buf.at(3), 0x40);
}

TEST(CryptoBufferTest, MemoryWipe) {
    CryptoBuffer buf(10, 0xFF);
    EXPECT_EQ(buf.size(), 10);

    buf.wipe();

    EXPECT_EQ(buf.size(), 0);
    EXPECT_TRUE(buf.empty());
}

TEST(CryptoBufferTest, MoveConstructor) {
    CryptoBuffer original(8, 0xAB);
    CryptoBuffer moved_to(std::move(original));

    EXPECT_EQ(moved_to.size(), 8);
    EXPECT_EQ(moved_to.at(0), 0xAB);

    EXPECT_EQ(original.size(), 0);
    EXPECT_TRUE(original.empty());

}