#include "l61_api.hpp"
#include <readline/readline.h>
#include <map>
#include "vals.hpp"

void lua_run(lua_State* L, std::string str)
{
    if (luaL_dostring(L, str.c_str()) != LUA_OK)
    {
        const char* str = lua_tostring(L, -1);
        std::cerr << "BAD_LUA: " << str << '\n';
        return;
    }
    return;
}


void elua_mode(lua_State* L, lua_Debug* ar)
{
    auto get_debug_flag = qdata->ctl->ctl_debug->get_debug_flag;
    auto set_debug_flag = qdata->ctl->ctl_debug->set_debug_flag;
    auto setlocc = qdata->ctl->set_locc;
    std::string input_str;
    set_debug_flag(0);
    while(input_str != "!!quit")
    {
        input_str = readline("[λ]> ");
        if (input_str == "!!quit")
        {
            return;
        }
        else if (input_str == "")
        {
            continue;
        }
        lua_run(L, input_str);
    }
    set_debug_flag(1);
    return;
}

int elua_v2(std::string raw_arg_str, std::vector<std::string> args, const lua_debug_pack_t& data)
{
    elua_mode(data.L, data.ar);
    return 0;
}

int dlcall(std::string raw_arg_str, std::vector<std::string> args, const lua_debug_pack_t& data);

int fview(std::string raw_arg_str, std::vector<std::string> args, const lua_debug_pack_t& data);

cmd_map_t cmd_map = {
    {"lua", &elua_v2},
    {"call", &dlcall},
    {"fview", &fview}
};

std::vector<std::string> splitString(const std::string& input);
FLAG is_cmd_name(std::string arg1, std::string arg2)
{
    size_t len = arg1.size();
    for(size_t i = 0; i < len; i++)
    {
        if (arg1[i] != arg2[i])
        {
            return 0;
        }
    }
    return 1;
}

lua_Debug* mk_debug(lua_State* L, lua_Debug* _ar)
{
    lua_getinfo(L, ">nSl", _ar);

    //lua_getinfo(L, ">L", _ar);
    return _ar;
}

C_CALL void exdebug_mode(lua_State* L, lua_Debug* ar)
{
    auto get_debug_flag = qdata->ctl->ctl_debug->get_debug_flag;
    auto set_debug_flag = qdata->ctl->ctl_debug->set_debug_flag;
    auto setlocc = qdata->ctl->set_locc;
    static FLAG war_msg = 1;
    static int step_ev = -44;
    std::string prp;
#define CRED "\033[01;91m"
#define BLK "\033[05m"
#define CNOC "\033[0m"
    if(war_msg)
    {
        std::cout << CRED << "\nWARNING: you are in the ldb++ mode\nthe (ex)debug mode is a EXTREME work in progress feature\n\n!!!MEMORY SAFETY IS NOT GUARANTEED!!!\n\n" << CNOC;
        std::cout << "Welcome to ldb++\n\n";
        war_msg = 0;
    }

    if((ar->event != step_ev) && (step_ev != -44))
    {
        return;
    }
    else
    {
        step_ev = -44;
    }

    if (lua_getinfo(L, "nSl", ar) == LUA_OK)
    {
        exit(1);
    }
    lua_debug_pack_t p = { L, ar };
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
    while(prp != "q" || prp != "exit")
    {
        prp = readline("[λ]: ");
        pre_prp = splitString(prp);

        if (prp == "return" || prp == "ret")
        {
            war_msg = 1;
            set_debug_flag(0);
            return;
        }
        else if(prp == "step")
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
            continue;
        }
        else if (prp == "finfo")
        {
            printf("Function: %s, Line: %d\n", ar->name, ar->linedefined);
            continue;
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
            continue;
        }
        else if(prp == "ttype")
        {
            std::cout << "top_stack_type: \"" + get_ttype(lua_type(L, -1)) << "\"\n";
            continue;
        }
        else if (is_cmd_name("get", prp))
        {
            auto args = splitString(prp);

            std::string luac = "print(" + args[1] + ")";
            set_debug_flag(0);
            lua_run(L, luac);
            set_debug_flag(1);
            continue;
        }
        else if (prp == "ptop")
        {
            set_debug_flag(0);
            lua_setglobal(L, "__temp_debug_dr15__");
            lua_run(L, "print(__temp_debug_dr15__)");
            set_debug_flag(1);
            continue;
        }
        else if (is_cmd_name("set", prp))
        {
            auto args = splitString(prp);
            if (args.size() < 3)
            {
                continue;
            }
            std::string luac = args[1] + " = " + args[2];
            set_debug_flag(0);
            lua_run(L, luac);
            set_debug_flag(1);

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
            continue;
        }
        else if(prp == "db-vid" || prp == "ldb-vid")
        {
            std::cout << "v1.2.0\n";
        }
        else if (prp == "eline")
        {
            std::cout << ar->short_src << ":" << ar->currentline << '\n';
            continue;
        }
        else if (prp == "line")
        {
            std::cout << ar->currentline << '\n';
            continue;
        }
        else if (prp == "q" || prp == "exit")
        {
            break;
        }
        else if (prp == "")
        {
            continue;
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
    }
    exit(0);
}

