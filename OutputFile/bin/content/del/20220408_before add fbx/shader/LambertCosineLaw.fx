#ifndef _LabertCosineLaw
#define _LabertCosineLaw

// 람베르트 난반사
float4x4 matWorld;
float4x4 matView;
float4x4 matProj;
float4 vWorldLightPosition;

struct VS_INPUT
{
    float4 vPosition : POSITION;
    float3 vNormal : NORMAL; // 법선 벡터
    float4 vColor : COLOR;
};

struct VS_OUTPUT
{
    float4 vPosition : POSITION;
    float3 vNormal : TEXCOORD0; // 반사광
    float4 vColor : COLOR;
};

VS_OUTPUT vs_main(VS_INPUT input)
{
    VS_OUTPUT output;
    output.vPosition = mul(input.vPosition, matWorld);
   
    float3 vLightDirection = output.vPosition.xyz - vWorldLightPosition.xyz;
    vLightDirection = normalize(vLightDirection);
   
    output.vPosition = mul(output.vPosition, matView);
    output.vPosition = mul(output.vPosition, matProj);
   
    float3 vWorldNormal = mul(input.vNormal, (float3x3) matWorld);
    vWorldNormal = normalize(vWorldNormal);
   
   
    output.vNormal = dot(-vLightDirection, vWorldNormal);
    output.vColor = input.vColor;
   /* Test
   output.vColor.x = 1.f;
   output.vColor.y = 0.f;
   output.vColor.z = 1.f;
   */
    return output;
}

struct PS_INPUT
{
    float3 vDiffuse : TEXCOORD0;
    float4 vColor : COLOR;
};


float4 ps_main(PS_INPUT input) : COLOR
{
    float4 vColor = input.vColor;
    float3 diffuse = saturate(input.vDiffuse);
    float4 vDiffuse = float4(diffuse, 1.f) * vColor;
    return vDiffuse;
   
}

#endif