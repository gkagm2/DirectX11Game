#pragma once
#include "CScript.h"

enum class E_GameState_sh {
    Lobby,
    GameOver,
};

class CGameManagerScript_sh : public CScript
{

private:
    E_GameState_sh m_eState;

public:
    virtual void Awake() override;
    virtual void Start() override;
    virtual void Update() override;

private:
    void SetGameState(E_GameState_sh _eState) { m_eState = _eState; }

public:
    E_GameState_sh GetGameState() { return m_eState; }

public:
    CGameManagerScript_sh();
    virtual ~CGameManagerScript_sh() override;
};