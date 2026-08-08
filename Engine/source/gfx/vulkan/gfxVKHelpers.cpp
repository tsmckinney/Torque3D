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

#include "platform/platform.h"

#include "gfx/vulkan/gfxVKHelpers.h"

const char* vendorIDToString(VkVendorId id)
{
   switch (id)
   {
   // Vendor IDs that don't have the "VK_" prefix (numerically represented) were sourced from https://pcisig.com/membership/member-companies.
   case 4130:
      return "AMD";
   case 4203:
      return "Apple";
   case 4318:
      return "NVIDIA";
   case 4454:
      return "Broadcom";
   case 5045:
      return "ARM";
   case 5197:
      return "Samsung";
   case 6091:
      return "Qualcomm";
   case 7893:
      return "Moore Threads";
   case 32902:
      return "Intel";
   case VK_VENDOR_ID_KHRONOS:
      return "Khronos";
   case VK_VENDOR_ID_VIV:
      return "VIV";
   case VK_VENDOR_ID_VSI:
      return "VSI";
   case VK_VENDOR_ID_KAZAN:
      return "Kazan";
   case VK_VENDOR_ID_CODEPLAY:
      return "Codeplay";
   case VK_VENDOR_ID_MESA:
      return "Mesa";
   case VK_VENDOR_ID_POCL:
      return "POCL";
   case VK_VENDOR_ID_MOBILEYE:
      return "MobilEye";
   default:
      return "Unknown Device";
   }
}

GFXVulkanQueueFamilyIndex::GFXVulkanQueueFamilyIndex()
{
   mHasValue = false;
}

GFXVulkanQueueFamilyIndex::GFXVulkanQueueFamilyIndex(U32 idx)
{
   mHasValue = true;
   mIndex = idx;
}

GFXVulkanQueueFamilyIndex::GFXVulkanQueueFamilyIndex(bool hasValue, U32 idx)
{
   mHasValue = hasValue;
   if (hasValue)
   {
      mIndex = idx;
   }
}

GFXVulkanQueueFamilyIndex::~GFXVulkanQueueFamilyIndex()
{
   mHasValue = false;
   mIndex = NULL;
}

void GFXVulkanQueueFamilyIndex::set(U32 i)
{
   mHasValue = true;
   mIndex = i;
}
void GFXVulkanQueueFamilyIndices::generateQFIndices(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface)
{
   U32 queueFamilyCount = 0;
   vkGetPhysicalDeviceQueueFamilyProperties2(physicalDevice, &queueFamilyCount, nullptr);
   // TODO: Try to do this with Vector<T>!!!
   Vector<VkQueueFamilyProperties> queueFamilies;
   queueFamilies.setSize(queueFamilyCount);
   vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, queueFamilies.address());

   int idx = 0;
   for(VkQueueFamilyProperties& queueFamily : queueFamilies)
   {
      if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
         this->mGraphicsFamily.set(idx);

      VkBool32 presentSupport = false;
      vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, idx, surface, &presentSupport);
      if (presentSupport)
         this->mPresentFamily.set(idx);

      if (this->areQFIndicesComplete())
         break;
      idx++;
   }
}
bool GFXVulkanQueueFamilyIndices::areQFIndicesComplete()
{
   return this->mGraphicsFamily.mHasValue && this->mPresentFamily.mHasValue;
}
