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

#ifndef _GFXVKHELPERS_H_
#define _GFXVKHELPERS_H_

#include "platform/platform.h"
#include "platform/platformVK.h"
#include <volk.h>
//-----------------------------------------------------------------------------

#include "core/util/tVector.h"
#include "gfx/gfxEnums.h"

// Vendor IDs (for card profiling)
extern const char* vendorIDToString(VkVendorId id);

// Queue
struct GFXVulkanQueueFamilyIndex
{
public:
   U32 mIndex = NULL;
   bool mHasValue = false;
   GFXVulkanQueueFamilyIndex();
   GFXVulkanQueueFamilyIndex(U32 idx);
   GFXVulkanQueueFamilyIndex(bool hasValue, U32 idx);
   ~GFXVulkanQueueFamilyIndex();
   void set(U32 i);
};
struct GFXVulkanQueueFamilyIndices
{
public:
   GFXVulkanQueueFamilyIndex mGraphicsFamily;
   GFXVulkanQueueFamilyIndex mPresentFamily;
   GFXVulkanQueueFamilyIndex mComputeFamily;
   void generateQFIndices(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface);
   bool areQFIndicesComplete();
};

extern bool checkPhysicalDeviceCompatibility(VkPhysicalDevice physicalDevice);
extern bool doesPhysicalDeviceSupportExtension(VkPhysicalDevice physicalDevice, String extensionName);

inline VkFilter minificationFilter(U32 minFilter)
{
   // the compiler should interpret this as array lookups
   switch (minFilter)
   {
   case GFXTextureFilterLinear:
      return VK_FILTER_LINEAR;
   default:
      return VK_FILTER_NEAREST;
   }
}
inline VkSamplerMipmapMode mipmappingFilter(U32 minFilter, U32 mipFilter, U32 /*mipLevels*/)
{
   // the compiler should interpret this as array lookups
   switch (minFilter)
   {
   case GFXTextureFilterLinear:
      switch (mipFilter)
      {
      case GFXTextureFilterLinear:
         return VK_SAMPLER_MIPMAP_MODE_LINEAR;
      case GFXTextureFilterPoint:
         return VK_SAMPLER_MIPMAP_MODE_NEAREST;
      default:
         return VK_SAMPLER_MIPMAP_MODE_LINEAR;
      }
   default:
      switch (mipFilter) {
      case GFXTextureFilterLinear:
         return VK_SAMPLER_MIPMAP_MODE_LINEAR;
      case GFXTextureFilterPoint:
         return VK_SAMPLER_MIPMAP_MODE_NEAREST;
      default:
         return VK_SAMPLER_MIPMAP_MODE_NEAREST;
      }
   }
}

#endif
