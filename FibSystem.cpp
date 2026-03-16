#include "FibSystem.h"
#include <sstream>
#include <stdexcept>
#include <algorithm>

std::vector<BigInt> FibSystem::generateFibonacci(BigInt limit) const {
    std::vector<BigInt> fib;
    fib.push_back(BigInt(1));
    fib.push_back(BigInt(2));

    while (true) {
        BigInt next = fib[fib.size() - 1] + fib[fib.size() - 2];
        if (next > limit && !limit.isZero()) {
            break;
        }
        fib.push_back(next);
        if (fib.size() > 10000) {
            break;
        }
    }

    return fib;
}

bool FibSystem::hasOnesSubstr(const std::string& bits) const {
    for (int i = 0; i + 1 < bits.length(); ++i) {
        if (bits[i] == '1' && bits[i+1] == '1') {
            return true;
        }
    }
    return false;
}

BigInt FibSystem::fromString(const std::string& str) {
    if (str.empty()) {
        return BigInt();
    }

    std::string bits;
    for (char c : str) {
        if (c == '0' || c == '1') {
            bits.push_back(c);
        } else if (!std::isspace(c)) {
            throw std::invalid_argument("Некорректный символ");
        }
    }

    if (bits.empty()) {
        return BigInt();
    }

    if (hasOnesSubstr(bits)) {
        throw std::invalid_argument("Несколько единиц подряд");
    }

    BigInt maxValue;
    if (bits.length() < 100) {
        maxValue = BigInt(1);
        BigInt two(2);
        for (int i = 0; i < bits.length(); ++i) {
            maxValue = maxValue * two;
        }
    } else {
        maxValue = BigInt(std::string(50, '9'));
    }

    std::vector<BigInt> fib = generateFibonacci(maxValue);

    while (fib.size() < bits.length()) {
        BigInt next = fib[fib.size() - 1] + fib[fib.size() - 2];
        fib.push_back(next);
    }

    BigInt result;
    for (int i = 0; i < bits.length(); ++i) {
        if (bits[i] == '1') {
            int idx = bits.length() - 1 - i;
            if (idx < fib.size()) {
                result = result + fib[idx];
            }
        }
    }

    return result;
}

std::string FibSystem::toString(BigInt value) {
    if (value.isZero()) {
        return "0";
    }

    std::vector<BigInt> fib = generateFibonacci(value);

    std::string bits;
    BigInt remainder = value;

    for (int i = (int)fib.size() - 1; i >= 0; --i) {
        if (fib[i] <= remainder) {
            bits.push_back('1');
            remainder = remainder - fib[i];
        } else {
            bits.push_back('0');
        }
    }

    size_t firstOne = bits.find('1');
    if (firstOne == std::string::npos) {
        return "0";
    }
    bits = bits.substr(firstOne);

    return bits;
}

bool FibSystem::isValid(const std::string& str) {
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

std::string FibSystem::getName() {
    return "Fibonacci";
}
