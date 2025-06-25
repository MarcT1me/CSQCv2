#pragma once
#include "VulkanContext.h"
#include "VulkanPipeline.h"
#include "VulkanTexture.h"
#include "VulkanSurface.h"

#include "VulkanEnums.h"


namespace MirageAPI::Vulkan
{
    public ref class VulkanRenderer
    {
        VulkanContext^ context;
        VulkanSurface^ surface;
        VulkanPipeline^ pipeline;

        VkCommandPool commandPool = VK_NULL_HANDLE;
        VkCommandBuffer commandBuffer = VK_NULL_HANDLE;

        void CreateCommandPool();
        void CreateCommandBuffer();

    public:
        VulkanRenderer(VulkanContext^ context, VulkanSurface^ surface, VulkanPipeline^ pipeline);
        ~VulkanRenderer();

        void BeginRenderPass(ClearColor  clearColor);
        void EndRenderPass();
        void SubmitCommands();

        void DrawTexture(VulkanTexture^ texture, int x, int y, int width, int height);
        void DrawRect(int x, int y, int width, int height, ClearColor color);
    };
}
