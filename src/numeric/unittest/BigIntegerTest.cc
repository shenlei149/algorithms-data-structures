#include "numeric/BigInteger.h"

#include <gtest/gtest.h>

TEST(BigInteger, ToString)
{
	using namespace guozi::numeric;
	std::string str_num = "123456789";
	auto num = BigInteger::FromString(str_num);
	ASSERT_EQ(num.ToString(), str_num);

	str_num = "";
	num = BigInteger::FromString(str_num);
	ASSERT_EQ(num.ToString(), str_num);

	str_num = "123";
	num = BigInteger::FromString(str_num);
	ASSERT_EQ(num.ToString(), str_num);

	str_num = "12345";
	num = BigInteger::FromString(str_num);
	ASSERT_EQ(num.ToString(), str_num);

	str_num = "12345678901234567";
	num = BigInteger::FromString(str_num);
	ASSERT_EQ(num.ToString(), str_num);

	str_num = "12345678901234567890";
	num = BigInteger::FromString(str_num);
	ASSERT_EQ(num.ToString(), str_num);
}
