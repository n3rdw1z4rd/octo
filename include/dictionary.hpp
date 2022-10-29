#pragma once

#include <string>
#include <map>

namespace octo
{
    typedef std::map<std::string, std::string> StringMap;
    typedef StringMap::iterator StringMapIt;

    class Dictionary
    {
    public:
        Dictionary();
        ~Dictionary();

        template <typename T>
        void set(const std::string, T);
    }
}