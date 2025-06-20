#pragma once

namespace MirageAPI
{
    public ref class MirageSystem
    {
    public:
        static bool Init(int major, int minor);
        static void Shutdown();
    };
}
