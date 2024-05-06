#pragma once

#include <stdexcept>

struct OrderNotFound : public std::runtime_error
{
    using std::runtime_error::runtime_error;
};