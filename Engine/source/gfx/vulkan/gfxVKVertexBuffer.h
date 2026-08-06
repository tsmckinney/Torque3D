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

#ifndef _GFXVKVERTEXBUFFER_H_
#define _GFXVKVERTEXBUFFER_H_

#include "platform/platform.h"

//-----------------------------------------------------------------------------

#include "gfx/gfxDevice.h"
#include "gfx/gfxInit.h"
#include "gfx/gfxFence.h"
#include "gfx/vulkan/gfxVKEnumTranslate.h"

class GFXVulkanVertexBuffer : public GFXVertexBuffer 
{
   unsigned char* tempBuf;
public:
   GFXVulkanVertexBuffer( GFXDevice *device, 
                        U32 numVerts, 
                        const GFXVertexFormat *vertexFormat, 
                        U32 vertexSize, 
                        GFXBufferType bufferType ) :
      GFXVertexBuffer(device, numVerts, vertexFormat, vertexSize, bufferType) {tempBuf =NULL;};
   void lock(U32 vertexStart, U32 vertexEnd, void **vertexPtr) override;
   void unlock() override;
   void prepare() override;

   void zombify() override;
   void resurrect() override;
};

#endif
