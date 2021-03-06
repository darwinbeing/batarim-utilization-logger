#include <memory>
#include <string>

#include <time.h>

#include "timestamp.hpp"

using std::unique_ptr;
using std::string;

unique_ptr<TimeInfo> get_time_info()
{
    const time_t timestamp = time(NULL);
    
    const size_t datetime_cstr_length = 50; // may need to be increased
    char datetime_cstr[datetime_cstr_length];
    // %c is preferred date time representation for current locale
    strftime(datetime_cstr, datetime_cstr_length, "%c", localtime(&timestamp));

    unique_ptr<TimeInfo> time_info(new TimeInfo);
    time_info->timestamp = timestamp;
    time_info->datetime_str = datetime_cstr;

    return time_info;
}