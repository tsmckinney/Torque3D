#include "../../gl/hlslCompat.glsl"
 
layout (location = 0) in float4 _hpos;
layout (location = 1) in float2 _texCoord;
layout (location = 2) in float2 _shiftdata;
layout (location = 3) in float4 _color;
 
#define IN_hpos _hpos
#define IN_texCoord _texCoord
#define IN_shiftdata _shiftdata
#define IN_color _color
 
layout (location = 0) out float4 OUT_col;
 
void main()
{
   float fade = 1.0 - abs(IN_shiftdata.y - 0.5) * 2.0;
   OUT_col.xyz = IN_color.xyz + pow(fade, 4) / 10;
   OUT_col.a = IN_color.a * fade;
}