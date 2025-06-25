#pragma once

#include <vector>

#include "VulkanContext.h"


namespace MirageAPI::Vulkan
{
    public ref class VulkanSurface
    {
        VulkanContext^ context;
        VkSurfaceKHR surface;

    internal:
        const int MAX_FRAMES_IN_FLIGHT = 2;
        uint32_t currentFrame = 0;
        uint32_t imageIndex = 0;
        bool framebufferResized = false;

        System::Collections::Generic::List<VkImage>^ swapChainImages;
        System::Collections::Generic::List<VkImageView>^ swapChainImageViews;
        System::Collections::Generic::List<VkFramebuffer>^ swapChainFramebuffers;

        VkRenderPass renderPass = VK_NULL_HANDLE;
        VkSwapchainKHR swapChain = VK_NULL_HANDLE;

        VkFormat swapChainImageFormat;
        VkExtent2D swapChainExtent;

        System::Collections::Generic::List<VkSemaphore>^ imageAvailableSemaphores;
        System::Collections::Generic::List<VkSemaphore>^ renderFinishedSemaphores;
        System::Collections::Generic::List<VkFence>^ inFlightFences;

        void CreateSwapChain();
        void CreateImageViews();
        void CreateRenderPass();
        void CreateFramebuffers();
        void CreateSyncObjects();

        void RecreateSwapChain();
        void CleanupSwapChain();

    public:
        property VkSurfaceKHR Surface { VkSurfaceKHR get() { return surface; } }
        property VulkanContext^ Context { VulkanContext^ get() { return context; } }

        property VkRenderPass RenderPass { VkRenderPass get() { return renderPass; } }
        property VkExtent2D Extent { VkExtent2D get() { return swapChainExtent; } }

        VulkanSurface(VulkanContext^ context, VkSurfaceKHR surface);
        ~VulkanSurface();

        void Resize(int width, int height);
        void BeginFrame();
        void EndFrame(VkCommandBuffer commandBuffer);
        void Present();
    };
}
