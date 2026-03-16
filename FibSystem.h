#ifndef FIBSYSTEM_H
#define FIBSYSTEM_H

#include "ISystem.h"

class FibSystem : public ISystem {
public:
    FibSystem() = default;

    BigInt fromString(const std::string& str) override;
    std::string toString(BigInt value) override;
    bool isValid(const std::string& str) override;
    std::string getName() override;

private:
    std::vector<BigInt> generateFibonacci(BigInt limit) const;
    bool hasOnesSubstr(const std::string& bits) const;
};

#endif
