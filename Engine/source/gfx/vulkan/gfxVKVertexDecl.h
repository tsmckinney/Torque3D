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

#ifndef _GFXVKVERTEXDECL_H_
#define _GFXVKVERTEXDECL_H_

#include "platform/platform.h"
#include <cstddef>

//-----------------------------------------------------------------------------

class GFXVertexFormat;
class GFXVKDevice;

class GFXVulkanVertexDecl : public GFXVertexDecl
{
public:
   GFXVulkanVertexDecl() : mFormat(NULL), mVertexAttribActiveMask(0) { std::fill_n(mVertexSize, 4, 0); }
   void init(const GFXVertexFormat *format);

   void prepareVertexFormat() const;
   void prepareBuffer_old(U32 stream, S32 mBuffer, S32 mDivisor) const;
   void updateActiveVertexAttrib(U32 lastActiveMask) const;

   struct TVkVertexAttribData
   {
      U32 stream;
      U32 attrIndex;
      U32 elementCount; // 1 - 4
      U32 type; // GL_FLOAT...
      bool normalized;
      size_t stride;
      void* pointerFirst;
   };
   
protected:
   friend class GFXGLDevice;
   const GFXVertexFormat *mFormat;
   U32 mVertexSize[4];
   U32 mVertexAttribActiveMask;
   Vector<TVkVertexAttribData> mVKVerticesFormat;

   void _initVerticesFormat(U32 stream);
   void _initVerticesFormat2();
};

#endif //GFX_GL_VERTEX_DECL
