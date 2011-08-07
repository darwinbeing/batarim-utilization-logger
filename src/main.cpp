#include <iostream>
#include <string>
#include <vector>
#include "cpu_usage/cpu_usage.hpp"
#include "ram_usage/ram_usage.hpp"
#include "active_window/active_window.hpp"

using std::endl;
using std::pair;
using std::vector;
using std::wcout;
using std::wstring;

int main()
{
    vector<pair<wstring, int> > cpu_usage_percentages;
    get_cpu_usage(cpu_usage_percentages);
    
    ram_usage_struct ram_usage;
    get_ram_usage(&ram_usage);

    wstring active_window_name = get_active_window_name();
    wstring active_window_process_name = get_active_window_process_name();
    
    typedef vector<pair<wstring, int> >::const_iterator const_iterator;
    for(const_iterator iter = cpu_usage_percentages.begin();
        iter != cpu_usage_percentages.end(); iter++) {
        
        wcout << L"CPU " << iter->first << L" usage: " << iter->second << L"%" 
            << endl;
    }
    wcout << L"Current RAM usage: " << ram_usage.percentage_usage << L"%" 
        << endl;
    wcout << L"Active window: " << active_window_name << endl;
    wcout << L"Active window process: " << active_window_process_name << endl;
}