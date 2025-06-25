#include "pch.h"
#include "VulkanShader.h"

namespace MirageAPI::Vulkan
{
    VulkanShader::VulkanShader(VulkanContext^ ctx, array<System::Byte>^ shaderCode, VkShaderStageFlagBits shaderStage)
        : context(ctx), stage(shaderStage)
    {
        pin_ptr<System::Byte> codePtr = &shaderCode[0];
        const uint32_t* codeData = reinterpret_cast<const uint32_t*>(codePtr);
        size_t codeSize = shaderCode->Length;

        VkShaderModuleCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        createInfo.codeSize = codeSize;
        createInfo.pCode = codeData;

        VkShaderModule sMod;
        if (vkCreateShaderModule(context->Device, &createInfo, nullptr, &sMod) != VK_SUCCESS)
        {
            throw gcnew System::Exception("Failed to create shader module!");
        }
        shaderModule = sMod;
    }

    VkPipelineShaderStageCreateInfo VulkanShader::GetStageCreateInfo()
    {
        VkPipelineShaderStageCreateInfo stageInfo{};
        stageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        const VkShaderStageFlagBits stg = stage;
        stageInfo.stage = stg;
        stageInfo.module = shaderModule;
        stageInfo.pName = "main";
        return stageInfo;
    }

    VulkanShader::~VulkanShader()
    {
        if (shaderModule != VK_NULL_HANDLE)
        {
            vkDestroyShaderModule(context->Device, shaderModule, nullptr);
        }
    }
}
