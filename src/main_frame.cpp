#include <wx/wx.h>

#include "main_frame.hpp"
#include "simple_orderbook.hpp"
#include "id_manager.hpp"

MainFrame::MainFrame(const wxString& title, std::shared_ptr<IOrderBook> pOrder_book) 
    : wxFrame(nullptr, wxID_ANY, title), m_pOrder_book(pOrder_book)
{
    wxPanel* panel = new wxPanel(this);
    m_pBuyingListView = new wxListView(panel);
    m_pSellingListView = new wxListView(panel);

    m_pBuyingListView->AppendColumn("Id");
    m_pBuyingListView->AppendColumn("Price");
    m_pBuyingListView->AppendColumn("Quantity");
    m_pSellingListView->AppendColumn("Id");
    m_pSellingListView->AppendColumn("Price");
    m_pSellingListView->AppendColumn("Quantity");

    populateListViews();

    auto buyTitle = new wxStaticText(panel, wxID_ANY, "Buying");
    auto sellTitle = new wxStaticText(panel, wxID_ANY, "Selling");
    auto addBtn = new wxButton(panel, wxID_ANY, "Add");
    auto remBtn = new wxButton(panel, wxID_ANY, "Erase");

    auto gridSizer = new wxFlexGridSizer(2, 2, 0, 0);
    gridSizer->AddGrowableCol(0);
    gridSizer->AddGrowableCol(1);
    gridSizer->AddGrowableRow(1);

    gridSizer->Add(buyTitle, 1, wxALIGN_LEFT | wxLEFT, 10);
    gridSizer->Add(sellTitle, 1, wxALIGN_LEFT | wxLEFT, 10);
    gridSizer->Add(m_pBuyingListView, 1, wxEXPAND);
    gridSizer->Add(m_pSellingListView, 1, wxEXPAND);

    auto leftVSizer = new wxBoxSizer(wxVERTICAL);
    

    auto mainSizer = new wxBoxSizer(wxHORIZONTAL);
    mainSizer->Add(leftVSizer, 1, wxEXPAND);
    mainSizer->Add(gridSizer, 1, wxEXPAND);
  
    panel->SetSizerAndFit(mainSizer);
} 

void MainFrame::populateListViews()
{
    auto pIds = IdManager::GetIds();
    
    int item = 0;
    for (auto&& id: *pIds)
    {
        if ((*m_pOrder_book)[id].type == Order::Type::BUY)
        {
            m_pBuyingListView->InsertItem(item, std::to_string(id));
            m_pBuyingListView->SetItem(item, 1, std::to_string((*m_pOrder_book)[id].price));
            m_pBuyingListView->SetItem(item, 2, std::to_string((*m_pOrder_book)[id].quantity));
        }
        else
        {
            m_pSellingListView->InsertItem(item, std::to_string(id));
            m_pSellingListView->SetItem(item, 1, std::to_string((*m_pOrder_book)[id].price));
            m_pSellingListView->SetItem(item, 2, std::to_string((*m_pOrder_book)[id].quantity));
        }
    }
    item++;
}
