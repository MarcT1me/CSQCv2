#include "pch.h"
#include "VulkanSurface.h"

namespace MirageAPI::Vulkan
{
    VulkanSurface::VulkanSurface(VulkanContext^ ctx, VkSurfaceKHR s)
        : context(ctx), surface(s)
    {
        swapChainImages = gcnew System::Collections::Generic::List<VkImage>();
        swapChainImageViews = gcnew System::Collections::Generic::List<VkImageView>();
        swapChainFramebuffers = gcnew System::Collections::Generic::List<VkFramebuffer>();
        
        imageAvailableSemaphores = gcnew System::Collections::Generic::List<VkSemaphore>();
        renderFinishedSemaphores = gcnew System::Collections::Generic::List<VkSemaphore>();
        inFlightFences = gcnew System::Collections::Generic::List<VkFence>();
        
        CreateSwapChain();
        CreateImageViews();
        CreateRenderPass();
        CreateFramebuffers();
        CreateSyncObjects();
    }

    VulkanSurface::~VulkanSurface()
    {
        CleanupSwapChain();

        for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
        {
            vkDestroySemaphore(context->Device, imageAvailableSemaphores[i], nullptr);
            vkDestroySemaphore(context->Device, renderFinishedSemaphores[i], nullptr);
            vkDestroyFence(context->Device, inFlightFences[i], nullptr);
        }
    }

    void VulkanSurface::CreateSyncObjects()
    {
        VkSemaphoreCreateInfo semaphoreInfo{};
        semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

        VkFenceCreateInfo fenceInfo{};
        fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
        fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

        for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
        {
            VkSemaphore imageSemaphore;
            if (vkCreateSemaphore(context->Device, &semaphoreInfo, nullptr, &imageSemaphore) != VK_SUCCESS)
                throw gcnew System::Exception("Failed to create synchronization objects (imageSemaphore)!");
            imageAvailableSemaphores->Add(imageSemaphore);
            
            VkSemaphore renderSemaphore;
            if (vkCreateSemaphore(context->Device, &semaphoreInfo, nullptr, &renderSemaphore) != VK_SUCCESS)
                throw gcnew System::Exception("Failed to create synchronization objects (renderSemaphore)!");
            renderFinishedSemaphores->Add(renderSemaphore);

            VkFence fence;
            if (vkCreateFence(context->Device, &fenceInfo, nullptr, &fence) != VK_SUCCESS)
                throw gcnew System::Exception("Failed to create synchronization objects (fence)!");
            inFlightFences->Add(fence);
        }
    }

    void VulkanSurface::CreateSwapChain()
    {
        VkSurfaceCapabilitiesKHR capabilities;
        vkGetPhysicalDeviceSurfaceCapabilitiesKHR(context->PhysicalDevice, surface, &capabilities);

        uint32_t formatCount;
        vkGetPhysicalDeviceSurfaceFormatsKHR(context->PhysicalDevice, surface, &formatCount, nullptr);

        std::vector<VkSurfaceFormatKHR> formats(formatCount);
        vkGetPhysicalDeviceSurfaceFormatsKHR(context->PhysicalDevice, surface, &formatCount, formats.data());

        // Выбираем формат поверхности
        VkSurfaceFormatKHR surfaceFormat = formats[0];
        for (const auto& availableFormat : formats)
        {
            if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB &&
                availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
            {
                surfaceFormat = availableFormat;
                break;
            }
        }
        swapChainImageFormat = surfaceFormat.format;

        // Выбираем режим презентации
        uint32_t presentModeCount;
        vkGetPhysicalDeviceSurfacePresentModesKHR(context->PhysicalDevice, surface, &presentModeCount, nullptr);

        std::vector<VkPresentModeKHR> presentModes(presentModeCount);
        vkGetPhysicalDeviceSurfacePresentModesKHR(context->PhysicalDevice, surface, &presentModeCount,
                                                  presentModes.data());

        VkPresentModeKHR presentMode = VK_PRESENT_MODE_FIFO_KHR;
        for (const auto& availablePresentMode : presentModes)
        {
            if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR)
            {
                presentMode = availablePresentMode;
                break;
            }
        }

