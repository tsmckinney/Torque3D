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

#ifndef _GFXVKENUMTRANSLATE_H_
#define _GFXVKENUMTRANSLATE_H_

#include "gfx/gfxEnums.h"

namespace GFXVulkanEnumTranslate
{
   void init();
};

extern U32 GFXVulkanPrimType[GFXPT_COUNT];
extern U32 GFXVulkanBlend[GFXBlend_COUNT];
extern U32 GFXVulkanBlendOp[GFXBlendOp_COUNT];
extern U32 GFXVulkanTextureFilter[GFXTextureFilter_COUNT];
extern U32 GFXVulkanTextureAddress[GFXAddress_COUNT];
extern U32 GFXVulkanCmpFunc[GFXCmp_COUNT];
extern U32 GFXVulkanStencilOp[GFXStencilOp_COUNT];

extern U32 GFXVulkanTextureFormat[GFXFormat_COUNT];
extern U32 GFXVulkanTextureType[GFXFormat_COUNT];
extern U32* GFXVulkanTextureSwizzle[GFXFormat_COUNT];

//extern U32 GFXVulkanBufferType[GFXBufferType_COUNT];
extern U32 GFXVulkanCullMode[GFXCull_COUNT];

extern U32 GFXVulkanFillMode[GFXFill_COUNT];

#endif
