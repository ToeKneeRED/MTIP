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

    after_build(function (target)
        local destination = path.join(os.projectdir(), "common");

        os.cp(target:targetfile(), destination)
        cprint("${bright green}[%s:after_build] Copied %s to %s${clear}", target:name(), target:filename(), destination)
    end)