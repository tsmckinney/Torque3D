#include "../../gl/hlslCompat.glsl"
 
layout (location = 6) in float2 vTexCoord0;
layout (location = 7) in float2 vTexCoord1;
layout (location = 1) in float3 vNormal;
layout (location = 0) in float4 vPosition;
layout (location = 2) in float4 vColor;
 
#define IN_texCoord vTexCoord0
#define IN_shiftdata vTexCoord1
#define IN_normal vNormal
#define IN_position vPosition
#define IN_color vColor
 
layout (location = 0) out float4 _hpos;
layout (location = 1) out float2 _texCoord;
layout (location = 2) out float2 _shiftdata;
layout (location = 3) out float4 _color;
 
#define OUT_hpos _hpos
#define OUT_texCoord _texCoord
#define OUT_shiftdata _shiftdata
#define OUT_color _color
 
uniform float4x4 modelview;
uniform float3   eyePos;
 
void main()
{
    OUT_hpos = tMul(modelview, IN_position);
    OUT_color = IN_color;
    OUT_texCoord = IN_texCoord;
    OUT_shiftdata = IN_shiftdata;
   
    gl_Position = OUT_hpos;
    correctSSP(gl_Position);
}