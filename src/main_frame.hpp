#pragma once

#include <wx/wx.h>
#include <wx/listctrl.h>

#include <memory>

#include "iorderbook.hpp"

class MainFrame : public wxFrame
{
public:
    MainFrame(const wxString& title, std::shared_ptr<IOrderBook> order_book);
private:
    void populateListViews();

    wxListView* m_pBuyingListView;
    wxListView* m_pSellingListView;

    std::shared_ptr<IOrderBook> m_pOrder_book;
};