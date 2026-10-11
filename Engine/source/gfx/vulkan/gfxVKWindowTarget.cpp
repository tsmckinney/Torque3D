//-----------------------------------------------------------------------------
// Copyright (c) 2012 GarageGames, LLC
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to
// deal in the Software without restriction, including without limitation the
// rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
// sell copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
// IN THE SOFTWARE.
//-----------------------------------------------------------------------------

#include "gfx/vulkan/gfxVKWindowTarget.h"
#include "gfx/vulkan/gfxVKTextureObject.h"
#include "gfx/vulkan/gfxVKHelpers.h"
#include "gfxVKWindowTarget.h"
#include <vk_mem_alloc.h>

GFXVulkanWindowTarget::GFXVulkanWindowTarget(PlatformWindow *win, GFXDevice *d) :
    GFXWindowTarget(win), mDevice(d)
{
   mSwapchain = GFXVulkanSwapChain();
}

GFXVulkanWindowTarget::~GFXVulkanWindowTarget()
{
   for (U32 swapFB = 0; swapFB < mSwapchain.mSwapFramebuffers.size(); swapFB++)
   {
      vkDestroyFramebuffer(GFXVK->getVKLogicalDevice(), mSwapchain.mSwapFramebuffers[swapFB], nullptr);
   }
   vkDestroyRenderPass(GFXVK->getVKLogicalDevice(), mRenderPass, nullptr);
   vkDestroyImageView(GFXVK->getVKLogicalDevice(), mSwapchain.mDepthView, nullptr);
   vmaDestroyImage(GFXVK->getVKAllocator(), mSwapchain.mDepthImage, mSwapchain.mDepthAlloc);
   for (U32 swapIV = 0; swapIV < mSwapchain.mSwapImageViews.size(); swapIV++)
   {
      vkDestroyImageView(GFXVK->getVKLogicalDevice(), mSwapchain.mSwapImageViews[swapIV], nullptr);
   }
   for (U32 swapImage = 0; swapImage < mSwapchain.mSwapImages.size(); swapImage++)
   {
      vkDestroyImage(GFXVK->getVKLogicalDevice(), mSwapchain.mSwapImages[swapImage], nullptr);
   }
   vkDestroySwapchainKHR(GFXVK->getVKLogicalDevice(), mSwapchain.mSwapchain, nullptr);
}

GFXFormat GFXVulkanWindowTarget::getDepthFormat()
{
   return mDepthFormat;
}

