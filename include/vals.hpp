//
// Created by tete on 5/26/24.
//
#pragma once
#ifndef VALS_HPP
#define VALS_HPP
#include "l61_api.hpp"
#include <iostream>
#include <array>
#include <map>
#include <sol/sol.hpp>

struct lua_debug_pack_t
{
    lua_State* L;
    lua_Debug* ar;
};

typedef int (*cmd_call_ptr)(std::string raw_arg_str, std::vector<std::string> args, const lua_debug_pack_t& data);
typedef std::map<std::string, cmd_call_ptr> cmd_map_t;
extern ex_edata_t* qdata;


std::string lget_input(std::string say = STR_NULL);
void lua_run(lua_State* L, std::string str);

__inline  std::string get_ttype(int e)
{
    switch(e)
    {
    case LUA_TSTRING:
        return std::string("string");
        break;
    case LUA_TBOOLEAN:
        return std::string("bool");
        break;
    case LUA_TFUNCTION:
        return std::string("function");
        break;
    case LUA_TNUMBER:
        return std::string("number");
        break;
    case LUA_TNIL:
        return std::string("nil");
        break;
    default:
        return std::string("");
        break;
    }
    return std::string("");
}

__inline  std::string get_ev(int e)
{
    switch(e)
    {
    case LUA_HOOKRET:
        return std::string("LUA_HOOKRET");
        break;
    case LUA_HOOKLINE:
        return std::string("LUA_HOOKLINE");
        break;
    case LUA_HOOKCALL:
        return std::string("LUA_HOOKCALL");
        break;
    case LUA_HOOKTAILCALL:
        return std::string("LUA_HOOKTAILCALL");
        break;
    default:
        return std::string("");
        break;
    }
    return std::string("");
}

__inline  std::vector<std::string> splitString(const std::string& input) {
    std::vector<std::string> result;
    std::string currentToken;
    bool insideQuotes = false;

    for (char c : input) {
        if (c == '\"' || c == '\'' || c == '(' || c == ')') {
            insideQuotes = !insideQuotes;
            currentToken += c;
        } else if (c == ' ' && !insideQuotes) {
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

extern cmd_map_t cmd_map;


#endif //VALS_HPP
