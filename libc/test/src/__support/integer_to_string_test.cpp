//===-- Unittests for IntegerToString -------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "src/__support/CPP/limits.h"
#include "src/__support/CPP/span.h"
#include "src/__support/CPP/string_view.h"
#include "src/__support/big_int.h"
#include "src/__support/integer_literals.h"
#include "src/__support/integer_to_string.h"
#include "src/__support/uint128.h"

#include "test/UnitTest/Test.h"

using LIBC_NAMESPACE::BigInt;
using LIBC_NAMESPACE::IntegerToString;
using LIBC_NAMESPACE::cpp::span;
using LIBC_NAMESPACE::cpp::string_view;
using LIBC_NAMESPACE::radix::Bin;
using LIBC_NAMESPACE::radix::Custom;
using LIBC_NAMESPACE::radix::Dec;
using LIBC_NAMESPACE::radix::Hex;
using LIBC_NAMESPACE::radix::Oct;
using LIBC_NAMESPACE::operator""_u128;
using LIBC_NAMESPACE::operator""_u256;

#define EXPECT(type, value, string_value)                                      \
  {                                                                            \
    const type buffer(value);                                                  \
    EXPECT_EQ(buffer.view(), string_view(string_value));                       \
  }

TEST(LlvmLibcIntegerToStringTest, UINT8) {
  using type = IntegerToString<uint8_t>;
  EXPECT(type, 0, "0");
  EXPECT(type, 1, "1");
  EXPECT(type, 12, "12");
  EXPECT(type, 123, "123");
  EXPECT(type, UINT8_MAX, "255");
  EXPECT(type, static_cast<uint8_t>(-1), "255");
}

TEST(LlvmLibcIntegerToStringTest, INT8) {
  using type = IntegerToString<int8_t>;
  EXPECT(type, 0, "0");
  EXPECT(type, 1, "1");
  EXPECT(type, 12, "12");
  EXPECT(type, 123, "123");
  EXPECT(type, -12, "-12");
  EXPECT(type, -123, "-123");
  EXPECT(type, INT8_MAX, "127");
  EXPECT(type, INT8_MIN, "-128");
}

TEST(LlvmLibcIntegerToStringTest, UINT16) {
  using type = IntegerToString<uint16_t>;
  EXPECT(type, 0, "0");
  EXPECT(type, 1, "1");
  EXPECT(type, 12, "12");
  EXPECT(type, 123, "123");
  EXPECT(type, 1234, "1234");
  EXPECT(type, 12345, "12345");
  EXPECT(type, UINT16_MAX, "65535");
  EXPECT(type, static_cast<uint16_t>(-1), "65535");
}

TEST(LlvmLibcIntegerToStringTest, INT16) {
  using type = IntegerToString<int16_t>;
  EXPECT(type, 0, "0");
  EXPECT(type, 1, "1");
  EXPECT(type, 12, "12");
  EXPECT(type, 123, "123");
  EXPECT(type, 1234, "1234");
  EXPECT(type, 12345, "12345");
  EXPECT(type, -1, "-1");
  EXPECT(type, -12, "-12");
  EXPECT(type, -123, "-123");
  EXPECT(type, -1234, "-1234");
  EXPECT(type, -12345, "-12345");
  EXPECT(type, INT16_MAX, "32767");
  EXPECT(type, INT16_MIN, "-32768");
}

TEST(LlvmLibcIntegerToStringTest, UINT32) {
  using type = IntegerToString<uint32_t>;
  EXPECT(type, 0, "0");
  EXPECT(type, 1, "1");
  EXPECT(type, 12, "12");
  EXPECT(type, 123, "123");
  EXPECT(type, 1234, "1234");
  EXPECT(type, 12345, "12345");
  EXPECT(type, 123456, "123456");
  EXPECT(type, 1234567, "1234567");
  EXPECT(type, 12345678, "12345678");
  EXPECT(type, 123456789, "123456789");
  EXPECT(type, 1234567890, "1234567890");
  EXPECT(type, UINT32_MAX, "4294967295");
  EXPECT(type, static_cast<uint32_t>(-1), "4294967295");
}

TEST(LlvmLibcIntegerToStringTest, INT32) {
  using type = IntegerToString<int32_t>;
  EXPECT(type, 0, "0");
  EXPECT(type, 1, "1");
  EXPECT(type, 12, "12");
  EXPECT(type, 123, "123");
  EXPECT(type, 1234, "1234");
  EXPECT(type, 12345, "12345");
  EXPECT(type, 123456, "123456");
  EXPECT(type, 1234567, "1234567");
  EXPECT(type, 12345678, "12345678");
  EXPECT(type, 123456789, "123456789");
  EXPECT(type, 1234567890, "1234567890");
  EXPECT(type, -1, "-1");
  EXPECT(type, -12, "-12");
  EXPECT(type, -123, "-123");
  EXPECT(type, -1234, "-1234");
  EXPECT(type, -12345, "-12345");
  EXPECT(type, -123456, "-123456");
  EXPECT(type, -1234567, "-1234567");
  EXPECT(type, -12345678, "-12345678");
  EXPECT(type, -123456789, "-123456789");
  EXPECT(type, -1234567890, "-1234567890");
  EXPECT(type, INT32_MAX, "2147483647");
  EXPECT(type, INT32_MIN, "-2147483648");
}

