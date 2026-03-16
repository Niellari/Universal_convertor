#ifndef FACTSYSTEM_H
#define FACTSYSTEM_H

#include "ISystem.h"

class FactSystem : public ISystem {
public:
    FactSystem() = default;

    BigInt fromString(const std::string& str) override;
    std::string toString(BigInt value) override;
    bool isValid(const std::string& str) override;
    std::string getName() override;

private:
    BigInt factorial(unsigned long long n) const;
};

#endif
