//
// Created by tete on 5/27/24.
//

#include "lua_call.hpp"

#include <iostream>
#include "l61_api.hpp"
#include <array>
#include <map>
#include "vals.hpp"




int dlcall(std::string raw_arg_str, std::vector<std::string> args, const lua_debug_pack_t& data)
{
    auto get_debug_flag = qdata->ctl->ctl_debug->get_debug_flag;
    auto set_debug_flag = qdata->ctl->ctl_debug->set_debug_flag;
    auto setlocc = qdata->ctl->set_locc;
    if (!(args.size() > 1))
    {
        return 1;
    }
    std::string lua_call = args[1];


    std::string luac = "print(" + lua_call + ")";
    set_debug_flag(0);
    lua_run(data.L, luac);
    set_debug_flag(1);

    return 0;
}
