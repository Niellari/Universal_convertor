#include "BigInt.h"
#include <sstream>
#include <iomanip>

BigInt::BigInt() : digits(1, 0) {}

BigInt::BigInt(unsigned long long value) {
    if (value == 0) {
        digits = std::vector<unsigned long long>(1, 0);
    } else {
        digits.push_back(value);
    }
}

BigInt::BigInt(const std::string& decimalString) {
    if (decimalString.empty() || decimalString == "0") {
        digits = std::vector<unsigned long long>(1, 0);
        return;
    }

    digits = std::vector<unsigned long long>(1, 0);

    for (char c : decimalString) {
        if (c < '0' || c > '9') {
            throw std::invalid_argument("Invalid digit");
        }

        unsigned long long digit = c - '0';

        unsigned long long carry = 0;
        for (int i = 0; i < digits.size(); ++i) {
            unsigned long long product = digits[i] * 10 + carry;
            digits[i] = product % BASE;
            carry = product / BASE;
        }
        if (carry > 0) {
            digits.push_back(carry);
        }

        unsigned long long sum = digits[0] + digit;
        digits[0] = sum % BASE;
        carry = sum / BASE;
        int i = 1;
        while (carry > 0 && i < digits.size()) {
            sum = digits[i] + carry;
            digits[i] = sum % BASE;
            carry = sum / BASE;
            ++i;
        }
        if (carry > 0) {
            digits.push_back(carry);
        }
    }

    normalize();
}

BigInt BigInt::operator+(const BigInt& other) const {
    BigInt result;
    result.digits.clear();

    unsigned long long carry = 0;
    int maxSize = std::max(digits.size(), other.digits.size());

    for (int i = 0; i < maxSize || carry; ++i) {
        unsigned long long sum = carry;
        if (i < digits.size()) {
            sum += digits[i];
        }
        if (i < other.digits.size()) {
            sum += other.digits[i];
        }

        result.digits.push_back((sum % BASE));
        carry = sum / BASE;
    }

    return result;
}

BigInt BigInt::operator-(const BigInt& other) const {
    if (*this < other) {
        throw std::underflow_error("Result is negative");
    }

    BigInt result;
    result.digits.clear();

    unsigned long long borrow = 0;
    for (int i = 0; i < digits.size(); ++i) {
        unsigned long long diff = digits[i] - borrow;
        if (i < other.digits.size()) {
            diff -= other.digits[i];
        }

        if (diff > digits[i]) {
            diff += BASE;
            borrow = 1;
        } else {
            borrow = 0;
        }

        result.digits.push_back(diff);
    }

    result.normalize();
    return result;
}

std::vector<unsigned long long> BigInt::add(const std::vector<unsigned long long>& a, const std::vector<unsigned long long>& b) {
    std::vector<unsigned long long> result;
    unsigned long long carry = 0;
    int maxSize = std::max(a.size(), b.size());

    for (int i = 0; i < maxSize || carry; ++i) {
        unsigned long long sum = carry;
        if (i < a.size()) {
            sum += a[i];
        }
        if (i < b.size()) {
            sum += b[i];
        }

        result.push_back(sum % BASE);
        carry = sum / BASE;
    }

    return result;
}

std::vector<unsigned long long> BigInt::subtract(const std::vector<unsigned long long>& a, const std::vector<unsigned long long>& b) {
    std::vector<unsigned long long> result;
    unsigned long long borrow = 0;

    for (int i = 0; i < a.size(); ++i) {
        unsigned long long diff = a[i] - borrow;
        if (i < b.size()) {
            diff -= b[i];
        }

        if (diff > a[i]) {
            diff += BASE;
            borrow = 1;
        } else {
            borrow = 0;
        }

        result.push_back(diff);
    }

    while (result.size() > 1 && result.back() == 0) {
        result.pop_back();
    }

    return result;
}

