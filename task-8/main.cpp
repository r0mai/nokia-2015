#include <iostream>
#include <fstream>
#include <stdexcept>
#include <chrono>
#include <array>
#include <deque>
#include <algorithm>


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

struct ZeroEvents
{
    struct WebServer
    {
        bool fail;
        TimePrec when;
        int requests;
        WebServer():fail{false}, requests{0}{}

        void checkEvent(const TimePrec& t)
        {
            std::clog << t.count() << " - ";

            // A
            bool a = fail && (t - when) > 5min;
            std::clog << "A: " << a << ", ";

            // B
            bool b = requests >= 400;
            std::clog << "B: " << b << std::endl;
            if( a && b )
            {
                std::cout << "0";
                gotRestart();
            }
        }

        void gotRestart()
        {
            fail = false;
            requests = 0;
        }

        void gotFail(TimePrec w)
        {
            if(!fail)
            {
                when = w;
                fail = true;
            }
        }

        void gotRequest()
        {
            if(fail)
                ++requests;
        }
    };

    std::array< WebServer, 5 > ec;

    void tick(const TimePrec& p)
    {
        for(auto& event : ec)
        {
            event.checkEvent(p);
        }
    }

    void webServerHappened(const Line& l)
    {
        auto& event = ec[l.num-11];
        if(l.message == "FAILURE")
        {
            event.gotFail(l.tp);
        }
        else if (l.message == "SERVICE_RESTART_COMPLETED")
        {
            event.gotRestart();
        }
        else if(l.message == "USER_REQUEST")
        {
            //event.gotRequest();

            // tobbes szamban van!
            for(auto& event : ec) // ????
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
        bool isDown;
        TimePrec when;

        Storage():isDown{false}{}

        bool checkC(const TimePrec& t)
        {
            while(!no_space_alarms.empty() && t - no_space_alarms.front() > 5min )
            {
                no_space_alarms.pop_front();
            }

            return no_space_alarms.size() >= 3;
        }

        bool checkD(const TimePrec& t)
        {
            return isDown && t - when > 3min;
        }

        void gotNoSpace(const TimePrec& t)
        {
            no_space_alarms.push_back(t);
        }

        void gotStart()
        {
            isDown = false;
        }

        void gotStop(const TimePrec& t)
        {
            isDown = true;
            when = t;
        }

        void gotRestart(const TimePrec& asd) // ????
        {
            no_space_alarms.clear();
            //isDown = false;
            when = std::min(when + 3min, asd);
        }
    };

    struct Distributor
    {
        bool do_anything;
        TimePrec lastDo;

        Distributor():do_anything{false}{}

        bool checkC(const TimePrec& p)
        {
            return !do_anything || p - lastDo > 5min;
        }

        void gotStg(const TimePrec& p)
        {
            do_anything = true;
            lastDo = p;
        }

        void gotRestart()
        {
            do_anything = false; // ????
        }
    };

    std::array< Storage, 10 > storages;
    std::array< Distributor, 2 > distributors;

    void tick(const TimePrec& p)
    {
        // C
        std::clog << p.count() << " - ";
        bool c = std::count_if(std::begin(storages), std::end(storages), [&p](Storage& s){ return s.checkC(p); }) >= 8;
        c &= std::count_if(std::begin(distributors), std::end(distributors), [&p](Distributor& d){ return d.checkC(p); }) >= 1;
        std::clog << "C: " << c << ", ";
        // D
        bool d = std::count_if(std::begin(storages), std::end(storages), [&p](Storage& s){ return s.checkD(p); }) >= 5;
        std::clog << "D: " << d << std::endl;

        if(c || d)
        {
            std::cout << "1";

            for(Storage& s : storages)
            {
                s.gotRestart(p);
            }

            for(Distributor& d : distributors)
            {
                d.gotRestart();
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
        else if(l.message == "STOP")
        {
            st.gotStop(l.tp);
        }
        else if(l.message == "START")
        {
            st.gotStart();
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

    ZeroEvents ze;
    OneEvents oe;
    Line l;
    while(input >> l)
    {
        ze.tick(l.tp-1ms);
        oe.tick(l.tp-1ms);
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
    std::cout << std::endl;
    return 0;
}
