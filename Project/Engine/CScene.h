#pragma once
#include "CObject.h"
#include "Interfaces.h"

class CLayer;
class CGameObject;
class CScene : public CObject, ILifeCycleInterface
{
private:
    CLayer* m_arrLayer[(UINT)E_Layer::End];

public:
    virtual void Awake() override;
    virtual void Start() override;
    virtual void PrevUpdate() override;
    virtual void Update() override;
    virtual void LateUpdate() override;
    virtual void FinalUpdate() override;
    virtual void Render() override;

public:
    void AddGameObject(CGameObject* _pObj, E_Layer _eLayer = E_Layer::Default);

    CGameObject* FindGameObject(const tstring& _strName, E_Layer _eLayer = E_Layer::End);

public:
    CScene();
    virtual ~CScene() override;
};