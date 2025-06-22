#pragma once

#include "VulkanContext.h"
#include "VulkanPipeline.h"
#include "../NativeWindow.h"
#include <vector>

namespace MirageAPI::Vulkan
{
    // class VulkanRenderer
    // {
    //     VulkanContext^ context;
    //     VulkanPipeline^ pipeline;
    //     Window::NativeWindow^ window;
    //
    //     VkSwapchainKHR swapChain = VK_NULL_HANDLE;
    //     std::vector<VkImage> swapChainImages;
    //     std::vector<VkImageView> swapChainImageViews;
    //     std::vector<VkFramebuffer> swapChainFramebuffers;
    //     VkFormat swapChainImageFormat;
    //     VkExtent2D swapChainExtent;
    //
    //     VkRenderPass renderPass = VK_NULL_HANDLE;
    //     VkCommandPool commandPool = VK_NULL_HANDLE;
    //     std::vector<VkCommandBuffer> commandBuffers;
    //
    //     void CreateSwapChain();
    //     void CreateImageViews();
    //     void CreateRenderPass();
    //     void CreateFramebuffers();
    //     void CreateCommandPool();
    //     void CreateCommandBuffers();
    //     void CleanupSwapChain();
    //
    // public:
    //     VulkanRenderer(VulkanPipeline^ pipeline, Window::NativeWindow^ window);
    //     ~VulkanRenderer();
    //     !VulkanRenderer();
    //
    //     void RecreateSwapChain();
    //     void DrawFrame(float r, float g, float b, float a);
    // };
}
