// UI 渲染 Shader

#type vertex
#version 330 core
layout(location = 0) in vec2 aPos;
layout(location = 1) in vec3 aUV;
layout(location = 2) in uint aClr;
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
    vec3 UV;
} texclr;

vec4 rgba2vec4(in uint color)
{
    float r = float(color & uint(0xFF) ) / 255.0;
    float g = float((color >>uint(8) ) & uint(0xFF)) / 255.0;
    float b = float((color >> uint(16)) & uint(0xFF)) / 255.0;
    float a = float((color >> uint(24)))  / 255.0;
    return vec4(r, g, b,a);
}

void main()
{
    texclr.Color = rgba2vec4(aClr);
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
    vec3 UV;
} texclr;
void main()
{
    if(texclr.UV.z == 0)
        fColor = texclr.Color * texture(sTexture, texclr.UV.st);
    else
        fColor = texclr.Color;
}