#include "pch.h"
#include "VulkanRenderer.h"

namespace MirageAPI::Vulkan
{
    //     VulkanRenderer::VulkanRenderer(VulkanPipeline^ pipe, Window::NativeWindow^ win)
    //         : pipeline(pipe), window(win)
    //     {
    //         CreateSwapChain();
    //         CreateImageViews();
    //         CreateRenderPass();
    //         CreateFramebuffers();
    //         CreateCommandPool();
    //         CreateCommandBuffers();
    //     }
    //
    //     VulkanRenderer::~VulkanRenderer()
    //     {
    //         CleanupSwapChain();
    //     }
    //
    //     VulkanRenderer::!VulkanRenderer()
    //     {
    //     }
    //
    //     void VulkanRenderer::CleanupSwapChain()
    //     {
    //         for (auto framebuffer : swapChainFramebuffers)
    //         {
    //             vkDestroyFramebuffer(context->Device, framebuffer, nullptr);
    //         }
    //
    //         for (auto imageView : swapChainImageViews)
    //         {
    //             vkDestroyImageView(context->Device, imageView, nullptr);
    //         }
    //
    //         if (renderPass != VK_NULL_HANDLE)
    //         {
    //             vkDestroyRenderPass(context->Device, renderPass, nullptr);
    //             renderPass = VK_NULL_HANDLE;
    //         }
    //
    //         if (swapChain != VK_NULL_HANDLE)
    //         {
    //             vkDestroySwapchainKHR(context->Device, swapChain, nullptr);
    //             swapChain = VK_NULL_HANDLE;
    //         }
    //     }
    //
    //     void VulkanRenderer::RecreateSwapChain()
    //     {
    //         CleanupSwapChain();
    //         CreateSwapChain();
    //         CreateImageViews();
    //         CreateRenderPass();
    //         CreateFramebuffers();
    //         CreateCommandBuffers();
    //     }
    //
    //
    //     void VulkanRenderer::CreateSwapChain()
    //     {
    //         // Получаем возможности поверхности
    //         VkSurfaceCapabilitiesKHR capabilities;
    //         vkGetPhysicalDeviceSurfaceCapabilitiesKHR(context->PhysicalDevice, window->VulkanSurface, &capabilities);
    //
    //         // Выбираем формат поверхности
    //         uint32_t formatCount;
    //         vkGetPhysicalDeviceSurfaceFormatsKHR(context->PhysicalDevice, window->VulkanSurface, &formatCount, nullptr);
    //         std::vector<VkSurfaceFormatKHR> formats(formatCount);
    //         vkGetPhysicalDeviceSurfaceFormatsKHR(context->PhysicalDevice, window->VulkanSurface, &formatCount,
    //                                              formats.data());
    //
    //         swapChainImageFormat = formats[0].format;
    //
    //         // Создаем swap chain
    //         VkSwapchainCreateInfoKHR createInfo{};
    //         createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    //         createInfo.surface = window->VulkanSurface;
    //         createInfo.minImageCount = capabilities.minImageCount + 1;
    //         createInfo.imageFormat = swapChainImageFormat;
    //         createInfo.imageColorSpace = formats[0].colorSpace;
    //         createInfo.imageExtent = capabilities.currentExtent;
    //         createInfo.imageArrayLayers = 1;
    //         createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
    //         createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
    //         createInfo.preTransform = capabilities.currentTransform;
    //         createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    //         createInfo.presentMode = VK_PRESENT_MODE_FIFO_KHR; // Всегда поддерживается
    //         createInfo.clipped = VK_TRUE;
    //
    //         if (vkCreateSwapchainKHR(context->Device, &createInfo, nullptr, &swapChain) != VK_SUCCESS)
    //         {
    //             throw gcnew System::Exception("Failed to create swap chain!");
    //         }
    //
    //         // Получаем изображения swap chain
    //         uint32_t imageCount;
    //         vkGetSwapchainImagesKHR(context->Device, swapChain, &imageCount, nullptr);
    //         swapChainImages.resize(imageCount);
    //         vkGetSwapchainImagesKHR(context->Device, swapChain, &imageCount, swapChainImages.data());
    //
    //         swapChainExtent = capabilities.currentExtent;
    //     }
    //     
    //     void VulkanRenderer::CreateFramebuffers()
    //     {
    //         swapChainFramebuffers.resize(swapChainImageViews.size());
    //         
    //         for (size_t i = 0; i < swapChainImageViews.size(); i++) {
    //             VkImageView attachments[] = {
    //                 swapChainImageViews[i]
    //             };
    //             
    //             VkFramebufferCreateInfo framebufferInfo{};
    //             framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    //             framebufferInfo.renderPass = renderPass;
    //             framebufferInfo.attachmentCount = 1;
    //             framebufferInfo.pAttachments = attachments;
    //             framebufferInfo.width = swapChainExtent.width;
    //             framebufferInfo.height = swapChainExtent.height;
    //             framebufferInfo.layers = 1;
    //             
    //             if (vkCreateFramebuffer(context->Device, &framebufferInfo, nullptr, &swapChainFramebuffers[i]) != VK_SUCCESS) {
    //                 throw gcnew System::Exception("Failed to create framebuffer!");
    //             }
    //         }
    //     }
    //     
    //     void VulkanRenderer::CreateCommandPool()
    //     {
    //         // Находим семейство графических очередей
    //         uint32_t queueFamilyCount = 0;
    //         vkGetPhysicalDeviceQueueFamilyProperties(context->PhysicalDevice, &queueFamilyCount, nullptr);
    //         std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    //         vkGetPhysicalDeviceQueueFamilyProperties(context->PhysicalDevice, &queueFamilyCount, queueFamilies.data());
    //         
    //         int graphicsQueueFamily = -1;
    //         for (int i = 0; i < queueFamilies.size(); i++) {
    //             if (queueFamilies[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
    //                 graphicsQueueFamily = i;
    //                 break;
    //             }
    //         }
    //         
    //         VkCommandPoolCreateInfo poolInfo{};
    //         poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    //         poolInfo.queueFamilyIndex = graphicsQueueFamily;
    //         poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    //         
    //         if (vkCreateCommandPool(context->Device, &poolInfo, nullptr, &commandPool) != VK_SUCCESS) {
    //             throw gcnew System::Exception("Failed to create command pool!");
    //         }
    //     }
    //     
    //     void VulkanRenderer::CreateCommandBuffers()
    //     {
    //         commandBuffers.resize(swapChainFramebuffers.size());
    //         
    //         VkCommandBufferAllocateInfo allocInfo{};
    //         allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    //         allocInfo.commandPool = commandPool;
    //         allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    //         allocInfo.commandBufferCount = (uint32_t)commandBuffers.size();
    //         
    //         if (vkAllocateCommandBuffers(context->Device, &allocInfo, commandBuffers.data()) != VK_SUCCESS) {
    //             throw gcnew System::Exception("Failed to allocate command buffers!");
    //         }
    //     }
    //     
    //     void VulkanRenderer::DrawFrame(float r, float g, float b, float a)
    //     {
    //         // Получаем индекс изображения для отрисовки
    //         uint32_t imageIndex;
    //         vkAcquireNextImageKHR(
    //             context->Device, 
    //             swapChain, 
    //             UINT64_MAX, 
    //             VK_NULL_HANDLE, 
    //             VK_NULL_HANDLE, 
    //             &imageIndex
    //         );
    //         
    //         // Начинаем запись команд
    //         VkCommandBufferBeginInfo beginInfo{};
    //         beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    //         
    //         if (vkBeginCommandBuffer(commandBuffers[imageIndex], &beginInfo) != VK_SUCCESS) {
    //             throw gcnew System::Exception("Failed to begin recording command buffer!");
    //         }
    //         
    //         // Настраиваем проход рендеринга
    //         VkRenderPassBeginInfo renderPassInfo{};
    //         renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    //         renderPassInfo.renderPass = renderPass;
    //         renderPassInfo.framebuffer = swapChainFramebuffers[imageIndex];
    //         renderPassInfo.renderArea.offset = {0, 0};
    //         renderPassInfo.renderArea.extent = swapChainExtent;
    //         
    //         VkClearValue clearColor = {{{r, g, b, a}}};
    //         renderPassInfo.clearValueCount = 1;
    //         renderPassInfo.pClearValues = &clearColor;
    //         
    //         vkCmdBeginRenderPass(commandBuffers[imageIndex], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
    //         
    //         // Здесь можно добавить команды рисования, но для очистки экрана достаточно:
    //         vkCmdEndRenderPass(commandBuffers[imageIndex]);
    //         
    //         if (vkEndCommandBuffer(commandBuffers[imageIndex]) != VK_SUCCESS) {
    //             throw gcnew System::Exception("Failed to record command buffer!");
    //         }
    //         
    //         // Отправляем команды на выполнение
    //         VkSubmitInfo submitInfo{};
    //         submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    //         submitInfo.commandBufferCount = 1;
    //         submitInfo.pCommandBuffers = &commandBuffers[imageIndex];
    //         
    //         vkQueueSubmit(context->GraphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
    //         
    //         // Презентуем изображение
    //         VkPresentInfoKHR presentInfo{};
    //         presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    //         presentInfo.swapchainCount = 1;
    //         presentInfo.pSwapchains = &swapChain;
    //         presentInfo.pImageIndices = &imageIndex;
    //         
    //         vkQueuePresentKHR(context->GraphicsQueue, &presentInfo);
    //         
    //         // Ждем завершения операций (для простоты)
    //         vkQueueWaitIdle(context->GraphicsQueue);
    //     }
}
