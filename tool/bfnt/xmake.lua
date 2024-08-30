--多语言翻译编译工具
target("bfnt")
    set_kind("binary")
    add_includedirs("./")
    add_files("*.cpp")
    set_rundir("../../res")