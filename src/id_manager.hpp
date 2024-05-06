#pragma once

#include <string>
#include <cstdint>
#include <queue>
#include <list>

class IdManager
{
public:
    IdManager(const IdManager&) = delete;
    static IdManager& GetInstance();

    static uint64_t NextId();
    static void RemoveId(uint64_t id);
    static const std::list<uint64_t>* GetIds();
private:
    IdManager() {}

    uint64_t NextIdImpl();
    void RemoveIdImpl(uint64_t id);
    const std::list<uint64_t>* GetIdsImpl();

    int64_t m_counter{0};
    std::queue<uint64_t> m_position_queue;
    std::list<uint64_t> ids;
};
