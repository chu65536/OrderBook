#include <wx/wx.h>
#include <wx/spinctrl.h>

#include <chrono>
#include <ctime>

#include "main_frame.hpp"
#include "standart_orderbook.hpp"
#include "simple_orderbook.hpp"
#include "id_manager.hpp"
#include "exceptions.hpp"

MainFrame::MainFrame(const wxString& title) 
    :   wxFrame(nullptr, wxID_ANY, title)
{
    m_pOrder_book = std::make_unique<StandartOrderbook>();

    wxPanel* panel = new wxPanel(this);
    makeListViews(panel);

    // right side panel (titles and tables)
    auto buyTitle = new wxStaticText(panel, wxID_ANY, "Buying");
    auto sellTitle = new wxStaticText(panel, wxID_ANY, "Selling");
    auto gridSizer = new wxFlexGridSizer(2, 2, 0, 0);
    gridSizer->AddGrowableCol(0);
    gridSizer->AddGrowableCol(1);
    gridSizer->AddGrowableRow(1);
    gridSizer->Add(buyTitle, 1, wxALIGN_LEFT | wxLEFT, 10);
    gridSizer->Add(sellTitle, 1, wxALIGN_LEFT | wxLEFT, 10);
    gridSizer->Add(m_pBuyingListView, 1, wxEXPAND);
    gridSizer->Add(m_pSellingListView, 1, wxEXPAND);

    // controls
    wxArrayString engines;
    engines.Add("Standart");
    engines.Add("Simple");
    auto engineSelector = new wxRadioBox(panel, wxID_ANY, "Select engine", wxDefaultPosition, wxDefaultSize, engines);
    engineSelector->SetSelection(0); // Standart is default
    engineSelector->Bind(wxEVT_RADIOBOX, &MainFrame::onEngineChange, this);

    auto addRandomButton = new wxButton(panel, wxID_ANY, "Add random orders");
    addRandomButton->Bind(wxEVT_BUTTON, &MainFrame::onAddRandomButton, this);
    auto randomNumber = new wxSpinCtrl(panel, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, 0, 1, 100000);
    randomNumber->Bind(wxEVT_SPINCTRL,&MainFrame::onNumberChange, this);

    auto clearButton = new wxButton(panel, wxID_ANY, "Clear orders");
    clearButton->Bind(wxEVT_BUTTON, &MainFrame::onClearButton, this);

    auto editByIdButton = new wxButton(panel, wxID_ANY, "Edit");
    editByIdButton->Bind(wxEVT_BUTTON, &MainFrame::onEditByIdButton, this);
    wxArrayString orderTypes; orderTypes.Add("Buy"); orderTypes.Add("Sell");
    auto idTextCtrl = new wxTextCtrl(panel, wxID_ANY, "id", wxDefaultPosition, wxSize(50,-1));
    idTextCtrl->Bind(wxEVT_TEXT, &MainFrame::onIdChange, this);
    auto orderTypeChoice = new wxChoice(panel, wxID_ANY, wxDefaultPosition, wxDefaultSize, orderTypes);
    orderTypeChoice->SetSelection(0);
    orderTypeChoice->Bind(wxEVT_CHOICE, &MainFrame::onTypeChange, this);
    auto priceCtrl = new wxSpinCtrlDouble(panel, wxID_ANY, "price", wxDefaultPosition, wxDefaultSize, 0, 0.0, 1000.0);
    priceCtrl->SetDigits(2);
    priceCtrl->Bind(wxEVT_SPINCTRLDOUBLE, &MainFrame::onPriceChange, this);
    auto quantityCtrl = new wxSpinCtrl(panel, wxID_ANY, "quantity", wxDefaultPosition, wxDefaultSize, 0, 0, 100);
    quantityCtrl->Bind(wxEVT_SPINCTRL, &MainFrame::onQuantityChange, this);

    m_logBox = new wxTextCtrl(panel, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE);
    m_logBox->SetEditable(false);

    // layouts
    auto generateHSizer = new wxBoxSizer(wxHORIZONTAL);
    generateHSizer->Add(addRandomButton, 1, wxEXPAND);
    generateHSizer->Add(randomNumber, 1, wxEXPAND);

    auto editHSizer = new wxBoxSizer(wxHORIZONTAL);
    editHSizer->Add(editByIdButton, 1, wxEXPAND);
    editHSizer->Add(idTextCtrl, 1, wxEXPAND);
    editHSizer->Add(orderTypeChoice, 1, wxEXPAND);
    editHSizer->Add(priceCtrl, 1, wxEXPAND);
    editHSizer->Add(quantityCtrl, 1, wxEXPAND);

    auto leftVSizer = new wxBoxSizer(wxVERTICAL);
    leftVSizer->Add(engineSelector, 0, wxEXPAND);
    leftVSizer->Add(generateHSizer, 0, wxEXPAND);
    leftVSizer->Add(editHSizer, 0, wxEXPAND);
    leftVSizer->Add(clearButton, 0, wxEXPAND);
    leftVSizer->Add(m_logBox, 1, wxEXPAND);

    auto mainSizer = new wxBoxSizer(wxHORIZONTAL);
    mainSizer->Add(leftVSizer, 1, wxEXPAND);
    mainSizer->Add(gridSizer, 1, wxEXPAND);
    mainSizer->SetMinSize(wxDefaultSize.GetWidth(), 500);
    
    panel->SetSizer(mainSizer);
    mainSizer->SetSizeHints(this);
} 

