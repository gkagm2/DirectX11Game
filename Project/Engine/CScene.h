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
    virtual void Render() override {};

public:
    CGameObject* FindGameObject(const tstring& _strName, E_Layer _eLayer = E_Layer::End);

    CLayer* GetLayer(E_Layer _eLayer) { return m_arrLayer[(UINT)_eLayer]; }
    CLayer* GetLayer(UINT _iLayerIdx) { return m_arrLayer[_iLayerIdx]; }

private:
    void _AddGameObject(CGameObject* _pObj, E_Layer _eLayer = E_Layer::Default, bool _bChangeChildLayer = true);
    void _UnRegisterAllObjects();

public:
    virtual bool SaveToScene(FILE* _pFile) override;
    virtual bool LoadFromScene(FILE* _pFile) override;

private:
    CLONE_DISABLE(CScene);
public:
    CScene();
    virtual ~CScene() override;
    friend class CEventManager;
    friend class CSceneManager;
};