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
    output.vPosition = float4(_in.vPosition * 2.f, 1.f);
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


//=====================
// FishEye Shader
// Mesh : Circle Mesh
#define CloneTex g_tex_0
//=====================

struct VTX_FISHEYE_OUT
{
    float4 vPosition : SV_Position;
    float2 vCenterPos : POSITION; // 투영좌표계 중앙 위치
    float2 vRadius : POSITION2; // 사각형의 반지름 길이(투영좌표계 기준)
    float2 vUV : TEXCOORD;
};

VTX_FISHEYE_OUT VS_FishEye(VTX_IN _in)
{
    VTX_FISHEYE_OUT output = (VTX_FISHEYE_OUT) 0.f;

    output.vPosition = mul(float4(_in.vPosition, 1.f), g_matWorldViewProj);
    float4 vCenterProj = mul(float4(0.f, 0.f, 0.f, 1.f), g_matWorldViewProj);
    output.vCenterPos = vCenterProj.xy / vCenterProj.w;
    
    float2 vProj = output.vPosition.xy / output.vPosition.w;
    
    //output.vRadius = float2(abs(vProj.x - output.vCenterPos.x), abs(vProj.y - output.vCenterPos.y));
    output.vRadius = float2(0.8, 0.8);
    
    output.vUV = _in.vUV;

    return output;
}

float4 PS_FishEye(VTX_FISHEYE_OUT _in) : SV_Target
{
    float2 vScreenUV = _in.vPosition.xy / g_vResolution;
    float4 vColor = (float4) 0.f;
	
    float aperture = 120;
    float apertureHalf = 0.4 * aperture * (PI / 180.0);
    float maxFactor = sin(apertureHalf);
  
    float2 uv;
    float2 vScreenProj = 2.0 * vScreenUV.xy - 1.0;
    float2 vLocalProj = 2.0 * _in.vUV - 1.0;
    
    float d = length(vLocalProj);
    if (d < (2.0 - maxFactor))
    {
        d = length(vLocalProj * maxFactor);
        float z = sqrt(1.f - d * d);
        float r = atan2(d, z) / PI;
        float phi = atan2(vLocalProj.y, vLocalProj.x);
    
        uv.x = r * cos(phi) + 0.5;
        uv.y = r * sin(phi) + 0.5;
        
        // UV 추가 보정
        float2 vLTuv = float2(_in.vCenterPos.x, _in.vCenterPos.y) * 0.5f + 0.5f;
        float2 vUVRadius = _in.vRadius * 0.5f;
	
        uv = (uv * vUVRadius) + 0.5f - vUVRadius / 2.f; // 범위 압축
    
        float2 vDiff = vLTuv - float2(0.5f, 0.5f); // 타겟위치로 이동    
        uv.x += vDiff.x;
        uv.y -= vDiff.y;
    }
    else
    {
        uv = vScreenUV.xy;
    }
    
    vColor = CloneTex.Sample(g_sam_0, uv);
    return vColor;
}

#endif