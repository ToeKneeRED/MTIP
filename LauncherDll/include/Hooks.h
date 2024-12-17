#pragma once
#include <iostream>
#include <sstream>
#include <vector>

struct Hooks
{
    static bool HookFunction(void* hookAddress, void* myFunc, int len);
    static void HookIAT(HMODULE hModule, const std::string& aFunctionToFind);
    template <typename T> static bool HookValue(void* aAddress, T acpNewValue);

    /*static bool HookString(void* aAddress, const char* acNewString);
    static bool HookWString(void* aAddress, const wchar_t* acNewString);*/
    Hooks() = default;
    Hooks(const Hooks&) = delete;
    Hooks(Hooks&&) = delete;
    Hooks& operator=(Hooks&) = delete;
    Hooks& operator=(Hooks&&) = delete;
    Hooks operator=(Hooks) = delete;
    ~Hooks() = default;

    void Free() const;

    std::vector<void*> addressesToFree{};
};

template <typename T> bool Hooks::HookValue(void* aAddress, T acpNewValue)
{
    T oldValue = *(static_cast<T*>(aAddress));

    // Change memory protection to allow writing
    DWORD oldProtect;
    VirtualProtect(aAddress, sizeof(acpNewValue), PAGE_EXECUTE_READWRITE, &oldProtect);

    // Modify the string at the specified address
    memcpy_s(static_cast<T*>(aAddress), sizeof(T), acpNewValue, sizeof(acpNewValue));

    // Restore original protection
    VirtualProtect(aAddress, sizeof(acpNewValue), oldProtect, &oldProtect);

    std::wstringstream stream;
    stream << std::hex << aAddress;
    std::wstring output = stream.str();
    output = L"0x" + output.erase(0, output.find_first_not_of('0'));

    std::wcout << L"Successfully hooked " << output.c_str() << L"\n";
    // std::wcout << L"Successfully hooked " << std::hex << aAddress << L"\n";
    return true;
}