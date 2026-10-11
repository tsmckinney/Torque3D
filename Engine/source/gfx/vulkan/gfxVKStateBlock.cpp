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

#include "gfx/vulkan/gfxVKStateBlock.h"
#include "core/crc.h"

namespace DictHash
{
   inline U32 hash(const GFXSamplerStateDesc& data)
   {
      return CRC::calculateCRC(&data, sizeof(GFXSamplerStateDesc));
   }
}

GFXVulkanStateBlock::GFXVulkanStateBlock(const GFXStateBlockDesc& desc)
{
   AssertFatal(GFXVK->getVKInstance(), "GFXVulkanStateBlock - Instance invalid!");
   AssertFatal(GFXVK->getVKPhysicalDevice(), "GFXVulkanStateBlock - Physical device invalid!");
   AssertFatal(GFXVK->getVKLogicalDevice(), "GFXVulkanStateBlock - Logical device invalid!");
   mDesc = desc;
   mCachedHash = desc.getHashValue();

   static Map<GFXSamplerStateDesc, VkSampler> mSamplersMap;

   for (int i = 0; i < GFX_TEXTURE_STAGE_COUNT; ++i)
   {
      GFXSamplerStateDesc& ssd = mDesc.samplers[i];
      Map<GFXSamplerStateDesc, VkSampler>::Iterator itr = mSamplersMap.find(ssd);
      if (itr == mSamplersMap.end())
      {
         VkSamplerCreateInfo samplerCreate{};
         samplerCreate.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
         samplerCreate.magFilter = minificationFilter(ssd.minFilter);
         samplerCreate.magFilter = GFXVulkanTextureFilter[ssd.magFilter];
         samplerCreate.mipmapMode = mipmappingFilter(ssd.minFilter, ssd.mipFilter, 1);
         samplerCreate.addressModeU = GFXVulkanTextureAddress[ssd.addressModeU];
         samplerCreate.addressModeV = GFXVulkanTextureAddress[ssd.addressModeV];
         samplerCreate.addressModeW = GFXVulkanTextureAddress[ssd.addressModeW];

         if (ssd.addressModeU == GFXAddressBorder ||
            ssd.addressModeV == GFXAddressBorder ||
            ssd.addressModeW == GFXAddressBorder)
         {
            LinearColorF bc = LinearColorF(ssd.borderColor);
            VkBorderColor color = VK_BORDER_COLOR_FLOAT_CUSTOM_EXT;

            VkSamplerCustomBorderColorCreateInfoEXT customBorderCreate{};
            customBorderCreate.sType = VK_STRUCTURE_TYPE_SAMPLER_CUSTOM_BORDER_COLOR_CREATE_INFO_EXT;
            customBorderCreate.customBorderColor.float32[0] = bc.red;
            customBorderCreate.customBorderColor.float32[1] = bc.green;
            customBorderCreate.customBorderColor.float32[2] = bc.blue;
            customBorderCreate.customBorderColor.float32[3] = bc.alpha;
            samplerCreate.pNext = &customBorderCreate;
            samplerCreate.borderColor = color;
         }

         //compare modes
         const bool comparison = ssd.samplerFunc != GFXCmpNever;
         samplerCreate.compareEnable = comparison;
         samplerCreate.compareOp = GFXVulkanCmpFunc[ssd.samplerFunc];
         //glSamplerParameteri(id, GL_TEXTURE_COMPARE_MODE, comparison ? GL_COMPARE_R_TO_TEXTURE_ARB : GL_NONE);
         GFXVulkanCardProfiler* vkCardProfiler = static_cast<GFXVulkanCardProfiler*>(GFXVK->getCardProfiler());
         
         if (vkCardProfiler->getDeviceFeatures().features.samplerAnisotropy)
         {
            samplerCreate.anisotropyEnable = true;
            samplerCreate.maxAnisotropy = ssd.maxAnisotropy;
         }
         AssertFatal(vkCreateSampler(GFXVK->getVKLogicalDevice(), &samplerCreate, nullptr, &mSamplerObjects[i]) == VK_SUCCESS, "Failed to create sampler!");

         mSamplersMap[ssd] = mSamplerObjects[i];
      }
      else
         mSamplerObjects[i] = itr->value;
   }
}

GFXVulkanStateBlock::~GFXVulkanStateBlock()
{
}

void GFXVulkanStateBlock::activate(GFXVulkanStateBlock* oldState)
{
#define STATE_CHANGE(state) (!oldState || oldState->mDesc.state != mDesc.state)

#undef STATE_CHANGE
}

U32 GFXVulkanStateBlock::getHashValue() const
{
   return mCachedHash;
}

const GFXStateBlockDesc& GFXVulkanStateBlock::getDesc() const
{
   return mDesc;
}

void GFXVulkanStateBlock::zombify()
{
}

void GFXVulkanStateBlock::resurrect()
{
}
