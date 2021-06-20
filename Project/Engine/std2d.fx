// Using 5.0 version
#ifndef _STD2D
#define _STD2D

#include "value.fx"

struct VTX_IN {
	// semantic : input layout�ʿ��� �ۼ��س��� ���� ���� ���� ������ �����ϴ� ����
	float3 vPosition : POSITION; // semantic(layout ����, ����)
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
// vPos, vColor�� �Է¹޾Ƽ� ó�����ִ� �Լ�
VTX_OUT VS_Std2D(VTX_IN _in)
{
	VTX_OUT output = (VTX_OUT)0.f; // �ʱ�ȭ
	
    float4 vWorldPos = mul(float4(_in.vPosition, 1.0f), g_matWorld);
    float4 vViewPos = mul(vWorldPos, g_matView);
    float4 vProjPos = mul(vViewPos, g_matProjection);
    
	// �����Ͷ��������� ���޵� ��ǥ�� w �� ����� ������ǥ�� ����.    
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
float4 PS_Std2D(VTX_OUT _in) : SV_Target {
    float4 vOutColor = float4(1.f, 0.f, 1.f, 1.f); // ����Ÿ ����
    
    // �ִϸ��̼� Ÿ���� ���
    if (bIsAnimating2D)
    {
        float2 vFinalLeftTop = vLeftTopUV - ((vBaseSizeUV * 0.5f) - (vFrameSizeUV * 0.5f)) - vOffsetSizeUV;
        float2 vAnimUV = vFinalLeftTop + vBaseSizeUV * _in.vUV;
                
        if (vLeftTopUV.x < vAnimUV.x && vAnimUV.x < (vLeftTopUV + vFrameSizeUV).x
            && vLeftTopUV.y < vAnimUV.y && vAnimUV.y < (vLeftTopUV + vFrameSizeUV).y)
        {
            return g_TexAnimAtlas.Sample(g_sam_0, vAnimUV);
        }
        else
        {
            clip(-1);
        }
    }
    else if(bTex_0)
        vOutColor = g_tex_0.Sample(g_sam_0, _in.vUV);
    
    return vOutColor;
}

////////////////
// Collider2D vertex shader
////////////////
VTX_OUT VS_Collider2D(VTX_IN _in)
{
    VTX_OUT output = (VTX_OUT) 0.f; // �ʱ�ȭ
	
    float4 vWorldPos = mul(float4(_in.vPosition, 1.0f), g_matWorld);
    float4 vViewPos = mul(vWorldPos, g_matView);
    float4 vProjPos = mul(vViewPos, g_matProjection);
	
    output.vPosition = vProjPos;
    output.vUV= _in.vUV;
    return output;
}

///////////////
// Collider2D pixel shader
///////////////
float4 PS_Collider2D(VTX_OUT _in) : SV_Target
{
    //if (0.01f < _in.vUV.x && _in.vUV.x < 0.99f
    //    && 0.01f < _in.vUV.y && _in.vUV.y < 0.99f)
    //{
    //    clip(-1);
    //}
    if(g_int_0)
        return float4(0.9f, 0.2f, 0.2f, 1.f); // red color
    
    return float4(0.2f, 0.9f, 0.2, 1.f); // green color
}

#endif