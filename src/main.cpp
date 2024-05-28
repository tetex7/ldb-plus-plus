#include <iostream>
#include "l61_api.hpp"
#include <array>
#include <map>
#include "vals.hpp"
#include <dlfcn.h>
#include <readline/readline.h>
#include "ute.hpp"


ex_edata_t* qdata = nullptr;

FLAG lua_mode = 0;

#define ENTER_KEY_CODE '\x0A'
#define BACK_SPACE_KEY_CODE '\x08'
#define DEBUG_INPUTS 0
C_CALL void edebug_mode(lua_State* L, lua_Debug* __ar);
C_CALL void exdebug_mode(lua_State* L, lua_Debug* __ar);

std::string lget_input(std::string say)
{
    if (say != STR_NULL)
    {
        std::cout << say << ' ';
    }
    std::vector<char> out_vet;
    char key = 0;
    while(key != ENTER_KEY_CODE)
    {
        key = std::cin.get();
#   if DEBUG_INPUTS == 1
        std::cout << "hex: " << std::hex << (int)key << std::dec << "\n";
#   endif
        if (key == BACK_SPACE_KEY_CODE)
        {
            out_vet.pop_back();
        }
        else if(key == ENTER_KEY_CODE)
        {
            break;
        }
        else
        {
            out_vet.push_back(key);
        }
    }
    out_vet.push_back('\0');

    std::string out = std::string(out_vet.data());

    return out;
}

/**
 * @anchor tete
 * @brief  The start point for a lex61
 * @param edata a pointer to API exchange data structure to facilitate API calls (MUST BE A C_CALL)
 */
C_CALL void init(ex_edata_t* edata)
{
    static __readonly DWORD req_vid = 30001;
    if(!(edata->api_version() >= req_vid))
    {
        std::cerr << "BAD API VID (L61_API_VID[" << edata->api_version() << "] >= " << req_vid << ")\n";
        exit(14);
    }

    auto& spaths = *edata->spaths;

    spaths.push_back(spaths[0] + "/libldb++");

    qdata = edata;
    edata->ctl->ctl_debug->set_debuger(exdebug_mode);
}

C_CALL void pre_lua_init()
{

}

C_CALL void __attribute__ ((constructor)) initLibrary() {
    //
    // Function that is called when the library is loaded
    //
    std::cout << "ldb++ loaded!!\n";
}



C_CALL void __attribute__ ((destructor)) cleanUpLibrary() {
    //
    // Function that is called when the library is closed.
    //
    //std::cout << "debugger unloaded!!\n";
}
