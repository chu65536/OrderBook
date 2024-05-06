#pragma once

#include <string>

#include "order.hpp"

class IOrderBook
{
public:
    virtual uint64_t Add(const Order& order) = 0;
    virtual void Erase(uint64_t id) = 0;
    virtual Order& operator[](uint64_t id) = 0;
    virtual void Show10Best() const = 0;

    virtual ~IOrderBook() = default;
};