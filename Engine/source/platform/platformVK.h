#ifndef PLATFORM_GL_H
#define PLATFORM_GL_H

class PlatformWindow;
#include <vulkan/vulkan.h>

namespace PlatformVK
{
   void init();

   void shutdown();

   bool createSurfaceVK(PlatformWindow* window, VkInstance instance, VkSurfaceKHR* surface);

   Vector<const char*> getInstanceExtensionsVK(PlatformWindow* window);
}

#endif //PLATFORM_GL_H
