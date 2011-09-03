#ifndef GUARD_UsageReporter_h
#define GUARD_UsageReporter_h

#include <functional>
#include <string>
#include <vector>

#include "IUsageResultGetter.hpp"
#include "ProcessList.hpp"

#ifdef WIN32
#include <Pdh.h>
struct PdhData
{
    PDH_HQUERY query;
    PDH_HCOUNTER counter;
};
#define BATARIM_CPU_INFO_DATA_STRUCTURE PdhData
#endif

#ifdef LINUX
#include <sstream>
#define BATARIM_CPU_INFO_DATA_STRUCTURE std::stringstream
#endif

class UsageReporter : public IUsageResultGetter
{
    public:
        UsageReporter()
        {
            std::shared_ptr<std::vector<std::pair<std::string, int>>> pu(
                new std::vector<std::pair<std::string, int>>);
            processor_usages_ = pu;
        }

        // Analyzes CPU and RAM usages
        // Will take at least one second to complete
        // Should be called first from main method
        bool analyze(std::string& error_message);

        virtual std::shared_ptr<std::vector<std::pair<std::string, int>>>
        get_processor_usages()
        {
            std::shared_ptr<std::vector<std::pair<std::string, int>>>
                proc_usage(processor_usages_);
            return proc_usage;
        }

        virtual int get_process_cpu_usage(unsigned int pid) const
        {
            int cpu_usage = (int)((double)process_list_.get_time(pid) / 
                elapsed_system_time_ * 100);
            return cpu_usage;
        }

        virtual int get_process_ram_usage(unsigned int pid) const
        {
            unsigned long long bytes_used = process_list_.get_ram(pid);
            int ram_usage = (int)((double)bytes_used / 
                total_physical_ram_ * 100);
            return ram_usage;
        }
    
    protected:
        std::shared_ptr<std::vector<std::pair<std::string, int>>>
        processor_usages_;
        
        ProcessList process_list_;
        unsigned long long elapsed_system_time_;
        unsigned long long total_physical_ram_;

        virtual bool initial_cpu_sweep_(BATARIM_CPU_INFO_DATA_STRUCTURE&) = 0;
        
        bool initial_populate_process_list_()
        {
            return populate_process_list_(&ProcessList::set_before_time);
        }
        
        virtual void pause_() = 0;
        virtual bool populate_processors_usage_(
            BATARIM_CPU_INFO_DATA_STRUCTURE& initial_data
        ) = 0;
        virtual void cleanup_processors_data_(
            BATARIM_CPU_INFO_DATA_STRUCTURE& cpu_info
        ) = 0;
        
        bool second_populate_process_list_()
        {
            return populate_process_list_(&ProcessList::set_after_time);
        }

        virtual unsigned long long get_current_system_time_() = 0;
        virtual bool populate_process_list_(
            std::function<void(
                ProcessList&, unsigned int, unsigned long long
                )> set_time
        ) = 0;
        virtual unsigned long long get_total_physical_ram_() const = 0;
        virtual void populate_process_list_ram_() = 0;
        virtual std::string get_human_readable_name_for_processor_entry_(
            std::string& provided_name
        ) = 0;
};

#endif
