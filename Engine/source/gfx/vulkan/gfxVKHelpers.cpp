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
#include <vk_mem_alloc.h>

const char* vendorIDToString(VkVendorId id)
{
   switch (id)
   {
   // Vendor IDs that don't have the "VK_" prefix (numerically represented) were sourced from https://pcisig.com/membership/member-companies.
   case 4130:
      return "Advanced Micro Devices, Inc.";
   case 4203:
      return "Apple Computer";
   case 4318:
      return "NVIDIA Corporation";
   case 4454:
      return "Broadcom Limited";
   case 5045:
      return "ARM";
   case 5197:
      return "Samsung Electronics";
   case 6091:
      return "Qualcomm Incorporated";
   case 6629:
      return "Huawei Technologies";
   case 7893:
      return "Moore Threads Technology";
   case 8493:
      return "Apex Technologies";
   case 32902:
      return "Intel Corporation";
   case VK_VENDOR_ID_KHRONOS:
      return "Khronos Group";
   case VK_VENDOR_ID_VIV:
      return "Viv";
   case VK_VENDOR_ID_VSI:
      return "Vsi";
   case VK_VENDOR_ID_KAZAN:
      return "Kazan software renderer";
   case VK_VENDOR_ID_CODEPLAY:
      return "Codeplay Software";
   case VK_VENDOR_ID_MESA:
      return "Mesa";
   case VK_VENDOR_ID_POCL:
      return "PoCL";
   case VK_VENDOR_ID_MOBILEYE:
      return "Mobileye";
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
   vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, NULL);
   Vector<VkQueueFamilyProperties> queueFamilies;
   queueFamilies.setSize(queueFamilyCount);
   vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, queueFamilies.address());

   S32 idx = 0;
   for(VkQueueFamilyProperties& queueFamily : queueFamilies)
   {
      if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
         this->mGraphicsFamily.set(idx);
      if (queueFamily.queueFlags & VK_QUEUE_COMPUTE_BIT)
         this->mComputeFamily.set(idx);

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
   return this->mGraphicsFamily.mHasValue
      && this->mPresentFamily.mHasValue
      && this->mComputeFamily.mHasValue;
}

bool checkPhysicalDeviceCompatibility(VkPhysicalDevice physicalDevice)
{
   // 1. Extension support
   U32 extensionCount;
   vkEnumerateDeviceExtensionProperties(physicalDevice, NULL, &extensionCount, NULL);
   Vector<VkExtensionProperties> availableExts;
   availableExts.setSize(extensionCount);
   vkEnumerateDeviceExtensionProperties(physicalDevice, NULL, &extensionCount, availableExts.address());

   Vector<const char*> availExtNames;
   for (const auto& ext : availableExts)
   {
      availExtNames.push_front(ext.extensionName);
   }

   Vector<const char*> reqdExts;
   reqdExts.push_back(VK_KHR_MAINTENANCE_5_EXTENSION_NAME);
   reqdExts.push_back(VK_KHR_PIPELINE_BINARY_EXTENSION_NAME);
   reqdExts.push_back(VK_KHR_SHADER_FLOAT_CONTROLS_EXTENSION_NAME);
   reqdExts.push_back(VK_KHR_SPIRV_1_4_EXTENSION_NAME);
   reqdExts.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);
   reqdExts.push_back(VK_KHR_SYNCHRONIZATION_2_EXTENSION_NAME);
   reqdExts.push_back(VK_EXT_CUSTOM_BORDER_COLOR_EXTENSION_NAME);
   reqdExts.push_back(VK_EXT_MEMORY_PRIORITY_EXTENSION_NAME);
   reqdExts.push_back(VK_EXT_PAGEABLE_DEVICE_LOCAL_MEMORY_EXTENSION_NAME);
   reqdExts.push_back(VK_EXT_SHADER_DEMOTE_TO_HELPER_INVOCATION_EXTENSION_NAME);
   Vector<const char*>::iterator avaiIter = availExtNames.begin();
   for (; avaiIter != availExtNames.end(); avaiIter++)
   {
      S32 avExtIdx = -1;
      for (U32 rqExtIdx = 0; rqExtIdx < reqdExts.size(); rqExtIdx++)
      {
         if (String::compare(reqdExts[rqExtIdx], *avaiIter) == 0)
         {
            avExtIdx = rqExtIdx;
            break;
         }
      }
      reqdExts.find_next(*avaiIter, 0);

      if (avExtIdx != -1)
      {
         reqdExts.erase(avExtIdx);
      }
   }

   VkPhysicalDeviceFeatures2KHR mDeviceFeatures{};
   mDeviceFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2;
   VkPhysicalDeviceShaderDemoteToHelperInvocationFeaturesEXT demoteToHelperFeatures{};
   demoteToHelperFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_DEMOTE_TO_HELPER_INVOCATION_FEATURES;
   mDeviceFeatures.pNext = &demoteToHelperFeatures;
   vkGetPhysicalDeviceFeatures2(physicalDevice, &mDeviceFeatures);
   VkPhysicalDeviceShaderDemoteToHelperInvocationFeaturesEXT& d2hf = (VkPhysicalDeviceShaderDemoteToHelperInvocationFeaturesEXT&)mDeviceFeatures.pNext;

   return reqdExts.empty() && d2hf.shaderDemoteToHelperInvocation;
}
