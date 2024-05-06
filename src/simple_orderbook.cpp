#include <iostream>

#include "simple_orderbook.hpp"
#include "exceptions.hpp"
#include "id_manager.hpp"

uint64_t SimpleOrderBook::Add(const Order& order)
{   
    uint64_t id = IdManager::NextId();
    m_orders[id] = std::make_shared<Order>(order);
    if (order.type == Order::Type::BUY) m_buying.insert(m_orders[id]);
    else m_selling.insert(m_orders[id]);
    return id;
}

void SimpleOrderBook::Erase(uint64_t id)
{
    checkIfExists(id);
    if (m_orders[id]->type == Order::Type::BUY) m_buying.erase(m_orders[id]);
    else m_selling.erase(m_orders[id]);
    m_orders.erase(id);
    IdManager::RemoveId(id);
}

Order& SimpleOrderBook::operator[](uint64_t id)
{
    checkIfExists(id);
    return *m_orders[id];
}

void SimpleOrderBook::Show10Best() const
{
}

void SimpleOrderBook::checkIfExists(uint64_t id)
{
    if (!m_orders.contains(id))
    {   
        throw OrderNotFound(std::to_string(id));
    }
}

std::unordered_map<uint64_t, std::shared_ptr<Order>>::iterator SimpleOrderBook::begin()
{
    return m_orders.begin();
}

std::unordered_map<uint64_t, std::shared_ptr<Order>>::iterator SimpleOrderBook::end()
{
    return m_orders.end();
}

