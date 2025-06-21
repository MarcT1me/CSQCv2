#pragma once
#include <vulkan/vulkan.h>
#include "VulkanContext.h"

namespace MirageAPI::Vulkan
{
    public ref class VulkanPipeline
    {
        VkPipeline pipeline = VK_NULL_HANDLE;
        VkPipelineLayout pipelineLayout = VK_NULL_HANDLE;
        VulkanContext^ context;

    public:
        VulkanPipeline(VulkanContext^ context);
        ~VulkanPipeline();
        !VulkanPipeline();

        void CreateRTXPipeline();
        void CreateGraphicsPipeline();

        property VkPipeline Pipeline { VkPipeline get() { return pipeline; } }
        property VulkanContext^ Context { VulkanContext^ get() { return context; } }
    };
}
