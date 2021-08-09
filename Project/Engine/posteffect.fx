#ifndef _POSTEFFECT
#define _POSTEFFECT
#include "value.fx"

//=====================
// Distortion Shader
// Mesh : Rect Mesh
#define CloneTex g_tex_0
//=====================

struct VTX_IN
{
    float3 vPosition : POSITION;
    float2 vUV : TEXCOORD;
};

struct VTX_OUT
{
    float4 vPosition : SV_Position;
    float2 vUV : TEXCOORD;
};

VTX_OUT VS_Distortion(VTX_IN _in)
{
    VTX_OUT output = (VTX_OUT) 0.f;
    output.vPosition = float4(_in.vPosition, 1.f);
    output.vUV = _in.vUV;

    return output;
}

float4 PS_Distortion(VTX_OUT _in) : SV_Target
{
    float2 vScreenUV = _in.vPosition.xy / g_vResolution;
    
    vScreenUV.x += sin(vScreenUV.y * 20.f + (g_fAccTime * 12.56)) * 0.05f;
    
    float4 vColor = g_tex_0.Sample(Sample_Anisotropic, vScreenUV);
    return vColor;
}

#endif