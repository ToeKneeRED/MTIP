set_languages("cxx23")

target("LauncherDll")
    set_kind("shared")

    add_files("src/*.cpp")
    add_headerfiles("include/*.h")
    add_includedirs("include/")
    set_pcxxheader("include/pch.h")

    add_deps("Tools")

    add_packages(
        "minhook"
    )

    add_rules("copy_to_common", {operations = {"after_build"}})