#pragma once
#include <vulkan/vulkan.h>
#include "VulkanContext.h"
#include "VulkanShader.h"

namespace MirageAPI::Vulkan
{
    public ref class VulkanPipeline
    {
        VkPipeline texturePipeline = VK_NULL_HANDLE;
        VkPipeline solidColorPipeline = VK_NULL_HANDLE;
        VkPipelineLayout pipelineLayout = VK_NULL_HANDLE;
        VulkanContext^ context;
        VkRenderPass renderPass;

        void CreateTexturePipeline(VulkanShader^ vertShader,
                                   VulkanShader^ fragShader);
        void CreateSolidColorPipeline(VulkanShader^ vertShader,
                                      VulkanShader^ fragShader);

    public:
        VulkanPipeline(VulkanContext^ context, VkRenderPass renderPass);
        ~VulkanPipeline();
        !VulkanPipeline();

        void CreatePipelines(
            VulkanShader^ textureVertShader,
            VulkanShader^ textureFragShader,
            VulkanShader^ solidVertShader,
            VulkanShader^ solidFragShader
        );

        property VkPipeline TexturePipeline { VkPipeline get() { return texturePipeline; } }
        property VkPipeline SolidColorPipeline { VkPipeline get() { return solidColorPipeline; } }
        property VkPipelineLayout PipelineLayout { VkPipelineLayout get() { return pipelineLayout; } }
    };
}
