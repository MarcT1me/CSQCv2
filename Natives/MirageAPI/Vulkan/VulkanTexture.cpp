#include "pch.h"
#include "VulkanTexture.h"
#include "VulkanEnums.h"

namespace MirageAPI::Vulkan
{
    VkFormat VulkanTexture::GetNativeFormat(PixelFormat format)
    {
        switch (format)
        {
        case PixelFormat::RGBA_SRGB: return VK_FORMAT_R8G8B8A8_SRGB;
        case PixelFormat::BGRA_SRGB: return VK_FORMAT_B8G8R8A8_SRGB;
        case PixelFormat::RGBA_FLOAT: return VK_FORMAT_R32G32B32A32_SFLOAT;
        default: throw gcnew System::ArgumentException("Unsupported pixel format");
        }
    }

    VulkanTexture::VulkanTexture(VulkanContext^ ctx, array<System::Byte>^ data, int w, int h, PixelFormat fmt)
        : context(ctx), width(w), height(h), format(fmt)
    {
        LoadFromData(data);
    }

    VulkanTexture::VulkanTexture(VulkanContext^ ctx, int w, int h, PixelFormat fmt)
        : context(ctx), width(w), height(h), format(fmt)
    {
        CreateRenderTarget();
    }

    VulkanTexture::~VulkanTexture()
    {
        if (view != VK_NULL_HANDLE)
        {
            vkDestroyImageView(context->Device, view, nullptr);
        }
        if (texture != VK_NULL_HANDLE)
        {
            vkDestroyImage(context->Device, texture, nullptr);
        }
        if (memory != VK_NULL_HANDLE)
        {
            vkFreeMemory(context->Device, memory, nullptr);
        }
    }

    void VulkanTexture::LoadFromData(array<System::Byte>^ data)
    {
        VkFormat form = GetNativeFormat(format);

        // Конвертируем управляемый массив в нативный указатель
        pin_ptr<System::Byte> pinnedData = &data[0];
        const uint8_t* imageData = pinnedData;
        VkDeviceSize imageSize = data->Length;

        // 1. Создаем staging buffer
        VkBuffer stagingBuffer;
        VkDeviceMemory stagingBufferMemory;

        VkBufferCreateInfo bufferInfo{};
        bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        bufferInfo.size = imageSize;
        bufferInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
        bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

        if (vkCreateBuffer(context->Device, &bufferInfo, nullptr, &stagingBuffer) != VK_SUCCESS)
        {
            throw gcnew System::Exception("Failed to create staging buffer!");
        }

        // 2. Выделяем память
        VkMemoryRequirements memRequirements;
        vkGetBufferMemoryRequirements(context->Device, stagingBuffer, &memRequirements);

        VkMemoryAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        allocInfo.allocationSize = memRequirements.size;
        allocInfo.memoryTypeIndex = context->FindMemoryType(
            memRequirements.memoryTypeBits,
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
        );

        if (vkAllocateMemory(context->Device, &allocInfo, nullptr, &stagingBufferMemory) != VK_SUCCESS)
        {
            throw gcnew System::Exception("Failed to allocate staging buffer memory!");
        }

        vkBindBufferMemory(context->Device, stagingBuffer, stagingBufferMemory, 0);

        // 3. Копируем данные
        void* mappedData;
        vkMapMemory(context->Device, stagingBufferMemory, 0, imageSize, 0, &mappedData);
        memcpy(mappedData, imageData, static_cast<size_t>(imageSize));
        vkUnmapMemory(context->Device, stagingBufferMemory);

        // 4. Создаем VkImage
        VkImageCreateInfo imageInfo{};
        imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
        imageInfo.imageType = VK_IMAGE_TYPE_2D;
        imageInfo.extent.width = static_cast<uint32_t>(width);
        imageInfo.extent.height = static_cast<uint32_t>(height);
        imageInfo.extent.depth = 1;
        imageInfo.mipLevels = 1;
        imageInfo.arrayLayers = 1;
        imageInfo.format = form;
        imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
        imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        imageInfo.usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
        imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
        imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
        imageInfo.flags = 0;

        VkImage tex;
        if (vkCreateImage(context->Device, &imageInfo, nullptr, &tex) != VK_SUCCESS)
        {
            throw gcnew System::Exception("Failed to create image!");
        }
        texture = tex;

        // 5. Выделяем память для изображения
        vkGetImageMemoryRequirements(context->Device, texture, &memRequirements);

        allocInfo.allocationSize = memRequirements.size;
        allocInfo.memoryTypeIndex = context->FindMemoryType(
            memRequirements.memoryTypeBits,
            VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
        );

        VkDeviceMemory mem;
        if (vkAllocateMemory(context->Device, &allocInfo, nullptr, &mem) != VK_SUCCESS)
        {
            throw gcnew System::Exception("Failed to allocate image memory!");
        }
        memory = mem;

        vkBindImageMemory(context->Device, texture, memory, 0);

        // 6. Копируем из буфера в изображение
        // ... [реализация копирования через командный буфер] ...

        // 7. Создаем image view
        VkImageViewCreateInfo viewInfo{};
        viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        viewInfo.image = texture;
        viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        viewInfo.format = form;
        viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        viewInfo.subresourceRange.baseMipLevel = 0;
        viewInfo.subresourceRange.levelCount = 1;
        viewInfo.subresourceRange.baseArrayLayer = 0;
        viewInfo.subresourceRange.layerCount = 1;

        VkImageView view;
        if (vkCreateImageView(context->Device, &viewInfo, nullptr, &view) != VK_SUCCESS)
        {
            throw gcnew System::Exception("Failed to create texture image view!");
        }
        this->view = view;

        // 8. Очищаем staging ресурсы
        vkDestroyBuffer(context->Device, stagingBuffer, nullptr);
        vkFreeMemory(context->Device, stagingBufferMemory, nullptr);
    }

