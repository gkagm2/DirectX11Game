#ifndef _TILEMAP
#define _TILEMAP

#include "value.fx"

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
    float2 vLTUV = g_vec2_0; // ������ �ϰ���� LeftTop UV
    float2 vTileUVSize = g_vec2_1; // ������ �� Ÿ�� �ϳ��� UV ������
    
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


#endif