#pragma once

#include "octo.hpp"

#include <iostream>

namespace octo
{
    void DictionaryTest()
    {
        // std::unordered_map<std::string, std::any> dict;

        // std::string name;
        // name = "charlie";
        // dict["test1"] = name;

        // std::cout << "dict['test1']: " << std::any_cast<std::string>(dict["test1"]) << "\n";

        // auto dict = Dict();
        auto dict = Dictionary();
        dict.set("test1", 5);
        dict.set("test2", "test2val");
        dict.set("test3", 16.72f);

        std::cout << "test1: " << dict.get<int>("test1") << "\n";
        // std::cout << "test2: " << dict.get<std::string>("test2") << "\n";
        // std::cout << "test3: " << dict.get<float>("test3") << "\n";
    }
}