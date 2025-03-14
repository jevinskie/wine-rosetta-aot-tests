// Minimal DLL with a factorial function, no CRT dependency
#define WIN32_LEAN_AND_MEAN
#include <stddef.h>
#include <windows.h>

__declspec(dllexport) unsigned long long FACTORIAL_BASE                      = 1;
__declspec(dllexport) unsigned long long (*factorial_ptr)(unsigned int)      = NULL;
__declspec(dllexport) unsigned long long (**factorial_ptr_ref)(unsigned int) = &factorial_ptr;

__declspec(dllexport) unsigned long long factorial(unsigned int n) {
    unsigned long long result = FACTORIAL_BASE;
    for (unsigned int i = 2; i <= n; ++i) {
        result *= i;
    }
    return result;
}

// DLL entry point - Required for a standalone DLL
BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved) {
    *factorial_ptr_ref = factorial;
    return TRUE; // Do nothing, always succeed
}
