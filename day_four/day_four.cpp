#include <iostream>
#include <fstream>
#include <bits/stdc++.h> 
#include <boost/algorithm/string.hpp> 
#include "boost/date_time/posix_time/posix_time.hpp"

class guard 
{
    public:

    guard(int guard_id)
    {
        id=guard_id;
    }

    void asleep_at(boost::posix_time::ptime t)
    {
        curr_time=t;
    }

    void wakeup_at(boost::posix_time::ptime t)
    { 
        // if(t.time_of_day().hours() != curr_time.time_of_day().hours())
        // {
        //     // this never actually occurs, easy!
        // }

        auto low = curr_time.time_of_day().minutes();
        auto diff = std::abs(t.time_of_day().minutes()-low);

        for(int i = 0; i < diff; i++)
        {
            auto key = low + i;
            if(key > 60)
            {
                key = key - 60;
            }
            
            auto search = mins.find(key);
            if (search != mins.end()) 
            {
                search->second++;
            } 
            else 
            {
                mins.insert(std::pair<int,int>(key,1));
            }
        }

        time_asleep = time_asleep + (t - curr_time);
    }

    int get_sleepiest_minute()
    {
        std::pair<int,int> sleepiest(0,0);

        for(auto const& [key,val]:mins)
        {
            if(val > sleepiest.second)
            {
                sleepiest = {key,val};
            }
        }

        return sleepiest.first;
    }

    bool operator == (guard compare)
    { 
        return compare.id==this->id;
    }

    int id;
    boost::posix_time::time_duration time_asleep; 
    boost::posix_time::ptime curr_time;
    std::map<int, int> mins;
};

guard get_sleepiest_guard(std::vector<guard> guard_list)
{
    guard max_guard = guard_list.front();
    boost::posix_time::time_duration max = max_guard.time_asleep;

    for(std::vector<int>::size_type i = 0; i != guard_list.size(); i++) {
        guard current_guard = guard_list.at(i);
        if(current_guard.time_asleep > max){
            max_guard = current_guard;
        }
    }

    return max_guard;
}

class file_line 
{
    public:

    file_line(std::string current_line)
    {
        time = boost::posix_time::time_from_string(current_line.substr(1,16));
        orig = current_line;
    }  

    int get_guard_number()
    {
        std::vector<std::string> intermediate;
        boost::split(intermediate,orig,boost::is_any_of("#"));

        std::vector<std::string> guard_str;
        boost::split(guard_str,intermediate.back(),boost::is_any_of(" "));

        return std::stoi(guard_str.front());
    }

    boost::posix_time::ptime time;
    std::string orig;
};

int main() 
{

    std::string line;
    std::ifstream input ("input.txt");
    std::vector<file_line> line_list;
    std::vector<guard> guard_list;

    if(!input.is_open())
    {
        return 0;
    }

    while(std::getline(input,line))
    {
        file_line curr(line);
        line_list.push_back(curr);
    }

    input.close();

    std::sort(line_list.begin(), line_list.end(), [](file_line a, file_line b){
        return a.time < b.time;
    });

    for(std::vector<int>::size_type i = 0; i != line_list.size(); i++)
    {
        file_line curr = line_list.at(i);

        if(curr.orig.find("#") != std::string::npos)
        {
            // register a guard
            auto num = curr.get_guard_number();
            guard tmp(num);

            auto pos = std::find(guard_list.begin(), guard_list.end(), tmp);
            if(pos != guard_list.end())
            {
                // move existing guard to back of list
                auto index = guard_list.begin() + std::distance(guard_list.begin(),pos);
                std::rotate(index, index + 1, guard_list.end());
            }
            else
            {
                // add new guard to list
                guard_list.push_back(tmp);
            }
        }
        else if(curr.orig.find("falls asleep") != std::string::npos)
        {
            // register latest guard fell asleep
            guard_list.back().asleep_at(curr.time);
        }
        else if(curr.orig.find("wakes up") != std::string::npos)
        {
            // register latest guard wakes up
            guard_list.back().wakeup_at(curr.time);
        }
    }

    guard sleepiest_guard = get_sleepiest_guard(guard_list);

    std::cout<<sleepiest_guard.get_sleepiest_minute()<<std::endl
             <<sleepiest_guard.id<<std::endl
             <<sleepiest_guard.get_sleepiest_minute()*sleepiest_guard.id<<std::endl;


    return 0;
}