set_languages("cxx23")

target("Launcher")
    set_kind("binary")
    set_default(true)

    add_files("src/*.cpp")
    add_extrafiles("resources/*")
    add_headerfiles("include/*.h")

    add_includedirs("resources/")
    add_includedirs("include/")

    add_deps("Tools")
