#include "../include/tests.hpp"

namespace octo
{
    void DictionaryTest()
    {
        auto dict = Dictionary();
        dict.set("test_key_1", "one");
        dict.set("test_key_2", "two");
        dict.set("test_key_3", 1);
        dict.set("test_key_4", 4.0);
    }
}