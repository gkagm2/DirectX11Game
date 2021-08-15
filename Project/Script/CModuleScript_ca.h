#pragma once
#include <Engine\CScript.h>

enum class E_PartsLevel_ca {
	Alpha,	//�ʷϻ�
	Bravo,	//�����
	Charlie,//��Ȳ��
	Delta,	//������
	Echo,	//��ȫ��
	Foxtrot,//�����
	Golf,	//�Ķ���
	Hotel,	//�ϴû�
	India,	//����
	Juliet,	//���
	Kilo,	//ȸ��
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
	float fWeight;		// ����  
	float fHp;			// ü��
};

class CModuleScript_ca : public CScript
{
private:
	vector<TPartConnectPoint_ca> m_vecPartsConnectPoint;

	CGameObject* m_pParentPartGameObj;
	vector<CGameObject*> m_pChildPartGameObj;

	E_PartsLevel_ca eLevel;
	float fWeight;		// ����  
	float fHp;			// ü��

public:
	virtual void Start() override;
	virtual void Update() override;

public:
	/*void ConnectPart(CGameObject _pPart, TPartConnectPoint_ca _tConnectPoint);
	void DisconnectPart();*/

public:
	CLONE(CModuleScript_ca);
	CModuleScript_ca();
	CModuleScript_ca(UINT _iScriptNum);
	virtual ~CModuleScript_ca() override;
};