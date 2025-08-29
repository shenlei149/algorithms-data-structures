#include <cstdint>
#include <string>
#include <vector>

namespace guozi::numeric
{
class BigInteger
{
public:
	std::string ToString();
	static BigInteger FromString(const std::string &str);

public: // friends
	friend BigInteger operator+(const BigInteger &lhs, const BigInteger &rhs);

private:
	/**
	 * @note for future... so far suppose integer is nonnegative
	 */
	bool sign_ { true };

	std::vector<uint16_t> digits_ {};

	static constexpr uint16_t Base { 10'000 };
	static constexpr uint16_t BaseLength { 4 };
};

BigInteger operator+(const BigInteger &lhs, const BigInteger &rhs);

} // namespace guozi::numeric