    void VulkanTexture::CreateRenderTarget()
    {
        VkFormat form = GetNativeFormat(format);

        this->width = width;
        this->height = height;
        this->format = format;

        // Создание изображения
        VkImageCreateInfo imageInfo{};
        imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
        imageInfo.imageType = VK_IMAGE_TYPE_2D;
        imageInfo.extent.width = static_cast<uint32_t>(width);
        imageInfo.extent.height = static_cast<uint32_t>(height);
        imageInfo.extent.depth = 1;
        imageInfo.mipLevels = 1;
        imageInfo.arrayLayers = 1;
        imageInfo.format = form;
        imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
        imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        imageInfo.usage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
        imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
        imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

        VkImage tex;
        if (vkCreateImage(context->Device, &imageInfo, nullptr, &tex) != VK_SUCCESS)
        {
            throw gcnew System::Exception("Failed to create image!");
        }
        texture = tex;

        // Выделение памяти
        VkMemoryRequirements memRequirements;
        vkGetImageMemoryRequirements(context->Device, texture, &memRequirements);

        VkMemoryAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        allocInfo.allocationSize = memRequirements.size;
        allocInfo.memoryTypeIndex = context->FindMemoryType(
            memRequirements.memoryTypeBits,
            VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
        );

        VkDeviceMemory mem;
        if (vkAllocateMemory(context->Device, &allocInfo, nullptr, &mem) != VK_SUCCESS)
        {
            throw gcnew System::Exception("Failed to allocate image memory!");
        }
        memory = mem;

        vkBindImageMemory(context->Device, texture, memory, 0);

        // Создание image view
        VkImageViewCreateInfo viewInfo{};
        viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        viewInfo.image = texture;
        viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        viewInfo.format = form;
        viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        viewInfo.subresourceRange.baseMipLevel = 0;
        viewInfo.subresourceRange.levelCount = 1;
        viewInfo.subresourceRange.baseArrayLayer = 0;
        viewInfo.subresourceRange.layerCount = 1;

        VkImageView view;
        if (vkCreateImageView(context->Device, &viewInfo, nullptr, &view) != VK_SUCCESS)
        {
            throw gcnew System::Exception("Failed to create texture image view!");
        }
        this->view = view;
    }
}
