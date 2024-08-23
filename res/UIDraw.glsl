// UI 渲染 Shader

#type vertex
#version 330 core
layout(location = 0) in vec2 aPos;
layout(location = 1) in vec2 aUV;
layout(location = 2) in uint aClr;

uniform vec2 uScale;
uniform vec2 uTranslate;

out struct { 
    vec4 Color;
    vec2 UV;
} texclr;

vec4 unpack(in uint color)
{
    float r = float(color & uint(0xFF) ) / 255.0;
    float g = float((color >>uint(8) ) & uint(0xFF)) / 255.0;
    float b = float((color >> uint(16)) & uint(0xFF)) / 255.0;
    float a = float((color >> uint(24)))  / 255.0;
    return vec4(r, g, b,a);
}

void main()
{
    texclr.Color = unpack(aClr);
    texclr.UV = aUV;
    gl_Position = vec4(aPos * uScale + uTranslate, 0, 1);
    gl_Position.y=-gl_Position.y;
}

#type fragment
#version 330 core
layout(location = 0) out vec4 fColor;
uniform sampler2D sTexture;
uniform int uiflag;
in struct { 
    vec4 Color;
    vec2 UV;
} texclr;


void main()
{
    uint flag=uint(uiflag);

    if((flag & 0x2u)!=0u)
    {
        fColor=texclr.Color;
        return;
    }
    if((flag & 0x4u)!=0u)
    {
        fColor=texture(sTexture,texclr.UV);
        return;
    }

     fColor=vec4(1.0,0.0,0.0,1.0);
}