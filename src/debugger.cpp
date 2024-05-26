#include "l61_api.hpp"
#include <readline/readline.h>

std::vector<std::string> splitString(const std::string& input) {
    std::vector<std::string> result;
    std::string currentToken;
    bool insideQuotes = false;

    for (char c : input) {
        if (c == '\"') {
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

extern ex_edata_t* qdata;


std::string lget_input(std::string say = STR_NULL);


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

std::string get_ttype(int e)
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

std::string get_ev(int e)
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

C_CALL void edebug_mode(lua_State* L = nullptr, lua_Debug* __ar = nullptr)
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
        else if(prp == "db-vid")
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
        }


    }
    exit(0);
}
