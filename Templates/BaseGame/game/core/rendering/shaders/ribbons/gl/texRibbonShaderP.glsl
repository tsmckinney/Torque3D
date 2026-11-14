#include "../../gl/hlslCompat.glsl"
#include "../../gl/torque.glsl"

layout (location = 0) in float4 _hpos;
layout (location = 1) in float2 _texCoord;
layout (location = 2) in float2 _shiftdata;
layout (location = 3) in float4 _color;
 
#define IN_hpos _hpos
#define IN_texCoord _texCoord
#define IN_shiftdata _shiftdata
#define IN_color _color
 
layout (location = 0) out float4 OUT_col;
layout(binding = 0) uniform sampler2D ribTex;

void main()
{
    float4 Tex = tex2D(ribTex,IN_texCoord);
	Tex.a *= IN_color.a;
	OUT_col = hdrEncode(Tex);
}