void MainFrame::makeListViews(wxWindow* parent)
{
    m_pBuyingListView = new wxListView(parent);
    m_pSellingListView = new wxListView(parent);

    m_pBuyingListView->AppendColumn("Id");
    m_pBuyingListView->AppendColumn("Price");
    m_pBuyingListView->AppendColumn("Quantity");
    m_pSellingListView->AppendColumn("Id");
    m_pSellingListView->AppendColumn("Price");
    m_pSellingListView->AppendColumn("Quantity");
}

void MainFrame::updateListViews()
{
    m_pBuyingListView->DeleteAllItems();
    m_pSellingListView->DeleteAllItems();

    auto start = std::chrono::high_resolution_clock::now();

    auto buying = m_pOrder_book->GetBuyingOrdersIdSorted();
    auto selling = m_pOrder_book->GetSellingOrdersIdSorted();

    auto stop = std::chrono::high_resolution_clock::now();
    std::chrono::duration<float> duration = stop - start;
    log("Sorted data", duration.count());

    int item = 0;
    for (auto&& id: buying)
    {
        m_pBuyingListView->InsertItem(item, std::to_string(id));
        m_pBuyingListView->SetItem(item, 1, std::to_string((*m_pOrder_book)[id].price));
        m_pBuyingListView->SetItem(item, 2, std::to_string((*m_pOrder_book)[id].quantity));
        item++;
    }
    item = 0;
    for (auto&& id: selling)
    {
        m_pSellingListView->InsertItem(item, std::to_string(id));
        m_pSellingListView->SetItem(item, 1, std::to_string((*m_pOrder_book)[id].price));
        m_pSellingListView->SetItem(item, 2, std::to_string((*m_pOrder_book)[id].quantity));
        item++;
    }

    log("List views updated");
}

void MainFrame::transferData()
{
    
    auto ids = IdManager::GetIds();
    std::vector<uint64_t> vIds(ids->cbegin(), ids->cend());

    auto start = std::chrono::high_resolution_clock::now();
    if(m_engine == Engines::STANDART)
    {
        auto new_orderbook = std::make_unique<StandartOrderbook>();
        for (auto&& id: vIds)
        {
            auto tmp = (*m_pOrder_book)[id];
            m_pOrder_book->Erase(id);
            new_orderbook->Add(tmp);
        }
        m_pOrder_book = std::move(new_orderbook);
    }
    else if(m_engine == Engines::SIMPLE)
    {
        auto new_orderbook = std::make_unique<SimpleOrderbook>();
        for (auto&& id: vIds)
        {
            auto tmp = (*m_pOrder_book)[id];
            m_pOrder_book->Erase(id);
            new_orderbook->Add(tmp);
        }
        m_pOrder_book = std::move(new_orderbook);
    }
    auto stop = std::chrono::high_resolution_clock::now();
    std::chrono::duration<float> duration = stop - start;

    log("Data transfered", duration.count());

    updateListViews();
}

