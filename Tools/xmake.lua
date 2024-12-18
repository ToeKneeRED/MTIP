set_languages("cxx23")

target("Tools")
    set_kind("static")

    add_packages("spdlog", {public = true})

    add_headerfiles("include/*.h")
    add_files("src/*.cpp")

    add_rules("copy_to_common", {operations = {"after_build"}})