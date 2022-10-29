#include "../include/dictionary.hpp"

namespace octo
{
    //     Dictionary::Dictionary() {}
    //     Dictionary::~Dictionary() {}

    //     void Dictionary::set(const std::string &key, const std::string &val)
    //     {
    //         map[key] = val;
    //     }

    //     void Dictionary::set(const std::string &key, int val)
    //     {
    //         map[key] = val;
    //     }

    //     void Dictionary::set(const std::string &key, float val)
    //     {
    //         map[key] = val;
    //     }

    //     template <typename T>
    //     T Dictionary::get(const std::string &key)
    //     {
    //         return std::any_cast<T>(map[key]);
    //     }

    //     void Dictionary::dump() {}

    Dictionary::Dictionary() {}
    Dictionary::~Dictionary() {}

    void Dictionary::set(const std::string &key, int val)
    {
        dict[key] = val;
    }

    void Dictionary::set(const std::string &key, const std::string &val)
    {
        dict[key] = val;
    }

    void Dictionary::set(const std::string &key, float val)
    {
        dict[key] = val;
    }

    template <typename T>
    T Dictionary::get(const std::string &key)
    {
        return std::any_cast<T>(dict[key]);
    }
}