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
#include "gfxVKWindowTarget.h"
#include <vk_mem_alloc.h>

GFXVulkanWindowTarget::GFXVulkanWindowTarget(PlatformWindow *win, GFXDevice *d) :
    GFXWindowTarget(win), mDevice(d)
{
   mSwapchain = GFXVulkanSwapChain();
}

GFXVulkanWindowTarget::~GFXVulkanWindowTarget()
{
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
   swapCreateInfo.minImageCount = surfaceCaps.surfaceCapabilities.minImageCount;
   swapCreateInfo.imageFormat = (VkFormat)GFXVulkanTextureFormat[getFormat()];
   swapCreateInfo.imageColorSpace = VK_COLORSPACE_SRGB_NONLINEAR_KHR;
   swapCreateInfo.imageExtent = mSwapchain.mExtent;
   swapCreateInfo.imageArrayLayers = 1; //...For now.
   swapCreateInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
   swapCreateInfo.preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
   swapCreateInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
   swapCreateInfo.presentMode = VK_PRESENT_MODE_FIFO_KHR;

   AssertFatal(vkCreateSwapchainKHR(GFXVK->getVKLogicalDevice(), &swapCreateInfo, NULL, &mSwapchain.mSwapchain) == VK_SUCCESS,
      "GFXVulkanWindowTarget::createSwapChain() - Failed to create the swapchain! Please make sure your graphics card supports the VK_KHR_swapchain extension.");

   U32 swapImageCount = 0;
   AssertFatal(vkGetSwapchainImagesKHR(GFXVK->getVKLogicalDevice(), mSwapchain.mSwapchain, &swapImageCount, NULL) == VK_SUCCESS,
      "GFXVulkanWindowTarget::createSwapChain() - Failed to count possible swapchain images! Please make sure your graphics card supports the VK_KHR_swapchain extension.");
   mSwapchain.mSwapImages.setSize(swapImageCount);
   AssertFatal(vkGetSwapchainImagesKHR(GFXVK->getVKLogicalDevice(), mSwapchain.mSwapchain, &swapImageCount, mSwapchain.mSwapImages.address()) == VK_SUCCESS,
      "GFXVulkanWindowTarget::createSwapChain() - Failed to create swapchain images! Please make sure your graphics card supports the VK_KHR_swapchain extension.");

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
      ivCreateInfos[swapIV].format = (VkFormat)GFXVulkanTextureFormat[getFormat()];
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
         "GFXVulkanWindowTarget::createSwapChain() - Failed to create swapchain image views! Please make sure your graphics card supports the VK_KHR_swapchain extension.");
   }

   Vector<VkFormat> depthFormats;
   depthFormats.push_back(VK_FORMAT_D32_SFLOAT_S8_UINT);
   depthFormats.push_back(VK_FORMAT_D24_UNORM_S8_UINT);
   for(VkFormat& format : depthFormats)
   {
      VkFormatProperties2 formatProperties;
      formatProperties.sType = VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2;
      formatProperties.pNext = NULL;
      vkGetPhysicalDeviceFormatProperties2(GFXVK->getVKPhysicalDevice(), format, &formatProperties);
      if (formatProperties.formatProperties.optimalTilingFeatures & VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT)
      {
         mDepthFormat = (GFXFormat)format;
         break;
      }
   }
   AssertISV(mDepthFormat != NULL, "GFXVulkanWindowTarget::createSwapChain() - Could not find a suitable depth texture format!");
}

void GFXVulkanWindowTarget::setupPipelineAndBuffers()
{
}

bool GFXVulkanWindowTarget::present() {
   return true;
}
