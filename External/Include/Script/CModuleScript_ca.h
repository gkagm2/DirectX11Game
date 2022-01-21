#pragma once
#include <Engine\CScript.h>

enum class E_ModuleLevel_ca {
	Nemesis,//아무것도 없는 // Command Module에서만 사용한다.
	Alpha,	//초록색
	Bravo,	//노란색
	Charlie,//주황색
	Delta,	//빨간색
	Echo,	//분홍색
	Foxtrot,//보라색
	Golf,	//파란색
	Hotel,	//하늘색
	India,	//옥색
	Juliet,	//흰색
	Kilo,	//회색
	End,
};
extern const TCHAR* ModuleLevelSTR_ca[(UINT)E_ModuleLevel_ca::End];

enum class E_ModuleType_ca {
	Girder1x1,
	Girder1x2,
	Laser,
	Booster,
	Command,
	End,
};
extern const TCHAR* ModuleTypeSTR_ca[(UINT)E_ModuleType_ca::End];

enum class E_ModuleSize_ca {
	Size1x1,
	Size1x2,
	Size_OnlyMain,
	End
};

enum class E_Direction_ca{
	Forward,
	Left,
	Right,
	Back,
};
struct TModuleConnector_ca {
	Vector3 vPosition;	// 연결 위치 (object coordination position)
	Vector3 vDirection;	// 연결 방향
	bool bIsMain;		// Main 연결구
	bool bIsConnectable;// 연결 가능한지 여부
	E_Direction_ca eDirection;
};

class CModuleScript_ca : public CScript
{
private:
	vector<TModuleConnector_ca> m_vecModuleConnectors;
	TModuleConnector_ca* m_pParentModuleConnector; // 연결된 부모의 모듈 컨넥터

	CGameObject* m_pParentModuleGameObj;
	vector<CGameObject*> m_pChildModuleGameObj;

	E_ModuleLevel_ca m_eModuleLevel;// 레벨
	E_ModuleType_ca m_eModuleType;	// 타입
	E_ModuleSize_ca m_eModuleSize;	// 모듈 사이즈
	float m_fWeight;				// 질량  
	float m_fHp;					// 체력

public:
	virtual void Start() override;
	virtual void Update() override;

public:
	virtual bool SaveToScene(FILE* _pFile) override;
	virtual bool LoadFromScene(FILE* _pFile) override;

public:
	E_ModuleLevel_ca GetModuleLevel() { return m_eModuleLevel; }
	E_ModuleType_ca GetModuleType() { return m_eModuleType; }
	E_ModuleSize_ca GetModuleSize() { return m_eModuleSize; }

	float GetWeight() { return m_fWeight; }
	void SetWeight(float _fWeight) { m_fWeight = _fWeight; }
	float GetHp() { return m_fHp; }
	void SetHp(float _fHp) { m_fHp = _fHp; }

protected:
	void _SetModuleType(E_ModuleType_ca _eModuleType) { m_eModuleType = _eModuleType; }

public:
	void SetModuleLevel(E_ModuleLevel_ca _eModuleLevel);
	void SetModuleSize(E_ModuleSize_ca _eModuleSize);

	vector<TModuleConnector_ca>& GetConnectors() { return m_vecModuleConnectors; }

	// 연결당하는 위치
	Vector3 FindNearestConnectionPosition(const Vector3& _vPosition);
	TModuleConnector_ca& FindNearestConnector(const Vector3& _vPosition);
	// 연결하는 위치
	Vector3 GetMainConnectionPosition();
	Vector3 GetMainConnectionLocalPosition();
	TModuleConnector_ca& MainConnector();
	

	void ConnectModule(TModuleConnector_ca& _tOtherConnector);
	void DisConnectModule();
	

private:
	virtual void _InitModuleLevel(E_ModuleLevel_ca _eLevel) {};
	virtual void _InitModuleSize(E_ModuleSize_ca _eModuleSize);

	/*void ConnectPart(CGameObject _pPart, TPartConnectPoint_ca _tConnectPoint);
	void DisconnectPart();*/

protected:
	void AddModuleConnectPoint(const TModuleConnector_ca& _tModuleConnectPoint) { m_vecModuleConnectors.push_back(_tModuleConnectPoint); }
protected:
	void _ClearModuleConnectPoint() { m_vecModuleConnectors.clear(); }

public:
	CLONE(CModuleScript_ca);
	CModuleScript_ca();
	CModuleScript_ca(UINT _iScriptNum, E_ModuleType_ca _eModuleType, E_ModuleLevel_ca _eModuleLevel, E_ModuleSize_ca _eModuleSize);
	virtual ~CModuleScript_ca() override;
};