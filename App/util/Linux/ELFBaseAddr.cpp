#include "util/ELFBaseAddr.h"
#include <string>

struct TextSegmentInfo
{
    uintptr_t text_vaddr;
    size_t    text_memsz;
    uintptr_t base_addr;
    bool      found;
};

#ifdef EMSCRIPTEN

uintptr_t staticElfTextVAddr()
{
    return 0;
}

intptr_t imageSlideElf()
{
    return 0;
}

uintptr_t elfDynamicBaseAddress()
{
    return 0;
}

size_t elfTextSize()
{
    return 0;
}

#else

#include <link.h>
#include <unistd.h>
#include <limits.h>
#include <cstring>

static int phdrCallback(struct dl_phdr_info* info, size_t /*size*/, void* data)
{
    TextSegmentInfo* out = reinterpret_cast<TextSegmentInfo*>(data);

    bool is_main_image = (info->dlpi_name == nullptr) || (info->dlpi_name[0] == '\0');

    if (!is_main_image)
    {
        char exe_path[PATH_MAX];
        ssize_t len = readlink("/proc/self/exe", exe_path, sizeof(exe_path) - 1);

        if (len > 0)
        {
            exe_path[len] = '\0';

            if (strcmp(info->dlpi_name, exe_path) == 0)
            {
                is_main_image = true;
            }
        }
    }

    if (!is_main_image)
    {
        return 0;
    }

    for (unsigned i = 0; i < info->dlpi_phnum; ++i)
    {
        const ElfW(Phdr)& ph = info->dlpi_phdr[i];

        if (ph.p_type == PT_LOAD && (ph.p_flags & PF_X))
        {
            out->text_vaddr = static_cast<uintptr_t>(ph.p_vaddr);
            out->text_memsz = static_cast<size_t>(ph.p_memsz);
            out->base_addr  = static_cast<uintptr_t>(info->dlpi_addr);
            out->found = true;
            return 1; // stop
        }
    }

    return 0; // continue
}

uintptr_t staticElfTextVAddr()
{
    TextSegmentInfo info = {};
    info.found = false;

    dl_iterate_phdr(phdrCallback, &info);

    return info.found ? info.text_vaddr : 0;
}

intptr_t imageSlideElf()
{
    TextSegmentInfo info = {};
    info.found = false;

    dl_iterate_phdr(phdrCallback, &info);

    return info.found ? static_cast<intptr_t>(info.base_addr) : 0;
}

uintptr_t elfDynamicBaseAddress()
{
    TextSegmentInfo info = {};
    info.found = false;

    dl_iterate_phdr(phdrCallback, &info);

    if (!info.found)
    {
        return 0;
    }

    return info.base_addr + info.text_vaddr;
}

size_t elfTextSize()
{
    TextSegmentInfo info = {};
    info.found = false;

    dl_iterate_phdr(phdrCallback, &info);

    return info.found ? info.text_memsz : 0;
}

#endif 