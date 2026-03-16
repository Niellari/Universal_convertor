#include "FactSystem.h"
#include <sstream>
#include <stdexcept>
#include <vector>

BigInt FactSystem::factorial(unsigned long long n) const {
    BigInt result(1);
    for (unsigned long long i = 2; i <= n; ++i) {
        BigInt multiplier(i);
        result = result * multiplier;
    }
    return result;
}

BigInt FactSystem::fromString(const std::string& str) {
    if (str.empty()) {
        return BigInt();
    }

    std::istringstream stream(str);
    std::vector<unsigned long long> digits_;
    std::string chunk;

    while (stream >> chunk) {
        unsigned long long d = std::stoull(chunk);
        digits_.push_back(d);
    }

    size_t len = digits_.size();
    for (size_t i = 0; i < len; ++i) {
        size_t pos = len - 1 - i;
        if (digits_[i] > pos + 1) {
            throw std::invalid_argument("Слишком большая цифра");
        }
    }

    BigInt result;
    for (size_t i = 0; i < len; ++i) {
        size_t pos = len - 1 - i;
        BigInt f = factorial(pos + 1);
        BigInt multiplier(digits_[i]);
        result = result + f * multiplier;
    }

    return result;
}

std::string FactSystem::toString(BigInt value) {
    if (value.isZero()) {
        return "0";
    }

    std::vector<BigInt> factorials;
    factorials.push_back(BigInt(1));

    BigInt nextFact(1);
    unsigned long long n = 1;
    do {
        ++n;
        BigInt multiplier(n);
        nextFact = nextFact * multiplier;
        if (nextFact > value) {
            break;
        }
        factorials.push_back(nextFact);
    } while (true);

    std::vector<unsigned long long> digits_;
    BigInt remainder = value;

    for (int i = factorials.size() - 1; i >= 0; --i) {
        BigInt quotient, rem;
        remainder.divideAndRemainder(factorials[i], quotient, rem);
        BigInt maxDigit(i + 1);
        if (quotient > maxDigit) {
            quotient = maxDigit;
            BigInt product = factorials[i] * maxDigit;
            rem = remainder - product;
        }
        digits_.push_back(quotient.getDigits()[0]);
        remainder = rem;
    }

    std::reverse(digits_.begin(), digits_.end());

    std::ostringstream stream;
    for (size_t i = 0; i < digits_.size(); ++i) {
        if (i > 0) {
            stream << " ";
        }
        stream << digits_[i];
    }

    return stream.str();
}

bool FactSystem::isValid(const std::string& str) {
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

std::string FactSystem::getName() {
    return "Factorial";
}
