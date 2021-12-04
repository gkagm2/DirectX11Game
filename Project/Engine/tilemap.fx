#ifndef _TILEMAP
#define _TILEMAP

#include "value.fx"

///////////////////////////////////////////////////
/////////////////// Tile Map //////////////////////

// �������� ������ Ÿ���� ����
#define TileXCount      g_int_0 // Ÿ�ϸ��� Ÿ�� ���� ����
#define TileYCount      g_int_1 // Ÿ�ϸ��� ���� ����

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
    float2 vUV : TEXCOORD;
};

VTX_TILEMAP_OUT VS_TileMap(VTX_TILEMAP_IN _in)
{
    VTX_TILEMAP_OUT output = (VTX_TILEMAP_OUT) 0.f;
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
    
    uint iImageIdx = g_TileBuffer[TileXCount * iBufferIdx.y + iBufferIdx.x].iTileIdx; // �̹����� �ε����� ����.
    
    uint iTileWidthCount = 1.f / AtlasTileUVSize.x;
    
    uint iCol = iImageIdx % iTileWidthCount;
    uint iRow = iImageIdx / iTileWidthCount;
    
    // n x m ���� Ÿ�ϸ��� ǥ���ϱ� ���ؼ� �Է����� ���� uv ���� �� ĭ�� �ȼ����� 0~1 �������� ���� �������� �Ѵ�.
    float2 vTileUV = _in.vUV * float2(TileXCount, TileYCount);
    vTileUV = frac(vTileUV);
    
    // Ÿ�� ������ 1���� ����
    vTileUV *= AtlasTileUVSize;
    
    // Ÿ�� �ε����� ���� ��, �� ��ġ�� �̵� 
    vTileUV = AtlasTileUVSize * float2(iCol, iRow) + vTileUV;
    
    vOutColor = AtlasTex.Sample(Sample_Point, vTileUV);
    return vOutColor;
}
#endif