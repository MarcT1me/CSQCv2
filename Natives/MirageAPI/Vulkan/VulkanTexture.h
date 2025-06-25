#pragma once

#include "VulkanContext.h"
#include "VulkanEnums.h"

namespace MirageAPI::Vulkan
{
    public ref class VulkanTexture
    {
        VkImage texture = VK_NULL_HANDLE;
        VkImageView view = VK_NULL_HANDLE;
        VkDeviceMemory memory = VK_NULL_HANDLE;
        VulkanContext^ context;
        int width, height;
        PixelFormat format;

    internal:
        static VkFormat GetNativeFormat(PixelFormat format);

    public:
        property VkImageView View { VkImageView get() { return view; } }
        property int Width { int get() { return width; } }
        property int Height { int get() { return height; } }
        property PixelFormat Format { PixelFormat get() { return format; } }

        VulkanTexture(VulkanContext^ context, array<System::Byte>^ data, int width, int height, PixelFormat format);
        VulkanTexture(VulkanContext^ context, int width, int height, PixelFormat format);
        ~VulkanTexture();

        void LoadFromData(array<System::Byte>^ data);
        void CreateRenderTarget();
    };
}
