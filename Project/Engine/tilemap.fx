#ifndef _TILEMAP
#define _TILEMAP

#include "value.fx"

///////////////////////////////////////////////////
/////////////////// Tile Map //////////////////////

// 렌더링시 생성할 타일의 개수
#define TileXCount      g_int_0 // 타일맵의 타일 가로 개수
#define TileYCount      g_int_1 // 타일맵의 세로 개수
#define LightEnable     g_int_2 // 라이팅 적용

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
    
    // 구조화 버퍼에 접근하기 위한 인덱스 구하기
    float2 vExUV = _in.vUV * float2(TileXCount, TileYCount); // 각 타일 크기를 0~1사이로 크기를 크게 함.
    int2 iBufferIdx = floor(vExUV); // 정수만 빼서 행열을 구함.
    
    int idx = TileXCount * iBufferIdx.y + iBufferIdx.x;
    
    int iImageIdx = g_TileBuffer[idx].iTileIdx; // 이미지의 인덱스를 구함.
    
    uint iTileWidthCount = 1.f / AtlasTileUVSize.x;
    
    uint iCol = (uint)iImageIdx % iTileWidthCount;
    uint iRow = (uint)iImageIdx / iTileWidthCount;
    
    // n x m 으로 타일맵을 표현하기 위해서 입력으로 들어온 uv 값을 각 칸의 픽셀들이 0~1 기준으로 값을 가져가게 한다.
    float2 vTileUV = _in.vUV * float2(TileXCount, TileYCount); // 타일 크기를 0~1사이로 크기 크게 함.
    vTileUV = frac(vTileUV); // 소수부분만 구함.
    
    if (iImageIdx < 0)
    {
        clip(-1);
        //vTileUV *= AtlasTileUVSize; // 타일 사이즈 1개로 맵핑.
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
    
    
    // 타일 사이즈 1개로 맵핑
    vTileUV *= AtlasTileUVSize;
    
    // 타일 인덱스로 얻은 행, 열 위치로 이동 
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
            // 광원처리
    
                float fLength = abs(length(g_Light2DBuffer[i].vLightPos.xy - _in.vWorldPos.xy));
            // saturate: 0~1사이의 값으로 만듬
            //float fRatio = saturate(1.f - (fLength / g_Light2DBuffer[i].fRange));
                float fRatio = cos(saturate(fLength / g_Light2DBuffer[i].fRange) * (3.1415926535f * 0.5f));
        
            // 분산광 설정
                finalColor.vDiffuse += g_Light2DBuffer[i].color.vDiffuse * fRatio;
            }
            else if (g_Light2DBuffer[i].iLightType == spotType)
            {
            // Spot Light
                float3 vForwardDir = normalize(g_Light2DBuffer[0].vLightDir.xyz); // light 방향
                float3 vDirToTarget = normalize(_in.vWorldPos.xyz - g_Light2DBuffer[0].vLightPos.xyz);
    
                float fRadian = dot(vForwardDir, vDirToTarget);
                float fAngle = acos(fRadian); //* 57.29578f; // radian  to degree
    
                if (fAngle < g_Light2DBuffer[0].fAngle * 0.5f)
                {
                    float fLength = abs(length(g_Light2DBuffer[i].vLightPos.xy - _in.vWorldPos.xy));
                // saturate: 0~1사이의 값으로 만듬
                //float fRatio = saturate(1.f - (fLength / g_Light2DBuffer[i].fRange));
                    float fRatio = cos(saturate(fLength / g_Light2DBuffer[i].fRange) * (3.1415926535f * 0.5f));
        
                // 분산광 설정
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