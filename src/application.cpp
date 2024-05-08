#include "application.hpp"
#include "main_frame.hpp"
#include "standart_orderbook.hpp"
#include "id_manager.hpp"

#include <memory>

wxIMPLEMENT_APP(Application);    

bool Application::OnInit()
{
    srand(time(0));

    MainFrame* main_frame = new MainFrame("Order book");
    main_frame->Show();
    return true;
}