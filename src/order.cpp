#include "order.hpp"

std::ostream& operator<<(std::ostream& os, const Order& order) 
{
    return os << order.ToString();
}

std::string Order::ToString() const
{
    std::string sType;
    if (type == Order::Type::BUY) sType = "Buy";
    else sType = "Sell";

    std::string sep = " | ";
    std::string result{};
    result += "Type: " + sType;
    result += sep + "Price: " + std::to_string(price);
    result += sep + "Quantity: " + std::to_string(quantity);

    return result;
}