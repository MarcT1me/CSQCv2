#include "pch.h"
#include "VulkanPipeline.h"

namespace MirageAPI::Vulkan
{
    VulkanPipeline::VulkanPipeline(VulkanContext^ ctx) : context(ctx)
    {
    }

    VulkanPipeline::~VulkanPipeline() { this->!VulkanPipeline(); }

    VulkanPipeline::!VulkanPipeline()
    {
        if (pipeline)
        {
            vkDestroyPipeline(context->Device, pipeline, nullptr);
            pipeline = VK_NULL_HANDLE;
        }

        if (pipelineLayout)
        {
            vkDestroyPipelineLayout(context->Device, pipelineLayout, nullptr);
            pipelineLayout = VK_NULL_HANDLE;
        }
    }

    void VulkanPipeline::CreateRTXPipeline()
    {
        if (!context->IsRTXSupported)
        {
            throw gcnew System::Exception("RTX is not supported on this device");
        }
    }

    void VulkanPipeline::CreateGraphicsPipeline()
    {
        VkPipelineShaderStageCreateInfo shaderStages[2] = {};
    }
}
