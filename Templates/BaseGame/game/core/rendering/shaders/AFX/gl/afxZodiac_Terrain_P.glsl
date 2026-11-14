
//~~~~~~~~~~~~~~~~~~~~//~~~~~~~~~~~~~~~~~~~~//~~~~~~~~~~~~~~~~~~~~//~~~~~~~~~~~~~~~~~~~~~//
// Arcane-FX - PIXEL SHADER
//
// afxZodiac_Terrain_P.glsl
//   This is the pixel shader for rendering zodiacs on terrain.
//   
// Copyright (C) Faust Logic, Inc.
//~~~~~~~~~~~~~~~~~~~~//~~~~~~~~~~~~~~~~~~~~//~~~~~~~~~~~~~~~~~~~~//~~~~~~~~~~~~~~~~~~~~~//

layout(binding = 0) uniform sampler2D zodiacMap;
uniform vec4 zodiacColor;

layout (location = 0) in vec4 hpos;
layout (location = 6) in vec2 texCoord;

out vec4 OUT_FragColor0;
//~~~~~~~~~~~~~~~~~~~~//

void main()
{
   OUT_FragColor0 = zodiacColor*texture(zodiacMap, texCoord);
}

//~~~~~~~~~~~~~~~~~~~~//~~~~~~~~~~~~~~~~~~~~//~~~~~~~~~~~~~~~~~~~~//~~~~~~~~~~~~~~~~~~~~~//
