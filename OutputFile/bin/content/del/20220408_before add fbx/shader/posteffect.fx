#ifndef _POSTEFFECT
#define _POSTEFFECT
#include "value.fx"
#include "function.fx"

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
    
    float4 vColor = CloneTex.Sample(Sample_Anisotropic, vScreenUV);
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

//=====================
// Blur Shader
// Mesh : Rect Mesh
#define CloneTex g_tex_0
#define WidthOrHeight g_int_0 // 가로 or 세로 flag
#define Amplitude   g_float_0 // 진폭
#define Compression g_float_1 // 압축 길이
#define ScreenScale g_float_2 // 화면 크기 스케일
#define Speed       g_float_3 // 출렁이는 속도
//=====================



float hash(in float2 uv)
{
    float seed = dot(uv, float2(Rand(g_fDeltaTime), g_fDeltaTime + 0.1f));
    return frac(38351.43f * sin(14.13f * seed));
}

void mainImage(out float4 fragColor, in float2 fragCoord)
{
    float2 uv = fragCoord / g_vResolution.xy;
    fragColor = float4(float3(hash(uv), hash(uv), hash(uv)), 1.0);
}


struct VTX_IN_BLUR
{
    float3 vPosition : POSITION;
    float2 vUV : TEXCOORD;
};

struct VTX_OUT_BLUR
{
    float4 vPosition : SV_Position;
    float2 vUV : TEXCOORD;
};

VTX_OUT_BLUR VS_Blur(VTX_IN_BLUR _in)
{
    VTX_OUT_BLUR output = (VTX_OUT_BLUR) 0.f;
    
    float4 vProjPos = mul(float4(_in.vPosition, 1.f), g_matWorldViewProj);
    output.vPosition = vProjPos;
    float fScale = ScreenScale;
    output.vUV = _in.vUV;

    return output;
}

float4 PS_Blur(VTX_OUT_BLUR _in) : SV_Target
{
    
    float2 vScreenUV = _in.vPosition.xy / g_vResolution;
    
    //if (0 == WidthOrHeight) // 가로
    //{
    //    vScreenUV.x += sin(vScreenUV.y * Amplitude + (g_fAccTime * 12.56f)) * Speed;
    //}
    //if (1 == WidthOrHeight) // 세로
    //{
    //    vScreenUV.y += sin(vScreenUV.x * Amplitude + (g_fAccTime * 12.56f)) * Speed;
    //}
    //if (2 == WidthOrHeight) // 양쪽 다
    //{
    //    vScreenUV.x += sin(vScreenUV.y * Amplitude + (g_fAccTime * 12.56f)) * Speed;
    //    vScreenUV.y += sin(vScreenUV.x * Amplitude + (g_fAccTime * 12.56f)) * Speed;
    //}
    float4 vColor = CloneTex.Sample(Sample_Anisotropic, vScreenUV);
    
    if (vColor.x > 0.f && vColor.x < 0.2f)
    {
        vColor.x = 0.f;
    }
    if (vColor.x > 0.2f && vColor.x < 0.4f)
    {
        vColor.x = 0.2f;
    }
    if (vColor.x > 0.4f && vColor.x < 0.6f)
    {
        vColor.x = 0.4f;
    }
    if (vColor.x > 0.6f && vColor.x < 0.8f)
    {
        vColor.x = 0.6f;
    }
    if (vColor.x > 0.8f && vColor.x < 1.f)
    {
        vColor.x = 0.8f;
    }
    
    if (vColor.y > 0.f && vColor.y < 0.2f)
    {
        vColor.y = 0.f;
    }
    if (vColor.y > 0.2f && vColor.y < 0.4f)
    {
        vColor.y = 0.2f;
    }
    if (vColor.y > 0.4f && vColor.y < 0.6f)
    {
        vColor.y = 0.4f;
    }
    if (vColor.y > 0.6f && vColor.y < 0.8f)
    {
        vColor.y = 0.6f;
    }
    if (vColor.y > 0.8f && vColor.y < 1.f)
    {
        vColor.y = 0.8f;
    }
    
    if (vColor.z > 0.f && vColor.z< 0.2f)
    {
        vColor.z = 0.f;
    }
    if (vColor.z > 0.2f && vColor.z < 0.4f)
    {
        vColor.z = 0.2f;
    }
    if (vColor.z > 0.4f && vColor.z < 0.6f)
    {
        vColor.z = 0.4f;
    }
    if (vColor.z > 0.6f && vColor.z < 0.8f)
    {
        vColor.z = 0.6f;
    }
    if (vColor.z > 0.8f && vColor.z < 1.f)
    {
        vColor.z = 0.8f;
    }
    
    return vColor;
}

//=====================
// FishEye Shader
// Mesh : Circle Mesh
#define CloneTex g_tex_0
#define NoiseTexture g_tex_1
//=====================

struct VTX_NOISE_OUT
{
    float4 vPosition : SV_Position;
    float2 vCenterPos : POSITION; // 투영좌표계 중앙 위치
    float2 vRadius : POSITION2; // 사각형의 반지름 길이(투영좌표계 기준)
    float2 vUV : TEXCOORD;
};

VTX_FISHEYE_OUT VS_Noise(VTX_IN _in)
{
    VTX_NOISE_OUT output = (VTX_NOISE_OUT) 0.f;

    output.vPosition = mul(float4(_in.vPosition, 1.f), g_matWorldViewProj);
    float4 vCenterProj = mul(float4(0.f, 0.f, 0.f, 1.f), g_matWorldViewProj);
    output.vCenterPos = vCenterProj.xy / vCenterProj.w;
    
    float2 vProj = output.vPosition.xy / output.vPosition.w;
    
    //output.vRadius = float2(abs(vProj.x - output.vCenterPos.x), abs(vProj.y - output.vCenterPos.y));
    output.vRadius = float2(0.8, 0.8);
    
    output.vUV = _in.vUV;

    return output;
}

float4 PS_Noise(VTX_NOISE_OUT _in) : SV_Target
{
    float2 vScreenUV = _in.vPosition.xy / g_vResolution;
    //vScreenUV *= _in.vRadius * 2.f;

    float2 uv = vScreenUV;
    float2 ouv = _in.vUV;
    //float r = 1.f;
    
    uv.x = uv.x + Rand(g_fAccTime * 10.f);
    uv.y = uv.y + Rand(g_fAccTime * 11.f);
    float4 vNoiseColor = NoiseTexture.Sample(g_sam_1, uv);
    float4 vTexColor = CloneTex.Sample(g_sam_1, _in.vUV);
    
    vTexColor *= vNoiseColor;
    
    return vTexColor;
}

#endif