set_languages("cxx23")

target("Tools")
    set_kind("static")

    add_packages("spdlog", {public = true})

    add_headerfiles("include/*.h")
    add_files("src/*.cpp")


    after_build(function (target)
        local destination = path.join(os.projectdir(), "common");

        os.cp(target:targetfile(), destination)
        cprint("${bright green}[%s:after_build] Copied %s to %s${clear}", target:name(), target:filename(), destination)
    end)