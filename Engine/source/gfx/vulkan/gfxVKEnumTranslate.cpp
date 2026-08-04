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

#include "platform/platform.h"
#include "gfx/vulkan/gfxVKEnumTranslate.h"
 
U32 GFXVulkanPrimType[GFXPT_COUNT];
U32 GFXVulkanBlend[GFXBlend_COUNT];
U32 GFXVulkanBlendOp[GFXBlendOp_COUNT];
U32 GFXVulkanTextureFilter[GFXTextureFilter_COUNT];
U32 GFXVulkanTextureAddress[GFXAddress_COUNT];
U32 GFXVulkanCmpFunc[GFXCmp_COUNT];
U32 GFXVulkanStencilOp[GFXStencilOp_COUNT];
U32 GFXVulkanTextureFormat[GFXFormat_COUNT];
U32 GFXVulkanTextureType[GFXFormat_COUNT];
U32* GFXVulkanTextureSwizzle[GFXFormat_COUNT];
//U32 GFXVulkanBufferType[GFXBufferType_COUNT];
U32 GFXVulkanCullMode[GFXCull_COUNT];
U32 GFXVulkanFillMode[GFXFill_COUNT];

void GFXVulkanEnumTranslate::init()
{
   // Buffer types
   //GFXVulkanBufferType[GFXBufferTypeStatic] = GL_STATIC_DRAW;
   //GFXVulkanBufferType[GFXBufferTypeDynamic] = GL_DYNAMIC_DRAW;
   //GFXVulkanBufferType[GFXBufferTypeVolatile] = GL_STREAM_DRAW;
   //GFXVulkanBufferType[GFXBufferTypeImmutable] = GL_STATIC_DRAW;

   // Primitives
   GFXVulkanPrimType[GFXPointList] = VK_PRIMITIVE_TOPOLOGY_POINT_LIST;
   GFXVulkanPrimType[GFXLineList] = VK_PRIMITIVE_TOPOLOGY_LINE_LIST;
   GFXVulkanPrimType[GFXLineStrip] = VK_PRIMITIVE_TOPOLOGY_LINE_STRIP;
   GFXVulkanPrimType[GFXTriangleList] = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
   GFXVulkanPrimType[GFXTriangleStrip] = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP;

   // Blend
   GFXVulkanBlend[GFXBlendZero] = VK_BLEND_FACTOR_ZERO;
   GFXVulkanBlend[GFXBlendOne] = VK_BLEND_FACTOR_ONE;
   GFXVulkanBlend[GFXBlendSrcColor] = VK_BLEND_FACTOR_SRC_COLOR;
   GFXVulkanBlend[GFXBlendInvSrcColor] = VK_BLEND_FACTOR_ONE_MINUS_SRC_COLOR;
   GFXVulkanBlend[GFXBlendSrcAlpha] = VK_BLEND_FACTOR_SRC_ALPHA;
   GFXVulkanBlend[GFXBlendInvSrcAlpha] = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
   GFXVulkanBlend[GFXBlendDestAlpha] = VK_BLEND_FACTOR_DST_ALPHA;
   GFXVulkanBlend[GFXBlendInvDestAlpha] = VK_BLEND_FACTOR_ONE_MINUS_DST_ALPHA;
   GFXVulkanBlend[GFXBlendDestColor] = VK_BLEND_FACTOR_DST_COLOR;
   GFXVulkanBlend[GFXBlendInvDestColor] = VK_BLEND_FACTOR_ONE_MINUS_DST_COLOR;
   GFXVulkanBlend[GFXBlendSrcAlphaSat] = VK_BLEND_FACTOR_SRC_ALPHA_SATURATE;
   
   // Blend op
   GFXVulkanBlendOp[GFXBlendOpAdd] = VK_BLEND_OP_ADD;
   GFXVulkanBlendOp[GFXBlendOpSubtract] = VK_BLEND_OP_SUBTRACT;
   GFXVulkanBlendOp[GFXBlendOpRevSubtract] = VK_BLEND_OP_REVERSE_SUBTRACT;
   GFXVulkanBlendOp[GFXBlendOpMin] = VK_BLEND_OP_MIN;
   GFXVulkanBlendOp[GFXBlendOpMax] = VK_BLEND_OP_MAX;

  
   // Comparison
   GFXVulkanCmpFunc[GFXCmpNever] = VK_COMPARE_OP_NEVER;
   GFXVulkanCmpFunc[GFXCmpLess] = VK_COMPARE_OP_LESS;
   GFXVulkanCmpFunc[GFXCmpEqual] = VK_COMPARE_OP_EQUAL;
   GFXVulkanCmpFunc[GFXCmpLessEqual] = VK_COMPARE_OP_LESS_OR_EQUAL;
   GFXVulkanCmpFunc[GFXCmpGreater] = VK_COMPARE_OP_GREATER;
   GFXVulkanCmpFunc[GFXCmpNotEqual] = VK_COMPARE_OP_NOT_EQUAL;
   GFXVulkanCmpFunc[GFXCmpGreaterEqual] = VK_COMPARE_OP_GREATER_OR_EQUAL;
   GFXVulkanCmpFunc[GFXCmpAlways] = VK_COMPARE_OP_ALWAYS;

   GFXVulkanTextureFilter[GFXTextureFilterNone] = VK_FILTER_NEAREST;
   GFXVulkanTextureFilter[GFXTextureFilterPoint] = VK_FILTER_NEAREST;
   GFXVulkanTextureFilter[GFXTextureFilterLinear] = VK_FILTER_LINEAR;
   GFXVulkanTextureFilter[GFXTextureFilterAnisotropic] = VK_FILTER_CUBIC_EXT;

   GFXVulkanTextureAddress[GFXAddressWrap] = VK_SAMPLER_ADDRESS_MODE_REPEAT;
   GFXVulkanTextureAddress[GFXAddressMirror] = VK_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT;
   GFXVulkanTextureAddress[GFXAddressClamp] = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
   GFXVulkanTextureAddress[GFXAddressBorder] = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER;
   GFXVulkanTextureAddress[GFXAddressMirrorOnce] = VK_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT;
   
   // Stencil ops
   GFXVulkanStencilOp[GFXStencilOpKeep] = VK_STENCIL_OP_KEEP;
   GFXVulkanStencilOp[GFXStencilOpZero] = VK_STENCIL_OP_ZERO;
   GFXVulkanStencilOp[GFXStencilOpReplace] = VK_STENCIL_OP_REPLACE;
   GFXVulkanStencilOp[GFXStencilOpIncrSat] = VK_STENCIL_OP_INCREMENT_AND_CLAMP;
   GFXVulkanStencilOp[GFXStencilOpDecrSat] = VK_STENCIL_OP_DECREMENT_AND_CLAMP;
   GFXVulkanStencilOp[GFXStencilOpInvert] = VK_STENCIL_OP_INVERT;
   
   GFXVulkanStencilOp[GFXStencilOpIncr] = VK_STENCIL_OP_INCREMENT_AND_WRAP;
   GFXVulkanStencilOp[GFXStencilOpDecr] = VK_STENCIL_OP_DECREMENT_AND_WRAP;   
   
   // Texture formats
   GFXVulkanTextureFormat[GFXFormatA8] = VK_FORMAT_R8_UNORM;
   GFXVulkanTextureFormat[GFXFormatL8] = VK_FORMAT_R8_UNORM;
   GFXVulkanTextureFormat[GFXFormatA8L8] = VK_FORMAT_R8G8_UNORM;
   GFXVulkanTextureFormat[GFXFormatR5G5B5A1] = VK_FORMAT_R5G5B5A1_UNORM_PACK16;
   GFXVulkanTextureFormat[GFXFormatR5G5B5X1] = VK_FORMAT_R5G5B5A1_UNORM_PACK16;
   GFXVulkanTextureFormat[GFXFormatL16] = VK_FORMAT_R16_UNORM;
   GFXVulkanTextureFormat[GFXFormatD16] = VK_FORMAT_D16_UNORM;
   GFXVulkanTextureFormat[GFXFormatR8G8B8] = VK_FORMAT_R8G8B8_UNORM;
   GFXVulkanTextureFormat[GFXFormatR8G8B8A8] = VK_FORMAT_R8G8B8A8_UNORM;
   GFXVulkanTextureFormat[GFXFormatR8G8B8X8] = VK_FORMAT_R8G8B8A8_UNORM;
   GFXVulkanTextureFormat[GFXFormatB8G8R8A8] = VK_FORMAT_B8G8R8A8_UNORM;
   GFXVulkanTextureFormat[GFXFormatR10G10B10A2] = VK_FORMAT_A2R10G10B10_UNORM_PACK32;
   GFXVulkanTextureFormat[GFXFormatR11G11B10] = VK_FORMAT_B10G11R11_UFLOAT_PACK32;   
   GFXVulkanTextureFormat[GFXFormatD32] = VK_FORMAT_D32_SFLOAT;
   GFXVulkanTextureFormat[GFXFormatD24X8] = VK_FORMAT_D24_UNORM_S8_UINT;
   GFXVulkanTextureFormat[GFXFormatD24S8] = VK_FORMAT_D24_UNORM_S8_UINT;
   GFXVulkanTextureFormat[GFXFormatD32FS8X24] = VK_FORMAT_D32_SFLOAT_S8_UINT;
   GFXVulkanTextureFormat[GFXFormatR16G16B16A16] = VK_FORMAT_R16G16B16A16_UNORM;
   GFXVulkanTextureFormat[GFXFormatBC1] = VK_FORMAT_BC1_RGBA_UNORM_BLOCK;
   GFXVulkanTextureFormat[GFXFormatBC2] = VK_FORMAT_BC2_UNORM_BLOCK;
   GFXVulkanTextureFormat[GFXFormatBC3] = VK_FORMAT_BC3_UNORM_BLOCK;
   GFXVulkanTextureFormat[GFXFormatBC4] = VK_FORMAT_BC4_UNORM_BLOCK;
   GFXVulkanTextureFormat[GFXFormatBC5] = VK_FORMAT_BC5_UNORM_BLOCK;
   //sRGB
   GFXVulkanTextureFormat[GFXFormatR8G8B8_SRGB] = VK_FORMAT_R8G8B8_SRGB;
   GFXVulkanTextureFormat[GFXFormatR8G8B8A8_SRGB] = VK_FORMAT_R8G8B8A8_SRGB;
   GFXVulkanTextureFormat[GFXFormatBC1_SRGB] = VK_FORMAT_BC1_RGBA_SRGB_BLOCK;
   GFXVulkanTextureFormat[GFXFormatBC2_SRGB] = VK_FORMAT_BC2_SRGB_BLOCK;
   GFXVulkanTextureFormat[GFXFormatBC3_SRGB] = VK_FORMAT_BC3_SRGB_BLOCK;

   static U32 Swizzle_GFXFormatA8[] = { VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_R };
   static U32 Swizzle_GFXFormatL[] = { VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_A };
   GFXVulkanTextureSwizzle[GFXFormatA8] = Swizzle_GFXFormatA8; // old GL_ALPHA8   
   GFXVulkanTextureSwizzle[GFXFormatL8] = Swizzle_GFXFormatL; // old GL_LUMINANCE8
   GFXVulkanTextureSwizzle[GFXFormatL16] = Swizzle_GFXFormatL; // old GL_LUMINANCE16

   GFXVulkanTextureFormat[GFXFormatR32F] = VK_FORMAT_R32_SFLOAT;

   GFXVulkanTextureFormat[GFXFormatR32G32B32A32F] = VK_FORMAT_R32G32B32A32_SFLOAT;

   GFXVulkanTextureFormat[GFXFormatR16F] = VK_FORMAT_R16_SFLOAT;

   GFXVulkanTextureFormat[GFXFormatR16G16F] = VK_FORMAT_R16G16_SFLOAT;

   GFXVulkanTextureFormat[GFXFormatR16G16B16A16F] = VK_FORMAT_R16G16B16A16_SFLOAT;

   GFXVulkanTextureFormat[GFXFormatR5G6B5] = VK_FORMAT_R5G6B5_UNORM_PACK16;

   GFXVulkanTextureFormat[GFXFormatR16G16] = VK_FORMAT_R16G16_UINT;

   // Cull - Opengl render upside down need to invert cull
   GFXVulkanCullMode[GFXCullNone] = VK_CULL_MODE_NONE;
   GFXVulkanCullMode[GFXCullCW] = VK_CULL_MODE_FRONT_BIT;
   GFXVulkanCullMode[GFXCullCCW] = VK_CULL_MODE_BACK_BIT;

   // Fill
   GFXVulkanFillMode[GFXFillPoint] = VK_POLYGON_MODE_POINT;
   GFXVulkanFillMode[GFXFillWireframe] = VK_POLYGON_MODE_LINE;
   GFXVulkanFillMode[GFXFillSolid] = VK_POLYGON_MODE_FILL;

   //cubemap face type
   //GFXVulkanFaceType[0] = GL_TEXTURE_CUBE_MAP_POSITIVE_X;
   //GFXVulkanFaceType[1] = GL_TEXTURE_CUBE_MAP_NEGATIVE_X;
   //GFXVulkanFaceType[2] = GL_TEXTURE_CUBE_MAP_POSITIVE_Y;
   //GFXVulkanFaceType[3] = GL_TEXTURE_CUBE_MAP_NEGATIVE_Y;
   //GFXVulkanFaceType[4] = GL_TEXTURE_CUBE_MAP_POSITIVE_Z;
   //GFXVulkanFaceType[5] = GL_TEXTURE_CUBE_MAP_NEGATIVE_Z;
}
