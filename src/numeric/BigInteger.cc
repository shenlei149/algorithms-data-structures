#include "numeric/BigInteger.h"

#include <iomanip>
#include <sstream>

#include <iostream>

namespace guozi::numeric
{

namespace
{
size_t
CountLeadingZeros(std::string_view sv)
{
	size_t count = 0;
	for (auto &&c : sv)
	{
		if (c == '0')
		{
			count++;
		}
		else
		{
			break;
		}
	}

	return count;
}
} // namespace

std::string
BigInteger::ToString()
{
	std::stringstream str {};
	for (auto digit = digits_.rbegin(); digit != digits_.rend(); digit++)
	{
		str << std::setfill('0') << std::setw(BaseLength) << *digit;
	}

	auto view = str.view();
	auto count = CountLeadingZeros(view);

	return std::string { view.substr(count) };
}

BigInteger
BigInteger::FromString(const std::string &str)
{
	BigInteger ret {};

	if (str.empty())
	{
		return ret;
	}

	auto length = str.size();
	auto index = length;
	while (true)
	{
		if (index == 0)
		{
			break;
		}

		if (index >= BaseLength)
		{
			index -= BaseLength;
			ret.digits_.push_back(uint16_t(std::stoi(str.substr(index, BaseLength))));
		}
		else
		{
			ret.digits_.push_back(uint16_t(std::stoi(str.substr(0, index))));
			index -= index;
		}
	}

	return ret;
}

BigInteger
operator+(const BigInteger &lhs, const BigInteger &rhs)
{
	BigInteger sum {};
	auto leftLength = lhs.digits_.size();
	auto rightLength = rhs.digits_.size();
	auto length = std::min(leftLength, rightLength);
	uint16_t carry { 0 };
	for (size_t i = 0; i < length; i++)
	{
		carry = lhs.digits_[i] + rhs.digits_[i] + carry;
		sum.digits_.push_back(carry % BigInteger::Base);
		carry /= BigInteger::Base;
	}

	if (leftLength > length)
	{
		for (size_t i = length; i < leftLength; i++)
		{
			carry = lhs.digits_[i] + carry;
			sum.digits_.push_back(carry % BigInteger::Base);
			carry /= BigInteger::Base;
		}
	}
	else if (rightLength > length)
	{
		for (size_t i = length; i < rightLength; i++)
		{
			carry = rhs.digits_[i] + carry;
			sum.digits_.push_back(carry % BigInteger::Base);
			carry /= BigInteger::Base;
		}
	}

	if (carry > 0)
	{
		sum.digits_.push_back(carry);
	}

	return sum;
}
} // namespace guozi::numeric
