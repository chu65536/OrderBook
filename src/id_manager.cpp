#include "id_manager.hpp"

#include <algorithm>

IdManager& IdManager::GetInstance()
{
    static IdManager instance;
    return instance;
}

uint64_t IdManager::NextId()
{
    return GetInstance().NextIdImpl();
}

void IdManager::RemoveId(uint64_t id)
{
    GetInstance().RemoveIdImpl(id);
}

const std::list<uint64_t>* IdManager::GetIds()
{
    return GetInstance().GetIdsImpl();
}

uint64_t IdManager::NextIdImpl()
{
    uint64_t result = m_counter++;
    if (!m_position_queue.empty())
    {
        m_counter = m_position_queue.front();
        m_position_queue.pop();
    }
    ids.push_back(result);
    return result;
}

void IdManager::RemoveIdImpl(uint64_t id)
{
    m_position_queue.push(m_counter);
    m_counter = id;
    ids.erase(std::find(ids.begin(), ids.end(), id));
}

const std::list<uint64_t>* IdManager::GetIdsImpl()
{
    return &ids;
}