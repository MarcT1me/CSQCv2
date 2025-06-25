#include "pch.h"
#include "VulkanRenderer.h"

#include "VulkanEnums.h"

namespace MirageAPI::Vulkan
{
    VulkanRenderer::VulkanRenderer(VulkanContext^ ctx, VulkanSurface^ surf, VulkanPipeline^ pipe)
        : context(ctx), surface(surf), pipeline(pipe)
    {
        CreateCommandPool();
        CreateCommandBuffer();
    }

    VulkanRenderer::~VulkanRenderer()
    {
        if (commandPool != VK_NULL_HANDLE)
        {
            vkDestroyCommandPool(context->Device, commandPool, nullptr);
        }
    }

    void VulkanRenderer::CreateCommandPool()
    {
        VkCommandPoolCreateInfo poolInfo{};
        poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        poolInfo.queueFamilyIndex = context->GraphicsQueueFamilyIndex;
        poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

        VkCommandPool cPool;
        if (vkCreateCommandPool(context->Device, &poolInfo, nullptr, &cPool) != VK_SUCCESS)
        {
            throw gcnew System::Exception("Failed to create command pool!");
        }
        commandPool = cPool;
    }

    void VulkanRenderer::CreateCommandBuffer()
    {
        VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.commandPool = commandPool;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandBufferCount = 1;

        VkCommandBuffer cBuffer;
        if (vkAllocateCommandBuffers(context->Device, &allocInfo, &cBuffer) != VK_SUCCESS)
        {
            throw gcnew System::Exception("Failed to allocate command buffers!");
        }
        commandBuffer = cBuffer;
    }

    void VulkanRenderer::BeginRenderPass(ClearColor clearColor)
    {
        VkClearValue clearValue;
        clearValue.color = {{clearColor.R, clearColor.G, clearColor.B, clearColor.A}};

        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

        if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS)
        {
            throw gcnew System::Exception("Failed to begin recording command buffer!");
        }

        VkRenderPassBeginInfo renderPassInfo{};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.renderPass = surface->RenderPass;
        renderPassInfo.framebuffer = surface->swapChainFramebuffers[surface->imageIndex];
        renderPassInfo.renderArea.offset = {0, 0};
        renderPassInfo.renderArea.extent = surface->Extent;
        renderPassInfo.clearValueCount = 1;
        renderPassInfo.pClearValues = &clearValue;

        vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
    }

    void VulkanRenderer::EndRenderPass()
    {
        vkCmdEndRenderPass(commandBuffer);
        if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS)
        {
            throw gcnew System::Exception("Failed to record command buffer!");
        }
    }

    void VulkanRenderer::SubmitCommands()
    {
        surface->EndFrame(commandBuffer);
        surface->Present();
    }

    void VulkanRenderer::DrawTexture(VulkanTexture^ texture, int x, int y, int width, int height)
    {
        // 1. Устанавливаем пайплайн
        vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline->TexturePipeline);

        // 2. Устанавливаем viewport и scissor
        VkViewport viewport;
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        viewport.width = static_cast<float>(surface->Extent.width);
        viewport.height = static_cast<float>(surface->Extent.height);
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;
        vkCmdSetViewport(commandBuffer, 0, 1, &viewport);

        VkRect2D scissor;
        scissor.offset = {0, 0};
        scissor.extent = surface->Extent;
        vkCmdSetScissor(commandBuffer, 0, 1, &scissor);

        // 3. Рисуем прямоугольник с текстурой
        // (В реальной реализации потребуются вертексный буфер и дескрипторы)
        vkCmdDraw(commandBuffer, 6, 1, 0, 0);
    }

    void VulkanRenderer::DrawRect(int x, int y, int width, int height, ClearColor color)
    {
        // Используем правильные свойства
        vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline->SolidColorPipeline);

        float colorValue[4] = {color.R, color.G, color.B, color.A};
        vkCmdPushConstants(
            commandBuffer,
            pipeline->PipelineLayout, // Исправленный доступ
            VK_SHADER_STAGE_FRAGMENT_BIT,
            0,
            sizeof(colorValue),
            colorValue
        );

        vkCmdDraw(commandBuffer, 6, 1, 0, 0);
    }
}
