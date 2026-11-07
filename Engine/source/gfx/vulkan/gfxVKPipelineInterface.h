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

#ifndef _GFXVKPIPELINEINTERFACE_H_
#define _GFXVKPIPELINEINTERFACE_H_

#include "platform/platform.h"
#include "platform/platformVK.h"
#include <volk.h>

//-----------------------------------------------------------------------------

#include "core/util/tVector.h"

class GFXVulkanPipelineInterface
{
public:
   GFXVulkanPipelineInterface();
   ~GFXVulkanPipelineInterface();

   bool canGeneratePipeline();
   void generatePipeline(VkPipeline mPipeline, VkPipelineLayout mPipelineLayout);

   Vector<VkDynamicState> mDynamicStates;
   VkPipelineDynamicStateCreateInfo mDynamicState{};
   VkPipelineVertexInputStateCreateInfo mVertexInputState{};
   VkPipelineInputAssemblyStateCreateInfo mInputAssemblyState{};
   VkPipelineViewportStateCreateInfo mViewportState{};
   VkPipelineRasterizationStateCreateInfo mRasterizer{};
   VkPipelineMultisampleStateCreateInfo mMultisampleState{};
   VkPipelineDepthStencilStateCreateInfo mDepthStencilState{};
   VkPipelineColorBlendAttachmentState mColorBlendAttachment{};
   VkPipelineColorBlendStateCreateInfo mColorBlendState{};
   Vector<VkPipelineShaderStageCreateInfo> mShaderStages;
   Vector<VkRenderPass> mRenderPasses;
};


#endif
