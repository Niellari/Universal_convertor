#ifndef TERSYSTEM_H
#define TERSYSTEM_H

#include "ISystem.h"

class TerSystem : public ISystem {
public:
    TerSystem() = default;

    BigInt fromString(const std::string& str) override;
    std::string toString(BigInt value) override;
    bool isValid(const std::string& str) override;
    std::string getName() override;

private:
    int parseDigit(char c) const;
    char digitToChar(int digit) const;
};

#endif
