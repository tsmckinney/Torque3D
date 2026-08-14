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
#if defined( TORQUE_SDL ) && !defined( TORQUE_DEDICATED )

#include "gfx/gfxCubemap.h"
#include "gfx/screenshot.h"

#include "gfx/vulkan/gfxVKDevice.h"
#include "gfx/vulkan/gfxVKEnumTranslate.h"
#include "gfx/vulkan/gfxVKVertexBuffer.h"
#include "gfx/vulkan/gfxVKPrimitiveBuffer.h"
#include "gfx/vulkan/gfxVKWindowTarget.h"
#include "gfx/vulkan/gfxVKTextureManager.h"
#include "gfx/vulkan/gfxVKTextureObject.h"
#include "gfx/vulkan/gfxVKCardProfiler.h"
#include <vk_mem_alloc.h>

#include "windowManager/sdl/sdlWindow.h"
#include "platform/platformVK.h"
#include "SDL.h"

//extern void loadGLCore();
//extern void loadGLExtensions(void* context);

void VKEnumerateVideoModes(Vector<GFXVideoMode>& outModes)
{
   S32 count = SDL_GetNumDisplayModes( 0 );
   if( count < 0)
   {
      AssertFatal(0, "");
      return;     
   }
   
   SDL_DisplayMode mode;
   for(S32 i = 0; i < count; ++i)
   {
      SDL_GetDisplayMode( 0, i, &mode);
      GFXVideoMode outMode;
      outMode.resolution.set( mode.w, mode.h );
      outMode.refreshRate = mode.refresh_rate;

      // BBP = 32 for some reason the engine knows it should be 32, but then we
      // add some extra code to break what the engine knows.
      //outMode.bitDepth = SDL_BYTESPERPIXEL( mode.format );     // sets bitdepths to 4
      //outMode.bitDepth = SDL_BITSPERPIXEL(mode.format);        // sets bitdepth to 24
      
      // hardcoded magic numbers ftw
      // This value is hardcoded in DX, probably to avoid the shenanigans going on here
      outMode.bitDepth = 32;                                   

      outMode.wideScreen = (mode.w / mode.h) > (4 / 3);
      outMode.fullScreen = true;
      
      outModes.push_back( outMode );
   }
}

void GFXVulkanDevice::enumerateAdapters( Vector<GFXAdapter*> &adapterList )
{
#ifdef TORQUE_TESTS_ENABLED
      return;
#endif

   AssertFatal( SDL_WasInit(SDL_INIT_VIDEO), "");

   PlatformVK::init(); // for hints about context creation

    // Create a dummy window & Vulkan context so that Vk functions can be used here
   SDL_Window* tempWindow =  SDL_CreateWindow(
        "",                                // window title
        SDL_WINDOWPOS_UNDEFINED,           // initial x position
        SDL_WINDOWPOS_UNDEFINED,           // initial y position
        640,                               // width, in pixels
        480,                               // height, in pixels
        SDL_WINDOW_VULKAN | SDL_WINDOW_HIDDEN // flags - see below
   );
   if (!tempWindow)
   {
      const char* err = SDL_GetError();
      Con::printf(err);
      AssertFatal(0, err);
      return;
   }

   SDL_ClearError();

   //SDL_GLContext tempContext = SDL_GL_CreateContext( tempWindow );
   //if( !tempContext )
   //{
   //   const char *err = SDL_GetError();
   //   Con::printf( err );
   //   AssertFatal(0, err );
   //   return;
   //}

   SDL_ClearError();
   //SDL_GL_MakeCurrent( tempWindow, tempContext );

   const char *err = SDL_GetError();
   if( err && err[0] )
   {
      Con::printf( err );
      AssertFatal(0, err );
   }

   //check for required extensions
   //if (!gglHasExtension(ARB_texture_cube_map_array))
   //{
   //   Con::warnf("Adapater supports OpenGL 3.3 but doesnt support GL_ARB_texture_cube_map_array");
   //   return;
   //}

   //if (!gglHasExtension(ARB_gpu_shader5))
   //{
   //   Con::warnf("Adapater supports OpenGL 3.3 but doesnt support GL_ARB_gpu_shader5");
   //   return;
   //}
    
   GFXAdapter *toAdd = new GFXAdapter;
   toAdd->mIndex = 0;

   dStrcpy(toAdd->mName, "Vulkan", GFXAdapter::MaxAdapterNameLen);

   toAdd->mType = Vulkan;
   F32 shaderModel = 4.0f;
   toAdd->mCreateDeviceInstanceDelegate = mCreateDeviceInstance;

   // Enumerate all available resolutions:
   VKEnumerateVideoModes(toAdd->mAvailableModes);

   // Add to the list of available adapters.
   adapterList.push_back(toAdd);

   // Cleanup window & open gl context
   SDL_DestroyWindow( tempWindow );
}

//void GFXVulkanDevice::enumerateVideoModes()
//{
//   mVideoModes.clear();
//   EnumerateVideoModes(mVideoModes);
//}

//bool GFXVulkanDevice::beginSceneInternal() 
//{
//   mCanCurrentlyRender = true;
//   return true;
//}

//U32 GFXVulkanDevice::getTotalVideoMemory()
//{
//   return getTotalVideoMemory_GL_EXT();
//}

//------------------------------------------------------------------------------


#endif
