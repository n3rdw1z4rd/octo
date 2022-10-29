#pragma once

#include <unordered_map>
#include <string>
#include <any>

namespace octo
{
    // class Dictionary
    // {
    // private:
    //     std::unordered_map<std::string, std::any> map;

    // public:
    //     Dictionary();
    //     ~Dictionary();

    //     void set(const std::string &, const std::string &);
    //     void set(const std::string &, int);
    //     void set(const std::string &, float);

    //     template <typename T>
    //     T get(const std::string &);

    //     void dump();
    // };

    class Dictionary
    {
    private:
        std::unordered_map<std::string, std::any> dict;

    public:
        Dictionary();
        ~Dictionary();

        void set(const std::string &, int);
        void set(const std::string &, const std::string &);
        void set(const std::string &, float);
        template <typename T>
        T get(const std::string &);
    };
}