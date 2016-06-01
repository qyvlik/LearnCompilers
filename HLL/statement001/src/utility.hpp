#ifndef UTILITY
#define UTILITY

#include <functional>
#include <sys/time.h>
#include <vector>
#include <string>

namespace qyvlik {

static inline long timeTick(const std::function<void(void)>& t)
{
    timeval start, end;

    gettimeofday( &start, NULL );

    t();

    gettimeofday( &end, NULL );

    //long timeuse =
    return 1000000 * ( end.tv_sec - start.tv_sec ) + end.tv_usec - start.tv_usec;
    //return end.tv_usec - start.tv_usec;
}


static inline std::vector<std::string> split(const std::string& str, const std::string& delim)
{
    size_t last = 0;
    size_t index = str.find_first_of(delim, last);

    std::vector<std::string> result;

    while (index != std::string::npos)
    {
        result.push_back(str.substr(last,index-last));
        last = index+1;
        index = str.find_first_of(delim, last);
    }

    if (index-last>0)
    {
        result.push_back(str.substr(last,index-last));
    }

    return result;
}



}

#endif // UTILITY

