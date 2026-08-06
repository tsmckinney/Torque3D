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
      return SDL_Vulkan_CreateSurface(windowSdl->getSDLWindow(), instance, surface);
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
