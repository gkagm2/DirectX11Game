#ifndef _TILEMAP
#define _TILEMAP

#include "value.fx"

///////////////////////////////////////////////////
/////////////////// Tile Map //////////////////////

// �������� ������ Ÿ���� ����
#define TileXCount      g_int_0 // Ÿ�ϸ��� Ÿ�� ���� ����
#define TileYCount      g_int_1 // Ÿ�ϸ��� ���� ����
#define LightEnable     g_int_2 // ������ ����

//#define TileIdx         g_int_2 // �̹��� �ε���

#define AtlasTex        g_tex_0 // ��Ʋ�� �ؽ���
#define AtlasResolution g_vec2_0 // ��Ʋ�� �ؽ����� ������
#define AtlasTileUVSize g_vec2_1 // ��Ʋ�� �ؽ��Ŀ��� Ÿ�� �ϳ��� UV ������

///////////////////////////////////////////////////
// using g_TileBuffer
struct VTX_TILEMAP_IN
{
    float3 vPosition : POSITION;
    float2 vUV : TEXCOORD;
};

struct VTX_TILEMAP_OUT
{
    float4 vPosition : SV_Position;
    float3 vWorldPos : POSITION;
    float2 vUV : TEXCOORD;
};

VTX_TILEMAP_OUT VS_TileMap(VTX_TILEMAP_IN _in)
{
    VTX_TILEMAP_OUT output = (VTX_TILEMAP_OUT) 0.f;
    output.vWorldPos = mul(float4(_in.vPosition, 1.0f), g_matWorld).xyz;
    output.vPosition = mul(float4(_in.vPosition, 1.f), g_matWorldViewProj);
    
    output.vUV = _in.vUV;
    return output;
}


float4 PS_TileMap(VTX_TILEMAP_OUT _in) : SV_Target
{
    float4 vOutColor = (float4) 0.f;
    
    // ����ȭ ���ۿ� �����ϱ� ���� �ε��� ���ϱ�
    float2 vExUV = _in.vUV * float2(TileXCount, TileYCount); // �� Ÿ�� ũ�⸦ 0~1���̷� ũ�⸦ ũ�� ��.
    int2 iBufferIdx = floor(vExUV); // ������ ���� �࿭�� ����.
    
    int idx = TileXCount * iBufferIdx.y + iBufferIdx.x;
    
    int iImageIdx = g_TileBuffer[idx].iTileIdx; // �̹����� �ε����� ����.
    
    uint iTileWidthCount = 1.f / AtlasTileUVSize.x;
    
    uint iCol = (uint)iImageIdx % iTileWidthCount;
    uint iRow = (uint)iImageIdx / iTileWidthCount;
    
    // n x m ���� Ÿ�ϸ��� ǥ���ϱ� ���ؼ� �Է����� ���� uv ���� �� ĭ�� �ȼ����� 0~1 �������� ���� �������� �Ѵ�.
    float2 vTileUV = _in.vUV * float2(TileXCount, TileYCount); // Ÿ�� ũ�⸦ 0~1���̷� ũ�� ũ�� ��.
    vTileUV = frac(vTileUV); // �Ҽ��κи� ����.
    
    if (iImageIdx < 0)
    {
        clip(-1);
        //vTileUV *= AtlasTileUVSize; // Ÿ�� ������ 1���� ����.
        //float2 vMinUV = vTileUV * float2(iBufferIdx.x, iBufferIdx.y);
        //float2 vMaxUV = vTileUV * float2(iBufferIdx.x + 1, iBufferIdx.y + 1);
        
        //if (vMinUV.x == vTileUV.x || vMaxUV.x == vTileUV.x || vMinUV.y == vTileUV.y || vMaxUV.y == vTileUV.y)
        //{
        //    return float4(1.f, 1.f, 1.f, 1.f);
        //}
        //else
        //{
        //    clip(-1);
        //}
    }
    
    
    // Ÿ�� ������ 1���� ����
    vTileUV *= AtlasTileUVSize;
    
    // Ÿ�� �ε����� ���� ��, �� ��ġ�� �̵� 
    vTileUV = AtlasTileUVSize * float2(iCol, iRow) + vTileUV;

 
    vOutColor = AtlasTex.Sample(Sample_Point, vTileUV);
    
    if (LightEnable)
    {
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
            else if (g_Light2DBuffer[i].iLightType == directType)
            {
                finalColor.vDiffuse += g_Light2DBuffer[i].color.vDiffuse;
            }
        }
        vOutColor.xyz = vOutColor.xyz * finalColor.vDiffuse.xyz;
    }
    
    return vOutColor;
}
#endif