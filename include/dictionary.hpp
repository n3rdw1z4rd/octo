#pragma once

#include <string>
#include <map>

namespace octo
{
    typedef std::map<std::string, std::string> StringMap;
    typedef StringMap::iterator StringMapIterator;

    class Dictionary
    {
    public:
        Dictionary();
        ~Dictionary();

        void set(const std::string &, const std::string &);

        template <typename T>
        void set(const std::string &, T &);

        std::string get(const std::string &);

        template <typename T>
        T get(const std::string &);

    protected:
        StringMapIterator find(const std::string &);

    private:
        StringMap _map;
    };
}