#ifndef GUARD_ProcessList_h
#define GUARD_ProcessList_h

#include <map>

namespace {
    struct ProcessInformation
    {
        unsigned long long before_time;
        unsigned long long after_time;
        unsigned long long ram_usage;
    };
}

class ProcessList
{
    public:
        unsigned long long get_time(unsigned int pid)
        {
            ProcessInformation process = processes_[pid];
            return process.after_time - process.before_time;
        }
        
        unsigned long long get_ram(unsigned int pid)
        {
            return processes_[pid].ram_usage;
        }

        void set_before_time(unsigned int pid, unsigned long long time)
        {
            processes_[pid].before_time = time;
        }

        void set_after_time(unsigned int pid, unsigned long long time)
        {
            processes_[pid].after_time = time;
        }

    private:
        std::map<unsigned int, ProcessInformation> processes_;
};

#endif
