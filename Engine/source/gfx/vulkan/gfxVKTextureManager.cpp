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

#include "gfx/vulkan/gfxVKTextureManager.h"
#include "gfx/vulkan/gfxVKTextureObject.h"
#include "gfx/vulkan/gfxVKWindowTarget.h"

//-----------------------------------------------------------------------------

GFXTextureObject* GFXVulkanTextureManager::_createTextureObject( U32 height, 
                                                      U32 width, 
                                                      U32 depth, 
                                                      GFXFormat format, 
                                                      GFXTextureProfile *profile, 
                                                      U32 numMipLevels, 
                                                      bool forceMips, 
                                                      S32 antialiasLevel,
                                                      U32 arraySize,
                                                      GFXTextureObject *inTex )
{ 
    GFXVulkanTextureObject *retTex;
    if ( inTex )
    {
        AssertFatal( dynamic_cast<GFXVulkanTextureObject*>( inTex ), "GFXVulkanTextureManager::_createTexture() - Bad inTex type!" );
        retTex = static_cast<GFXVulkanTextureObject*>( inTex );
    }      
    else
    {
        retTex = new GFXVulkanTextureObject( GFX, profile );
        retTex->registerResourceWithDevice( GFX );
    }
    SAFE_DELETE( retTex->mBitmap );
    retTex->mBitmap = new GBitmap(width, height);
    return retTex;
}