TEST(LlvmLibcIntegerToStringTest, UINT64) {
  using type = IntegerToString<uint64_t>;
  EXPECT(type, 0, "0");
  EXPECT(type, 1, "1");
  EXPECT(type, 12, "12");
  EXPECT(type, 123, "123");
  EXPECT(type, 1234, "1234");
  EXPECT(type, 12345, "12345");
  EXPECT(type, 123456, "123456");
  EXPECT(type, 1234567, "1234567");
  EXPECT(type, 12345678, "12345678");
  EXPECT(type, 123456789, "123456789");
  EXPECT(type, 1234567890, "1234567890");
  EXPECT(type, 1234567890123456789, "1234567890123456789");
  EXPECT(type, UINT64_MAX, "18446744073709551615");
  EXPECT(type, static_cast<uint64_t>(-1), "18446744073709551615");
}

TEST(LlvmLibcIntegerToStringTest, INT64) {
  using type = IntegerToString<int64_t>;
  EXPECT(type, 0, "0");
  EXPECT(type, 1, "1");
  EXPECT(type, 12, "12");
  EXPECT(type, 123, "123");
  EXPECT(type, 1234, "1234");
  EXPECT(type, 12345, "12345");
  EXPECT(type, 123456, "123456");
  EXPECT(type, 1234567, "1234567");
  EXPECT(type, 12345678, "12345678");
  EXPECT(type, 123456789, "123456789");
  EXPECT(type, 1234567890, "1234567890");
  EXPECT(type, 1234567890123456789, "1234567890123456789");
  EXPECT(type, -1, "-1");
  EXPECT(type, -12, "-12");
  EXPECT(type, -123, "-123");
  EXPECT(type, -1234, "-1234");
  EXPECT(type, -12345, "-12345");
  EXPECT(type, -123456, "-123456");
  EXPECT(type, -1234567, "-1234567");
  EXPECT(type, -12345678, "-12345678");
  EXPECT(type, -123456789, "-123456789");
  EXPECT(type, -1234567890, "-1234567890");
  EXPECT(type, -1234567890123456789, "-1234567890123456789");
  EXPECT(type, INT64_MAX, "9223372036854775807");
  EXPECT(type, INT64_MIN, "-9223372036854775808");
}

TEST(LlvmLibcIntegerToStringTest, UINT64_Base_8) {
  using type = IntegerToString<int64_t, Oct>;
  EXPECT(type, 0, "0");
  EXPECT(type, 012345, "12345");
  EXPECT(type, 0123456701234567012345, "123456701234567012345");
  EXPECT(type, static_cast<int64_t>(01777777777777777777777),
         "1777777777777777777777");
}

TEST(LlvmLibcIntegerToStringTest, UINT64_Base_16) {
  using type = IntegerToString<uint64_t, Hex>;
  EXPECT(type, 0, "0");
  EXPECT(type, 0x12345, "12345");
  EXPECT(type, 0x123456789abcdef, "123456789abcdef");
  EXPECT(type, 0xffffffffffffffff, "ffffffffffffffff");
  using TYPE = IntegerToString<uint64_t, Hex::Uppercase>;
  EXPECT(TYPE, 0x123456789abcdef, "123456789ABCDEF");
}

TEST(LlvmLibcIntegerToStringTest, UINT64_Base_2) {
  using type = IntegerToString<uint64_t, Bin>;
  EXPECT(type, 0, "0");
  EXPECT(type, 0b111100001100, "111100001100");
  EXPECT(type, 0b100100011101010111100, "100100011101010111100");
  EXPECT(type, 0xffffffffffffffff,
         "1111111111111111111111111111111111111111111111111111111111111111");
}

TEST(LlvmLibcIntegerToStringTest, UINT128_Base_16) {
  using type = IntegerToString<UInt128, Hex::WithWidth<32>>;
  EXPECT(type, 0, "00000000000000000000000000000000");
  EXPECT(type, 0x12345, "00000000000000000000000000012345");
  EXPECT(type, 0x12340000'00000000'00000000'00000000_u128,
         "12340000000000000000000000000000");
  EXPECT(type, 0x00000000'00000000'12340000'00000000_u128,
         "00000000000000001234000000000000");
  EXPECT(type, 0x00000000'00000001'23400000'00000000_u128,
         "00000000000000012340000000000000");
}

