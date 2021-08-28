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
	
    float4 vProjPos = mul(float4(_in.vPosition, 1.0f), g_matWorldViewProj);
    
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
    

    
    int directType = 0;
    int pointType = 1;
    int spotType = 2;
    
    TLightColor finalColor = (TLightColor) 0.f;
    for (int i = 0; i < g_iLight2DCount.x; ++i)
    {
        if (g_Light2DBuffer[i].iLightType == pointType)
        {
            //Point Light
            // ����ó��
    
            float fLength = abs(length(g_Light2DBuffer[i].vLightPos.xy - _in.vWorldPos.xy));
            // saturate: 0~1������ ������ ����
            //float fRatio = saturate(1.f - (fLength / g_Light2DBuffer[i].fRange));
            float fRatio = cos(saturate(fLength / g_Light2DBuffer[i].fRange) * (3.1415926535f * 0.5f));
        
            // �л걤 ����
            finalColor.vDiffuse += g_Light2DBuffer[i].color.vDiffuse * fRatio;
        }
        else if (g_Light2DBuffer[i].iLightType == spotType)
        {
            // Spot Light
            float3 vForwardDir = normalize(g_Light2DBuffer[0].vLightDir.xyz); // light ����
            float3 vDirToTarget = normalize(_in.vWorldPos.xyz - g_Light2DBuffer[0].vLightPos.xyz);
    
            float fRadian = dot(vForwardDir, vDirToTarget);
            float fAngle = acos(fRadian); //* 57.29578f; // radian  to degree
    
            if (fAngle < g_Light2DBuffer[0].fAngle * 0.5f)
            {
                float fLength = abs(length(g_Light2DBuffer[i].vLightPos.xy - _in.vWorldPos.xy));
                // saturate: 0~1������ ������ ����
                //float fRatio = saturate(1.f - (fLength / g_Light2DBuffer[i].fRange));
                float fRatio = cos(saturate(fLength / g_Light2DBuffer[i].fRange) * (3.1415926535f * 0.5f));
        
                // �л걤 ����
                finalColor.vDiffuse += g_Light2DBuffer[i].color.vDiffuse * fRatio;
            }
            else
            {
            //vOutColor.xyz = vOutColor.xyz * finalColor.vDiffuse.xyz;
            }
        }
        else
        {
            
        }
    }
    vOutColor.xyz = vOutColor.xyz * finalColor.vDiffuse.xyz;
    
    //vOutColor.x = saturate(vOutColor.x);
    //vOutColor.y = saturate(vOutColor.y);
    //vOutColor.z = saturate(vOutColor.z);
    return vOutColor;
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

#define iConnectColor g_int_0 // 0 or 1 
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
    if (iConnectColor)
        return float4(0.9f, 0.2f, 0.2f, 1.f); // red color
    
    return float4(0.2f, 0.9f, 0.2, 1.f); // green color
}

///////////////////////////////////////////////////
/////////////////// Tile Map //////////////////////

///////////////////////////////////////////////////

struct VTX_TILEMAP_IN
{
    float3 vPosition : POSITION;
    float2 vUV : TEXCOORD;
};

struct VTX_TILEMAP_OUT
{
    float4 vPosition : SV_Position;
    float2 vUV : TEXCOORD;
};

VTX_TILEMAP_OUT VS_TileMap(VTX_TILEMAP_IN _in)
{
    VTX_TILEMAP_OUT output = (VTX_TILEMAP_OUT) 0.f;
    output.vPosition = mul(float4(_in.vPosition, 1.f), g_matWorldViewProj); 
    output.vUV = _in.vUV;
    return output;
}

// ������ �� ������ Ÿ���� ����
#define iXCnt g_int_0 
#define iYCnt g_int_1
float4 PS_TileMap(VTX_TILEMAP_OUT _in) : SV_Target
{
    float2 vLTUV = g_vec2_0;         // ������ �ϰ���� LeftTop UV
    float2 vTileUVSize = g_vec2_1;   // ������ �� Ÿ�� �ϳ��� UV ������
    
    // Ÿ���� 2x2�� ��� ����� 2x2�� ������ش�. �̰��� vOriginUV�� ������ �ִ�.
    float2 vOriginUV = float2(_in.vUV.x * (float) iXCnt, _in.vUV.y * (float) iYCnt);
    
    int idx = floor(vOriginUV.y) * g_int_0 + floor(vOriginUV.x);
    vLTUV = g_TileBuffer[idx].vLeftTopUV;
    float2 vUV = vLTUV + float2(frac(vOriginUV.x) * vTileUVSize.x, frac(vOriginUV.y) * vTileUVSize.y);
    //vUV.x = vUV.x / (float) iXCnt;
    //vUV.y = vUV.y / (float) iYCnt;
    float4 vOutColor = g_tex_0.Sample(g_sam_1, vUV);
    return vOutColor;
    
    /*
    //int idx = floor(vOriginUV.y) * iXCnt + floor(vOriginUV.x);
    g_TileBuffer[wnatIndex].vLeftTopUV - g_TileBuffer[wnatIndex].vTileSizeUV;
    
    // vLT�� ���� �ؽ����� LT���� ��.
    float2 vUV = vLTUV + float2(frac(vOriginUV.x) * g_TileBuffer[wnatIndex].vLeftTopUV.x, frac(vOriginUV.y) * g_TileBuffer[wnatIndex].vLeftTopUV.y);
    float4 vOutColor = g_tex_0.Sample(g_sam_1, vUV);
    //float4 vOutColor = g_tex_0.Sample(g_sam_1, _in.vUV);
    return vOutColor;
*/
}

////////////
// UI
////////////
struct VTX_UI_IN
{
    float3 vPos : POSITION;
    float2 vUV : TEXCOORD;
    uint iInstID : SV_InstanceID; // �ν��Ͻ� ID 
};

struct VTX_UI_OUT
{
    float3 vViewPos : POSITION;
    float2 vUV : TEXCOORD;
    float fInstID : FOG; // �ν��Ͻ� ID
};

////////////////
// Vertex shader
////////////////
// vPos, vColor�� �Է¹޾Ƽ� ó�����ִ� �Լ�
VTX_OUT VS_Canvas(VTX_IN _in)
{
    VTX_OUT output = (VTX_OUT) 0.f; // �ʱ�ȭ
	
    float4 vWorldPos = mul(float4(_in.vPosition, 1.0f), g_matRectWorld);
    float4 vViewPos = mul(vWorldPos, g_matRectView);
    float4 vProjPos = mul(vViewPos, g_matProjection);
    
	// �����Ͷ��������� ���޵� ��ǥ�� w �� ����� ������ǥ�� ����.    
    output.vPosition = vProjPos;
    output.vColor = _in.vColor;
    output.vUV = _in.vUV;
    return output;
}

///////////////
// Pixel shader
///////////////
float4 PS_Canvas(VTX_OUT _in) : SV_Target
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
    
    return vOutColor;
}

#endif