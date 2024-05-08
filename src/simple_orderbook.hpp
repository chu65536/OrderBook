#pragma once

#include <unordered_map>
#include <memory>
#include <set>
#include <vector>

#include "iorderbook.hpp"


class SimpleOrderbook : public IOrderBook
{
public:
    SimpleOrderbook();
    SimpleOrderbook(std::vector<Order>&& orders);

    uint64_t Add(const Order& order) override;
    void Erase(uint64_t id) override;
    Order& operator[](uint64_t id) override;
    std::vector<uint64_t> GetBuyingOrdersIdSorted() const override;
    std::vector<uint64_t> GetSellingOrdersIdSorted() const override;

private:
    std::unordered_map<uint64_t, Order> m_orders;
};