#ifndef _PARTICLE_UPDATE
#define _PARTICLE_UPDATE
#include "value.fx"
#include "struct.fx"
#include "function.fx"

// ----------------------
// Particle Update Shader
#define iParticleMaxCount   g_int_0
#define fStartSpeed         g_float_0
#define fEndSpeed           g_float_1

#define fMinLifeTime        g_float_2
#define fMaxLifeTime        g_float_3

#define vObjectPos  g_vec4_0
#define vRadius     g_vec4_1
// ----------------------

RWStructuredBuffer<TParticle> g_particle : register(u0);
RWStructuredBuffer<TSharedParticleData> g_sharedParticle : register(u1);

[numthreads(1024, 1, 1)]
void CS_ParticleUpdate(int3 _iThreadID : SV_DispatchThreadID)
{
    // ��� ��ƼŬ�� ��Ȱ��ȭ ���¸� ����
    if (iParticleMaxCount <= _iThreadID.x)
        return;
    
    // �׾�����
    if (!g_particle[_iThreadID.x].iAlive)
    {
        int iOriginValue = g_sharedParticle[0].m_iSpawnCount;
        int iExchanged = 0;
        
        while (0 < iOriginValue)
        {
            int iInputValue = iOriginValue - 1;
            //InterlockedExchange(g_Shared[0].m_iSpawnCount, iInputValue, iExchanged);                        
            InterlockedCompareExchange(g_sharedParticle[0].m_iSpawnCount, iOriginValue, iInputValue, iExchanged);
            
            // ��÷
            if (iExchanged == iOriginValue)
            {
                // Ȱ��ȭ
                g_particle[_iThreadID.x].iAlive = 1;
                break;
            }
            iOriginValue = iInputValue;
        }
        
        if (1 == g_particle[_iThreadID.x].iAlive)
        {
            // ��ġ�� ������ �������� ����
            float fKey = (float) _iThreadID.x / (float) iParticleMaxCount;
            float4 vRand = (float) 0.f;
            vRand.x = Rand(fKey);
            vRand.y = Rand(vRand.x);
            vRand.z = Rand(vRand.y);
            vRand.w = Rand(vRand.z);
    
            vRand = vRand * 2.f - 1.f;
            
            float3 worldPos = (float3) (_iThreadID.x, 0, 0);
            g_particle[_iThreadID.x].vWorldPos = worldPos * 10.f;
            //g_particle[_iThreadID.x].vWorldPos = vObjectPos.xyz + (vRadius.xyz * vRand.xyz);
            g_particle[_iThreadID.x].vWorldDir = normalize(vRand.xyz);
            g_particle[_iThreadID.x].fCurTime = 0.f;
            g_particle[_iThreadID.x].fMaxTime = 5.f;
            //g_particle[_iThreadID.x].fMaxTime = fMinLifeTime + (fMaxLifeTime - fMinLifeTime) * vRand.w;
        }
}
    else // �������
    {
        g_particle[_iThreadID.x].fCurTime += g_fDeltaTime;
    
        // �ð� �ʰ� �� ����
        if (g_particle[_iThreadID.x].fMaxTime < g_particle[_iThreadID.x].fCurTime)
        {
            g_particle[_iThreadID.x].iAlive = 0;
            return;
        }
    
        float fLifeRatio = g_particle[_iThreadID.x].fCurTime / g_particle[_iThreadID.x].fMaxTime;
        float fCurSpeed = (fEndSpeed - fStartSpeed) * fLifeRatio + fStartSpeed;
    
        g_particle[_iThreadID.x].vWorldPos += g_particle[_iThreadID.x].vWorldDir * fCurSpeed * g_fDeltaTime;
    }
}
#endif