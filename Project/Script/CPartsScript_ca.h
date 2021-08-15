#pragma once
#include <Engine\CScript.h>

enum class E_PartsLevel_ca {
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
	End
};

enum class E_PartsType_ca {
	Girder,
	Laser,
};

struct TPartConnectPoint_ca {
	Vector3 vPosition;
	Vector3 vDirection;
};

struct TPartsInfo {
	E_PartsLevel_ca eLevel;
	float fWeight;		// 질량  
	float fHp;			// 체력
};

class CPartsScript_ca : public CScript
{
private:
	vector<TPartConnectPoint_ca> m_vecPartsConnectPoint;

	CGameObject* m_pParentPartGameObj;
	vector<CGameObject*> m_pChildPartGameObj;

	E_PartsLevel_ca eLevel;
	float fWeight;		// 질량  
	float fHp;			// 체력

public:
	virtual void Start() override;
	virtual void Update() override;

public:
	/*void ConnectPart(CGameObject _pPart, TPartConnectPoint_ca _tConnectPoint);
	void DisconnectPart();*/

public:
	CLONE(CPartsScript_ca);
	CPartsScript_ca();
	CPartsScript_ca(UINT _iScriptNum);
	virtual ~CPartsScript_ca() override;
};