#include <iostream>
#include <fstream>
#include <boost/algorithm/string.hpp> 
#include "boost/date_time/posix_time/posix_time.hpp"

std::vector<char> unique_letters(std::string str)
{
    std::vector<char> res;
    std::map<char,bool> shit;
    std::vector<char> parts(str.begin(), str.end());

    for(std::vector<int>::size_type i = 0; i < parts.size(); i++)
    {
        shit.insert(std::pair<char,bool>(std::tolower(parts.at(i)), true));
    }

    for(auto const& [key,val]:shit)
    {
        res.push_back(key);
    }

    return res;
}

std::string part_one_inner(std::string str)
{
    std::vector<char> parts(str.begin(), str.end());
    std::string res;
    
    for(std::vector<int>::size_type i = 0; i != parts.size() - 1; i++)
    {
        if(i >= parts.size())
        {
            // unsure of this case tbh
            return res;
        }

        auto curr = parts.at(i);
        auto next = parts.at(i+1);

        if(
            (std::tolower(curr)==next && curr==std::toupper(next)) ||
            (std::toupper(curr)==next && curr==std::tolower(next))
        )
        {
            std::string param(parts.begin() + i + 2, parts.end() );
            return res + part_one_inner(param);
        }
        else
        {
            res += curr;
        }
    }
    
    return res + parts.back();
}

std::string part_one(std::string str)
{
    std::string input = str;
    std::string res = part_one_inner(input);

    while(input != res)
    {
        input = res;
        res = part_one_inner(input);
    }
    
    return res;
}

int part_two(std::string str)
{
    auto uni = unique_letters(str);
    auto lowest = str.length();

    for(std::vector<int>::size_type i = 0; i != uni.size(); i++)
    {
        auto curr = str;
        auto letter = uni.at(i);

        std::cout<<"letter: "<<letter<<", string size: "<<curr.length()<<std::endl;

        curr.erase(std::remove(curr.begin(), curr.end(), std::tolower(letter)), curr.end());
        curr.erase(std::remove(curr.begin(), curr.end(), std::toupper(letter)), curr.end());
        
        auto res = part_one(curr).length();

        std::cout<<res<<" : "<<lowest<<std::endl;

        if(res < lowest)
        {
            lowest = res;
        }
    }

    return lowest;
}

int main()
{
    std::string line;
    std::ifstream input ("input.txt");

    if(!input.is_open())
    {
        return 1;
    }

    while(std::getline(input,line))
    {
        std::cout<<part_one(line).length()<<std::endl
                 <<part_two(line)<<std::endl;
    }

    input.close();

    return 0;
}