void GFXVulkanWindowTarget::createSwapChain()
{
   VkPhysicalDeviceSurfaceInfo2KHR surfaceInfo{};
   surfaceInfo.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SURFACE_INFO_2_KHR;
   surfaceInfo.surface = GFXVK->getVKSurface();
   VkSurfaceCapabilities2KHR surfaceCaps{};
   surfaceCaps.sType = VK_STRUCTURE_TYPE_SURFACE_CAPABILITIES_2_KHR;
   AssertFatal(vkGetPhysicalDeviceSurfaceCapabilities2KHR(GFXVK->getVKPhysicalDevice(), &surfaceInfo, &surfaceCaps) == VK_SUCCESS,
      "GFXVulkanWindowTarget::createSwapChain() - Failed to get the surface capabilities of your graphics card! Please make sure it supports the VK_KHR_surface extension for your operating system.");

   mSwapchain.mExtent = surfaceCaps.surfaceCapabilities.currentExtent;

   VkSwapchainCreateInfoKHR swapCreateInfo{};
   swapCreateInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
   swapCreateInfo.surface = GFXVK->getVKSurface();
   swapCreateInfo.minImageCount = surfaceCaps.surfaceCapabilities.minImageCount; // FIXME: This causes a performance warning on certain computers, like my laptop. Hypothetically, I could clamp this value to 3 to *force* triple buffering, but I'd rather not lie to the API.
   swapCreateInfo.imageFormat = GFXVulkanTextureFormat[getFormat()];
   swapCreateInfo.imageColorSpace = VK_COLORSPACE_SRGB_NONLINEAR_KHR;
   swapCreateInfo.imageExtent = mSwapchain.mExtent;
   swapCreateInfo.imageArrayLayers = 1; //...For now.
   swapCreateInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
   swapCreateInfo.preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
   swapCreateInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
   swapCreateInfo.presentMode = VK_PRESENT_MODE_FIFO_KHR;

   AssertFatal(vkCreateSwapchainKHR(GFXVK->getVKLogicalDevice(), &swapCreateInfo, NULL, &mSwapchain.mSwapchain) == VK_SUCCESS,
      "GFXVulkanWindowTarget::createSwapChain() - Failed to create the swapchain! Please make sure your graphics card supports the VK_KHR_swapchain extension for your operating system.");

   U32 swapImageCount = 0;
   AssertFatal(vkGetSwapchainImagesKHR(GFXVK->getVKLogicalDevice(), mSwapchain.mSwapchain, &swapImageCount, NULL) == VK_SUCCESS,
      "GFXVulkanWindowTarget::createSwapChain() - Failed to count possible swapchain images! Please make sure your graphics card supports the VK_KHR_swapchain extension for your operating system.");
   mSwapchain.mSwapImages.setSize(swapImageCount);
   AssertFatal(vkGetSwapchainImagesKHR(GFXVK->getVKLogicalDevice(), mSwapchain.mSwapchain, &swapImageCount, mSwapchain.mSwapImages.address()) == VK_SUCCESS,
      "GFXVulkanWindowTarget::createSwapChain() - Failed to create swapchain images! Please make sure your graphics card supports the VK_KHR_swapchain extension for your operating system.");

   mSwapchain.mSwapImageViews.setSize(mSwapchain.mSwapImages.size());
   Vector<VkImageViewCreateInfo> ivCreateInfos;
   ivCreateInfos.setSize(swapImageCount);
   for (U32 swapIV = 0; swapIV < mSwapchain.mSwapImageViews.size(); swapIV++)
   {
      VkImageViewUsageCreateInfo ivUsageCI{};
      ivUsageCI.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_USAGE_CREATE_INFO;
      ivUsageCI.usage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
      ivCreateInfos[swapIV].sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
      ivCreateInfos[swapIV].image = mSwapchain.mSwapImages[swapIV];
      ivCreateInfos[swapIV].viewType = VK_IMAGE_VIEW_TYPE_2D;
      ivCreateInfos[swapIV].format = GFXVulkanTextureFormat[getFormat()];
      ivCreateInfos[swapIV].flags = 0;
      ivCreateInfos[swapIV].pNext = &ivUsageCI;

      ivCreateInfos[swapIV].components.r = VK_COMPONENT_SWIZZLE_R;
      ivCreateInfos[swapIV].components.g = VK_COMPONENT_SWIZZLE_G;
      ivCreateInfos[swapIV].components.b = VK_COMPONENT_SWIZZLE_B;
      ivCreateInfos[swapIV].components.a = VK_COMPONENT_SWIZZLE_A;

      ivCreateInfos[swapIV].subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
      ivCreateInfos[swapIV].subresourceRange.baseMipLevel = 0;
      ivCreateInfos[swapIV].subresourceRange.levelCount = 1;
      ivCreateInfos[swapIV].subresourceRange.baseArrayLayer = 0;
      ivCreateInfos[swapIV].subresourceRange.layerCount = 1;

      AssertFatal(vkCreateImageView(GFXVK->getVKLogicalDevice(), &ivCreateInfos[swapIV], NULL, &mSwapchain.mSwapImageViews[swapIV]) == VK_SUCCESS,
         "GFXVulkanWindowTarget::createSwapChain() - Failed to create swapchain image views! Please make sure your graphics card supports the VK_KHR_swapchain extension for your operating system.");
   }

   Vector<VkFormat> depthFormats;
   depthFormats.push_back(VK_FORMAT_D16_UNORM);
   depthFormats.push_back(VK_FORMAT_D24_UNORM_S8_UINT);
   depthFormats.push_back(VK_FORMAT_D32_SFLOAT);
   depthFormats.push_back(VK_FORMAT_D32_SFLOAT_S8_UINT);
   for(VkFormat& format : depthFormats)
   {
      VkFormatProperties2 formatProperties{};
      formatProperties.sType = VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2;
      formatProperties.pNext = NULL;
      vkGetPhysicalDeviceFormatProperties2(GFXVK->getVKPhysicalDevice(), format, &formatProperties);
      if ((formatProperties.formatProperties.optimalTilingFeatures & VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT) == VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT)
      {
         for (size_t i = 0; i < sizeof(GFXVulkanTextureFormat); i++)
         {
            if (GFXVulkanTextureFormat[i] == (U32)format) {
               mDepthFormat = (GFXFormat)i;
            }
         }
         break;
      }
   }
   AssertISV(mDepthFormat != NULL, "GFXVulkanWindowTarget::createSwapChain() - Could not find a suitable depth texture format!");

   // yay, I finally figured this out!
   VkImageCreateInfo depthImageCI{};
   depthImageCI.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
   depthImageCI.imageType = VK_IMAGE_TYPE_2D;
   depthImageCI.format = GFXVulkanTextureFormat[mDepthFormat];
   depthImageCI.extent.width = mSwapchain.mExtent.width;
   depthImageCI.extent.height = mSwapchain.mExtent.height;
   depthImageCI.extent.depth = 1;
   depthImageCI.mipLevels = 1;
   depthImageCI.arrayLayers = 1;
   depthImageCI.samples = VK_SAMPLE_COUNT_1_BIT;
   depthImageCI.tiling = VK_IMAGE_TILING_OPTIMAL;
   depthImageCI.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
   depthImageCI.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
   depthImageCI.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
   depthImageCI.flags = 0;
   VmaAllocationCreateInfo allocationCI{};
   allocationCI.flags = VMA_ALLOCATION_CREATE_DEDICATED_MEMORY_BIT;
   allocationCI.usage = VMA_MEMORY_USAGE_AUTO_PREFER_DEVICE;
   AssertFatal(vmaCreateImage(GFXVK->getVKAllocator(), &depthImageCI, &allocationCI, &mSwapchain.mDepthImage, &mSwapchain.mDepthAlloc, nullptr) == VK_SUCCESS, 
      "GFXVulkanWindowTarget::createSwapChain() - Failed to create a depth texture!");

   VkImageViewUsageCreateInfo divUsageCI{};
   divUsageCI.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_USAGE_CREATE_INFO;
   divUsageCI.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;

   VkImageViewCreateInfo divCreate{};
   divCreate.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
   divCreate.image = mSwapchain.mDepthImage;
   divCreate.viewType = VK_IMAGE_VIEW_TYPE_2D;
   divCreate.format = (VkFormat)GFXVulkanTextureFormat[getDepthFormat()];
   divCreate.pNext = &divUsageCI;

   divCreate.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
   divCreate.subresourceRange.levelCount = 1;
   divCreate.subresourceRange.layerCount = 1;
   AssertFatal(vkCreateImageView(GFXVK->getVKLogicalDevice(), &divCreate, NULL, &mSwapchain.mDepthView) == VK_SUCCESS,
      "GFXVulkanWindowTarget::createSwapChain() - Failed to create a depth image view!");
}

