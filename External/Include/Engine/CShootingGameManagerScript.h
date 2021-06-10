#pragma once
#include "CScript.h"
class CShootingGameManager : public CScript
{
private:
    enum class E_GameState_Shooting {
        Lobby,
        GameOver,
    };
    E_GameState_Shooting m_eState;

public:
    virtual void Awake() override;
    virtual void Start() override;
    virtual void Update() override;

private:
    void SetGameState(E_GameState_Shooting _eState) { m_eState = _eState; }

public:
    E_GameState_Shooting GetGameState() { return m_eState; }

public:
    CShootingGameManager();
    virtual ~CShootingGameManager() override;
};