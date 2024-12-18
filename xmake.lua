set_languages("c11", "cxx23")
set_policy("build.ccache", false)
set_policy("package.requires_lock", false)

if(is_plat("windows")) then
    add_cxflags("/bigobj")
    set_arch("x64")

    -- :(
    add_syslinks("kernel32")
    add_syslinks("user32")
    add_syslinks("Gdi32")
end

add_rules("mode.debug", "mode.releasedbg", "mode.release")
add_rules("plugin.vsxmake.autoupdate")

add_requires("spdlog v1.13.0", "minhook v1.3.3")

if is_mode("debug") then
    add_defines("DEBUG")
end

add_includedirs("Tools/include", {public = true})

-- sub-project directories
includes("Tools")
includes("LauncherDll")
includes("Launcher")

set_defaultmode("releasedbg")

-- custom rules
rule("copy_to_common") -- ex. add_rules("copy_to_common", {"operations", {"after_build"}})
    before_build(function (target)
        local operations = target:extraconf("rules", "copy_to_common", "operations") or {}

        if table.contains(operations, "before_build") then
            local destination = path.join(os.projectdir(), "common")

            os.cp(target:targetfile(), destination)
            cprint("${bright green}[%s:before_build] Copied %s to %s${clear}",
                target:name(), target:filename(), destination)
        end
    end)

    after_build(function (target)
        local operations = target:extraconf("rules", "copy_to_common", "operations") or {}

        if table.contains(operations, "after_build") then
            local destination = path.join(os.projectdir(), "common")

            os.cp(target:targetfile(), destination)
            cprint("${bright green}[%s:after_build] Copied %s to %s${clear}",
                target:name(), target:filename(), destination)
        end
    end)

    before_clean(function (target)
        local operations = target:extraconf("rules", "copy_to_common", "operations") or {}

        if table.contains(operations, "before_clean") then
            local destination = path.join(os.projectdir(), "common")

            os.cp(target:targetfile(), destination)
            cprint("${bright green}[%s:before_clean] Copied %s to %s${clear}",
                target:name(), target:filename(), destination)
        end
    end)

    after_clean(function (target)
        local operations = target:extraconf("rules", "copy_to_common", "operations") or {}

        if table.contains(operations, "after_clean") then
            local destination = path.join(os.projectdir(), "common")

            os.cp(target:targetfile(), destination)
            cprint("${bright green}[%s:after_clean] Copied %s to %s${clear}",
                target:name(), target:filename(), destination)
        end
    end)
rule_end()