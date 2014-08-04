#include <windows.h>
#include "Timer.h"

unsigned long Timer::m_appStartTime = 0;

unsigned int Timer::getTimeMS()
{
    SYSTEMTIME time;


    GetSystemTime(&time);
    unsigned int millis = (time.wSecond * 1000) + time.wMilliseconds;

    if(m_appStartTime == 0)
    {
        m_appStartTime = millis;
    }

    return (millis - m_appStartTime);
}
