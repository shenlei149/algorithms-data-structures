#include "numeric/BigInteger.h"

#include <iomanip>
#include <sstream>

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

BigInteger::BigInteger(std::vector<uint16_t> data)
	: digits_(std::move(data))
{
}

BigInteger
BigInteger::operator<<(uint16_t shiftSize)
{
	if (IsZero())
	{
		return *this;
	}

	auto origin = this->ToString();
	return FromString(origin + std::string(shiftSize, '0'));
}

bool
BigInteger::IsZero() const
{
	return digits_.empty();
}

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

	const auto &leftDigits = lhs.digits_;
	const auto &rightDigits = rhs.digits_;
	auto leftLength = leftDigits.size();
	auto rightLength = rightDigits.size();
	auto length = std::min(leftLength, rightLength);
	uint16_t carry { 0 };
	for (size_t i = 0; i < length; i++)
	{
		carry = leftDigits[i] + rightDigits[i] + carry;
		sum.digits_.push_back(carry % BigInteger::Base);
		carry /= BigInteger::Base;
	}

	if (leftLength > length)
	{
		for (size_t i = length; i < leftLength; i++)
		{
			carry = leftDigits[i] + carry;
			sum.digits_.push_back(carry % BigInteger::Base);
			carry /= BigInteger::Base;
		}
	}
	else if (rightLength > length)
	{
		for (size_t i = length; i < rightLength; i++)
		{
			carry = rightDigits[i] + carry;
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

BigInteger
operator-(const BigInteger &lhs, const BigInteger &rhs)
{
	// suppose lhs > rhs

	if (rhs.IsZero())
	{
		return lhs;
	}

	const auto &leftDigits = lhs.digits_;
	const auto &rightDigits = rhs.digits_;
	BigInteger diff {};
	auto leftLength = leftDigits.size();
	auto rightLength = rightDigits.size();
	uint16_t carry { 0 };
	for (size_t i = 0; i < rightLength; i++)
	{
		if (leftDigits[i] >= rightDigits[i] + carry)
		{
			diff.digits_.push_back(leftDigits[i] - rightDigits[i] - carry);
			carry = 0;
		}
		else
		{
			diff.digits_.push_back(BigInteger::Base + leftDigits[i] - rightDigits[i] - carry);
			carry = 1;
		}
	}

	if (leftLength > rightLength)
	{
		for (size_t i = rightLength; i < leftLength; i++)
		{
			if (leftDigits[i] >= carry)
			{
				diff.digits_.push_back(leftDigits[i] - carry);
				carry = 0;
			}
			else
			{
				diff.digits_.push_back(BigInteger::Base + leftDigits[i] - carry);
				carry = 1;
			}
		}
	}

	// TODO normalize...
	return diff;
}

BigInteger
operator*(const BigInteger &lhs, const BigInteger &rhs)
{
	if (lhs.IsZero() || rhs.IsZero())
	{
		return {};
	}

	const auto &leftDigits = lhs.digits_;
	const auto &rightDigits = rhs.digits_;
	auto leftLength = leftDigits.size();
	auto rightLength = rightDigits.size();

	if (leftLength == 1 && rightLength == 1)
	{
		BigInteger mul {};
		uint32_t num1 = leftDigits.front();
		uint32_t num2 = rightDigits.front();
		uint32_t num = num1 * num2;

		mul.digits_.push_back(num % BigInteger::Base);
		num /= BigInteger::Base;
		if (num > 0)
		{
			mul.digits_.push_back(num);
		}

		return mul;
	}

	auto length = std::max(leftLength, rightLength);
	length = std::bit_ceil(length);
	auto half = length / 2;

	BigInteger leftSecond { std::vector(leftDigits.begin(), leftDigits.begin() + std::min(half, leftLength)) };
	BigInteger leftFirst {};
	if (leftLength > half)
	{
		leftFirst.digits_ = std::vector(leftDigits.begin() + half, leftDigits.end());
	}

	BigInteger rightSecond { std::vector(rightDigits.begin(), rightDigits.begin() + std::min(half, rightLength)) };
	BigInteger rightFirst {};
	if (rightLength > half)
	{
		rightFirst.digits_ = std::vector(rightDigits.begin() + half, rightDigits.end());
	}

	auto lower = leftSecond * rightSecond;
	auto high = leftFirst * rightFirst;
	auto middle = ((leftFirst + leftSecond) * (rightFirst + rightSecond)) - high - lower;

	return (high << (length * BigInteger::BaseLength)) + (middle << (half * BigInteger::BaseLength)) + lower;
}

} // namespace guozi::numeric
