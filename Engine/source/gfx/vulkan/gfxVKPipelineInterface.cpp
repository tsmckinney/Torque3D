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

#include "gfx/vulkan/gfxVKPipelineInterface.h"
#include "gfx/vulkan/gfxVKWindowTarget.h"

GFXVulkanPipelineInterface::GFXVulkanPipelineInterface()
{
}

GFXVulkanPipelineInterface::~GFXVulkanPipelineInterface()
{
}

bool GFXVulkanPipelineInterface::canGeneratePipeline()
{
   bool allDefined = mDynamicState.sType != NULL;
   allDefined &= mVertexInputState.sType != NULL;
   allDefined &= mInputAssemblyState.sType != NULL;
   allDefined &= mViewportState.sType != NULL;
   allDefined &= mRasterizer.sType != NULL;
   allDefined &= mMultisampleState.sType != NULL;
   allDefined &= mDepthStencilState.sType != NULL;
   allDefined &= mColorBlendState.sType != NULL;

   return allDefined;
}

void GFXVulkanPipelineInterface::generatePipeline(VkPipeline mPipeline, VkPipelineLayout mPipelineLayout)
{
   VkGraphicsPipelineCreateInfo pipelineCI{};
   pipelineCI.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
   pipelineCI.stageCount = mShaderStages.size();
   pipelineCI.pStages = mShaderStages.address();
   pipelineCI.pVertexInputState = &mVertexInputState;
   pipelineCI.pInputAssemblyState = &mInputAssemblyState;
   pipelineCI.pViewportState = &mViewportState;
   pipelineCI.pRasterizationState = &mRasterizer;
   pipelineCI.pMultisampleState = &mMultisampleState;
   pipelineCI.pDepthStencilState = &mDepthStencilState;
   pipelineCI.pColorBlendState = &mColorBlendState;
   pipelineCI.pDynamicState = &mDynamicState;
   pipelineCI.layout = mPipelineLayout;
   pipelineCI.renderPass = static_cast<GFXVulkanWindowTarget*>(GFXVK->getActiveRenderTarget())->mMainRenderPass;
   pipelineCI.subpass = 0;
   AssertFatal(vkCreateGraphicsPipelines(GFXVK->getVKLogicalDevice(), GFXVK->getVKPipelineCache(), 1, &pipelineCI, nullptr, &mPipeline) == VK_SUCCESS,
      "GFXVulkanPipelineInterface::generatePipeline - Failed to generate pipeline!");
}
