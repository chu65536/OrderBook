#pragma once

#include <wx/wx.h>

#include <memory>

#include "iorderbook.hpp"


class Application : public wxApp
{
private:
    bool OnInit() override;

    void initOrderBook();

    std::shared_ptr<IOrderBook> m_pOrder_book;
};