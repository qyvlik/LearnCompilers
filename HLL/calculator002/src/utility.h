#ifndef UTILITY
#define UTILITY

#include <functional>
#include <sys/time.h>

long timeTick(const std::function<void(void)>& t)
{
    timeval start, end;

    gettimeofday( &start, NULL );

    t();

    gettimeofday( &end, NULL );

    //long timeuse =
    return 1000000 * ( end.tv_sec - start.tv_sec ) + end.tv_usec - start.tv_usec;
    //return end.tv_usec - start.tv_usec;
}

#endif // UTILITY

