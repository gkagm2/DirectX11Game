#ifndef _VALUE
#define _VALUE

#include "struct.fx"
// 상수 버퍼 레지스터	
cbuffer TRANSFORM : register(b0)
{
    row_major Matrix g_matWorld; // 행기반 v (L-)
    row_major Matrix g_matView;
    row_major Matrix g_matProjection;
    
    row_major Matrix g_matWorldInv; // world inverse
    row_major Matrix g_matViewInv;
    
    row_major Matrix g_matWorldView;     // world * view matrix
    row_major Matrix g_matWorldViewProj; // world * view * projection matrix
}

cbuffer MATERIAL_PARAM : register(b1)
{
    float4 vDiffuse;
    float4 vSpectrum;
    float4 vAmbiant;
    
    int g_int_0;
    int g_int_1;
    int g_int_2;
    int g_int_3;
    
    float g_float_0;
    float g_float_1;
    float g_float_2;
    float g_float_3;
    
    float2 g_vec2_0;
    float2 g_vec2_1;
    float2 g_vec2_2;
    float2 g_vec2_3;
    
    float4 g_vec4_0;
    float4 g_vec4_1;
    float4 g_vec4_2;
    float4 g_vec4_3;
    
    row_major matrix g_mat_0;
    row_major matrix g_mat_1;
    row_major matrix g_mat_2;
    row_major matrix g_mat_3;
    
    int bTex_0;
    int bTex_1;
    int bTex_2;
    int bTex_3;
    int bTex_4;
    int bTex_5;
    int bTex_6;
    int bTex_7;
    int bTexArr_0;
    int bTexArr_1;
    int bTexCube_0;
    int bTexCube_1;
    
}

cbuffer ANIMATION2D_DATA : register(b2)
{
    float2  vLeftTopUV;
    float2  vFrameSizeUV;
    float2  vOffsetPosUV;
    float2  vBaseSizeUV;
    int     bIsAnimating2D;
    int3    anim2Dpadding;
};

cbuffer GLOBAL_CONST : register(b3)
{
    float2  g_vResolution;      // 위도우 화면 해상도
    float2  g_vNoiseResolution; // Noise Textrue 해상도
    float   g_fDeltaTime;
    float   g_fAccTime;         // 누적시간
    int     g_iLight2DCount; // 2D 광원 개수
    int     g_iLight3DCount; // 3D 광원 개수
}

cbuffer RECT_TRANSFORM : register(b4)
{
    row_major Matrix g_matRectWorld; // 행기반 v (L-)
    row_major Matrix g_matRectView;
    row_major Matrix g_matRectProjection;
}

// Texture register (t)
Texture2D g_tex_0 : register(t0);
Texture2D g_tex_1 : register(t1);
Texture2D g_tex_2 : register(t2);
Texture2D g_tex_3 : register(t3);
Texture2D g_tex_4 : register(t4);
Texture2D g_tex_5 : register(t5);
Texture2D g_tex_6 : register(t6);
Texture2D g_tex_7 : register(t7);

Texture2DArray g_texarr_0 : register(t8);
Texture2DArray g_texarr_1 : register(t9);

TextureCube g_texcube_0 : register(t10);
TextureCube g_texcube_1 : register(t11);

Texture2D g_TexAnimAtlas : register(t50); // Animation's 2D  Atlas Texture

StructuredBuffer<TLightInfo> g_Light2DBuffer : register(t51);
StructuredBuffer<TTileInfo> g_TileBuffer : register(t52);
Texture2D g_TexNoise : register(t53);

// Sampler Stage
SamplerState g_sam_0 : register(s0); // Anisotropic
SamplerState g_sam_1 : register(s1); // Point
#define Sample_Anisotropic g_sam_0
#define Sample_Point g_sam_1

// sampler2D : 2D 텍스쳐에서 텍셀 하나를 구해오는데 사용.
sampler2D DiffuseSampler2D;
sampler3D DiffuseSampler3D;
samplerCUBE DiffuseSamplerCube;

#define PI 3.1415926535
#endif