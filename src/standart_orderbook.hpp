#pragma once

#include <unordered_map>
#include <memory>
#include <set>
#include <vector>

#include "iorderbook.hpp"


class StandartOrderbook : public IOrderBook
{
public:
    struct BuyingComparator
    {
        bool operator()(uint64_t lhs, uint64_t rhs) const
        {
            if ((*ob)[lhs].price != (*ob)[rhs].price)
                return (*ob)[lhs].price > (*ob)[rhs].price;
            if ((*ob)[lhs].quantity != (*ob)[rhs].quantity)
                return (*ob)[lhs].quantity > (*ob)[rhs].quantity;
            return lhs > rhs;
        }

        StandartOrderbook* ob;
    };

    struct SellingComparator
    {
        bool operator()(uint64_t lhs, uint64_t rhs) const
        {
            if ((*ob)[lhs].price != (*ob)[rhs].price)
                return (*ob)[lhs].price < (*ob)[rhs].price;
            if ((*ob)[lhs].quantity != (*ob)[rhs].quantity)
                return (*ob)[lhs].quantity > (*ob)[rhs].quantity;
            return lhs > rhs;
        }

        StandartOrderbook* ob;
    };
    using Orders = std::unordered_map<uint64_t, std::unique_ptr<Order>>;
    using BuyingSet = std::set<uint64_t, BuyingComparator>;
    using SellingSet = std::set<uint64_t, SellingComparator>;

    StandartOrderbook();

    uint64_t Add(const Order& order) override;
    void Erase(uint64_t id) override;
    Order& operator[](uint64_t id) override;
    std::vector<uint64_t> GetBuyingOrdersIdSorted() const override;
    std::vector<uint64_t> GetSellingOrdersIdSorted() const override;
private:
    void checkIfExists(uint64_t id) const;

    Orders m_orders;
    BuyingSet m_buying;
    SellingSet m_selling;
};