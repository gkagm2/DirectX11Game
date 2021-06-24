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
            vOutColor = g_TexAnimAtlas.Sample(g_sam_0, vAnimUV);
        else
            clip(-1);
    }
    else if(bTex_0)
        vOutColor = g_tex_0.Sample(g_sam_0, _in.vUV);
    
    return vOutColor;
}

/////////////////////////////////
struct VTX_OUT_LIGHT
{
    float4 vPosition : SV_Position;
    float3 vWorldPos : POSITION;
    float2 vUV : TEXCOORD;
};

////////////////
// Light2D Vertex shader
////////////////
// vPos, vColor�� �Է¹޾Ƽ� ó�����ִ� �Լ�
VTX_OUT_LIGHT VS_Std2D_Light2D(VTX_IN _in)
{
    VTX_OUT_LIGHT output = (VTX_OUT_LIGHT) 0.f; // �ʱ�ȭ
	
    float4 vWorldPos = mul(float4(_in.vPosition, 1.0f), g_matWorld);
    float4 vViewPos = mul(vWorldPos, g_matView);
    float4 vProjPos = mul(vViewPos, g_matProjection);
    
	// �����Ͷ��������� ���޵� ��ǥ�� w �� ����� ������ǥ�� ����.    
    output.vPosition = vProjPos;
    output.vWorldPos = vWorldPos.xyz;
    output.vUV = _in.vUV;
    return output;
}

///////////////
// Light2D Pixel shader
///////////////
float4 PS_Std2D_Light2D(VTX_OUT_LIGHT _in) : SV_Target
{
    float4 vOutColor = float4(1.f, 0.f, 1.f, 1.f); // ����Ÿ ����
    
    // �ִϸ��̼� Ÿ���� ���
    if (bIsAnimating2D)
    {
        float2 vFinalLeftTop = vLeftTopUV - ((vBaseSizeUV * 0.5f) - (vFrameSizeUV * 0.5f)) - vOffsetSizeUV;
        float2 vAnimUV = vFinalLeftTop + vBaseSizeUV * _in.vUV;
                
        if (vLeftTopUV.x < vAnimUV.x && vAnimUV.x < (vLeftTopUV + vFrameSizeUV).x
            && vLeftTopUV.y < vAnimUV.y && vAnimUV.y < (vLeftTopUV + vFrameSizeUV).y)
            vOutColor = g_TexAnimAtlas.Sample(g_sam_0, vAnimUV);
        else
            clip(-1);
    }
    else if (bTex_0)
        vOutColor = g_tex_0.Sample(g_sam_0, _in.vUV);
    
    // Spot Light
    TLightColor finalColor = (TLightColor) 0.f;
    float eyeAngle = g_arrLight2D[0].fAngle; // �þ߰�
    float3 vForwardDir = normalize(g_arrLight2D[0].vLightDir.xyz); // light ����
    float vLightPos = g_arrLight2D[0].vLightPos.xyz;
    
    float3 vDirToTarget = normalize(_in.vWorldPos);
    
    float fRadian = dot(vForwardDir, vDirToTarget);
    float fAngle = acos(fRadian); //* 57.29578f; // radian  to degree
    
    
    if (fAngle < eyeAngle * 0.5f)
    {
        TLightColor finalColor = (TLightColor) 0.f;
        for (int i = 0; i < g_iLight2DCount.x; ++i)
        {
            float fLength = abs(length(g_arrLight2D[i].vLightPos.xy - _in.vWorldPos.xy));
        // saturate: 0~1������ ������ ����
        //float fRatio = saturate(1.f - (fLength / g_arrLight2D[i].fRange));
            float fRatio = cos(saturate(fLength / g_arrLight2D[i].fRange) * (3.1415926535f * 0.5f));
        
        // �л걤 ����
            finalColor.vDiffuse += g_arrLight2D[i].color.vDiffuse * fRatio;
        }
        vOutColor.xyz = vOutColor.xyz * finalColor.vDiffuse.xyz;
        vOutColor.xyz = vOutColor.xyz * g_arrLight2D[0].color.vDiffuse.xyz;
        return vOutColor;
    }
    else
    {
        TLightColor finalColor = (TLightColor) 0.f;
        vOutColor.xyz = vOutColor.xyz * finalColor.vDiffuse.xyz;
        return float4(0.f, 0.f, 0.f, 1.f);
    }
    
    
    //Point Light
    // ����ó��
    /*TLightColor finalColor0 = (TLightColor) 0.f;
    for (int i = 0; i < g_iLight2DCount.x; ++i)
    {
        float fLength = abs(length(g_arrLight2D[i].vLightPos.xy - _in.vWorldPos.xy));
        // saturate: 0~1������ ������ ����
        //float fRatio = saturate(1.f - (fLength / g_arrLight2D[i].fRange));
        float fRatio = cos(saturate(fLength / g_arrLight2D[i].fRange) * (3.1415926535f * 0.5f));
        
        // �л걤 ����
        finalColor0.vDiffuse += g_arrLight2D[i].color.vDiffuse * fRatio;
    }
    vOutColor.xyz = vOutColor.xyz * finalColor0.vDiffuse.xyz;
    return vOutColor;*/
    
}
///////////////////////


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