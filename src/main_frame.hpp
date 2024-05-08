#pragma once

#include <wx/wx.h>
#include <wx/listctrl.h>

#include <memory>

#include "iorderbook.hpp"

enum class Engines
{
    STANDART,
    SIMPLE
};

class MainFrame : public wxFrame
{
public:
    MainFrame(const wxString& title);
private:
    void makeListViews(wxWindow* parent);
    void updateListViews();
    void transferData();
    void log(const std::string& msg, float time = 0.f);

    void onEngineChange(wxCommandEvent& evt);
    void onAddRandomButton(wxCommandEvent& evt);
    void onClearButton(wxCommandEvent& evt);
    void onNumberChange(wxCommandEvent& evt);
    void onEditByIdButton(wxCommandEvent& evt);
    void onIdChange(wxCommandEvent& evt);
    void onTypeChange(wxCommandEvent& evt);
    void onPriceChange(wxCommandEvent& evt);
    void onQuantityChange(wxCommandEvent& evt);

    wxListView* m_pBuyingListView;
    wxListView* m_pSellingListView;
    wxTextCtrl* m_logBox;

    std::unique_ptr<IOrderBook> m_pOrder_book;
    Engines m_engine{Engines::STANDART};
    unsigned int m_ordersNumber{1};
    uint64_t m_order_edit_id{-1ull};
    Order m_order_edit{Order::Type::BUY, 0.0, 0};
};