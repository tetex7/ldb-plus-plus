--Copyright (C) 2024  Tete

--This program is free software: you can redistribute it and/or modify
--it under the terms of the GNU General Public License as published by
--the Free Software Foundation, either version 3 of the License, or any later version.

--This program is distributed in the hope that it will be useful,
--but WITHOUT ANY WARRANTY; without even the implied warranty of
--MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
--GNU General Public License for more details.

--You should have received a copy of the GNU General Public License
--along with this program.  If not, see <https://www.gnu.org/licenses/>.

allowMountTableError = false
allowCopyOnMount = true
--bash = libmount("libbash")
--libmount("utils")

---@type gcc_t
gcc =  libmount("libgcc")
color = libmount("libcolor")
gcc_flags = " -fPIC -O0 -g "
bas_flags = gcc_flags..""
c_flags = bas_flags.."--std=gnu17"
cxx_flags = bas_flags.."--std=gnu++2a"

name = "test.lex61"
src_files = fs.lfl("./src")--list_files(PWD.."/src")

function mk_lib(ofiles)
    ar = gcc.PRFIX.."ar"
    print(color.GREEN.."Makeing LIB :  "..color.BLUE.."lib"..name..".a"..color.NO_COL)
    arf1 = " crD "..PWD.."/lib"..name..".a"
    return exec(ar..arf1..ofiles)
end


INC = "-I./include"

function mk_lex(ofiles)
    print(color.GREEN.."Makeing lex61 :  "..color.BLUE..name..".lex61"..color.NO_COL)
    return exec(gcc.CXX..gcc_flags.."-shared -o "..name.." "..ofiles)
end


function main(argc, argv)
    if argc == 2 then
        if argv[2] == "clean" then
            fs.delet(PWD.."/bin")
            fs.delet(PWD.."/lib"..name..".a")
            fs.delet(PWD.."/"..name)
            sys.exit(0)
        end
    end
    if not fs.exists(PWD.."/bin") then
            fs.mkdir(PWD.."/bin")
    end

    gcc.form()
    for i = 1, l61.taplelen(src_files) do
        if fs.getEx(src_files[i]) == ".cpp" then--string.find(, ".cpp") then
            --print(gcc.CXX.." "..cxx_flags.." "..INC.." ".." -c "..src_files[i].." -o ./bin/"..fs.raw_filename(src_files[i])..".o")
            comp_print("Compiling",fs.raw_filename(src_files[i]), fs.raw_filename(src_files[i])..".o")
            exec(gcc.CXX.." "..cxx_flags.." "..INC.." ".." -c  "..src_files[i].." -o ./bin/"..fs.raw_filename(src_files[i])..".o")
        elseif fs.getEx(src_files[i]) == ".c" then
            comp_print("Compiling", fs.raw_filename(src_files[i]), fs.raw_filename(src_files[i])..".o")
            --print(gcc.CC.." "..c_flags.." "..INC.." ".." -c "..src_files[i].." -o ./bin/"..fs.raw_filename(src_files[i])..".o")
            exec(gcc.CC.." "..c_flags.." "..INC.." ".." -c "..src_files[i].." -o ./bin/"..fs.raw_filename(src_files[i])..".o")
        end
    end
    local bin_files = fs.list_files(PWD.."/bin")
    local o_files = ""
    for i = 1, l61.taplelen(bin_files) do
        if fs.getEx(fs.getEx(bin_files[i])) == ".o" then
           o_files = o_files.." ./bin/"..bin_files[i]
        end
    end
    --print(gcc.CXX..o_files.." -o ./test")
    print(mk_lex(o_files))
    return 0
end
