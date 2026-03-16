#ifndef ISYSTEM_H
#define ISYSTEM_H

#include "BigInt.h"

class ISystem {
public:
    virtual ~ISystem() = default;

    virtual BigInt fromString(const std::string& str) = 0;
    virtual std::string toString(BigInt value) = 0;
    virtual bool isValid(const std::string& str) = 0;
    virtual std::string getName() = 0;
};

#endif
