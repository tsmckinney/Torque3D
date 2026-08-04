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

#ifndef _GFXVKCUBEMAPARRAY_H_
#define _GFXVKCUBEMAPARRAY_H_

//-----------------------------------------------------------------------------

#include "core/strings/stringFunctions.h"
#include "gfx/vulkan/gfxVKEnumTranslate.h"
#include "gfx/gfxCubemap.h"
#include "gfx/screenshot.h"
#include "gfx/gfxPrimitiveBuffer.h"
#include "gfx/gfxCardProfile.h"
#include "gfx/gfxTextureManager.h"
#include "gfx/bitmap/gBitmap.h"
#include "core/util/safeDelete.h"

class GFXVulkanCubemapArray : public GFXCubemapArray
{
   friend class GFXDevice;
private:
   // should only be called by GFXDevice
   void setToTexUnit(U32 tuNum) override { };

public:
   void init(GFXCubemapHandle *cubemaps, const U32 cubemapCount) override { };
   void init(const U32 cubemapCount, const U32 cubemapFaceSize, const GFXFormat format) override { };
   void updateTexture(const GFXCubemapHandle &cubemap, const U32 slot) override { };
   void copyTo(GFXCubemapArray *pDstCubemap) override { }
   virtual ~GFXVulkanCubemapArray() {};
   void zombify() override {}
   void resurrect() override {}
};

#endif