/*C_CALL void exdebug_mode(lua_State* L, lua_Debug* ar)
{
    auto get_debug_flag = qdata->ctl->ctl_debug->get_debug_flag;
    auto set_debug_flag = qdata->ctl->ctl_debug->set_debug_flag;
    auto setlocc = qdata->ctl->set_locc;
    static FLAG war_msg = 1;
    static int step_ev = -44;
    std::string prp;
#define CRED "\033[01;91m"
#define CNOC "\033[0m"
    if(war_msg)
    {
        std::cout << CRED << "\nWARNING: you are in the ldb++ mode\nthe (ex)debug mode is a EXTREME work in progress feature\n\n!!!MEMORY SAFETY IS NOT GUARANTEED!!!\n\n" << CNOC;
        std::cout << "Welcome to ldb++\n\n";
        war_msg = 0;
    }

    if((ar->event != step_ev) && (step_ev != -44))
    {
        return;
    }
    else
    {
        step_ev = -44;
    }

    if (lua_getinfo(L, "nSl", ar) == LUA_OK)
    {
        exit(1);
    }
    lua_debug_pack_t p = { L, ar };
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
    while(prp != "q" || prp != "exit")
    {
        prp = readline("[λ]: ");

        if (prp == "return" || prp == "ret")
        {
            war_msg = 1;
            set_debug_flag(0);
            return;
        }
        else if (prp == "q" || prp == "exit")
        {
            break;
        }


    }
    exit(0);
}*/

/*C_CALL void edebug_mode(lua_State* L, lua_Debug* __ar)
{
    auto get_debug_flag = qdata->ctl->ctl_debug->get_debug_flag;
    auto set_debug_flag = qdata->ctl->ctl_debug->set_debug_flag;
    auto setlocc = qdata->ctl->set_locc;
    static FLAG war_msg = 1;
    static int step_ev = -44;
    std::string prp;
#define CRED "\033[01;91m"
#define CNOC "\033[0m"
    if(war_msg)
    {
        std::cout << CRED << "\nWARNING: you are in the ldb++ mode\nthe debug mode is a EXTREME work in progress feature\n\n!!!MEMORY SAFETY IS NOT GUARANTEED!!!\n\n" << CNOC;
        std::cout << "Welcome to ldb++\n\n";
        war_msg = 0;
    }

    if((__ar->event != step_ev) && (step_ev != -44))
    {
        return;
    }
    else
    {
        step_ev = -44;
    }

     //std::cout << "event[" << get_ev(ar->event) << "]\n\n";
    while(prp != "q" || prp != "exit")
    {

        prp = readline("[λ]: ");
setlocc(8781);
        if (prp == "lua")
        {
            elua_mode(L, __ar);
        }
        else if (prp == "return" || prp == "ret")
        {
            war_msg = 1;
            set_debug_flag(0);
            return;
        }
        else if(prp == "step")
        {
            return;
        }
        else if((prp == "event") || (prp == "ev"))
        {
            std::cout << "event[" << get_ev(__ar->event) << "]\n";
        }
        else if(prp == "ttype")
        {
            std::cout << "top_stack_type: \"" + get_ttype(lua_type(L, -1)) << "\"\n";
        }
        else if (is_cmd_name("get", prp))
        {
            auto args = splitString(prp);

            std::string luac = "print(" + args[1] + ")";
            set_debug_flag(0);
            lua_run(L, luac);
            set_debug_flag(1);
        }
        else if (prp == "ptop")
        {
            set_debug_flag(0);
            lua_setglobal(L, "__temp_debug_dr15__");
            lua_run(L, "print(__temp_debug_dr15__)");
            set_debug_flag(1);
        }
        else if (is_cmd_name("set", prp))
        {
            auto args = splitString(prp);
            if (args.size() < 3)
            {
                continue;
            }
            std::string luac = args[1] + " = " + args[2];
            set_debug_flag(0);
            lua_run(L, luac);
            set_debug_flag(1);
        }
        else if (is_cmd_name("call", prp))
        {
            auto args = splitString(prp);
            if (args.size() < 2)
            {
                continue;
            }
            std::string luac = "print(" + args[1] + ")";
            set_debug_flag(0);
            lua_run(L, luac);
            set_debug_flag(1);
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
            return;
        }
        else if(prp == "vid")
        {
            lua_run(L, "print(L61_VID)");
        }
        else if(prp == "db-vid" || prp == "ldb-vid")
        {
            std::cout << "v1.2.0\n";
        }
        else if (prp == "q" || prp == "exit")
        {
            break;
        }
        else if (prp == "")
        {
            continue;
        }
        else
        {
            std::cout << "ERROR: bad cmd: \"" << prp << "\"\n";
            continue;
        }
    }
    exit(0);
}*/
