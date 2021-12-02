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

// 렌더링 시 생성할 타일의 개수
#define iXCnt g_int_0 
#define iYCnt g_int_1
float4 PS_TileMap(VTX_TILEMAP_OUT _in) : SV_Target
{
    float2 vLTUV = g_vec2_0; // 렌더링 하고싶은 LeftTop UV
    float2 vTileUVSize = g_vec2_1; // 렌더링 할 타일 하나의 UV 사이즈
    
    // 타일이 2x2일 경우 사이즈를 2x2로 만들어준다. 이것을 vOriginUV가 가지고 있다.
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
    
    // vLT는 원본 텍스쳐의 LT여야 함.
    float2 vUV = vLTUV + float2(frac(vOriginUV.x) * g_TileBuffer[wnatIndex].vLeftTopUV.x, frac(vOriginUV.y) * g_TileBuffer[wnatIndex].vLeftTopUV.y);
    float4 vOutColor = g_tex_0.Sample(g_sam_1, vUV);
    //float4 vOutColor = g_tex_0.Sample(g_sam_1, _in.vUV);
    return vOutColor;
*/
}


#endif