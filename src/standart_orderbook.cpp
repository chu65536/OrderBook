#include <iostream>

#include "standart_orderbook.hpp"
#include "exceptions.hpp"
#include "id_manager.hpp"


StandartOrderbook::StandartOrderbook()
{
    auto buyingCmp = BuyingComparator{this};
    auto sellingCmp = SellingComparator{this};

    m_buying = std::set<uint64_t, BuyingComparator>(buyingCmp);
    m_selling = std::set<uint64_t, SellingComparator>(sellingCmp);
}

uint64_t StandartOrderbook::Add(const Order& order)
{   
    uint64_t id = IdManager::NextId();
    m_orders[id] = std::make_unique<Order>(order);
    if (order.type == Order::Type::BUY) m_buying.insert(id);
    else m_selling.insert(id);

    return id;
}

void StandartOrderbook::Erase(uint64_t id)
{
    checkIfExists(id);
    if (m_orders[id]->type == Order::Type::BUY) m_buying.erase(id);
    else m_selling.erase(id);
    m_orders.erase(id);
    IdManager::RemoveId(id);
}

Order& StandartOrderbook::operator[](uint64_t id)
{
    checkIfExists(id);
    return *m_orders[id];
}

std::vector<uint64_t> StandartOrderbook::GetBuyingOrdersIdSorted() const
{
    return std::vector(m_buying.cbegin(), m_buying.cend());
}

std::vector<uint64_t> StandartOrderbook::GetSellingOrdersIdSorted() const
{
    return std::vector(m_selling.cbegin(), m_selling.cend());
}

void StandartOrderbook::checkIfExists(uint64_t id) const
{
    if (!m_orders.contains(id))
    {   
        throw OrderNotFound(std::to_string(id));
    }
}