TEST(LlvmLibcIntegerToStringTest, UINT64_Base_36) {
  using type = IntegerToString<uint64_t, Custom<36>>;
  EXPECT(type, 0, "0");
  EXPECT(type, 12345, "9ix");
  EXPECT(type, 1047601316295595, "abcdefghij");
  EXPECT(type, 2092218013456445, "klmnopqrst");
  EXPECT(type, 0xffffffffffffffff, "3w5e11264sgsf");

  using TYPE = IntegerToString<uint64_t, Custom<36>::Uppercase>;
  EXPECT(TYPE, 1867590395, "UVWXYZ");
}

TEST(LlvmLibcIntegerToStringTest, UINT256_Base_16) {
  using UInt256 = LIBC_NAMESPACE::UInt<256>;
  using type = IntegerToString<UInt256, Hex::WithWidth<64>>;
  EXPECT(
      type,
      0x0000000000000000000000000000000000000000000000000000000000000000_u256,
      "0000000000000000000000000000000000000000000000000000000000000000");
  EXPECT(
      type,
      0x0000000000000000000000000000000000000000000000000000000000012345_u256,
      "0000000000000000000000000000000000000000000000000000000000012345");
  EXPECT(
      type,
      0x0000000000000000000000000000000012340000000000000000000000000000_u256,
      "0000000000000000000000000000000012340000000000000000000000000000");
  EXPECT(
      type,
      0x0000000000000000000000000000000123400000000000000000000000000000_u256,
      "0000000000000000000000000000000123400000000000000000000000000000");
  EXPECT(
      type,
      0x1234000000000000000000000000000000000000000000000000000000000000_u256,
      "1234000000000000000000000000000000000000000000000000000000000000");
}

TEST(LlvmLibcIntegerToStringTest, NegativeInterpretedAsPositive) {
  using BIN = IntegerToString<int8_t, Bin>;
  using OCT = IntegerToString<int8_t, Oct>;
  using DEC = IntegerToString<int8_t, Dec>;
  using HEX = IntegerToString<int8_t, Hex>;
  EXPECT(BIN, -1, "11111111");
  EXPECT(OCT, -1, "377");
  EXPECT(DEC, -1, "-1"); // Only DEC format negative values
  EXPECT(HEX, -1, "ff");
}

TEST(LlvmLibcIntegerToStringTest, Width) {
  using BIN = IntegerToString<uint8_t, Bin::WithWidth<4>>;
  using OCT = IntegerToString<uint8_t, Oct::WithWidth<4>>;
  using DEC = IntegerToString<uint8_t, Dec::WithWidth<4>>;
  using HEX = IntegerToString<uint8_t, Hex::WithWidth<4>>;
  EXPECT(BIN, 1, "0001");
  EXPECT(HEX, 1, "0001");
  EXPECT(OCT, 1, "0001");
  EXPECT(DEC, 1, "0001");
}

TEST(LlvmLibcIntegerToStringTest, Prefix) {
  // WithPrefix is not supported for Decimal
  using BIN = IntegerToString<uint8_t, Bin::WithPrefix>;
  using OCT = IntegerToString<uint8_t, Oct::WithPrefix>;
  using HEX = IntegerToString<uint8_t, Hex::WithPrefix>;
  EXPECT(BIN, 1, "0b1");
  EXPECT(HEX, 1, "0x1");
  EXPECT(OCT, 1, "01");
  EXPECT(OCT, 0, "0"); // Zero is not prefixed for octal
}

TEST(LlvmLibcIntegerToStringTest, Uppercase) {
  using HEX = IntegerToString<uint64_t, Hex::Uppercase>;
  EXPECT(HEX, 0xDEADC0DE, "DEADC0DE");
}

TEST(LlvmLibcIntegerToStringTest, Sign) {
  // WithSign only compiles with DEC
  using DEC = IntegerToString<int8_t, Dec::WithSign>;
  EXPECT(DEC, -1, "-1");
  EXPECT(DEC, 0, "+0");
  EXPECT(DEC, 1, "+1");
}

