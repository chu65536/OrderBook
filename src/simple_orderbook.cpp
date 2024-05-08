#include <ranges>
#include <algorithm>

#include "simple_orderbook.hpp"
#include "id_manager.hpp"
#include "exceptions.hpp"

SimpleOrderbook::SimpleOrderbook() {}

uint64_t SimpleOrderbook::Add(const Order& order)
{
    uint64_t id = IdManager::NextId();
    m_orders[id] = order;
    return id;
}

void SimpleOrderbook::Erase(uint64_t id)
{
    if (m_orders.find(id) == m_orders.end()) throw OrderNotFound(std::to_string(id));
    m_orders.erase(id);
    IdManager::RemoveId(id);
}

Order& SimpleOrderbook::operator[](uint64_t id)
{
    if (m_orders.find(id) == m_orders.end()) throw OrderNotFound(std::to_string(id));
    return m_orders[id];
}

std::vector<uint64_t> SimpleOrderbook::GetBuyingOrdersIdSorted() const
{
    std::vector<std::pair<uint64_t, Order>> buying;
    for (const auto& [id, order]: m_orders)
    {
        if (order.type == Order::Type::BUY) buying.emplace_back(id, order);
    }
    
    using P = std::pair<uint64_t, Order>;
    auto cmp = [](const P& lhs, const P& rhs) 
    {
        return lhs.second.price > rhs.second.price;
    };

    std::ranges::sort(buying, cmp);
    std::vector<uint64_t> res;
    res.reserve(buying.size());
    for (const auto& p: buying) res.emplace_back(p.first);
    return res;
}

std::vector<uint64_t> SimpleOrderbook::GetSellingOrdersIdSorted() const
{
    std::vector<std::pair<uint64_t, Order>> selling;
    for (const auto& [id, order]: m_orders)
    {
        if (order.type == Order::Type::SELL) selling.emplace_back(id, order);
    }
    
    using P = std::pair<uint64_t, Order>;
    auto cmp = [](const P& lhs, const P& rhs) 
    {
        return lhs.second.price < rhs.second.price;
    };

    std::ranges::sort(selling, cmp);
    std::vector<uint64_t> res;
    res.reserve(selling.size());
    for (const auto& p: selling) res.emplace_back(p.first);
    return res;
}