#include "../include/dictionary.hpp"
#include "../include/utils.hpp"

namespace octo
{
    Dictionary::Dictionary() {}
    Dictionary::~Dictionary() {}

    StringMapIterator Dictionary::find(const std::string &key)
    {
        StringMapIterator it;

        if (_map.size() > 0)
        {
            it = _map.begin();

            while (it != _map.end())
            {
                if (it->first == key)
                {
                    break;
                }

                ++it;
            }

            return it;
        }
    }

    void Dictionary::set(const std::string &key, const std::string &value)
    {
        StringMapIterator it;

        if ((it = find(key)) != _map.end())
        {
            it->second = value;
        }
        else
        {
            _map[key] = value;
        }
    }

    template <typename T>
    void Dictionary::set(const std::string &key, T &value)
    {
        std::string _value = ConvertType<std::string>(value);
        set(key, _value);
    }

    std::string Dictionary::get(const std::string &key)
    {
        StringMapIterator it;

        if ((it = find(key)) == _map.end())
        {
            return "";
        }

        return it->second;
    }

    template <typename T>
    T Dictionary::get(const std::string &key)
    {
        return ConvertType<T>(get(key));
    }
}