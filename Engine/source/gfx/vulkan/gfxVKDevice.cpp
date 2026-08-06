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

#include "core/strings/stringFunctions.h"
#include "gfx/screenshot.h"
#include "gfx/gfxCardProfile.h"
#include "gfx/vulkan/gfxVKVertexBuffer.h"
#include "app/version.h"
#include "gfx/vulkan/gfxVKDevice.h"

GFXAdapter::CreateDeviceInstanceDelegate GFXVulkanDevice::mCreateDeviceInstance(GFXVulkanDevice::createInstance); 

//
// GFXVulkanStateBlock
//
class GFXVulkanStateBlock : public GFXStateBlock
{
public:
   /// Returns the hash value of the desc that created this block
   U32 getHashValue() const override { return 0; };

   /// Returns a GFXStateBlockDesc that this block represents
   const GFXStateBlockDesc& getDesc() const override { return mDefaultDesc; }

   //
   // GFXResource
   //
   void zombify() override { }
   /// When called the resource should restore all device sensitive information destroyed by zombify()
   void resurrect() override { }
private:
   GFXStateBlockDesc mDefaultDesc;
};

static VKAPI_ATTR VkBool32 VKAPI_CALL validationDebugCallback(
   VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
   VkDebugUtilsMessageTypeFlagsEXT messageType,
   const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
   void* pUserData)
{
   Con::printf("Validation layer:" + (String)pCallbackData->pMessage);
   return VK_FALSE;
}
static VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger)
{
   auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
   if (func != nullptr)
   {
      return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
   }
   else
   {
      return VK_ERROR_EXTENSION_NOT_PRESENT;
   }
}
static void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo) 
{
   createInfo = {};
   createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
   createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
   createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
   createInfo.pfnUserCallback = validationDebugCallback;
}
static void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator) 
{
   auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
   if (func != nullptr)
   {
      func(instance, debugMessenger, pAllocator);
   }
}

//
// GFXVulkanDevice
//

GFXDevice *GFXVulkanDevice::createInstance( U32 adapterIndex )
{
   return new GFXVulkanDevice();
}

void GFXVulkanDevice::setupDebugMessenger()
{
   if (!mEnableValidationLayers) return;
   
   VkDebugUtilsMessengerCreateInfoEXT createInfo;
   populateDebugMessengerCreateInfo(createInfo);
   AssertWarn(CreateDebugUtilsMessengerEXT(mInstance, &createInfo, nullptr, &mDebugMessenger) == VK_SUCCESS, "Failed to set up debug messenger!");
}

bool GFXVulkanDevice::checkValidationLayerSupport()
{
   uint32_t layerCount;
   vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

   Vector<VkLayerProperties> availableLayers;
   availableLayers.setSize(layerCount);
   vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.address());
   
   for (const char* layerName : mValidationLayers) 
   {
      bool layerFound = false;

      for (const auto& layerProperties : availableLayers)
      {
         if (dStrcmp(layerName, layerProperties.layerName))
         {
            layerFound = true;
            break;
         }
      }
      if (!layerFound)
      {
         return false;
      }
   }
   return true;
}

Vector<const char *> GFXVulkanDevice::getRequiredExtensions()
{
   Vector<const char*> extensions;

   extensions.push_back(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);

   if (mEnableValidationLayers) 
   {
      extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
   }

   return extensions;
}

