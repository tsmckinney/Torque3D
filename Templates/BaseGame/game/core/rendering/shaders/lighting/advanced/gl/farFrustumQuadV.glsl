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

#include "../../../gl/hlslCompat.glsl"
#include "farFrustumQuad.glsl"

layout (location = 0) in vec4 vPosition;
layout (location = 1) in vec3 vNormal;
layout (location = 3) in vec3 vTangent;
layout (location = 6) in vec2 vTexCoord0;

uniform vec4 rtParams0;
layout (location = 1) out vec4 hpos;
layout (location = 0) out vec2 uv0;
layout (location = 4) out vec3 wsEyeRay;
layout (location = 5) out vec3 vsEyeRay;

void main()
{   
   hpos = vec4( vTexCoord0, 0, 1 );   

   // Get a RT-corrected UV from the SS coord
   uv0 = getUVFromSSPos( hpos.xyz, rtParams0 );
   gl_Position = hpos;   
   
   // Interpolators will generate eye rays the 
   // from far-frustum corners.
   wsEyeRay = vTangent;
   vsEyeRay = vNormal;
   
   correctSSP(gl_Position);
}
