//
// Created by tete on 5/27/24.
//

#include <iostream>
#include "l61_api.hpp"
#include <array>
#include <map>
#include "vals.hpp"
#include <fstream>

template<typename... bi_args>
__inline std::string STRex(bi_args... vals)
{
    std::stringstream ss;
    (ss << ... << vals);
    return ss.str();
}

static std::vector<std::string> splitlineString(const std::string& input) {
    std::vector<std::string> result;
    std::string currentToken;


    for (char c : input) {
        if (c == '\n') {
            if (!currentToken.empty()) {
                result.push_back(currentToken);
                currentToken.clear();
            }
        } else {
            currentToken += c;
        }
    }

    // Add the last token if it exists
    if (!currentToken.empty()) {
        result.push_back(currentToken);
    }

    return result;
}


int fview(std::string raw_arg_str, std::vector<std::string> args, const lua_debug_pack_t& data)
{

    if ( std::string(data.ar->short_src) == "[C]")
    {
        std::cout << "C lang\n";
        return 1;
    }

    if (data.ar->currentline == -1)
    {
        std::cout << "currentline is -1\n";
        return 1;
    }

    std::ifstream file = std::ifstream(data.ar->short_src);
    //std::cout << da;
    std::string line;
    std::vector<std::string> lines;

    while (std::getline(file, line)) {
        lines.push_back(line);
    }
    size_t line_org = data.ar->currentline;
    size_t begn_line = line_org - 5;
    size_t end_line = line_org + 5;
    if (end_line > lines.size())
    {
        end_line = lines.size() + 1;
    }

    if (args[1] == "all")
    {
        begn_line = 0;
        end_line = lines.size() + 1;
    }

    std::cout << data.ar->short_src << ":\n";

    for (size_t i = begn_line; i < end_line; i++)
    {
        if (i < 9)
        {
            if (i == line_org)
            {
                std::cout << i+1 << "->  | " << lines[i] << '\n';
            }
            else
            {
                std::cout << i+1 << "    | " << lines[i] << '\n';
            }
        }
        else
        {
            if (i == line_org)
            {
                std::cout << i+1 << "-> | " << lines[i] << '\n';
            }
            else
            {
                std::cout << i+1 << "   | " << lines[i] << '\n';
            }
        }

    }
    std::cout << "END OF VIEW\n";
    return 0;
}