std::vector<unsigned long long> BigInt::karatsuba(const std::vector<unsigned long long>& a, const std::vector<unsigned long long>& b) {
    if (a.empty() || b.empty()) {
        return std::vector<unsigned long long>(1, 0);
    }

    if (a.size() == 1 && b.size() == 1) {
        unsigned long long product = a[0] * b[0];
        std::vector<unsigned long long> result;
        result.push_back(product % BASE);
        unsigned long long high = product / BASE;
        if (high > 0) {
            result.push_back(high);
        }
        return result;
    }

    if (a.size() > b.size()) {
        return karatsuba(b, a);
    }

    if (a.size() < b.size()) {
        std::vector<unsigned long long> aExtended = a;
        aExtended.resize(b.size(), 0);
        return karatsuba(aExtended, b);
    }

    int n = a.size();
    if (n == 1) {
        unsigned long long product = a[0] * b[0];
        std::vector<unsigned long long> result;
        result.push_back(product % BASE);
        unsigned long long high = product / BASE;
        if (high > 0) {
            result.push_back(high);
        }
        return result;
    }

    int half = (n + 1) / 2;

    std::vector<unsigned long long> aLow(a.begin(), a.begin() + std::min(half, n));
    std::vector<unsigned long long> aHigh(a.begin() + std::min(half, n), a.end());
    std::vector<unsigned long long> bLow(b.begin(), b.begin() + std::min(half, n));
    std::vector<unsigned long long> bHigh(b.begin() + std::min(half, n), b.end());

    while (aHigh.size() > 1 && aHigh.back() == 0) {
        aHigh.pop_back();
    }
    while (bHigh.size() > 1 && bHigh.back() == 0) {
        bHigh.pop_back();
    }

    std::vector<unsigned long long> z0 = karatsuba(aLow, bLow);
    std::vector<unsigned long long> z2 = karatsuba(aHigh, bHigh);

    std::vector<unsigned long long> aSum = add(aLow, aHigh);
    std::vector<unsigned long long> bSum = add(bLow, bHigh);
    std::vector<unsigned long long> z1 = karatsuba(aSum, bSum);
    z1 = subtract(z1, z0);
    z1 = subtract(z1, z2);

    std::vector<unsigned long long> result(z0);

    if (!z1.empty() && !(z1.size() == 1 && z1[0] == 0)) {
        if (result.size() < half + z1.size()) {
            result.resize(half + z1.size(), 0);
        }
        unsigned long long carry = 0;
        for (int i = 0; i < z1.size() || carry; ++i) {
            unsigned long long sum = carry;
            if (i < z1.size()) {
                sum += z1[i];
            }
            if (half + i < result.size()) {
                sum += result[half + i];
            }
            if (half + i < result.size()) {
                result[half + i] = sum % BASE;
            } else {
                result.push_back(sum % BASE);
            }
            carry = sum / BASE;
        }
    }

    if (!z2.empty() && !(z2.size() == 1 && z2[0] == 0)) {
        int shift = 2 * half;
        if (result.size() < shift + z2.size()) {
            result.resize(shift + z2.size(), 0);
        }
        unsigned long long carry = 0;
        for (int i = 0; i < z2.size() || carry; ++i) {
            unsigned long long sum = carry;
            if (i < z2.size()) {
                sum += z2[i];
            }
            if (shift + i < result.size()) {
                sum += result[shift + i];
            }

            if (shift + i < result.size()) {
                result[shift + i] = sum % BASE;
            } else {
                result.push_back(sum % BASE);
            }
            carry = sum / BASE;
        }
    }

    while (result.size() > 1 && result.back() == 0) {
        result.pop_back();
    }

    return result;
}

BigInt BigInt::operator*(const BigInt& other) const {
    if (isZero() || other.isZero()) {
        return BigInt();
    }

    BigInt result;
    result.digits = karatsuba(digits, other.digits);
    return result;
}

void BigInt::divideAndRemainder(const BigInt& divisor, BigInt& quotient, BigInt& remainder) const {
    if (divisor.isZero()) {
        throw std::invalid_argument("Деление на НОЛЬ!!!");
    }
    if (*this < divisor) {
        quotient = BigInt();
        remainder = *this;
        return;
    }
    if (divisor == BigInt(1)) {
        quotient = *this;
        remainder = BigInt();
        return;
    }

    quotient.digits.clear();
    quotient.digits.resize(digits.size(), 0);
    remainder = BigInt();

    for (int i = (int)digits.size() - 1; i >= 0; --i) {
        if (!remainder.isZero()) {
            std::vector<unsigned long long> newRemainder;
            newRemainder.reserve(remainder.digits.size() + 1);
            unsigned long long carry = 0;
            for (int j = 0; j < remainder.digits.size(); ++j) {
                unsigned long long product = remainder.digits[j] * BASE + carry;
                newRemainder.push_back(product % BASE);
                carry = product / BASE;
            }
            if (carry > 0) {
                newRemainder.push_back(carry);
            }
            remainder.digits = std::move(newRemainder);
        }

        if (remainder.isZero()) {
            if (digits[i] != 0) {
                remainder.digits = std::vector<unsigned long long>(1, digits[i]);
            }
        } else {
            remainder.digits[0] += digits[i];
            unsigned long long carry = remainder.digits[0] / BASE;
            remainder.digits[0] %= BASE;
            int j = 1;
            while (carry > 0 && j < remainder.digits.size()) {
                unsigned long long sum = remainder.digits[j] + carry;
                remainder.digits[j] = sum % BASE;
                carry = sum / BASE;
                ++j;
            }
            if (carry > 0) {
                remainder.digits.push_back(carry);
            }
        }

        unsigned long long q = 0;
        unsigned long long low = 0, high = BASE - 1;

        while (low <= high) {
            unsigned long long mid = low + (high - low) / 2;
            BigInt midBig(mid);
            BigInt product = divisor * midBig;
            if (product <= remainder) {
                q = mid;
                low = mid + 1;
            } else {
                if (mid == 0) break;
                high = mid - 1;
            }
        }

        quotient.digits[i] = q;

        if (q > 0) {
            BigInt qBig(q);
            BigInt product = divisor * qBig;
            remainder = remainder - product;
        }
    }

    quotient.normalize();
    remainder.normalize();
}

