#ifndef _VALUE
#define _VALUE

#include "struct.fx"
// 상수 버퍼 레지스터	
cbuffer TRANSFORM : register(b0)
{
    row_major Matrix g_matWorld; // 행기반 v (L-)
    row_major Matrix g_matView;
    row_major Matrix g_matProjection;
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
    float2 vLeftTopUV;
    float2 vFrameSizeUV;
    float2 vOffsetSizeUV;
    float2 vBaseSizeUV;
    int bIsAnimating2D;
    int3 padding;
};

cbuffer LIGHT2D : register(b3)
{
    TLightInfo g_arrLight2D[100];
    int4 g_iLight2DCount;
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

// Sampler Stage
SamplerState g_sam_0 : register(s0); // Anisotropic
SamplerState g_sam_1 : register(s1); // Point
#endif