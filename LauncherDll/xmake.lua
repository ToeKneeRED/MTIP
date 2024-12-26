set_languages("cxx23")

target("LauncherDll")
    set_kind("shared")
    set_extension(".dll")

    add_files("src/*.cpp")
    add_headerfiles("include/*.h")
    add_includedirs("include/")
    set_pcxxheader("include/pch.h")

    add_deps("Tools")
    add_packages("minhook","qt5base", "qt5core", "qt5gui", "qt5widgets")

    add_rules("copy_to_common", {operations = {"after_build"}})