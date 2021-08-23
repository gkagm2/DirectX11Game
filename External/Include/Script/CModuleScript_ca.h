#pragma once
#include <Engine\CScript.h>

enum class E_ModuleLevel_ca {
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

enum class E_CommandModuleLevel_ca {
	Nemesis,//아무것도 없는
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

enum class E_ModuleType_ca {
	Girder,
	Laser,
	Booster,
	Command,
	End,
};

enum class E_ModuleSize_ca {
	Size1x1,
	Size1x2,
	End
};

struct TModuleConnectPoint_ca {
	Vector3 vPosition;	// 연결 위치 (object coordination position)
	Vector3 vDirection;	// 연결 방향
	bool bIsMain;		// Main 연결구
};

class CModuleScript_ca : public CScript
{
private:
	vector<TModuleConnectPoint_ca> m_vecModuleConnectPoint;

	CGameObject* m_pParentModuleGameObj;
	vector<CGameObject*> m_pChildModuleGameObj;

	E_ModuleLevel_ca m_eModuleLevel;// 레벨
	E_ModuleType_ca m_eModuleType;	// 타입
	float m_fWeight;				// 질량  
	float m_fHp;					// 체력

public:
	virtual void Start() override;
	virtual void Update() override;

public:
	E_ModuleLevel_ca GetModuleLevel() { return m_eModuleLevel; }
	E_ModuleType_ca GetModuleType() { return m_eModuleType; }

	float GetWeight() { return m_fWeight; }
	void SetWeight(float _fWeight) { m_fWeight = _fWeight; }
	float GetHp() { return m_fHp; }
	void SetHp(float _fHp) { m_fHp = _fHp; }


	const vector<TModuleConnectPoint_ca>& GetConnectPoints() { return m_vecModuleConnectPoint; }

	// 연결당하는 위치
	const Vector3& FindNearestConnectionPosition(const Vector3& _vPositon);
	// 연결하는 위치
	const Vector3& GetMainConnectionPosition();

public:
	virtual void InitModule(E_ModuleLevel_ca _eLevel) {};
	virtual void InitModuleSize(E_ModuleSize_ca _eModuleSize);

	/*void ConnectPart(CGameObject _pPart, TPartConnectPoint_ca _tConnectPoint);
	void DisconnectPart();*/

protected:
	void AddModuleConnectPoint(const TModuleConnectPoint_ca& _tModuleConnectPoint) { m_vecModuleConnectPoint.push_back(_tModuleConnectPoint); }
protected:
	void _ClearModuleConnectPoint() { m_vecModuleConnectPoint.clear(); }

public:
	CLONE(CModuleScript_ca);
	CModuleScript_ca();
	CModuleScript_ca(UINT _iScriptNum);
	virtual ~CModuleScript_ca() override;
};