/*
l61 api header
Copyright (C) 2024  tete

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#pragma once
#ifndef LAPI_HPP
#define LAPI_HPP 1


#ifndef __cplusplus
#   error "not C++"
#endif
#define __readonly const

#include <lua.hpp>
#include <vector>
#include <iostream>
#include <stack>
#include <cstdint>
#include <string.h>
#include <memory>
#include <cstring>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sol/sol.hpp>
#ifndef TLOAD
typedef bool FLAG;

typedef __readonly bool CFLAG;

#ifndef WIN32
typedef unsigned long       DWORD;

typedef unsigned char       BYTE;

typedef unsigned short      WORD;
#endif

typedef BYTE BYTE_FLAG;

typedef __readonly BYTE CBYTE_FLAG;


typedef signed long       SDWORD;

typedef signed char       SBYTE;

typedef signed short      SWORD;

typedef __uint128_t       QWORD;
typedef __int128_t       SQWORD;

typedef const char* cstring;

typedef void* VOID_PTR;
typedef void (*FREE_FUN)();
typedef volatile void* VVOID_PTR;

typedef void(*VOID_FPTR)();

typedef void* HANDLE;

typedef uint8_t address8_t;
typedef uint16_t address16_t;
typedef size_t address_t;

#define STR_NULL ""//"\0\0\0\'NULL STR\'\0\0"
#define DI_PATH "/opt/l61"
#define __readonly const
#define SWITCH(v) v = !v
#define rlen(v) (v - 1)
#define pass continue
#define arrlen(val) ((size_t)(sizeof(val) / sizeof(*val)))
#define alen(v) rlen(arrlen(v))
#define arr(v) v*
#define mk_segfault() *(volatile char*)NULL = 0
#define API_MODE __attribute__ ((packed))

#endif
#define C_CALL extern "C"

struct lib_load_data
{
    std::string as;
    std::string lib;
    FLAG tap;
};

struct confg_t
{
    std::string confg_vid;
    FLAG lib_pre_load;
    std::vector<lib_load_data> ldata;
    std::vector<std::string> exs;
};

//extern WORD sage;

enum class bmsg_t : DWORD
{
    BMSG_GET_DATA = 0x10,
    BMSG_SET_DATA = 0x1F,

};

extern "C" {

    struct debug_ctl_t
    {
        WORD(*get_debuger)(lua_Hook debuger);
        FLAG(*get_debug_flag)();
        void(*set_debug_flag)(FLAG v);
    };

    struct ctlfun_t {
        void(*sync)();
        WORD(*get_locc)();
        void(*set_locc)(WORD v);
        debug_ctl_t* ctl_debug;
    };
}

struct bdata_t
{
    std::stack<std::pair<bmsg_t, size_t>>* bmsg_stack;
};

struct l61_stat
{
    lua_State* L;
    std::string work_path;
    std::string make_file_path;
    std::string bin_path;
    std::string user_name;
    std::string user_home;
    confg_t conf;
    sol::state_view lstate;
};

struct mem_data_t
{
    void* ptr;
    size_t len;
};


struct ex_edata_t
{
    l61_stat* const stat;
    std::vector<std::string>* const spath;
    ctlfun_t* ctl;

};

/*__inline ex_edata_t* mk_new_edata(ex_edata_t* edata)
{
    debug_ctl_t* debug_ctl = new debug_ctl_t{};

    std::memcpy(debug_ctl, edata->ctl->ctl_debug, sizeof(debug_ctl_t));

    ctlfun_t* ctlc = new ctlfun_t{edata->ctl->sync, edata->ctl->get_locc, edata->ctl->set_locc, debug_ctl};
    ex_edata_t* ret = new ex_edata_t {edata->stat, edata->spath, ctlc};
    return ret;
}*/

constexpr size_t edata_s = sizeof(ex_edata_t);
typedef std::pair<ex_edata_t*, int> eapi_t;

__inline eapi_t open_eapi(const char* id)
{
    int obj = shm_open(id, O_RDONLY, 0666);
    ex_edata_t* ptr = (ex_edata_t*)mmap(0, edata_s, PROT_READ, MAP_SHARED, obj, 0);
    return std::pair(ptr, obj);
}

__inline void close_eapi(std::string eid)
{
    shm_unlink(eid.c_str());
}

typedef void(*L61_EX_LIB_FUN)(ex_edata_t*);

#endif
