#include <SDL.h>
#include <SDL_vulkan.h>
#include "windowManager/sdl/sdlWindow.h"
#include "console/console.h"

#include "platform/platformVK.h"

#include "gfx/vulkan/gfxVKHelpers.h"

namespace PlatformVK
{

   void init()
   {
      SDL_Vulkan_LoadLibrary(nullptr);
   }
   void shutdown()
   {
      SDL_Vulkan_UnloadLibrary();
   }

   bool createSurfaceVK(PlatformWindow* window, VkInstance instance, VkSurfaceKHR* surface)
   {
      PlatformWindowSDL* windowSdl = dynamic_cast<PlatformWindowSDL*>(window);
      AssertFatal(windowSdl, "");
      if( !windowSdl )
         return false;
      bool surf = SDL_Vulkan_CreateSurface(windowSdl->getSDLWindow(), instance, surface);
      if (!surf)
      {
         const char* err = SDL_GetError();
         Con::printf( err );
         AssertFatal(0, err );
      }
      return surf;
   }

   Vector<const char*> getInstanceExtensionsVK(PlatformWindow* window)
   {
      Vector<const char*> extensions;
      PlatformWindowSDL* windowSdl = dynamic_cast<PlatformWindowSDL*>(window);
      AssertFatal(windowSdl, "");
      U32 extensionCount = 0;
      bool extsNamed = SDL_Vulkan_GetInstanceExtensions(windowSdl->getSDLWindow(), &extensionCount, nullptr);
      if (!extsNamed)
      {
         const char* err = SDL_GetError();
         Con::printf(err);
         AssertFatal(0, err);
      }
      SDL_ClearError();
      extensions.push_back(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);
      extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
      extensions.push_back(VK_KHR_GET_SURFACE_CAPABILITIES_2_EXTENSION_NAME);
      U32 additionalExtensions = extensions.size();
      extensions.setSize(extensionCount + additionalExtensions);
      U32 totalExtensionCount = extensionCount + additionalExtensions;
      extsNamed = SDL_Vulkan_GetInstanceExtensions(windowSdl->getSDLWindow(), &totalExtensionCount, extensions.address()+additionalExtensions);
      if (!extsNamed)
      {
         const char* err = SDL_GetError();
         Con::printf(err);
         AssertFatal(0, err);
      }
      SDL_ClearError();
      return extensions;
   }

   //void* CreateContextGL( PlatformWindow *window )
   //{
   //    init();
//
   //    PlatformWindowSDL* windowSdl = dynamic_cast<PlatformWindowSDL*>(window);
   //    AssertFatal(windowSdl, "");
//
   //    if( !windowSdl )
   //        return NULL;
//
   //    SDL_ClearError();
   //    SDL_GLContext ctx = SDL_GL_CreateContext( windowSdl->getSDLWindow() );
   //    if( !ctx )
   //    {
   //        const char *err = SDL_GetError();
   //        Con::printf( err );
   //        AssertFatal(0, err );
   //    }
//
   //    return ctx;
   //}
//
   //void MakeCurrentGL( PlatformWindow *window, void *glContext )
   //{
   //    PlatformWindowSDL* windowSdl = dynamic_cast<PlatformWindowSDL*>(window);
   //    AssertFatal( windowSdl && glContext, "" );
//
   //    SDL_ClearError();
   //    SDL_GL_MakeCurrent( windowSdl->getSDLWindow(), glContext );
//
   //    const char *err = SDL_GetError();
   //    if( err && err[0] )
   //    {
   //        Con::printf( err );
   //        AssertFatal(0, err );
   //    }
   //}
//
   //void setVSync(const int i)
   //{
   //   PRESERVE_FRAMEBUFFER();
   //   // Nvidia needs to have the default framebuffer bound or the vsync calls fail
   //   glBindFramebuffer(GL_FRAMEBUFFER, 0);
   //    if( i == 1 || i == -1 )
   //    {
   //        int ret = SDL_GL_SetSwapInterval(-1);
//
   //        if( ret == -1)
   //            SDL_GL_SetSwapInterval(1);
   //    }
   //    else
   //        SDL_GL_SetSwapInterval(0);
//
   //}

}
