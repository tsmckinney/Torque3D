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
#include "gfx/vulkan/gfxVKDevice.h"
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

   mDynamicStates.push_back(VK_DYNAMIC_STATE_VIEWPORT);
   if (mDesc.stencilEnable) {
      mDynamicStates.push_back(VK_DYNAMIC_STATE_STENCIL_TEST_ENABLE);
   }
   mDynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
   mDynamicState.dynamicStateCount = mDynamicStates.size();
   mDynamicState.pDynamicStates = mDynamicStates.address();

   // TODO: Probably a good idea to move this to the vertex buffer code when I implement it! ;)
   mVertexInputState.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
   mVertexInputState.vertexAttributeDescriptionCount = 0;
   mVertexInputState.pVertexBindingDescriptions = nullptr;
   mVertexInputState.vertexBindingDescriptionCount = 0;
   mVertexInputState.pVertexAttributeDescriptions = nullptr;

   VkViewport viewport{};
   viewport.x = GFXVK->getViewport().point.x;
   viewport.y = GFXVK->getViewport().point.y;
   viewport.width = (F32)GFXVK->getViewport().extent.x;
   viewport.height = (F32)GFXVK->getViewport().extent.y;
   viewport.minDepth = 0.0f;
   viewport.maxDepth = 1.0f;
   mViewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
   mViewportState.viewportCount = 1;
   mViewportState.pViewports = &viewport;
   mViewportState.scissorCount = 0; // Torque3D doesn't use scissors. Unsure if that's a planned feature or not.

   mRasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
   mRasterizer.depthClampEnable = VK_FALSE; // TODO: Figure out whether shadow rendering needs this enabled!
   mRasterizer.rasterizerDiscardEnable = VK_FALSE;
   mRasterizer.polygonMode = GFXVulkanFillMode[mDesc.fillMode];
   mRasterizer.lineWidth = 1.0f;
   mRasterizer.cullMode = GFXVulkanCullMode[mDesc.cullMode];
   mRasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE; // FIXME: Hardcoded for now, see if this is defined in the state block!
   mRasterizer.depthBiasEnable = mDesc.zBias != 0.0f;
   mRasterizer.depthBiasConstantFactor = mDesc.zBias;
   mRasterizer.depthBiasSlopeFactor = mDesc.zSlopeBias;

   mMultisampleState.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
   mMultisampleState.sampleShadingEnable = VK_FALSE; // TODO: Implement later!
   mMultisampleState.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

   mColorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
   mColorBlendAttachment.blendEnable = VK_FALSE; // TODO: Implement later!

   mColorBlendState.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
   mColorBlendState.logicOpEnable = VK_FALSE;
   mColorBlendState.attachmentCount = 1;
   mColorBlendState.pAttachments = &mColorBlendAttachment;

   GFXVK->getVKPipelineInterface()->mDynamicState = mDynamicState;
   GFXVK->getVKPipelineInterface()->mViewportState = mViewportState;
   GFXVK->getVKPipelineInterface()->mRasterizer = mRasterizer;
   GFXVK->getVKPipelineInterface()->mMultisampleState = mMultisampleState;
   GFXVK->getVKPipelineInterface()->mColorBlendAttachment = mColorBlendAttachment;
   GFXVK->getVKPipelineInterface()->mColorBlendState = mColorBlendState;

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
   Con::printf("Activating state block!");
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
