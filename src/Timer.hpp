#include <map>
#include <string>

#include <Esp.h>

namespace Timer{
    
using handler = std::function<void()>;

class Timer
{
public:
    Timer(handler f, unsigned int interval = 1000) : m_event(f), m_interval(interval)
    {

    };

    void update(){
        auto currentMillis = millis();
        if (currentMillis - m_previousMillis > m_interval)
        {
            action();
            m_previousMillis = currentMillis;
        }
    };

    unsigned int getInterval() {
        return m_interval;
    }

    void setInterval(unsigned int interval)
    {
        m_interval = interval;
    }

    ~Timer() = default;

private:
    void action(){
        m_event();
    };

    handler m_event;
    unsigned int m_previousMillis = 0;
    unsigned int m_interval = 1000;
};

static std::map<std::string, Timer> TimerList;

static void Create(std::string const& name, handler f, unsigned int interval = 1000){
    TimerList.emplace(name, Timer(f, interval));
}

static void update(){
    for(auto & [name, timer]: TimerList)
    {
        timer.update();
    }
}

static void SetInterval(std::string const& name, unsigned int new_interval)
{
    if (TimerList.find(name) != TimerList.end())
    {
        TimerList.at(name).setInterval(new_interval);
    }
}
}