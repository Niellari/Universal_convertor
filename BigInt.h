#ifndef BIGINT_H
#define BIGINT_H

#include "includes.h"

class BigInt {
public:
    BigInt();
    BigInt(unsigned long long value);
    BigInt(const std::string& decimalString);

    BigInt(const BigInt&) = default;
    BigInt(BigInt&&) = default;
    BigInt& operator=(const BigInt&) = default;
    BigInt& operator=(BigInt&&) = default;

    BigInt operator+(const BigInt& other) const;
    BigInt operator-(const BigInt& other) const;
    BigInt operator*(const BigInt& other) const;

    void divideAndRemainder(const BigInt& divisor, BigInt& quotient, BigInt& remainder) const;
    BigInt operator/(const BigInt& other) const;
    BigInt operator%(const BigInt& other) const;

    BigInt& operator+=(const BigInt& other);
    BigInt& operator-=(const BigInt& other);
    BigInt& operator*=(const BigInt& other);
    BigInt& operator/=(const BigInt& other);
    BigInt& operator%=(const BigInt& other);

    bool operator==(const BigInt& other) const;
    bool operator!=(const BigInt& other) const;
    bool operator<(const BigInt& other) const;
    bool operator>(const BigInt& other) const;
    bool operator<=(const BigInt& other) const;
    bool operator>=(const BigInt& other) const;

    BigInt& operator++();
    BigInt operator++(int);
    BigInt& operator--();
    BigInt operator--(int);

    bool isZero() const;
    std::string toString() const;
    std::string toStringWithBase(unsigned long long base) const;
    static BigInt fromString(const std::string& str, unsigned long long base);

    std::vector<unsigned long long>& getDigits() {
        return digits;
    }

    const std::vector<unsigned long long>& getDigits() const {
        return digits;
    }

    void normalize();

private:
    std::vector<unsigned long long> digits;
    int compare(const BigInt& other) const;

    static std::vector<unsigned long long> karatsuba(const std::vector<unsigned long long>& a, const std::vector<unsigned long long>& b);
    static std::vector<unsigned long long> add(const std::vector<unsigned long long>& a, const std::vector<unsigned long long>& b);
    static std::vector<unsigned long long> subtract(const std::vector<unsigned long long>& a, const std::vector<unsigned long long>& b);
};

#endif
