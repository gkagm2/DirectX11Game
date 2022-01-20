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
	
    // 1.0f -> �̵� ��Ʈ ����
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
        float2 vFinalLeftTop = vLeftTopUV - ((vBaseSizeUV * 0.5f) - (vFrameSizeUV * 0.5f)) - vOffsetPosUV;
        float2 vAnimUV = vFinalLeftTop + vBaseSizeUV * _in.vUV;
                
        if (vLeftTopUV.x < vAnimUV.x && vAnimUV.x < (vLeftTopUV + vFrameSizeUV).x
            && vLeftTopUV.y < vAnimUV.y && vAnimUV.y < (vLeftTopUV + vFrameSizeUV).y)
            vOutColor = g_TexAnimAtlas.Sample(Sample_Point, vAnimUV);
        else
            clip(-1);
    }
    else if(bTex_0)
        vOutColor = g_tex_0.Sample(Sample_Point, _in.vUV);
    
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
        float2 vFinalLeftTop = vLeftTopUV - ((vBaseSizeUV * 0.5f) - (vFrameSizeUV * 0.5f)) - vOffsetPosUV;
        float2 vAnimUV = vFinalLeftTop + vBaseSizeUV * _in.vUV;
                
        if (vLeftTopUV.x < vAnimUV.x && vAnimUV.x < (vLeftTopUV + vFrameSizeUV).x
            && vLeftTopUV.y < vAnimUV.y && vAnimUV.y < (vLeftTopUV + vFrameSizeUV).y)
            vOutColor = g_TexAnimAtlas.Sample(g_sam_1, vAnimUV);
        else
            clip(-1);
    }
    else if (bTex_0)
        vOutColor = g_tex_0.Sample(Sample_Point, _in.vUV);
    

    
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
            float3 vForwardDir = normalize(g_Light2DBuffer[i].vLightDir.xyz); // light ����
            float3 vDirToTarget = normalize(_in.vWorldPos.xyz - g_Light2DBuffer[i].vLightPos.xyz);
    
            float fRadian = dot(vForwardDir, vDirToTarget);
            float fAngle = acos(fRadian); //* 57.29578f; // radian  to degree
            float fRatio = 0.f;
            if (fAngle < g_Light2DBuffer[i].fAngle * 0.5f)
            {
                float fLength = abs(length(g_Light2DBuffer[i].vLightPos.xy - _in.vWorldPos.xy));
                // saturate: 0~1������ ������ ����
                //float fRatio = saturate(1.f - (fLength / g_Light2DBuffer[i].fRange));
                fRatio = cos(saturate(fLength / g_Light2DBuffer[i].fRange) * (3.1415926535f * 0.5f));
            }
             // �л걤 ����
             finalColor.vDiffuse += g_Light2DBuffer[i].color.vDiffuse * fRatio;
        }
        else if(g_Light2DBuffer[i].iLightType == directType)
        {
            finalColor.vDiffuse += g_Light2DBuffer[i].color.vDiffuse;
        }
    }
    vOutColor.xyz = vOutColor.xyz * finalColor.vDiffuse.xyz;
    
    vOutColor.x = saturate(vOutColor.x);
    vOutColor.y = saturate(vOutColor.y);
    vOutColor.z = saturate(vOutColor.z);
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
#define vDefaultRectColor g_vec4_0;
#define vConnectRectColor g_vec4_1;
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
        //return vConnectRectColor;
        return float4(0.9f, 0.2f, 0.2f, 1.f); // red color
    //return vDefaultRectColor
    return float4(0.2f, 0.9f, 0.2, 1.f); // green color
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

/////
// UI
#define vUiColor g_vec4_0
/////

////////////////
// Vertex shader
////////////////
// vPos, vColor�� �Է¹޾Ƽ� ó�����ִ� �Լ�
VTX_OUT VS_Canvas(VTX_IN _in)
{
    VTX_OUT output = (VTX_OUT) 0.f; // �ʱ�ȭ
	
    float4 vWorldPos = mul(float4(_in.vPosition, 1.0f), g_matRectWorld);
    float4 vViewPos = mul(vWorldPos, g_matRectView);
    float4 vProjPos = mul(vViewPos, g_matRectProjection);
    
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
    // Alpha 0.5 �̸��� ��� �������� �ƿ� �������� ��.
    float4 vOutColor = float4(1.f, 0.f, 1.f, 1.f); // ����Ÿ ����
    
    // �ִϸ��̼� Ÿ���� ���
    if (bIsAnimating2D)
    {
        float2 vFinalLeftTop = vLeftTopUV - ((vBaseSizeUV * 0.5f) - (vFrameSizeUV * 0.5f)) - vOffsetPosUV;
        float2 vAnimUV = vFinalLeftTop + vBaseSizeUV * _in.vUV;
                
        if (vLeftTopUV.x < vAnimUV.x && vAnimUV.x < (vLeftTopUV + vFrameSizeUV).x
            && vLeftTopUV.y < vAnimUV.y && vAnimUV.y < (vLeftTopUV + vFrameSizeUV).y)
            vOutColor = g_TexAnimAtlas.Sample(Sample_Point, vAnimUV);
        else
            clip(-1);
    }
    else if (bTex_0)
    {
        vOutColor = g_tex_0.Sample(Sample_Point, _in.vUV);
    }
    // ���� ȥ��
    vOutColor = vOutColor * vUiColor;
    return vOutColor;
}

/////////////
// Line Strip
#define vLineColor g_vec4_0
/////////////



struct LINE_VTX_IN
{
	// semantic : input layout�ʿ��� �ۼ��س��� ���� ���� ���� ������ �����ϴ� ����
    float3 vPosition : POSITION; // semantic(layout ����, ����)
    float4 vColor : COLOR;
};

