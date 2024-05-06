#include "application.hpp"
#include "main_frame.hpp"
#include "simple_orderbook.hpp"
#include "id_manager.hpp"

#include <memory>

wxIMPLEMENT_APP(Application);

void Application::initOrderBook()
{
    srand(time(0));
    SimpleOrderBook result;
    const int n = 100;

    for (int i = 0; i < n; ++i)
    {   
        bool bType = rand() % 2;
        Order::Type type = bType ? Order::Type::BUY : Order::Type::SELL;
        double price = (50000.0 + rand() % 10001) / 100.0;
        uint64_t quantity = rand() % 91 + 10;
        Order order{type, price, quantity};

        result.Add(order);
    }

    m_pOrder_book = std::make_shared<SimpleOrderBook>(result);
}      

bool Application::OnInit()
{
    initOrderBook();

    MainFrame* main_frame = new MainFrame("Order book", m_pOrder_book);
    main_frame->Show();
    return true;
}