#include "numeric/BigInteger.h"

#include <gtest/gtest.h>

using namespace guozi::numeric;

TEST(BigInteger, ToString)
{
	std::string strNum = "123456789";
	auto num = BigInteger::FromString(strNum);
	EXPECT_EQ(num.ToString(), strNum);

	strNum = "";
	num = BigInteger::FromString(strNum);
	EXPECT_EQ(num.ToString(), strNum);

	strNum = "123";
	num = BigInteger::FromString(strNum);
	EXPECT_EQ(num.ToString(), strNum);

	strNum = "12345";
	num = BigInteger::FromString(strNum);
	EXPECT_EQ(num.ToString(), strNum);

	strNum = "12345678901234567";
	num = BigInteger::FromString(strNum);
	EXPECT_EQ(num.ToString(), strNum);

	strNum = "12345678901234567890";
	num = BigInteger::FromString(strNum);
	EXPECT_EQ(num.ToString(), strNum);

	strNum = "3141592653589793238462643383279502884197169399375105820974944592";
	num = BigInteger::FromString(strNum);
	EXPECT_EQ(num.ToString(), strNum);

	strNum = "2718281828459045235360287471352662497757247093699959574966967627";
	num = BigInteger::FromString(strNum);
	EXPECT_EQ(num.ToString(), strNum);
}

TEST(BigInteger, OperatorAdd)
{
	std::string strNum1 = "123456789";
	auto num1 = BigInteger::FromString(strNum1);
	std::string strNum2 = "123";
	auto num2 = BigInteger::FromString(strNum2);
	auto sum = num1 + num2;
	EXPECT_EQ(sum.ToString(), "123456912");

	strNum1 = "12345";
	num1 = BigInteger::FromString(strNum1);
	strNum2 = "12345678901234567";
	num2 = BigInteger::FromString(strNum2);
	sum = num1 + num2;
	EXPECT_EQ(sum.ToString(), "12345678901246912");

	strNum1 = "3141592653589793238462643383279502884197169399375105820974944592";
	num1 = BigInteger::FromString(strNum1);
	strNum2 = "2718281828459045235360287471352662497757247093699959574966967627";
	num2 = BigInteger::FromString(strNum2);
	sum = num1 + num2;
	EXPECT_EQ(sum.ToString(), "5859874482048838473822930854632165381954416493075065395941912219");
}

TEST(BigInteger, OperatorSub)
{
	std::string strNum1 = "123456789";
	auto num1 = BigInteger::FromString(strNum1);
	std::string strNum2 = "123";
	auto num2 = BigInteger::FromString(strNum2);
	auto diff = num1 - num2;
	EXPECT_EQ(diff.ToString(), "123456666");

	strNum1 = "12345";
	num1 = BigInteger::FromString(strNum1);
	strNum2 = "12345678901234567";
	num2 = BigInteger::FromString(strNum2);
	diff = num2 - num1;
	EXPECT_EQ(diff.ToString(), "12345678901222222");

	strNum1 = "3141592653589793238462643383279502884197169399375105820974944592";
	num1 = BigInteger::FromString(strNum1);
	strNum2 = "2718281828459045235360287471352662497757247093699959574966967627";
	num2 = BigInteger::FromString(strNum2);
	diff = num1 - num2;
	EXPECT_EQ(diff.ToString(), "423310825130748003102355911926840386439922305675146246007976965");
}

TEST(BigInteger, OperatorMul)
{
	std::string strNum1 = "123456789";
	auto num1 = BigInteger::FromString(strNum1);
	std::string strNum2 = "123";
	auto num2 = BigInteger::FromString(strNum2);
	auto mul = num1 * num2;
	EXPECT_EQ(mul.ToString(), "15185185047");

	strNum1 = "12345";
	num1 = BigInteger::FromString(strNum1);
	strNum2 = "12345678901234567";
	num2 = BigInteger::FromString(strNum2);
	mul = num1 * num2;
	EXPECT_EQ(mul.ToString(), "152407406035740729615");

	strNum1 = "4592";
	num1 = BigInteger::FromString(strNum1);
	strNum2 = "7627";
	num2 = BigInteger::FromString(strNum2);
	mul = num1 * num2;
	EXPECT_EQ(mul.ToString(), "35023184");

	strNum1 = "74944592";
	num1 = BigInteger::FromString(strNum1);
	strNum2 = "66967627";
	num2 = BigInteger::FromString(strNum2);
	mul = num1 * num2;
	EXPECT_EQ(mul.ToString(), "5018861482723184");

	strNum1 = "5105820974944592";
	num1 = BigInteger::FromString(strNum1);
	strNum2 = "9959574966967627";
	num2 = BigInteger::FromString(strNum2);
	mul = num1 * num2;
	EXPECT_EQ(mul.ToString(), "50851806767876401952806582723184");

	strNum1 = "2884197169399375105820974944592";
	num1 = BigInteger::FromString(strNum1);
	strNum2 = "62497757247093699959574966967627";
	num2 = BigInteger::FromString(strNum2);
	mul = num1 * num2;
	EXPECT_EQ(mul.ToString(), "180255854545876931315273184330549073013374895871952806582723184");

	strNum1 = "3141592653589793238462643383279502884197169399375105820974944592";
	num1 = BigInteger::FromString(strNum1);
	strNum2 = "2718281828459045235360287471352662497757247093699959574966967627";
	num2 = BigInteger::FromString(strNum2);
	mul = num1 * num2;
	EXPECT_EQ(
		mul.ToString(),
		"8539734222673567065463550869546574495034888535765114961879601127067743044893204848617875072216249073013374895871952806582723184");
}