struct LINE_VTX_OUT
{
    float4 vPosition : SV_Position;
    float4 vColor : COLOR;
};

LINE_VTX_OUT VS_LineStrip(LINE_VTX_IN _in)
{
    LINE_VTX_OUT output = (LINE_VTX_OUT) 0.f; // �ʱ�ȭ
	
    // 1.0f -> �̵� ��Ʈ ����
    float4 vProjPos = mul(float4(_in.vPosition, 1.0f), g_matWorldViewProj);
    
	// �����Ͷ��������� ���޵� ��ǥ�� w �� ����� ������ǥ�� ����.    
    output.vPosition = vProjPos;
    output.vColor = _in.vColor;
    return output;
}

float4 PS_LineStrip(LINE_VTX_OUT _in) : SV_Target
{
    float4 vOutColor = float4(1.f, 0.f, 1.f, 1.f); // ����Ÿ ����
    vOutColor = _in.vColor;
    
    return vOutColor;
}

///////////
// Fog Shader

///////////

VTX_OUT VS_Fog2D(VTX_IN _in)
{
    float4 vWorldPos = mul(float4(_in.vPosition, 1.0f), g_matWorld);
    float4 vViewPos = mul(vWorldPos, g_matView);
    float4 vProjPos = mul(vViewPos, g_matProjection);

    VTX_OUT output;
    
    output.vColor = _in.vColor;
    output.vPosition = vProjPos;
    output.vUV = _in.vUV;
    return output;
}

float4 PS_Fog2D(VTX_OUT _in) : SV_Target
{
    float4 vOutColor = float4(1.f, 0.f, 1.f, 1.0f);
    vOutColor = g_tex_0.Sample(Sample_Anisotropic, _in.vUV);
    vOutColor.w = 0.5f;
    // ������ ���� ���� �ؾߵǳ�?
    return vOutColor;
}

/////////////
// Button UI
/////////////
#define vUIColor g_vec4_0
// vColor�� �Է¹޾Ƽ� ó�����ִ� �Լ�
VTX_OUT VS_ButtonUI(VTX_IN _in)
{
    VTX_OUT output = (VTX_OUT) 0.f; // �ʱ�ȭ
	
    float4 vWorldPos = mul(float4(_in.vPosition, 1.0f), g_matRectWorld);
    float4 vViewPos = mul(vWorldPos, g_matRectView);
    float4 vProjPos = mul(vViewPos, g_matRectProjection);
    
	// �����Ͷ��������� ���޵� ��ǥ�� w �� ����� ������ǥ�� ����.    
    output.vPosition = vProjPos;
    output.vColor = _in.vColor;
    output.vUV = _in.vUV;
    return output;
}

float4 PS_ButtonUI(VTX_OUT _in) : SV_Target
{
    // Alpha 0.5 �̸��� ��� �������� �ƿ� �������� ��.
    float4 vOutColor = float4(1.f, 0.f, 1.f, 1.f); // ����Ÿ ����
    vOutColor = g_tex_0.Sample(Sample_Anisotropic, _in.vUV);
    // ���� ȥ��
    vOutColor = vOutColor * vUIColor;
    return vOutColor;
}

//////////////////////////////
///////////// Grid ///////////
#define TileXCount      g_int_0 // Ÿ�ϸ��� Ÿ�� ���� ����
#define TileYCount      g_int_1 // Ÿ�ϸ��� ���� ����

//#define TileIdx         g_int_2 // �̹��� �ε���

#define AtlasTex        g_tex_0 // ��Ʋ�� �ؽ���
#define AtlasResolution g_vec2_0 // ��Ʋ�� �ؽ����� ������
#define AtlasTileUVSize g_vec2_1 // ��Ʋ�� �ؽ��Ŀ��� Ÿ�� �ϳ��� UV ������

#define vDefaultRectColor g_vec4_0; // �⺻ ����
/////////////

////////////////
// Grid (frame) vertex shader
////////////////
VTX_OUT VS_Grid(VTX_IN _in)
{
    VTX_OUT output = (VTX_OUT) 0.f; // �ʱ�ȭ
	
    float4 vWorldPos = mul(float4(_in.vPosition, 1.0f), g_matWorld);
    float4 vViewPos = mul(vWorldPos, g_matView);
    float4 vProjPos = mul(vViewPos, g_matProjection);
	
    output.vPosition = vProjPos;
    output.vUV = _in.vUV;
    return output;
}


///////////////
// Grid (frame) pixel shader
///////////////
float4 PS_Grid(VTX_OUT _in) : SV_Target
{
    return vDefaultRectColor;
}


////////////////
// Line
#define LineColor g_vec4_0
////////////////
struct VTX_IN_LINE
{
    float3 vPosition : POSITION;
    float4 vColor : COLOR;
};
struct VTX_OUT_LINE
{
    float4 vPosition : SV_Position;
    float4 vColor : COLOR;
};

////////////////
// Line vertex shader
////////////////
VTX_OUT VS_Line(VTX_IN _in)
{
    VTX_OUT output = (VTX_OUT) 0.f; // �ʱ�ȭ
	
    float4 vWorldPos = mul(float4(_in.vPosition, 1.0f), g_matWorld);
    float4 vViewPos = mul(vWorldPos, g_matView);
    float4 vProjPos = mul(vViewPos, g_matProjection);
	
    output.vPosition = vProjPos;
    output.vColor = _in.vColor;
    return output;
}

///////////////
// Line pixel shader
///////////////
float4 PS_Line(VTX_OUT _in) : SV_Target
{
    return LineColor;
}
#endif