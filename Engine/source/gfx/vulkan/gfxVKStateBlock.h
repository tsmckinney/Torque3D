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

#ifndef _GFXVKSTATEBLOCK_H_
#define _GFXVKSTATEBLOCK_H_

#include "platform/platform.h"
#include "core/strings/stringFunctions.h"
#include "gfx/gfxCardProfile.h"
#include "gfx/vulkan/gfxVKDevice.h"

// It's worth noting that Vulkan does not automatically handle state in the background like OpenGL does.
// Because of this, we will handle the pipeline state here ourselves.
class GFXVulkanStateBlock : public GFXStateBlock
{
public:
   GFXVulkanStateBlock(const GFXStateBlockDesc& desc);
   virtual ~GFXVulkanStateBlock();
   void activate(GFXVulkanStateBlock* oldState);
   /// Returns the hash value of the desc that created this block
   U32 getHashValue() const override;

   /// Returns a GFXStateBlockDesc that this block represents
   const GFXStateBlockDesc& getDesc() const override;

   //
   // GFXResource
   //
   void zombify() override;
   /// When called the resource should restore all device sensitive information destroyed by zombify()
   void resurrect() override;
private:
   GFXStateBlockDesc mDesc;
   U32 mCachedHash;

   VkSampler mSamplerObjects[GFX_TEXTURE_STAGE_COUNT];
};

#endif