        // Определяем размеры swapchain
        if (capabilities.currentExtent.width != UINT32_MAX)
        {
            swapChainExtent = capabilities.currentExtent;
        }
        else
        {
            // Если размер не определен, используем размер окна
            swapChainExtent = {
                static_cast<uint32_t>(capabilities.currentExtent.width),
                static_cast<uint32_t>(capabilities.currentExtent.height)
            };
        }

        // Создаем swap chain
        VkSwapchainCreateInfoKHR createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
        createInfo.surface = surface;
        createInfo.minImageCount = capabilities.minImageCount + 1;

        VkFormat imageFormat = swapChainImageFormat;
        createInfo.imageFormat = imageFormat;
        createInfo.imageColorSpace = surfaceFormat.colorSpace;
        createInfo.imageExtent = swapChainExtent;
        createInfo.imageArrayLayers = 1;
        createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
        createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;

        createInfo.preTransform = capabilities.currentTransform;
        createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
        createInfo.presentMode = presentMode;
        createInfo.clipped = VK_TRUE;

        VkSwapchainKHR sChain;
        if (vkCreateSwapchainKHR(context->Device, &createInfo, nullptr, &sChain) != VK_SUCCESS)
        {
            throw gcnew System::Exception("Failed to create swap chain!");
        }
        swapChain = sChain;

