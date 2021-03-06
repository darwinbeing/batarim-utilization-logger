#ifndef GUARD_active_window_module_h
#define GUARD_active_window_module_h

#include <memory>

#include "IActiveWindow.hpp"

std::unique_ptr<IActiveWindow> get_active_window();

#endif
