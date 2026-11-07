
//~~~~~~~~~~~~~~~~~~~~//~~~~~~~~~~~~~~~~~~~~//~~~~~~~~~~~~~~~~~~~~//~~~~~~~~~~~~~~~~~~~~~//
// Arcane-FX - VERTEX SHADER
//
// afxZodiac_Polysoup_V.glsl
//   This is the vertex shader for rendering zodiacs on polysoup models.
//   
// Copyright (C) Faust Logic, Inc.
//~~~~~~~~~~~~~~~~~~~~//~~~~~~~~~~~~~~~~~~~~//~~~~~~~~~~~~~~~~~~~~//~~~~~~~~~~~~~~~~~~~~~//

#include "../../gl/hlslCompat.glsl"

uniform mat4 modelview;

out vec2 texCoord;

layout (location = 0) in vec4 vPosition;
layout (location = 2) in vec4 vColor;
layout (location = 6) in vec2 vTexCoord0;

//~~~~~~~~~~~~~~~~~~~~//

void main()
{
   texCoord = vTexCoord0.st;
   gl_Position = modelview * vPosition;
   correctSSP(gl_Position);
}

//~~~~~~~~~~~~~~~~~~~~//~~~~~~~~~~~~~~~~~~~~//~~~~~~~~~~~~~~~~~~~~//~~~~~~~~~~~~~~~~~~~~~//