void GFXVulkanWindowTarget::setupRenderPassAndFramebuffer()
{
   VkAttachmentDescription colorAttachment{};
   colorAttachment.format = GFXVulkanTextureFormat[getFormat()];
   colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
   colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
   colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
   colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
   colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
   colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
   colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

   VkAttachmentReference colorAttachmentRef{};
   colorAttachmentRef.attachment = 0;
   colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

   VkSubpassDescription subpass{};
   subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
   subpass.colorAttachmentCount = 1;
   subpass.pColorAttachments = &colorAttachmentRef;

   VkRenderPassCreateInfo renderPassCI{};
   renderPassCI.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
   renderPassCI.attachmentCount = 1;
   renderPassCI.pAttachments = &colorAttachment;
   renderPassCI.subpassCount = 1;
   renderPassCI.pSubpasses = &subpass;

   AssertFatal(vkCreateRenderPass(GFXVK->getVKLogicalDevice(), &renderPassCI, NULL, &mRenderPass) == VK_SUCCESS,
      "GFXVulkanWindowTarget::setupRenderPassAndFramebuffer() - Failed to create a render pass!");

   mSwapchain.mSwapFramebuffers.setSize(mSwapchain.mSwapImageViews.size());
   for (VkImageView imageView : mSwapchain.mSwapImageViews)
   {
      S32 i = mSwapchain.mSwapImageViews.find_next(imageView); // FIXME: This might not properly handle "duplicate" image views.
      VkImageView attachments[] = {
         imageView
      };

      VkFramebufferCreateInfo framebufferCI{};
      framebufferCI.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
      framebufferCI.renderPass = mRenderPass;
      framebufferCI.attachmentCount = 1;
      framebufferCI.pAttachments = attachments;
      framebufferCI.width = mSwapchain.mExtent.width;
      framebufferCI.height = mSwapchain.mExtent.height;
      framebufferCI.layers = 1;
      AssertFatal(vkCreateFramebuffer(GFXVK->getVKLogicalDevice(), &framebufferCI, NULL, &mSwapchain.mSwapFramebuffers[i]) == VK_SUCCESS,
         "GFXVulkanWindowTarget::setupRenderPassAndFramebuffer() - Failed to create one of the needed framebuffers!");
   }
}

bool GFXVulkanWindowTarget::present() {
   return true;
}
