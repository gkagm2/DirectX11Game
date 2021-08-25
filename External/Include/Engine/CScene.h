#pragma once
#include "CObject.h"
#include "Interfaces.h"

class CLayer;
class CGameObject;
class CScene : public CObject, ILifeCycleInterface
{
private:
    CLayer* m_arrLayer[MAX_SIZE_LAYER];
    vector<CGameObject*> m_vecRootObjs;

public:
    virtual void Awake() override;
    virtual void Start() override;
    virtual void PrevUpdate() override;
    virtual void Update() override;
    virtual void LateUpdate() override;
    virtual void FinalUpdate() override;
    virtual void Render() override {};

public:
    CGameObject* FindGameObject(const tstring& _strName, UINT _iLayer = MAX_SIZE_LAYER);
    void GetRootGameObjects(vector<CGameObject*>& _vecRootObjs);

    CLayer* GetLayer(UINT _iLayer) { return m_arrLayer[_iLayer]; }
    void GetGameObjects(vector<CGameObject*>& _vecObjs, UINT _iLayer = MAX_SIZE_LAYER);

private:
    void _AddGameObject(CGameObject* _pObj, UINT _iLayer = 0, bool _bChangeChildLayer = true);
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