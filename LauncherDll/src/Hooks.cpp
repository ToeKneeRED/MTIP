#include "pch.h"
#include "Hooks.h"
#include "Addresses.h"

#include <iostream>
#include <sstream>
#include <string>

// no clue if this works lol
bool Hooks::HookFunction(void* hookAddress, void* myFunc, int len)
{
    if (len >= 5)
    {
        // get read/write access to the length of memory at hookAddress
        DWORD protection;
        VirtualProtect(hookAddress, len, PAGE_EXECUTE_READWRITE, &protection);

        // subtract hooked address' address from calling function's address (plus another 5 bytes for jmp instruction)
        // finds where to jump to reach function
        DWORD relativeAddress = (reinterpret_cast<DWORD>(myFunc) - reinterpret_cast<DWORD>(hookAddress)) - 5;

        // write jmp
        *(BYTE*)hookAddress = 0xE9; // jmp
        *(DWORD*)((DWORD)hookAddress + 1) = relativeAddress;

        // restore protection level for length of memory starting at hookAddress
        DWORD temp;
        VirtualProtect(hookAddress, len, protection, &temp);

        return true;
    }

    return false;
}

std::string AddressToString(LPVOID address)
{
    std::ostringstream oss;
    oss << std::hex << reinterpret_cast<uintptr_t>(address);
    return oss.str();
}

