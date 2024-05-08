#pragma once

#include <wx/wx.h>

#include <memory>

#include "iorderbook.hpp"


class Application : public wxApp
{
    bool OnInit() override;
};