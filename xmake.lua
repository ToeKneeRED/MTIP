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