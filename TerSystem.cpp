#include "TerSystem.h"
#include <sstream>
#include <stdexcept>
#include <algorithm>

int TerSystem::parseDigit(char c) const {
    switch (c) {
    case '0': return 0;
    case '1': return 1;
    case 'T': case 't': return -1;
    default:
        throw std::invalid_argument(std::string("Некорректный символ: ") + c);
    }
}

char TerSystem::digitToChar(int digit) const {
    switch (digit) {
    case 0: return '0';
    case 1: return '1';
    case -1: return 'T';
    default:
        throw std::invalid_argument("Некорректный символ");
    }
}

BigInt TerSystem::fromString(const std::string& str) {
    if (str.empty()) {
        return BigInt();
    }

    std::vector<int> digits_;
    for (char c : str) {
        if (std::isspace(c)) {
            continue;
        }
        digits_.push_back(parseDigit(c));
    }

    if (digits_.empty()) {
        return BigInt();
    }

    BigInt result;
    BigInt power(1);

    for (auto it = digits_.rbegin(); it != digits_.rend(); ++it) {
        int d = *it;
        if (d > 0) {
            BigInt multiplier(d);
            result = result + power * multiplier;
        } else if (d < 0) {
            BigInt multiplier(static_cast<unsigned long long>(-d));
            result = result - power * multiplier;
        }
        power = power * BigInt(3);
    }

    return result;
}

std::string TerSystem::toString(BigInt value) {
    if (value.isZero()) {
        return "0";
    }

    BigInt temp = value;
    std::vector<int> digits_;

    while (!temp.isZero()) {
        BigInt quotient, remainder;
        BigInt divisor(3);
        temp.divideAndRemainder(divisor, quotient, remainder);

        int r = remainder.getDigits()[0];

        if (r == 2) {
            digits_.push_back(-1);
            BigInt one(1);
            quotient = quotient + one;
        } else if (r == 1) {
            digits_.push_back(1);
        } else {
            digits_.push_back(0);
        }

        temp = quotient;
    }

    std::string result;
    for (auto it = digits_.rbegin(); it != digits_.rend(); ++it) {
        result.push_back(digitToChar(*it));
    }

    return result;
}

bool TerSystem::isValid(const std::string& str) {
    try {
        fromString(str);
        return true;
    } catch (std::invalid_argument& e) {
        return false;
    } catch (std::underflow_error& e) {
        return false;
    } catch (std::out_of_range& e) {
        return false;
    }
}

std::string TerSystem::getName() {
    return "Balanced Ternary";
}
