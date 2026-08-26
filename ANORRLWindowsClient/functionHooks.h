#pragma once

namespace ARL {
    // Returns the new entry point
    void* hotpatchHook(void* origFunction, void* hookFunction);
    void* hotpatchUnhook(void* pfn);
    bool hookingApiHooked();

}