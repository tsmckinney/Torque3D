#ifndef PLATFORM_GL_H
#define PLATFORM_GL_H

class PlatformWindow;

namespace PlatformVK
{
   void init();

   void shutdown();

   void getExtensionsSDLVK(PlatformWindow *window, U32 extensionCount, const char** extensionNames);
}

#endif //PLATFORM_GL_H