TEST(LlvmLibcIntegerToStringTest, BigInt_Base_10) {
  uint64_t int256_max_w64[4] = {
      0xFFFFFFFFFFFFFFFF,
      0xFFFFFFFFFFFFFFFF,
      0xFFFFFFFFFFFFFFFF,
      0x7FFFFFFFFFFFFFFF,
  };
  uint64_t int256_min_w64[4] = {
      0,
      0,
      0,
      0x8000000000000000,
  };
  uint32_t int256_max_w32[8] = {
      0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
      0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x7FFFFFFF,
  };
  uint32_t int256_min_w32[8] = {
      0, 0, 0, 0, 0, 0, 0, 0x80000000,
  };
  uint16_t int256_max_w16[16] = {
      0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
      0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x7FFF,
  };
  uint16_t int256_min_w16[16] = {
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x8000,
  };

  using unsigned_type_w64 = IntegerToString<BigInt<256, false, uint64_t>, Dec>;
  EXPECT(unsigned_type_w64, 0, "0");
  EXPECT(unsigned_type_w64, 1, "1");
  EXPECT(unsigned_type_w64, -1,
         "115792089237316195423570985008687907853269984665640564039457584007913"
         "129639935");
  EXPECT(unsigned_type_w64, int256_max_w64,
         "578960446186580977117854925043439539266349923328202820197287920039565"
         "64819967");
  EXPECT(unsigned_type_w64, int256_min_w64,
         "578960446186580977117854925043439539266349923328202820197287920039565"
         "64819968");

  using unsigned_type_w32 = IntegerToString<BigInt<256, false, uint32_t>, Dec>;
  EXPECT(unsigned_type_w32, 0, "0");
  EXPECT(unsigned_type_w32, 1, "1");
  EXPECT(unsigned_type_w32, -1,
         "115792089237316195423570985008687907853269984665640564039457584007913"
         "129639935");
  EXPECT(unsigned_type_w32, int256_max_w32,
         "578960446186580977117854925043439539266349923328202820197287920039565"
         "64819967");
  EXPECT(unsigned_type_w32, int256_min_w32,
         "578960446186580977117854925043439539266349923328202820197287920039565"
         "64819968");

  using unsigned_type_w16 = IntegerToString<BigInt<256, false, uint16_t>, Dec>;
  EXPECT(unsigned_type_w16, 0, "0");
  EXPECT(unsigned_type_w16, 1, "1");
  EXPECT(unsigned_type_w16, -1,
         "115792089237316195423570985008687907853269984665640564039457584007913"
         "129639935");
  EXPECT(unsigned_type_w16, int256_max_w16,
         "578960446186580977117854925043439539266349923328202820197287920039565"
         "64819967");
  EXPECT(unsigned_type_w16, int256_min_w16,
         "578960446186580977117854925043439539266349923328202820197287920039565"
         "64819968");

  using signed_type_w64 = IntegerToString<BigInt<256, true, uint64_t>, Dec>;
  EXPECT(signed_type_w64, 0, "0");
  EXPECT(signed_type_w64, 1, "1");
  EXPECT(signed_type_w64, -1, "-1");
  EXPECT(signed_type_w64, int256_max_w64,
         "578960446186580977117854925043439539266349923328202820197287920039565"
         "64819967");
  EXPECT(signed_type_w64, int256_min_w64,
         "-57896044618658097711785492504343953926634992332820282019728792003956"
         "564819968");

  using signed_type_w32 = IntegerToString<BigInt<256, true, uint32_t>, Dec>;
  EXPECT(signed_type_w32, 0, "0");
  EXPECT(signed_type_w32, 1, "1");
  EXPECT(signed_type_w32, -1, "-1");
  EXPECT(signed_type_w32, int256_max_w32,
         "578960446186580977117854925043439539266349923328202820197287920039565"
         "64819967");
  EXPECT(signed_type_w32, int256_min_w32,
         "-57896044618658097711785492504343953926634992332820282019728792003956"
         "564819968");

  using signed_type_w16 = IntegerToString<BigInt<256, true, uint16_t>, Dec>;
  EXPECT(signed_type_w16, 0, "0");
  EXPECT(signed_type_w16, 1, "1");
  EXPECT(signed_type_w16, -1, "-1");
  EXPECT(signed_type_w16, int256_max_w16,
         "578960446186580977117854925043439539266349923328202820197287920039565"
         "64819967");
  EXPECT(signed_type_w16, int256_min_w16,
         "-57896044618658097711785492504343953926634992332820282019728792003956"
         "564819968");
}

TEST(LlvmLibcIntegerToStringTest, BufferOverrun) {
  { // Writing '0' in an empty buffer requiring zero digits : works
    const auto view =
        IntegerToString<int, Dec::WithWidth<0>>::format_to(span<char>(), 0);
    ASSERT_TRUE(view.has_value());
    ASSERT_EQ(*view, string_view(""));
  }
  char buffer[1];
  { // Writing '1' in a buffer of one char : works
    const auto view = IntegerToString<int>::format_to(buffer, 1);
    ASSERT_TRUE(view.has_value());
    ASSERT_EQ(*view, string_view("1"));
  }
  { // Writing '11' in a buffer of one char : fails
    const auto view = IntegerToString<int>::format_to(buffer, 11);
    ASSERT_FALSE(view.has_value());
  }
}
