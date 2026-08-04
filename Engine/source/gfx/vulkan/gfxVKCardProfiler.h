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

#ifndef _GFXVKCARDPROFILER_H_
#define _GFXVKCARDPROFILER_H_

#include "platform/platform.h"
//-----------------------------------------------------------------------------
#include "gfx/vulkan/gfxVKDevice.h"
#include "gfx/vulkan/gfxVKEnumTranslate.h"
#include "gfx/vulkan/gfxVKHelpers.h"
#include "core/strings/stringFunctions.h"
#include "gfx/gfxCardProfile.h"
#include "core/util/safeDelete.h"

class GFXVulkanCardProfiler: public GFXCardProfiler
{
private:
    friend class GFXVulkanDevice;
    typedef GFXCardProfiler Parent;
public:

    ///
    const String &getRendererString() const override;

protected:

    void setupCardCapabilities() override;

    bool _queryCardCap(const String &query, U32 &foundResult) override;
    bool _queryFormat(const GFXFormat fmt, const GFXTextureProfile *profile, bool &inOutAutogenMips) override;
    
public:
    void init() override;
    String mRenderString;
};
#endif
