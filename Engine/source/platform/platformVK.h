#ifndef PLATFORM_GL_H
#define PLATFORM_GL_H

class PlatformWindow;
#include <vulkan/vulkan.h>

namespace PlatformVK
{
   void init();

   void shutdown();

   bool createSurfaceVK(PlatformWindow* window, VkInstance instance, VkSurfaceKHR* surface);

   void getExtensionsSDLVK(PlatformWindow *window, Vector<const char*> extensions);
}

#endif //PLATFORM_GL_H
