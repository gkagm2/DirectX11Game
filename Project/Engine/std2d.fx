// Using 5.0 version
#ifndef _STD2D
#define _STD2D

#include "value.fx"

struct VTX_IN {
	// semantic : input layout쪽에서 작성해놓은 정적 내부 구조 정보를 설명하는 역할
	float3 vPosition : POSITION; // semantic(layout 연동, 설명)
	float4 vColor : COLOR;
    float2 vUV : TEXCOORD;
};

struct VTX_OUT {
	float4 vPosition : SV_Position;
	float4 vColor : COLOR;
    float2 vUV : TEXCOORD;
};

////////////////
// Vertex shader
////////////////
// vPos, vColor를 입력받아서 처리해주는 함수
VTX_OUT VS(VTX_IN _in)
{
	VTX_OUT output = (VTX_OUT)0.f; // 초기화
	
    float4 vWorldPos = mul(float4(_in.vPosition, 1.0f), g_matWorld);
    float4 vViewPos = mul(vWorldPos, g_matView);
    float4 vProjPos = mul(vViewPos, g_matProjection);
	
    output.vPosition = vProjPos;
	output.vColor = _in.vColor;
    output.vUV = _in.vUV;
	return output;
}

///////////////
// Rasterizer
///////////////

///////////////
// Pixel shader
///////////////
float4 PS(VTX_OUT _in) : SV_Target {
    float2 vOutUV = _in.vUV;
    float4 vOutColor = g_tex_0.Sample(g_sam_0, vOutUV);
	
    if (g_int_0)
    {
        vOutColor.x = 1.0f;
        vOutColor.yz = 0.0f;
    }
	
	return vOutColor;
}

////////////////
// Collider2D vertex shader
////////////////
VTX_OUT VS_Collider2D(VTX_IN _in)
{
    VTX_OUT output = (VTX_OUT) 0.f; // 초기화
	
    float4 vWorldPos = mul(float4(_in.vPosition, 1.0f), g_matWorld);
    float4 vViewPos = mul(vWorldPos, g_matView);
    float4 vProjPos = mul(vViewPos, g_matProjection);
	
    output.vPosition = vProjPos;
    return output;
}

///////////////
// Collider2D pixel shader
///////////////
float4 PS_Collider2D(VTX_OUT _in) : SV_Target
{
    return float4(0.2f, 0.9f, 0.2, 1.f);
}

#endif