GFXVulkanDevice::GFXVulkanDevice()
{
   PlatformVK::init();
   GFXVulkanEnumTranslate::init();
   mPixelShaderVersion = 4.0f;
   mValidationLayers.push_back("VK_LAYER_KHRONOS_validation");
   if (mEnableValidationLayers)
   {
      AssertFatal(checkValidationLayerSupport(), "Vulkan validation layers were requested, but not available.");
   }
   
   // Version number is major * 1000 + minor * 100 + revision * 10...
   VkApplicationInfo appInfo{};
   appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
   appInfo.pApplicationName = TORQUE_APP_NAME;
   appInfo.applicationVersion = VK_MAKE_VERSION(floor(TORQUE_APP_VERSION/1000),floor(TORQUE_APP_VERSION/100),floor(TORQUE_APP_VERSION/10));
   appInfo.pEngineName = getEngineProductString();
   appInfo.engineVersion = VK_MAKE_VERSION(floor(TORQUE_GAME_ENGINE/1000),floor(TORQUE_GAME_ENGINE/100),floor(TORQUE_GAME_ENGINE/10));
   appInfo.apiVersion = VK_API_VERSION_1_4;

   VkInstanceCreateInfo createInfo{};
   createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
   createInfo.pApplicationInfo = &appInfo;

   mRequiredExtensions = getRequiredExtensions();

   createInfo.enabledExtensionCount = (uint32_t) mRequiredExtensions.size();
   createInfo.ppEnabledExtensionNames = mRequiredExtensions.address();

   if (mEnableValidationLayers)
   {
      VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
      createInfo.enabledLayerCount = static_cast<uint32_t>(mValidationLayers.size());
      createInfo.ppEnabledLayerNames = mValidationLayers.address();
      populateDebugMessengerCreateInfo(debugCreateInfo);
      createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*) &debugCreateInfo;
   } 
   else 
   {
      createInfo.enabledLayerCount = 0;
      createInfo.pNext = nullptr;
   }

   AssertFatal(vkCreateInstance(&createInfo, nullptr, &mInstance) == VK_SUCCESS, "Failed to create Vulkan instance! Please make sure your graphics card supports Vulkan before relaunching.");

   mClip.set(0, 0, 800, 800);

   mTextureManager = new GFXVulkanTextureManager();
   gScreenShot = new ScreenShot();
   mCardProfiler = new GFXVulkanCardProfiler();
   mCardProfiler->init();

   GFXVulkanCardProfiler* vkCardProfiler = static_cast<GFXVulkanCardProfiler*>(mCardProfiler);

   GFXVulkanQueueFamilyIndices queueFamilies = generateQFIndices(vkCardProfiler->mPhysicalDevice);

   VkDeviceQueueCreateInfo queueCreateInfo{};
   queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
   queueCreateInfo.queueFamilyIndex = queueFamilies.mGraphicsFamily.mIndex;
   queueCreateInfo.queueCount = 1;
   F32 queuePriority = 1.0f;
   queueCreateInfo.pQueuePriorities = &queuePriority;

   VkDeviceCreateInfo logicalDeviceCreateInfo{};
   logicalDeviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
   logicalDeviceCreateInfo.pQueueCreateInfos = &queueCreateInfo;
   logicalDeviceCreateInfo.queueCreateInfoCount = 1;
   logicalDeviceCreateInfo.pEnabledFeatures = &vkCardProfiler->mDeviceFeatures.features;
   logicalDeviceCreateInfo.enabledExtensionCount = 0;
   AssertFatal(vkCreateDevice(vkCardProfiler->mPhysicalDevice, &logicalDeviceCreateInfo, nullptr, &mVKDevice) == VK_SUCCESS,
      "Failed to create Vulkan logical device! Please make sure your graphics card supports Vulkan before relaunching.");
}

GFXVulkanDevice::~GFXVulkanDevice()
{
   if (mEnableValidationLayers)
   {
      DestroyDebugUtilsMessengerEXT(mInstance, mDebugMessenger, nullptr);
   }
   vkDestroyDevice(mVKDevice, nullptr);
   vkDestroyInstance(mInstance, nullptr);
   PlatformVK::shutdown();
}

GFXVertexBuffer *GFXVulkanDevice::allocVertexBuffer( U32 numVerts, 
                                                   const GFXVertexFormat *vertexFormat,
                                                   U32 vertSize, 
                                                   GFXBufferType bufferType,
                                                   void* data ) 
{
   return new GFXVulkanVertexBuffer(GFX, numVerts, vertexFormat, vertSize, bufferType);
}

GFXPrimitiveBuffer *GFXVulkanDevice::allocPrimitiveBuffer( U32 numIndices, 
                                                         U32 numPrimitives, 
                                                         GFXBufferType bufferType,
                                                         void* data ) 
{
   return new GFXVulkanPrimitiveBuffer(GFX, numIndices, numPrimitives, bufferType);
}

GFXCubemap* GFXVulkanDevice::createCubemap()
{ 
   return new GFXVulkanCubemap(); 
};

GFXCubemapArray* GFXVulkanDevice::createCubemapArray()
{
   return new GFXVulkanCubemapArray();
};

GFXTextureArray* GFXVulkanDevice::createTextureArray()
{
   return new GFXVulkanTextureArray();
};

GFXWindowTarget* GFXVulkanDevice::allocWindowTarget(PlatformWindow* window)
{
   GFXVulkanWindowTarget* target = new GFXVulkanWindowTarget(window, GFX);
   return target;
}
GFXShader* GFXVulkanDevice::createShader() 
{
   GFXVulkanShader* shader = new GFXVulkanShader(this);
   shader->registerResourceWithDevice(this);
   return shader;
}

void GFXVulkanDevice::enumerateAdapters( Vector<GFXAdapter*> &adapterList )
{
   // Add the NULL renderer
   GFXAdapter *toAdd = new GFXAdapter();

   toAdd->mIndex = 0;
   toAdd->mType  = Vulkan;
   toAdd->mCreateDeviceInstanceDelegate = mCreateDeviceInstance;

   GFXVideoMode vm;
   vm.bitDepth = 32;
   vm.resolution.set(800,600);
   toAdd->mAvailableModes.push_back(vm);

   dStrcpy(toAdd->mName, "GFX Vulkan Device", GFXAdapter::MaxAdapterNameLen);

   adapterList.push_back(toAdd);
}

void GFXVulkanDevice::init( const GFXVideoMode &mode, PlatformWindow *window )
{
   mCardProfiler = new GFXVulkanCardProfiler();
   mCardProfiler->init();
}

GFXStateBlockRef GFXVulkanDevice::createStateBlockInternal(const GFXStateBlockDesc& desc)
{
   return new GFXVulkanStateBlock();
}

//
// Register this device with GFXInit
//
class GFXVulkanRegisterDevice
{
public:
   GFXVulkanRegisterDevice()
   {
      GFXInit::getRegisterDeviceSignal().notify(&GFXVulkanDevice::enumerateAdapters);
   }
};

static GFXVulkanRegisterDevice pNullRegisterDevice;
