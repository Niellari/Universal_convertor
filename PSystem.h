#ifndef PSYSTEM_H
#define PSYSTEM_H

#include "ISystem.h"

class PSystem : public ISystem {
public:
    explicit PSystem(unsigned long long base);

    BigInt fromString(const std::string& str) override;
    std::string toString(BigInt value) override;
    bool isValid(const std::string& str) override;
    std::string getName() override;

    unsigned long long getBase() const {
        return base;
    }

private:
    unsigned long long base;

    unsigned long long parseDigit(const std::string& str) const;
};

#endif
