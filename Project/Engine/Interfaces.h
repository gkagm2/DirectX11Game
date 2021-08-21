#pragma once

// Game Object Life Cycle Interface
class ILifeCycleInterface
{
	virtual void Awake() = 0;
	virtual void Start() = 0;
	virtual void PrevUpdate() = 0;
	virtual void Update() = 0;
	virtual void LateUpdate() = 0;
	virtual void FinalUpdate() = 0;
	virtual void Render() = 0;
};

class ISaveLoadFileInterface {
	virtual bool SaveToFile(FILE* _pFile) = 0;
	virtual bool LoadFromFile(FILE* _pFile) = 0;
};

class ISaveLoadSceneInterface {
	virtual bool SaveToScene(FILE* _pFile) = 0;
	virtual bool LoadFromScene(FILE* _pFile) = 0;
};


class ICollision2DInterface {
	virtual void OnCollisionEnter2D(CCollider2D* _pOther) = 0;
	virtual void OnCollisionStay2D(CCollider2D* _pOther) = 0;
	virtual void OnCollisionExit2D(CCollider2D* _pOther) = 0;
};

class ICollision3DInterface {
	virtual void OnCollisionEnter(CCollider3D* _pOther) = 0;
	virtual void OnCollisionStay(CCollider3D* _pOther) = 0;
	virtual void OnCollisionExit(CCollider3D* _pOther) = 0;
};