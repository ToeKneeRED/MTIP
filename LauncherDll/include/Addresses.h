#pragma once

#define BASE_ADDRESS (reinterpret_cast<char*>(GetModuleHandle(nullptr)) + 0x1000) // 0x1000 for headers

#define BUILD_BUTTON_TEXT (reinterpret_cast<void*>(BASE_ADDRESS + (0x21D00)))