        uint32_t imageCount;
        vkGetSwapchainImagesKHR(context->Device, swapChain, &imageCount, nullptr);
        vkGetSwapchainImagesKHR(context->Device, swapChain, &imageCount, &swapChainImages);
    }

    void VulkanSurface::CreateImageViews()
    {
        for (size_t i = 0; i < swapChainImages->Count; i++)
        {
            VkImageViewCreateInfo createInfo{};
            createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
            createInfo.image = swapChainImages[i];
            createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;

            VkFormat imageFormat = swapChainImageFormat;
            createInfo.format = imageFormat;
            createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
            createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
            createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
            createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

            createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            createInfo.subresourceRange.baseMipLevel = 0;
            createInfo.subresourceRange.levelCount = 1;
            createInfo.subresourceRange.baseArrayLayer = 0;
            createInfo.subresourceRange.layerCount = 1;

            if (vkCreateImageView(context->Device, &createInfo, nullptr, &swapChainImageViews[i]) != VK_SUCCESS)
            {
                throw gcnew System::Exception("Failed to create image views!");
            }
        }
    }

    void VulkanSurface::CreateRenderPass()
    {
        VkAttachmentDescription colorAttachment{};

        VkFormat colorFormat = swapChainImageFormat;
        colorAttachment.format = colorFormat;
        colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
        colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
        colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

        VkAttachmentReference colorAttachmentRef;
        colorAttachmentRef.attachment = 0;
        colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

        VkSubpassDescription subpass{};
        subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
        subpass.colorAttachmentCount = 1;
        subpass.pColorAttachments = &colorAttachmentRef;

        VkRenderPassCreateInfo renderPassInfo{};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
        renderPassInfo.attachmentCount = 1;
        renderPassInfo.pAttachments = &colorAttachment;
        renderPassInfo.subpassCount = 1;
        renderPassInfo.pSubpasses = &subpass;

        VkRenderPass rPass;
        if (vkCreateRenderPass(context->Device, &renderPassInfo, nullptr, &rPass) != VK_SUCCESS)
        {
            throw gcnew System::Exception("Failed to create render pass!");
        }
        renderPass = rPass;
    }

    void VulkanSurface::CreateFramebuffers()
    {
        for (size_t i = 0; i < swapChainImageViews->Count; i++)
        {
            VkImageView attachments[] = {swapChainImageViews[i]};

            VkFramebufferCreateInfo framebufferInfo{};
            framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
            framebufferInfo.renderPass = renderPass;
            framebufferInfo.attachmentCount = 1;
            framebufferInfo.pAttachments = attachments;
            framebufferInfo.width = swapChainExtent.width;
            framebufferInfo.height = swapChainExtent.height;
            framebufferInfo.layers = 1;

            if (vkCreateFramebuffer(context->Device, &framebufferInfo, nullptr, &swapChainFramebuffers[i]) !=
                VK_SUCCESS)
            {
                throw gcnew System::Exception("Failed to create framebuffer!");
            }
        }
    }

    void VulkanSurface::CleanupSwapChain()
    {
        for (auto framebuffer : swapChainFramebuffers)
        {
            vkDestroyFramebuffer(context->Device, framebuffer, nullptr);
        }
        for (auto imageView : swapChainImageViews)
        {
            vkDestroyImageView(context->Device, imageView, nullptr);
        }
        if (renderPass != VK_NULL_HANDLE)
        {
            vkDestroyRenderPass(context->Device, renderPass, nullptr);
        }
        if (swapChain != VK_NULL_HANDLE)
        {
            vkDestroySwapchainKHR(context->Device, swapChain, nullptr);
        }
    }

    void VulkanSurface::RecreateSwapChain()
    {
        CleanupSwapChain();

        CreateSwapChain();
        CreateImageViews();
        CreateRenderPass();
        CreateFramebuffers();
    }

    void VulkanSurface::Resize(int width, int height)
    {
        framebufferResized = true;
    }

    void VulkanSurface::BeginFrame()
    {
        vkWaitForFences(context->Device, 1, &inFlightFences[currentFrame], VK_TRUE, UINT64_MAX);

        VkResult result = vkAcquireNextImageKHR(
            context->Device,
            swapChain,
            UINT64_MAX,
            imageAvailableSemaphores[currentFrame],
            VK_NULL_HANDLE,
            static_cast<uint32_t*>(&imageIndex));

        if (result == VK_ERROR_OUT_OF_DATE_KHR)
        {
            RecreateSwapChain();
            return;
        }
        if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
        {
            throw gcnew System::Exception("Failed to acquire swap chain image!");
        }

        vkResetFences(context->Device, 1, &inFlightFences[currentFrame]);
    }

    void VulkanSurface::EndFrame(VkCommandBuffer commandBuffer)
    {
        VkSubmitInfo submitInfo{};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

        VkSemaphore waitSemaphores[] = {imageAvailableSemaphores[currentFrame]};
        VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
        submitInfo.waitSemaphoreCount = 1;
        submitInfo.pWaitSemaphores = waitSemaphores;
        submitInfo.pWaitDstStageMask = waitStages;
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &commandBuffer;

        VkSemaphore signalSemaphores[] = {renderFinishedSemaphores[currentFrame]};
        submitInfo.signalSemaphoreCount = 1;
        submitInfo.pSignalSemaphores = signalSemaphores;

        if (vkQueueSubmit(context->GraphicsQueue, 1, &submitInfo, inFlightFences[currentFrame]) != VK_SUCCESS)
        {
            throw gcnew System::Exception("Failed to submit draw command buffer!");
        }
    }

    void VulkanSurface::Present()
    {
        VkPresentInfoKHR presentInfo{};
        presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
        presentInfo.waitSemaphoreCount = 1;
        presentInfo.pWaitSemaphores = &renderFinishedSemaphores[currentFrame];

        VkSwapchainKHR swapChains[] = {swapChain};
        presentInfo.swapchainCount = 1;
        presentInfo.pSwapchains = swapChains;
        const uint32_t imageCount = imageIndex;
        presentInfo.pImageIndices = &imageCount;

        VkResult result = vkQueuePresentKHR(context->PresentQueue, &presentInfo);

        if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || framebufferResized)
        {
            framebufferResized = false;
            RecreateSwapChain();
        }
        else if (result != VK_SUCCESS)
        {
            throw gcnew System::Exception("Failed to present swap chain image!");
        }

        currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
    }
}
