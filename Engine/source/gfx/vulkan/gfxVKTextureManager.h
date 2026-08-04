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

#ifndef _GFXVKTEXTUREMANAGER_H_
#define _GFXVKTEXTUREMANAGER_H_

#include "platform/platform.h"

//-----------------------------------------------------------------------------

#include "gfx/gfxDevice.h"
#include "gfx/gfxTextureManager.h"

#include "gfx/vulkan/gfxVKEnumTranslate.h"

class GFXVulkanTextureManager : public GFXTextureManager
{
public:
   GFXTextureObject* createTexture(GBitmap* bmp, const String& resourceName, GFXTextureProfile* profile, bool deleteBmp) override { return nullptr; } // _createNullTextureObject();}
   GFXTextureObject* createTexture(DDSFile* dds, GFXTextureProfile* profile, bool deleteDDS) override { return nullptr; }
   GFXTextureObject* createTexture(const Torque::Path& path, GFXTextureProfile* profile) override { return nullptr; }
   GFXTextureObject* createTexture(U32 width, U32 height, void* pixels, GFXFormat format, GFXTextureProfile* profile) override { return nullptr; }
   GFXTextureObject* createTexture(U32 width, U32 height, U32 depth, GFXFormat format, GFXTextureProfile* profile, U32 numMipLevels = 1, U32 arraySize = 1) override { return nullptr; }
   GFXTextureObject* createTexture(U32 width, U32 height, GFXFormat format, GFXTextureProfile* profile, U32 numMipLevels, S32 antialiasLevel, U32 arraySize = 1) override { return nullptr; }
   GFXTextureObject* createCompositeTexture(GBitmap* bmp[4], U32 inputKey[4], const String& resourceName, GFXTextureProfile* profile, bool deleteBmp) override { return nullptr; }
protected:
      GFXTextureObject *_createTextureObject( U32 height, 
                                                      U32 width, 
                                                      U32 depth, 
                                                      GFXFormat format, 
                                                      GFXTextureProfile *profile, 
                                                      U32 numMipLevels, 
                                                      bool forceMips = false, 
                                                      S32 antialiasLevel = 0,
                                                      U32 arraySize = 1,
                                                      GFXTextureObject *inTex = NULL ) override;

      /// Load a texture from a proper DDSFile instance.
      bool _loadTexture(GFXTextureObject *texture, DDSFile *dds) override{ return true; };

      /// Load data into a texture from a GBitmap using the internal API.
      bool _loadTexture(GFXTextureObject *texture, GBitmap *bmp) override{ return true; };

      /// Load data into a texture from a raw buffer using the internal API.
      ///
      /// Note that the size of the buffer is assumed from the parameters used
      /// for this GFXTextureObject's _createTexture call.
      bool _loadTexture(GFXTextureObject *texture, void *raw) override{ return true; };

      /// Refresh a texture using the internal API.
      bool _refreshTexture(GFXTextureObject *texture) override{ return true; };

      /// Free a texture (but do not delete the GFXTextureObject) using the internal
      /// API.
      ///
      /// This is only called during zombification for textures which need it, so you
      /// don't need to do any internal safety checks.
      bool _freeTexture(GFXTextureObject *texture, bool zombify=false) override { return true; };

      virtual U32 _getTotalVideoMemory() { return 0; };
      virtual U32 _getFreeVideoMemory() { return 0; };
};

#endif
