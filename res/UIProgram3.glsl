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


   flat out uint Color;
   out vec3 UV;


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
    Color = aClr;//rgba2vec4(aClr);
    UV = aUV;
    gl_Position = vec4(aPos * uScale + uTranslate, 0, 1);
    gl_Position.y=-gl_Position.y;
}

#type fragment
#version 330 core
layout(location = 0) out vec4 fColor;
uniform sampler2D sTexture;

  flat in  uint Color;
   in  vec3 UV;

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
    vec4 color=rgba2vec4(Color);
    if(UV.z == 0)
        fColor = color * texture(sTexture, UV.st);
    else{
        fColor = color;
    }
}