#include "../sdk/sdk.hpp"

DWORD WINAPI Attach(LPVOID h_module) 
{
    /// Initializing
    sdk::setup();
    return 1ul;
};

BOOL APIENTRY DllMain(HMODULE h_module, DWORD ul_reason, LPVOID lp_reserved)
{
    if (ul_reason == DLL_PROCESS_ATTACH) 
    {
        DisableThreadLibraryCalls(h_module);
        sdk::gp_base = (void*)h_module;
        if (auto h_thread = CreateThread(nullptr, 0U, Attach, h_module, 0UL, nullptr); h_thread != nullptr)
            CloseHandle(h_thread);
        return TRUE;
    }
    return FALSE;
}

