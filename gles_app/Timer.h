#pragma once

class Timer
{
public:
    static unsigned int getTimeMS();

private:
    static unsigned long m_appStartTime;
};
