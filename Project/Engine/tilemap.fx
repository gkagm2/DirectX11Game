#ifndef _TILEMAP
#define _TILEMAP

#include "value.fx"

///////////////////////////////////////////////////
/////////////////// Tile Map //////////////////////

// 렌더링시 생성할 타일의 개수
#define TileXCount      g_int_0 // 타일맵의 타일 가로 개수
#define TileYCount      g_int_1 // 타일맵의 세로 개수

//#define TileIdx         g_int_2 // 이미지 인덱스

#define AtlasTex        g_tex_0 // 아틀라스 텍스쳐
#define AtlasResolution g_vec2_0 // 아틀라스 텍스쳐의 사이즈
#define AtlasTileUVSize g_vec2_1 // 아틀라스 텍스쳐에서 타일 하나의 UV 사이즈

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
    
    // 구조화 버퍼에 접근하기 위한 인덱스 구하기
    float2 vExUV = _in.vUV * float2(TileXCount, TileYCount); // 각 타일 크기를 0~1사이로 크기를 크게 함.
    int2 iBufferIdx = floor(vExUV); // 정수만 빼서 행열을 구함.
    
    int idx = TileXCount * iBufferIdx.y + iBufferIdx.x;
    
    if (idx < 0)
    {
        vOutColor = float4(1.f, 1.f, 1.f, 1.f);
        return vOutColor;
    }
    
    uint iImageIdx = g_TileBuffer[idx].iTileIdx; // 이미지의 인덱스를 구함.
    
    uint iTileWidthCount = 1.f / AtlasTileUVSize.x;
    
    uint iCol = iImageIdx % iTileWidthCount;
    uint iRow = iImageIdx / iTileWidthCount;
    
    // n x m 으로 타일맵을 표현하기 위해서 입력으로 들어온 uv 값을 각 칸의 픽셀들이 0~1 기준으로 값을 가져가게 한다.
    float2 vTileUV = _in.vUV * float2(TileXCount, TileYCount);
    vTileUV = frac(vTileUV);
    
    // 타일 사이즈 1개로 맵핑
    vTileUV *= AtlasTileUVSize;
    
    // 타일 인덱스로 얻은 행, 열 위치로 이동 
    vTileUV = AtlasTileUVSize * float2(iCol, iRow) + vTileUV;
    
    vOutColor = AtlasTex.Sample(Sample_Point, vTileUV);
    return vOutColor;
}
#endif