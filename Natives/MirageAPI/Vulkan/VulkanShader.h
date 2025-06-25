#pragma once
#include "VulkanContext.h"

namespace MirageAPI::Vulkan
{
    public ref class VulkanShader
    {
        VkShaderModule shaderModule = VK_NULL_HANDLE;
        VulkanContext^ context;
        VkShaderStageFlagBits stage;

    public:
        VulkanShader(VulkanContext^ context, array<System::Byte>^ shaderCode, VkShaderStageFlagBits stage);
        ~VulkanShader();

        VkPipelineShaderStageCreateInfo GetStageCreateInfo();
    };
}
