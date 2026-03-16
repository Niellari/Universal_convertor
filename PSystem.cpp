#include "PSystem.h"
#include <sstream>
#include <stdexcept>
#include <cctype>

PSystem::PSystem(unsigned long long base) : base(base) {
    if (base < 2 || base > BASE) {
        throw std::invalid_argument("Некорректная база");
    }
}

unsigned long long PSystem::parseDigit(const std::string& str) const {
    if (str.empty()) {
        throw std::invalid_argument("Пустой символ");
    }

    if (str.length() == 1 && base <= 36) {
        char c = str[0];
        unsigned long long digit = 0;
        if (c >= '0' && c <= '9') {
            digit = c - '0';
        } else if (c >= 'A' && c <= 'Z') {
            digit = 10 + (c - 'A');
        } else if (c >= 'a' && c <= 'z') {
            digit = 10 + (c - 'a');
        } else {
            throw std::invalid_argument("Некорректный символ");
        }
        if (digit >= base) {
            throw std::invalid_argument("Цифра не соответствует базе");
        }
        return digit;
    }

    BigInt bigDigit(str);
    BigInt bigBase(base);
    if (bigDigit >= bigBase) {
        throw std::invalid_argument("Цифра не соответствует базе");
    }
    return bigDigit.getDigits()[0];
}

BigInt PSystem::fromString(const std::string& str) {
    if (str.empty()) {
        return BigInt();
    }

    std::vector<unsigned long long> digits_;
    std::istringstream stream(str);
    std::string chunk;

    bool hasSpaces = str.find(' ') != std::string::npos;

    if (hasSpaces) {
        while (stream >> chunk) {
            digits_.push_back(parseDigit(chunk));
        }
    } else {
        if (base <= 36) {
            for (char c : str) {
                if (std::isspace(c)) {
                    continue;
                }
                std::string s(1, c);
                digits_.push_back(parseDigit(s));
            }
        } else {
            digits_.push_back(parseDigit(str));
        }
    }

    BigInt result;
    for (unsigned long long d : digits_) {
        BigInt multiplier(base);
        BigInt addend(d);
        result = result * multiplier + addend;
    }

    return result;
}

std::string PSystem::toString(BigInt value) {
    if (value.isZero()) {
        return "0";
    }
    if (base <= 36) {
        char digits[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
        std::string result;
        BigInt temp = value;

        while (!temp.isZero()) {
            BigInt quotient, remainder;
            BigInt divisor(base);
            temp.divideAndRemainder(divisor, quotient, remainder);
            unsigned long long d = remainder.getDigits()[0];
            result.push_back(digits[d]);
            temp = quotient;
        }

        std::reverse(result.begin(), result.end());
        return result;
    } else {
        return value.toStringWithBase(base);
    }
}

bool PSystem::isValid(const std::string& str) {
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

std::string PSystem::getName() {
    return std::to_string(base) + "-ary";
}