void MainFrame::log(const std::string& msg, float time)
{   
    std::time_t time_t = std::time(0);
    auto ltm = std::localtime(&time_t);
    std::string sH = std::to_string(ltm->tm_hour); if (sH.size() == 1) sH = '0' + sH;
    std::string sM = std::to_string(ltm->tm_min); if (sM.size() == 1) sM = '0' + sM;
    std::string sS = std::to_string(ltm->tm_sec); if (sS.size() == 1) sS = '0' + sS;
    std::string sTime = '[' + sH + ':' + sM + ':' + sS + "] ";

    std::string sEngine = (m_engine == Engines::SIMPLE ? "Simple" : "Standart");
    std::string logLine = sTime + sEngine + ", " + msg + ", " + std::to_string(time) + '\n';
    m_logBox->AppendText(logLine); 
}

void MainFrame::onEngineChange(wxCommandEvent& evt)
{
    std::string sEngine = evt.GetString().ToStdString();
    if (sEngine == "Standart") m_engine = Engines::STANDART;
    else if (sEngine == "Simple") m_engine = Engines::SIMPLE;
    log("Engine changed");

    transferData();
} 

void MainFrame::onAddRandomButton(wxCommandEvent& evt)
{   
    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < m_ordersNumber; ++i)
    {   
        bool bType = rand() % 2;
        Order::Type type = bType ? Order::Type::BUY : Order::Type::SELL;
        double price = (50000.0 + rand() % 10001) / 100.0;
        uint64_t quantity = rand() % 91 + 10;
        Order order{type, price, quantity};

        m_pOrder_book->Add(order);
    }
    auto stop = std::chrono::high_resolution_clock::now();
    std::chrono::duration<float> duration = stop - start;
    log("Added random orders", duration.count());

    updateListViews();
}

void MainFrame::onClearButton(wxCommandEvent& evt)
{
    auto ids = IdManager::GetIds();
    std::vector<uint64_t> vIds(ids->cbegin(), ids->cend());

    auto start = std::chrono::high_resolution_clock::now();
    for (auto&& id: vIds)
    {
        m_pOrder_book->Erase(id);
    }
    auto stop = std::chrono::high_resolution_clock::now();
    std::chrono::duration<float> duration = stop - start;

    log("Cleared", duration.count());

    updateListViews();
}

void MainFrame::onNumberChange(wxCommandEvent& evt)
{
    m_ordersNumber = evt.GetInt();
}

void MainFrame::onEditByIdButton(wxCommandEvent& evt)
{
    try
    {
        m_pOrder_book->Erase(m_order_edit_id);
        m_pOrder_book->Add(m_order_edit);
        log("Order successfully edited");
        updateListViews();
    }
    catch(const OrderNotFound& ex)
    {   
        log("Order not found");
    }
    catch(const std::exception& ex)
    {
        log(ex.what());
    }
}

void MainFrame::onIdChange(wxCommandEvent& evt)
{
    try
    {
        m_order_edit_id = std::stoull(evt.GetString().ToStdString());
    }
    catch(const std::exception& ex) {}
}

void MainFrame::onTypeChange(wxCommandEvent& evt)
{
    m_order_edit.type = (evt.GetString().ToStdString() == "Buy" ? Order::Type::BUY : Order::Type::SELL);
}

void MainFrame::onPriceChange(wxCommandEvent& evt)
{
    m_order_edit.price = std::stod(evt.GetString().ToStdString()); 
}

void MainFrame::onQuantityChange(wxCommandEvent& evt)
{
    m_order_edit.quantity = evt.GetInt();
}