void Hooks::HookIAT(HMODULE hModule, const std::string& aFunctionToFind)
{
    LPVOID imageBase = GetModuleHandleA(NULL);
    PIMAGE_DOS_HEADER dosHeaders = (PIMAGE_DOS_HEADER)imageBase;
    PIMAGE_NT_HEADERS ntHeaders = (PIMAGE_NT_HEADERS)((DWORD_PTR)imageBase + dosHeaders->e_lfanew);
    PIMAGE_IMPORT_DESCRIPTOR importDescriptor = NULL;
    IMAGE_DATA_DIRECTORY importsDirectory = ntHeaders->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT];
    importDescriptor = (PIMAGE_IMPORT_DESCRIPTOR)(importsDirectory.VirtualAddress + (DWORD_PTR)imageBase);
    LPCSTR libraryName = NULL;
    HMODULE library = NULL;
    PIMAGE_IMPORT_BY_NAME functionName = NULL;
    const std::string hashedString[] = {
        "?",     "@",    "QEAA",     "PEAV",      "MEBAAV", "AEBV",  "EEAAX",  "MEAAX",        "UEBAAV",
        "MEBAH", "PEBV", "HPEAPEAX", "UEBAPEBU",  "MEBA",   "UEAA1", "UEAA",   "UEAAPEAXPEBD", "V$",
        "UEBA",  "PEAX", "PEBD",     "QEBA",      "PEAX",   "PEBD",  "SAAV",   "PEAA",         "SAX",
        "PEAU",  "AEAV", "YAX",      "MEAA",      "YAH",    "_N",    "MEAA",   "CAH",          "_J",
        "_K",    "HPEA", "PEAH",     "YAHPEBE00", "PEBU1",  "$",     "AV",     "@H",           "PEA01",
        "YA",    "1",    "PEBE00",   "@X",        "@U",     "@Y",    "SAA",    "PEBH",         "PEBU3",
        "00H",   "AEAH", "W421",     "HHH",       "@V",     "AEA",   "PEADH0", "PEAD",         "H0",
        "HHV"};
    // <function> <class> <return type (if none then void)> <args> <possibly argument class (might be prepended with
    // W4)> function:	starts with 0 = constructor; starts with 1 = destructor args:		starts with W4/4 = thing
    // after is argument's class args:		constructors may have an arg that starts with Q/QE/QE0?

    while (importDescriptor->Name != NULL)
    {
        libraryName = (LPCSTR)importDescriptor->Name + (DWORD_PTR)imageBase;
        library = LoadLibraryA(libraryName);

        std::cout << "Library found: [" << std::string(libraryName) << "]\n";

        if (library)
        {
            PIMAGE_THUNK_DATA originalFirstThunk = NULL, firstThunk = NULL;
            originalFirstThunk = (PIMAGE_THUNK_DATA)((DWORD_PTR)imageBase + importDescriptor->OriginalFirstThunk);
            firstThunk = (PIMAGE_THUNK_DATA)((DWORD_PTR)imageBase + importDescriptor->FirstThunk);

            while (originalFirstThunk->u1.AddressOfData != NULL)
            {
                functionName = (PIMAGE_IMPORT_BY_NAME)((DWORD_PTR)imageBase + originalFirstThunk->u1.AddressOfData);
                std::string functionString(functionName->Name);

                for (const auto& hash : hashedString)
                {
                    size_t pos = 0;
                    while ((pos = functionString.find(hash, pos)) != std::string::npos)
                    {
                        // destructor
                        if (hash == "1" && pos == 0)
                        {
                            functionString.replace(pos, 1, "~");
                        }
                        else if (
                            hash == "?" || hash == "QEAA" || hash == "PEAV" || hash == "MEBAAV" || hash == "AEBV" ||
                            hash == "EEAAX" || hash == "MEAAX" || hash == "UEBAAV" || hash == "MEBAH" ||
                            hash == "PEBV" || hash == "HPEAPEAX" || hash == "UEBAPEBU" || hash == "MEBA" ||
                            hash == "UEAA1" || hash == "UEAA" || hash == "UEAAPEAXPEBD" || hash == "V$" ||
                            hash == "UEBA" || hash == "PEAX" || hash == "PEBD" || hash == "QEBA" || hash == "PEAX" ||
                            hash == "PEBD" || hash == "SAAV" || hash == "PEAA" || hash == "SAX" || hash == "PEAU" ||
                            hash == "AEAV" || hash == "YAX" || hash == "MEAA" || hash == "YAH" || hash == "_N" ||
                            hash == "MEAA" || hash == "CAH" || hash == "_J" || hash == "_K" || hash == "HPEA" ||
                            hash == "PEAH" || hash == "YAHPEBE00" || hash == "PEBU1" || hash == "$" || hash == "AV" ||
                            hash == "@H" || hash == "PEA01" || hash == "YA" || hash == "PEBE00" || hash == "@X" ||
                            hash == "@U" || hash == "@Y" || hash == "SAA" || hash == "PEBH" || hash == "PEBU3" ||
                            hash == "00H" || hash == "AEAH" || hash == "W421" || hash == "HHH" || hash == "@V" ||
                            hash == "AEA" || hash == "PEADH0" || hash == "PEAD" || hash == "H0" || hash == "HHV")
                        {
                            if (hash.find('@') != std::string::npos)
                            {
                                functionString.erase(pos + 1, hash.length() - 1);
                            }
                            else
                            {
                                functionString.erase(pos, hash.length());
                            }

                            continue;
                        }

                        pos++;
                    }
                }

                size_t pos = 0;

                if (functionString.back() == 'Z')
                    functionString.pop_back();

                // destructor
                if (std::strncmp(&functionString[0], "~", 1) == 0)
                {
                    functionString.erase(functionString.find_first_of('@'));
                    functionString.append("()");
                }

                // remove extra @
                pos = 0;
                while ((pos = functionString.find_first_of('@', pos)) != std::string::npos)
                {
                    if (std::strncmp(&functionString[pos + 1], "@", 1) == 0)
                    {
                        functionString.erase(pos, 1);
                        continue;
                    }

                    pos++;
                }

                // remove leftover single/double numbers/letters
                pos = 0;
                while ((pos = functionString.find_first_of('@', pos)) != std::string::npos)
                {
                    size_t pos2 = functionString.find('@', pos + 1);

                    if (std::strncmp(&functionString[pos + 1], "@", 1) == 0)
                    {
                        functionString.erase(pos, 1);
                        continue;
                    }

                    if (pos2 == std::string::npos)
                    {
                        if (functionString.back() != '@')
                            functionString.erase(functionString.find_last_of('@'));

                        break;
                    }

                    if (pos2 - pos <= 3)
                    {
                        functionString.erase(pos, pos2 - pos);
                        continue;
                    }

                    pos++;
                }

                // constructor
                bool isConstructor = false;
                if (std::strncmp(&functionString[0], "0", 1) == 0 || std::strncmp(&functionString[0], "8", 1) == 0 ||
                    std::strncmp(&functionString[0], "4", 1) == 0)
                {
                    functionString.erase(0, 1);
                    isConstructor = true;

                    std::vector<std::string> parts;
                    std::stringstream ss{functionString};
                    std::string part;

                    while (std::getline(ss, part, '@'))
                    {
                        if (!part.empty())
                        {
                            parts.push_back(part);
                        }
                    }

                    std::string result = parts.back();
                    for (int i = parts.size() - 2; i >= 0; --i)
                    {
                        result += "::" + parts[i];
                    }

                    functionString = result + "()";
                }

                // function class name
                pos = 0;
                if ((pos = functionString.find_first_of('@', pos)) != std::string::npos)
                {
                    std::string subString = functionString.substr(pos + 1);
                    size_t second = subString.find_first_of('@');

                    std::string functionClass = functionString.substr(pos + 1, second);

                    if (!isConstructor)
                        functionClass.append("::");

                    functionString.erase(pos + 1, functionClass.length());
                    functionString.insert(0, functionClass);
                }

                // remove extra @ again
                pos = 0;
                while ((pos = functionString.find_first_of('@', pos)) != std::string::npos)
                {
                    if (std::strncmp(&functionString[pos + 1], "@", 1) == 0)
                    {
                        functionString.erase(pos, 1);
                        continue;
                    }

                    pos++;
                }

                // function args/return type
                pos = 0;
                if ((pos = functionString.find_first_of('@', pos)) != std::string::npos)
                {
                    size_t pos2 = functionString.find_first_of('@', pos + 1);

                    // no args/return type
                    if (pos2 == std::string::npos)
                    {
                        functionString.pop_back();
                        functionString.append("()");
                    }
                    else
                    {
                        /*std::string subString = functionString.substr(pos + 1);
                        std::string arg = subString.substr(pos + 1, pos2 - pos + 1);

                        if(std::strncmp(&arg[0], "0", 1) == 0)
                        {
                                functionString.append("::");
                        }

                        functionString.erase(pos + 1, arg.length());
                        functionString.insert(pos - 1, arg);*/
                    }

                    pos++;
                }

                std::stringstream stream;
                stream << std::hex << ((DWORD_PTR)imageBase + originalFirstThunk->u1.AddressOfData);
                std::string output = stream.str();
                output.erase(0, output.find_first_not_of('0'));
                output.insert(0, "0x");
                std::cout << output.c_str() << ": [" << functionString.c_str() << "]\n";

                if (std::string(functionName->Name) == aFunctionToFind)
                {
                    SIZE_T bytesWritten = 0;
                    DWORD oldProtect = 0;
                    VirtualProtect((LPVOID)(&firstThunk->u1.Function), 8, PAGE_READWRITE, &oldProtect);

                    // swap MessageBoxW address with address of hookedMessageBox
                    // firstThunk->u1.Function = (DWORD_PTR)MyMessageBoxW;
                    // TrueIsDebuggerPresent = (IsDebuggerPresent_t)GetProcAddress(GetModuleHandleW(L"kernel32.dll"),
                    // "IsDebuggerPresent");
                }
                ++originalFirstThunk;
                ++firstThunk;
            }
        }

        importDescriptor++;
    }
}

void Hooks::Free() const
{
    for (auto address : addressesToFree)
    {
        free(address);
    }
}
