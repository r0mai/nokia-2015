#include <iostream>
#include <fstream>
#include <stdexcept>
#include <chrono>
#include <array>
#include <deque>
#include <algorithm>
#include <queue>
#include <sstream>

using namespace std::literals;

template<class T, class U>
std::istream& operator>>(std::istream& i, std::chrono::duration<T, U>& d)
{
    T tmp;
    i >> tmp;
    d = std::chrono::duration<T, U>{tmp};
    return i;
}

using TimePrec = std::chrono::milliseconds;
using FalseEvents = std::priority_queue< TimePrec , std::vector<TimePrec>, std::greater<TimePrec> >;

struct Line
{
    int num;
    std::string message;
    TimePrec tp;

    friend std::istream& operator>>(std::istream& i, Line& l)
    {
        char tmp;
        std::chrono::hours h;
        std::chrono::minutes m;
        std::chrono::seconds s;
        TimePrec ms;
        i >> l.message >> h >> tmp >> m >> tmp >> s >> tmp >> ms >> l.num >> l.message;
        l.tp = h + m + s + ms;
        return i;
    }
};

std::stringstream ss;
struct ZeroEvents
{
    struct WebServer
    {
        bool fail;
        TimePrec when;

        int requests;

        WebServer():fail{false}, requests{0}{}

        bool checkEvent(const TimePrec& t)
        {
            //std::clog << t.count() << " - ";

            // A
            bool a = fail && (t - when) >= 5min;
            //std::clog << "A: " << a << ", ";

            // B
            bool b = requests >= 400;
            //std::clog << "B: " << b << std::endl;

            if( a && b )
            {
                ss << "0";
                when = t;
                requests = 0;
                return true;
            }
            return false;
        }

        void gotRestart()
        {
            fail = false;
            requests = 0;
        }

        bool gotFail(TimePrec w)
        {
            if(!fail)
            {
                when = w;
                fail = true;
                return true;
            }
            return false;
        }

        void gotRequest()
        {
            if(fail)
            {
                ++requests;
            }
        }
    };

    ZeroEvents(FalseEvents& falseEvents):
        falseEvents(falseEvents){}

    FalseEvents& falseEvents;

    std::array< WebServer, 5 > ec;

    void tick(const TimePrec& p)
    {
        bool needRestart = false;
        for(auto& event : ec)
        {
            needRestart |= event.checkEvent(p);
        }

        if(needRestart)
        {
            falseEvents.push(p + 5min);
        }
    }

    void webServerHappened(const Line& l)
    {
        auto& event = ec[l.num-11];
        if(l.message == "FAILURE")
        {
            if(event.gotFail(l.tp))
            {
                falseEvents.push(l.tp + 5min);
            }
        }
        else if (l.message == "SERVICE_RESTART_COMPLETED")
        {
            event.gotRestart();
        }
        else if(l.message == "USER_REQUEST")
        {
            // tobbes szamban van!
            //for(auto& event : ec) // ????
            {
                event.gotRequest();
            }
        }
    }
};


struct OneEvents
{
    struct Storage
    {
        std::deque<TimePrec> no_space_alarms;

        bool checkC(const TimePrec& t)
        {
            while(!no_space_alarms.empty() && t - no_space_alarms.front() > 5min )
            {
                no_space_alarms.pop_front();
            }

            return no_space_alarms.size() >= 1;
        }

        void gotNoSpace(const TimePrec& t)
        {
            no_space_alarms.push_back(t);
        }

        void restart(const TimePrec& asd) // ????
        {
            no_space_alarms.clear();
        }
    };

    struct Distributor
    {
        std::deque<TimePrec> anythings;

        bool checkC(const TimePrec& p)
        {
            while(!anythings.empty() && p - anythings.front() > 5min )
            {
                anythings.pop_front();
            }
            return anythings.size() < 15;
        }

        void gotStg(const TimePrec& p)
        {
            anythings.push_back(p);
        }

        void restart()
        {
            //anythings.clear();
        }
    };

    OneEvents(FalseEvents& falseEvents):
        falseEvents(falseEvents), lastTrue{9h}{}

    FalseEvents& falseEvents;
    std::array< Storage, 10 > storages;
    std::array< Distributor, 2 > distributors;
    TimePrec lastTrue;

    void tick(const TimePrec& p)
    {
        // C
        //std::clog << p.count() << " - ";
        bool c = std::count_if(std::begin(storages), std::end(storages), [&p](Storage& s){ return s.checkC(p); }) >= 8;
        c &= std::count_if(std::begin(distributors), std::end(distributors), [&p](Distributor& d){ return d.checkC(p); }) >= 1;
        //std::clog << "C: " << c << ", ";

        if(c && p - lastTrue >= 5min)
        {
        
            lastTrue = p;
            
            ss << "1";
            for(Storage& s : storages)
            {
                s.restart(p);
            }

            for(Distributor& d : distributors)
            {
                d.restart();
            }
        }
    }

    void storageEventHappened(const Line& l)
    {
        auto& st = storages[l.num-20];
        if(l.message == "NO_SPACE")
        {
            st.gotNoSpace(l.tp);
        }
    }

    void distributorEventHappened(const Line& l)
    {
        distributors[l.num - 30].gotStg(l.tp);
    }
};

int main()
{
    bool file = true;
    std::ifstream input_f("07516091bcd356947518fe4e948b826e.log");
    std::istream& input(file && input_f.is_open() ? input_f : std::cin);

    FalseEvents falseEvents;
    ZeroEvents ze(falseEvents);
    OneEvents oe(falseEvents);
    Line l;

    while(input >> l)
    {
        while(!falseEvents.empty() && falseEvents.top() <= l.tp)
        {
            TimePrec falseEvent = falseEvents.top();
            falseEvents.pop();
            ze.tick(falseEvent);
            oe.tick(falseEvent);
        }

        if(11 <= l.num && l.num <= 15)
        {
            ze.webServerHappened(l);
        }
        else if(20 <= l.num && l.num <= 29)
        {
            oe.storageEventHappened(l);
        }
        else if(30 <= l.num && l.num <= 31)
        {
            oe.distributorEventHappened(l);
        }
        ze.tick(l.tp);
        oe.tick(l.tp);
    }

    std::clog << ss.str() << std::endl;
    std::cout << std::stol(ss.str(), nullptr, 2) << std::endl;
    return 0;
}
