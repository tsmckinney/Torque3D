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
#include "gfx/vulkan/gfxVKCardProfiler.h"
#include "gfx/vulkan/gfxVKEnumTranslate.h"


const char* vendorIDToString(VkVendorId id) 
{
   switch (id)
   {
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
void GFXVulkanCardProfiler::init()
{
   VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
   uint32_t deviceCount = 0;
   VkInstance instance = GFXVK->getVKInstance();
   vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);
   AssertFatal(deviceCount > 0, "This machine does not support Vulkan!");
   Vector<VkPhysicalDevice> devices;
   devices.setSize(deviceCount);
   vkEnumeratePhysicalDevices(instance, &deviceCount, devices.address());
    
   VkPhysicalDeviceProperties deviceProperties;
   vkGetPhysicalDeviceProperties(devices[0], &deviceProperties);

   mCardDescription = deviceProperties.deviceName;
   mChipSet = vendorIDToString(static_cast<VkVendorId>(deviceProperties.vendorID)); // That was more finicky than it needed to be...
   mVersionString = deviceProperties.apiVersion;
   mRenderString = "Vulkan (WIP)";

   Parent::init(); // other code notes that not calling this is "BAD".
}

const String &GFXVulkanCardProfiler::getRendererString() const 
{
   return mRenderString;
}

void GFXVulkanCardProfiler::setupCardCapabilities()
{ 

};

bool GFXVulkanCardProfiler::_queryCardCap(const String &query, U32 &foundResult)
{ 
   return false; 
};

bool GFXVulkanCardProfiler::_queryFormat(const GFXFormat fmt, const GFXTextureProfile *profile, bool &inOutAutogenMips)
{ 
   return GFXVulkanTextureFormat[fmt] != VK_FORMAT_UNDEFINED;
};
