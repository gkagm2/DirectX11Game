#pragma once
#include <Engine\CScript.h>

enum class E_ModuleLevel_ca {
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
	End,
};

enum class E_CommandModuleLevel_ca {
	Nemesis,//�ƹ��͵� ����
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
	End,
};

enum class E_ModuleType_ca {
	Girder,
	Laser,
	Booster,
	Command,
	End,
};

struct TModuleConnectPoint_ca {
	Vector3 vPosition;	// ���� ��ġ
	Vector3 vDirection;	// ���� ����
};

class CModuleScript_ca : public CScript
{
private:
	vector<TModuleConnectPoint_ca> m_vecPartsConnectPoint;

	CGameObject* m_pParentModuleGameObj;
	vector<CGameObject*> m_pChildModuleGameObj;

	E_ModuleLevel_ca m_eModuleLevel;// ����
	E_ModuleType_ca m_eModuleType;	// Ÿ��
	float m_fWeight;				// ����  
	float m_fHp;					// ü��

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

public:
	virtual void InitModule(E_ModuleLevel_ca _eLevel) {};
	/*void ConnectPart(CGameObject _pPart, TPartConnectPoint_ca _tConnectPoint);
	void DisconnectPart();*/

public:
	CLONE(CModuleScript_ca);
	CModuleScript_ca();
	CModuleScript_ca(UINT _iScriptNum);
	virtual ~CModuleScript_ca() override;
};