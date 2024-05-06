#pragma once

#include <iostream>
#include <cstdint>

struct Order
{
    friend std::ostream& operator<<(std::ostream& os, const Order& order);
    std::string ToString() const;

    enum class Type { BUY, SELL };

    Type type;
    double price;
    uint64_t quantity;
};