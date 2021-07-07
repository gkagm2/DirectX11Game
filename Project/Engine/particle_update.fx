#ifndef _PARTICLE_UPDATE
#define _PARTICLE_UPDATE
#include "value.fx"
#include "struct.fx"

// ----------------------
// Particle Update Shader
#define iParticleMaxCount g_int_0
#define iSpawnCount g_int_1 // 이번 프레임에 생성할 파티클 개수
#define fStartSpeed g_float_0
#define fEndSpeed g_float_1

// ----------------------

RWStructuredBuffer<TParticle> g_particle : register(u0);
RWStructuredBuffer<TSharedParticleData> g_sharedParticle : register(u1);

[numthreads(1024, 1, 1)]
void CS_ParticleUpdate(int3 _iThreadID : SV_DispatchThreadID)
{
    int id = _iThreadID.x;
    // 담당 파티클이 비활성화 상태면 종료
    if (iParticleMaxCount <= id)
        return;
    

    // 죽었으면
    if (!g_particle[id].iAlive)
    {
        // 스레드간의 동기화가 필요하다
        if (0 < g_sharedParticle[0].m_iSpawnCount)
        {
            g_particle[id].iAlive = 1;
        }
    }
    else // 살았으면
    {
        g_particle[id].fCurTime += g_fDeltaTime;
    
        // 시간 초과 시 죽음
        if (g_particle[id].fMaxTime < g_particle[id].fCurTime)
        {
            g_particle[id].iAlive = 0;
            return;
        }
    
        float fLifeRatio = g_particle[id].fCurTime / g_particle[id].fMaxTime;
        float fCurSpeed = (fEndSpeed - fStartSpeed) * fLifeRatio + fStartSpeed;
    
        g_particle[id].vWorldPos += g_particle[id].vWorldDir * fCurSpeed * g_fDeltaTime;
    }
    
    
}
#endif