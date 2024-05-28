//
// Created by tete on 5/27/24.
//

#include "l61_api.hpp"
#include <readline/readline.h>
#include <map>
#include "vals.hpp"

void lua_run(lua_State* L, std::string str);

FLAG is_cmd_name(std::string arg1, std::string arg2);
/*void db_runer(std::string prp, lua_debug_pack_t* p)
{
    auto get_debug_flag = qdata->ctl->ctl_debug->get_debug_flag;
    auto set_debug_flag = qdata->ctl->ctl_debug->set_debug_flag;
    auto setlocc = qdata->ctl->set_locc;
    static int step_ev = -44;

    auto ar = p->ar;
    auto L = p->L;
    static  FLAG STEP_LUA = 0;
    static FLAG STEP_LINE = 0;
    if (STEP_LUA)
    {
        if (std::string(ar->short_src) == "[C]")
        {
            return;
        }
        else
        {
            std::cout << "done\n";
            STEP_LUA = 0;
        }
    }

    if (STEP_LINE)
    {
        if (ar->currentline == -1)
        {
            return;
        }
        else
        {
            std::cout << "done\n";
            STEP_LINE = 0;
        }
    }
    static FLAG STEP_FUN = 0;
    if (STEP_FUN)
    {
        if (ar->name == NULL)
        {
            return;
        }
        else
        {
            std::cout << "done\n";
            STEP_FUN = 0;
        }
    }



    std::vector<std::string> pre_prp;

    //std::cout << "event[" << get_ev(ar->event) << "]\n\n";

    pre_prp = splitString(prp);


    if(prp == "step")
    {
        step_ev = LUA_HOOKLINE;
        return;
    }
    else if(prp == "step_lua")
    {
        STEP_LUA = 1;
        return;
    }
    else if(prp == "step_line")
    {
            STEP_LINE = 1;
        return;
    }
    else if(prp == "step_fun")
    {
        STEP_FUN = 1;
        return;
    }
    else if((prp == "event") || (prp == "ev"))
    {
        std::cout << "event[" << get_ev(ar->event) << "]\n";
        return;
    }
    else if (prp == "finfo")
    {
        printf("Function: %s, Line: %d\n", ar->name, ar->linedefined);
        return;
    }
    else if (prp == "lang_ctx")
    {
        std::string src = ar->short_src;
        if(src == "[C]")
        {
            printf("LANG: \"C\"\n");
        }
        else
        {
            printf("LANG: \"LUA\"\n");
            printf("LINE %i\n", ar->currentline);
            printf("NAME: \"%s\"\n", ar->name);
            printf("FILE: \"%s\"\n", src.c_str());
            printf("DEF_LINE: [%i]\n", ar->linedefined);
            printf("ATTS: \"%s\"\n", ar->namewhat);
        }
        return;
    }
    else if(prp == "ttype")
    {
        std::cout << "top_stack_type: \"" + get_ttype(lua_type(L, -1)) << "\"\n";
        return;;
    }
    else if (is_cmd_name("get", prp))
    {
        auto args = splitString(prp);

        std::string luac = "print(" + args[1] + ")";
        set_debug_flag(0);
        lua_run(L, luac);
        set_debug_flag(1);
        return;
    }
    else if (prp == "ptop")
    {
        set_debug_flag(0);
        lua_setglobal(L, "__temp_debug_dr15__");
        lua_run(L, "print(__temp_debug_dr15__)");
        set_debug_flag(1);
        return;
    }
    else if (is_cmd_name("set", prp))
    {
        auto args = splitString(prp);
        if (args.size() < 3)
        {
            return;
        }
        std::string luac = args[1] + " = " + args[2];
        set_debug_flag(0);
        lua_run(L, luac);
        set_debug_flag(1);
        return;

    }
    else if(is_cmd_name("step_to", prp))
    {
        auto args = splitString(prp);
        if (args[1] == "ret")
        {
            step_ev = LUA_HOOKRET;
        }
        else if(args[1] == "call")
        {
            step_ev = LUA_HOOKCALL;
        }
        else if(args[1] == "line")
        {
            step_ev = LUA_HOOKLINE;
        }
        return;
    }
    else if(prp == "vid")
    {
        lua_run(L, "print(L61_VID)");
    }
    else if(is_cmd_name("step_to_line", prp))
    {
        auto args = splitString(prp);

        int gline = std::stoi(args[1]);
            //go_to_line = gline;
        return;
    }
    else if(prp == "db-vid" || prp == "ldb-vid")
    {
        std::cout << "v1.2.0\n";
    }
    else if (prp == "eline")
    {
        std::cout << ar->short_src << ":" << ar->currentline << '\n';
        return;
    }
    else if (prp == "line")
    {
        std::cout << ar->currentline << '\n';
        return;
    }

    if (!cmd_map.contains(pre_prp[0]))
    {
        std::cout << "ERROR: bad cmd: \"" << pre_prp[0] << "\"\n";
    }
    else
    {
        cmd_call_ptr cmd = cmd_map[pre_prp[0]];
        cmd(prp, pre_prp, p);
    }
}*/