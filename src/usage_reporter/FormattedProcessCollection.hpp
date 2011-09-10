#ifndef GUARD_FormattedPerformanceCollection_h
#define GUARD_FormattedPerformanceCollection_h

#include <map>
#include <memory>

#include "../utilities/utilities.hpp"
#include "ProcessUsageInfo.hpp"
#include "RawProcessCollection.hpp"
#include "RawProcessCollectionFactory.hpp"

class FormattedProcessCollection
{
    public:

        FormattedProcessCollection()
        {
            raw_process_info_ = get_raw_process_collection();
        }

        // NOTE: must call first init followed by update after a one
        // second delay
        void init()
        {
            raw_process_info_->init();
            before_system_time_ = get_current_system_time_();
        }

        void update()
        {
            raw_process_info_->update();
            elapsed_system_time_ =
                get_current_system_time_() - before_system_time_;
            total_physical_ram_ = get_total_physical_ram_();

            typedef RawProcessCollection::const_iterator Iter;
            for(Iter iter = raw_process_info_->begin();
                iter != raw_process_info_->end(); ++iter) {

                ProcessUsageInfo proc_info;
                proc_info.process_name = 
                    batarim::get_process_name(iter->first);
                unsigned long long elapsed_time =
                    iter->second.after_time - iter->second.before_time;
                proc_info.cpu_usage = calculate_cpu_usage_(elapsed_time);
                unsigned long long ram_bytes_used = iter->second.ram_usage;
                proc_info.ram_usage = calculate_ram_usage_(ram_bytes_used);
                usage_info_[iter->first] = proc_info;
            }
        }

        const ProcessUsageInfo& get_process(unsigned int pid) const
        {
            return usage_info_.at(pid);
        }
        
        const ProcessUsageInfo& get_process_with_highest_cpu_usage() const;

        const ProcessUsageInfo& get_process_with_highest_ram_usage() const;

    protected:

        virtual unsigned long long get_current_system_time_() const = 0;
        virtual unsigned long long get_total_physical_ram_() const = 0;

    private:
        
        std::shared_ptr<RawProcessCollection> raw_process_info_;
        std::map<unsigned int, ProcessUsageInfo> usage_info_;

        unsigned long long before_system_time_;
        unsigned long long elapsed_system_time_;
        unsigned long long total_physical_ram_;

        int calculate_cpu_usage_(unsigned long long elapsed_process_time) const
        {
            int cpu_usage = (int)((double)elapsed_process_time /
                elapsed_system_time_ * 100);
            return cpu_usage;
        }

        int calculate_ram_usage_(unsigned long long ram_bytes_used) const
        {
            int ram_usage = (int)((double)ram_bytes_used /
                total_physical_ram_ * 100);
            return ram_usage;
        }
};

#endif
