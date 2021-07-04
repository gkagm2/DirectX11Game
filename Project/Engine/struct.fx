#ifndef _STRUCT
#define _STRUCT

struct TLightColor
{
    float4 vDiffuse;
    float4 vSpeculer;
    float4 vAmbient;
};

struct TLightInfo
{
    TLightColor color;
    
    float4  vLightPos;
    float4  vLightDir;
    int     iLightType;
    float   fRange;
    float   fAngle;
    int     iPad;
};

struct TTileInfo
{
    float2 vLeftTop;
    float2 vRightBottom;
    float2 vTileSize;
    float2 vLeftTopUV; // Å¸ÀÏÀÇ ÁÂ»ó´Ü ÁÂÇ¥ UV
    float2 vRightBottomUV; // Å¸ÀÏÀÇ ¿ìÇÏ´ÜÁÂÇ¥ UV
    float2 vTileSizeUV;
    
    //float2 vLeftTopUV; // Å¸ÀÏÀÇ ÁÂ»ó´Ü ÁÂÇ¥ UV
    //float2 vRightBottomUV; // Å¸ÀÏÀÇ ÁÂÇÏ´Ü ÁÂÇ¥ UV
};

struct TParticle
{
    float3 vWorldPos;
    float3 vWorldDir;
    
    float fCurTime;
    float fMaxTime;
    int iAlive;
    int3 iPadding;
};

#endif
