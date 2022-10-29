#pragma once

namespace octo
{
    template <typename OUT, typename IN>
    OUT ConvertType(IN in)
    {
        std::stringstream ss;
        ss << in;
        OUT o;
        ss >> o;
        return o;
    }
}