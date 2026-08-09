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

GFXVulkanWindowTarget::GFXVulkanWindowTarget(PlatformWindow *win, GFXDevice *d) :
    GFXWindowTarget(win), mDevice(d)
{
    
}

GFXVulkanWindowTarget::~GFXVulkanWindowTarget()
{

}

void GFXVulkanWindowTarget::createSwapChain()
{
   VkPhysicalDeviceSurfaceInfo2KHR surfaceInfo{};
   surfaceInfo.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SURFACE_INFO_2_KHR;
   surfaceInfo.surface = GFXVK->getVKSurface();
   VkSurfaceCapabilities2KHR surfaceCaps{};
   surfaceCaps.sType = VK_STRUCTURE_TYPE_SURFACE_CAPABILITIES_2_KHR;
   AssertFatal(vkGetPhysicalDeviceSurfaceCapabilities2KHR(GFXVK->getVKPhysicalDevice(), &surfaceInfo, &surfaceCaps) == VK_SUCCESS,
      "Failed to get the surface capabilities of your graphics card! Please make sure it supports Vulkan before relaunching.");

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
   AssertFatal(vkCreateSwapchainKHR(GFXVK->getVKLogicalDevice(), &swapCreateInfo, nullptr, &mSwapchain.mSwapchain) == VK_SUCCESS,
      "Failed to create the swapchain! Please make sure your graphics card supports Vulkan before relaunching.");
}
