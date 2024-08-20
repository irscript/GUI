// UI 渲染 Shader

#type vertex
#version 330 core
layout(location = 0) in vec2 aPos;
layout(location = 1) in vec2 aUV;
layout(location = 2) in vec4 aClr;
/*
layout (std140) uniform uPushConstant
{
    vec2 uScale;// 缩放
    vec2 uTranslate;// 平移
};*/

uniform vec2 uScale;// 缩放
uniform vec2 uTranslate;// 平移

out struct { 
    vec4 Color;
    vec2 UV;
} texclr;

void main()
{
    texclr.Color = aClr;
    texclr.UV = aUV;
    gl_Position = vec4(aPos * uScale + uTranslate, 0, 1);
    gl_Position.y=-gl_Position.y;
}

#type fragment
#version 330 core
layout(location = 0) out vec4 fColor;
uniform sampler2D sTexture;
in struct { 
    vec4 Color;
    vec2 UV;
} texclr;
void main()
{
    fColor = texclr.Color * texture(sTexture, texclr.UV.st);
}