BigInt BigInt::operator/(const BigInt& other) const {
    BigInt quotient, remainder;
    divideAndRemainder(other, quotient, remainder);
    return quotient;
}

BigInt BigInt::operator%(const BigInt& other) const {
    BigInt quotient, remainder;
    divideAndRemainder(other, quotient, remainder);
    return remainder;
}

BigInt& BigInt::operator+=(const BigInt& other) {
    *this = *this + other;
    return *this;
}

BigInt& BigInt::operator-=(const BigInt& other) {
    *this = *this - other;
    return *this;
}

BigInt& BigInt::operator*=(const BigInt& other) {
    *this = *this * other;
    return *this;
}

BigInt& BigInt::operator/=(const BigInt& other) {
    *this = *this / other;
    return *this;
}

BigInt& BigInt::operator%=(const BigInt& other) {
    *this = *this % other;
    return *this;
}

int BigInt::compare(const BigInt& other) const {
    if (digits.size() != other.digits.size()) {
        if(digits.size() < other.digits.size()){
            return -1;
        } else{
            return 1;
        }
    }
    for (int i = digits.size() - 1; i >= 0; --i) {
        if (digits[i] != other.digits[i]) {
            if(digits[i] < other.digits[i]){
                return -1;
            } else{
                return 1;
            }
        }
    }
    return 0;
}

bool BigInt::operator==(const BigInt& other) const {
    return compare(other) == 0;
}

bool BigInt::operator!=(const BigInt& other) const {
    return compare(other) != 0;
}

bool BigInt::operator<(const BigInt& other) const {
    return compare(other) < 0;
}

bool BigInt::operator>(const BigInt& other) const {
    return compare(other) > 0;
}

bool BigInt::operator<=(const BigInt& other) const {
    return compare(other) <= 0;
}

bool BigInt::operator>=(const BigInt& other) const {
    return compare(other) >= 0;
}

BigInt& BigInt::operator++() {
    BigInt one(1);
    *this += one;
    return *this;
}

BigInt BigInt::operator++(int) {
    BigInt tmp = *this;
    ++(*this);
    return tmp;
}

BigInt& BigInt::operator--() {
    BigInt one(1);
    *this -= one;
    return *this;
}

BigInt BigInt::operator--(int) {
    BigInt tmp = *this;
    --(*this);
    return tmp;
}

bool BigInt::isZero() const {
    return digits.size() == 1 && digits[0] == 0;
}

void BigInt::normalize() {
    while (digits.size() > 1 && digits.back() == 0) {
        digits.pop_back();
    }
    if (digits.empty()) {
        digits.push_back(0);
    }
}

std::string BigInt::toString() const {
    if (isZero()) {
        return "0";
    }

    BigInt temp = *this;
    std::vector<unsigned long long> chunks;

    unsigned long long chunkBase = 1e9;

    while (!temp.isZero()) {
        BigInt quotient, remainder;
        BigInt divisor(chunkBase);
        temp.divideAndRemainder(divisor, quotient, remainder);
        chunks.push_back(remainder.digits[0]);
        temp = quotient;
    }

    std::ostringstream stream;
    stream << chunks.back();

    for (int i = chunks.size() - 2; i >= 0; --i) {
        stream << std::setw(9) << std::setfill('0') << chunks[i];
    }

    return stream.str();
}

std::string BigInt::toStringWithBase(unsigned long long base) const {
    if (base < 2 || base > BASE) {
        throw std::invalid_argument("Некорректная база");
    }

    if (isZero()) {
        return "0";
    }

    BigInt temp = *this;
    std::vector<unsigned long long> digits_;

    while (!temp.isZero()) {
        BigInt quotient, remainder;
        BigInt divisor(base);
        temp.divideAndRemainder(divisor, quotient, remainder);
        digits_.push_back(remainder.digits[0]);
        temp = quotient;
    }

    std::ostringstream stream;
    for (auto it = digits_.rbegin(); it != digits_.rend(); ++it) {
        if (it != digits_.rbegin()) {
            stream << " ";
        }
        stream << *it;
    }

    return stream.str();
}

BigInt BigInt::fromString(const std::string& str, unsigned long long base) {
    if (base < 2 || base > BASE) {
        throw std::invalid_argument("Invalid base");
    }

    BigInt result;
    std::istringstream stream(str);
    std::vector<unsigned long long> digits_;
    std::string chunk;

    while (stream >> chunk) {
        unsigned long long d = std::stoull(chunk);
        if (d >= base) {
            throw std::invalid_argument("Digit >= base");
        }
        digits_.push_back(d);
    }

    for (auto it = digits_.rbegin(); it != digits_.rend(); ++it) {
        BigInt multiplier(base);
        BigInt addend(*it);
        result = result * multiplier + addend;
    }

    return result;
}
