#pragma once

#include <unordered_map>
#include <memory>
#include <set>

#include "iorderbook.hpp"

struct BuyingComparator
{
    bool operator()(const std::shared_ptr<Order>& lhs, const std::shared_ptr<Order>& rhs) const
    {
        return lhs->price > rhs->price;
    }
};

struct SellingComparator
{
    bool operator()(const std::shared_ptr<Order>& lhs, const std::shared_ptr<Order>& rhs) const
    {
        return lhs->price < rhs->price;
    }
};

class SimpleOrderBook : public IOrderBook
{
public:
    uint64_t Add(const Order& order) override;
    void Erase(uint64_t id) override;
    virtual Order& operator[](uint64_t id) override;
    void Show10Best() const override;

    std::unordered_map<uint64_t, std::shared_ptr<Order>>::iterator begin();
    std::unordered_map<uint64_t, std::shared_ptr<Order>>::iterator end();
private:
    void checkIfExists(uint64_t id);

    std::unordered_map<uint64_t, std::shared_ptr<Order>> m_orders;
    std::set<std::shared_ptr<Order>, BuyingComparator> m_buying;
    std::set<std::shared_ptr<Order>, SellingComparator> m_selling;
};