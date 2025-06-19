#pragma once

namespace MirageAPI
{
    public ref class MirageSystem
    {
    public:
        static bool init(int major, int minor);
        static void shutdown();
    };
}
