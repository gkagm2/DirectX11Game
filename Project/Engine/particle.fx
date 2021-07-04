#ifndef _PARTICLE
#define _PARTICLE

#include "value.fx"
#include "struct.fx"

// --------------------
// Particle Render Shader
// Mesh : point mesh
#define vStartColor g_vec4_0.xyz
#define vEndColor   g_vec4_1.xyz

#define vStartScale g_vec4_2.xyz
#define vEndScale   g_vec4_3.xyz
// --------------------

StructuredBuffer<TParticle> g_particle : register(t12);

struct VS_IN
{
    float3 vPos : POSITION;
    float2 vUV : TEXCOORD;
    
    uint iInstID : SV_InstanceID; // 인스턴싱 ID
};

struct VS_OUT
{
    float3 vViewPos : POSITION;
    float2 vUV : TEXCOORD;
    
    float fInstID : FOG; // output에는 uint형이 없음. float로 선언
};

VS_OUT VS_Particle(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
    
    float3 vWorldPos = g_particle[_in.iInstID].vWorldPos;
    output.vViewPos = mul(float4(vWorldPos, 1.f), g_matView).xyz;

    output.vUV = _in.vUV;
    output.fInstID = (float) _in.iInstID;
    
    return output;
}

struct GS_OUT
{
    float4 vPosition : SV_Position;
    float2 vUV : TEXCOORD;
    
    uint iInstID : SV_InstanceID;
};

[maxvertexcount(6)]
void GS_Particle(point VS_OUT _in[1], inout TriangleStream<GS_OUT> _outputStream)
{
    GS_OUT output[4] =
    {
        (GS_OUT) 0.f, (GS_OUT) 0.f, (GS_OUT) 0.f, (GS_OUT) 0.f
    };
    
    uint id = (uint) _in[0].fInstID;
    if (0 == g_particle[id].iAlive)
        return;
    
    float fRatio = g_particle[id].fCurTime / g_particle[id].fMaxTime;
    float3 fCurScale = (vStartScale + (vEndScale - vStartScale) * fRatio) * 0.5f;
    
    // 0  1
    //
    // 3  2
    
    output[0].vPosition = mul(float4(_in[0].vViewPos + float3(-fCurScale.x, fCurScale.y, 0.f), 1.f), g_matProjection);
    output[1].vPosition = mul(float4(_in[0].vViewPos + float3(fCurScale.x, fCurScale.y, 0.f), 1.f), g_matProjection);
    output[2].vPosition = mul(float4(_in[0].vViewPos + float3(fCurScale.x, -fCurScale.y, 0.f), 1.f), g_matProjection);
    output[3].vPosition = mul(float4(_in[0].vViewPos + float3(-fCurScale.x, -fCurScale.y, 0.f), 1.f), g_matProjection);
    
    output[0].vUV = float2(0.f, 0.f);
    output[1].vUV = float2(1.f, 0.f);
    output[2].vUV = float2(1.f, 1.f);
    output[3].vUV = float2(0.f, 1.f);
    
    output[0].iInstID = id;
    output[1].iInstID = id;
    output[2].iInstID = id;
    output[3].iInstID = id;

    _outputStream.Append(output[0]);
    _outputStream.Append(output[1]);
    _outputStream.Append(output[2]);
    
    _outputStream.Append(output[0]);
    _outputStream.Append(output[2]);
    _outputStream.Append(output[3]);
    
    _outputStream.RestartStrip(); // 모양 설정
}

float4 PS_Particle(GS_OUT _in) : SV_Target
{
    float4 vOutColor = (float4) 0.f;
    
    float fRatio = g_particle[_in.iInstID].fCurTime / g_particle[_in.iInstID].fMaxTime;
    float3 vCurColor = vStartColor + (vEndColor - vStartColor) * fRatio;
    vOutColor = float4(vCurColor, 1.f);
    return vOutColor;
}

#endif