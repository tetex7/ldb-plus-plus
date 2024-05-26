#include <iostream>
#include "l61_api.hpp"

ex_edata_t* qdata = nullptr;

#define ENTER_KEY_CODE '\x0A'
#define BACK_SPACE_KEY_CODE '\x08'
#define DEBUG_INPUTS 0
C_CALL void edebug_mode(lua_State* L, lua_Debug* __ar);

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

C_CALL void init(ex_edata_t* edata)//ex_edata_t* edata)
{
    qdata = edata;
    edata->ctl->ctl_debug->get_debuger(edebug_mode);
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
