#include <iostream>
#include <fstream>
#include <boost/algorithm/string.hpp> 
#include "boost/date_time/posix_time/posix_time.hpp"

typedef struct {
    boost::posix_time::ptime time;
    std::string orig;
} line_t;

typedef struct {
    int id;
    bool is_sleeping;
    boost::posix_time::time_duration time_asleep; 
    boost::posix_time::ptime asleep_at;
    std::map<int, int> mins;
} guard_t;

typedef struct {
    guard_t guard;
    int key;
    int val;
} freq_t;

freq_t most_freq_min_asleep(std::vector<guard_t> list)
{
    freq_t res = {list.front(),0,0};

    for(std::vector<int>::size_type i = 0; i != list.size(); i++)
    {
        // get most frq min of curr
        auto curr = list.at(i);
        int m_key = 0;
        int m_val = 0;

        for(auto const& [key,val]:curr.mins)
        {
            if(val > m_val)
            {
                m_key = key;
                m_val = val;
            }
        }

        if(m_val > res.val)
        {
            res = {curr,m_key,m_val};
        }
    }

    return res;
}

bool has_item(std::vector<guard_t> list, int id)
{
    for(std::vector<int>::size_type i = 0; i != list.size(); i++)
    {
        if(list.at(i).id == id)
        {
            return true;
        }
    }
    return false;
}

guard_t get_item(std::vector<guard_t> list, int id)
{
    guard_t item;
    for(std::vector<int>::size_type i = 0; i != list.size(); i++)
    {
        if(list.at(i).id == id)
        {
            item = list.at(i);
        }
    }  
    return item;
}

guard_t get_sleepiest_guard(std::vector<guard_t> list)
{
    auto max = list.front();
    for(std::vector<int>::size_type i = 0; i != list.size(); i++)
    {
        auto curr = list.at(i);
        if(curr.time_asleep > max.time_asleep)
        {
            max = curr;
        }
    }
    return max;
}

int get_sleepiest_minute(guard_t item)
{
    std::pair<int,int> sleepiest(0,0);

    for(auto const& [key,val]:item.mins)
    {
        if(val >= sleepiest.second)
        {
            sleepiest={key,val};
        }
    }

    return sleepiest.first;
}

int main() 
{
    std::string line;
    std::ifstream input ("input.txt");
    std::vector<line_t> lines;
    std::vector<guard_t> guards;

    if(!input.is_open())
    {
        return 1;
    }

    while(std::getline(input,line))
    {
        auto time = boost::posix_time::time_from_string(line.substr(1,16));
        line_t curr = { time, line };
        lines.push_back(curr);
    }

    input.close();

    std::sort(lines.begin(), lines.end(), [](line_t a, line_t b){
        return a.time < b.time;
    });

    for(std::vector<int>::size_type i = 0; i != lines.size(); i++)
    {
        auto curr = lines.at(i);

        if(curr.orig.find("#") != std::string::npos)
        {
            // move guard to front
            std::vector<std::string> intermediate;
            boost::split(intermediate,curr.orig,boost::is_any_of("#"));

            std::vector<std::string> guard_str;
            boost::split(guard_str,intermediate.back(),boost::is_any_of(" "));

            auto id = std::stoi(guard_str.front());

            if(has_item(guards,id))
            {
                // move our guard to back
                guard_t item = get_item(guards,id);
                guards.push_back(item);
            }
            else
            {
                // push new guard into vec
                boost::posix_time::time_duration td;
                boost::posix_time::ptime pt;
                std::map<int,int> mins;
                guard_t item = {id, false, td, pt, mins};
                guards.push_back(item);
            }
        }
        else if(curr.orig.find("asleep") != std::string::npos)
        {
            // register latest guard as asleep
            guards.back().asleep_at = curr.time;
            guards.back().is_sleeping = true;
        }
        else if(curr.orig.find("wakes") != std::string::npos)
        {
            // register latest guard as awake
            // guards.back().asleep_at = curr.time;
            guards.back().time_asleep = guards.back().time_asleep + (curr.time - guards.back().asleep_at);
            guards.back().is_sleeping = false;

            auto low = guards.back().asleep_at.time_of_day().minutes();
            auto diff = std::abs(curr.time.time_of_day().minutes()-low);

            for(int i = 0; i < diff; i++)
            {
                auto key = low + i;
                if(key > 60)
                {
                    key = key - 60;
                }
                
                auto search = guards.back().mins.find(key);
                if (search != guards.back().mins.end()) 
                {
                    search->second++;
                } 
                else 
                {
                    guards.back().mins.insert(std::pair<int,int>(key,1));
                }
            }
        }
        else
        {
            return 1;
        }

    }

    auto sleepiest = get_sleepiest_guard(guards);
    auto sleepiest_min = get_sleepiest_minute(sleepiest);

    std::cout<<"sleepiest guard: "<<sleepiest.id<<std::endl
             <<"sleepiest min: "<<sleepiest_min<<std::endl
             <<sleepiest.id*sleepiest_min<<std::endl;

    auto most_freq = most_freq_min_asleep(guards);

    std::cout<<most_freq.guard.id * most_freq.key<<std::endl;

    return 0;
}