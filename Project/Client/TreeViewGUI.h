#pragma once
#include "GUI.h"

class TreeViewGUI;
class TreeViewNode;


// SEL_CHANGE : Select Change (���õȰ� ����Ǿ��� ����� �ݹ� �Լ�)
typedef void (GUI::* SELECT_CHANGE_CALLBACK)(TreeViewNode* _pNode);
typedef void (GUI::* DRAG_DROP_CALLBACK)(TreeViewNode* _pDragedItem, TreeViewNode* _pDropedItem);

class TreeViewNode
{
private:
	TreeViewGUI* m_pOwner;
	TreeViewNode* m_pParentNode;
	vector<TreeViewNode*> m_vecChildNodes;

	string m_strName;
	DWORD_PTR m_dwData;
	bool m_bUseFrame;

	UINT m_iStyleFlag;

public:
	void Update();

public:
	void SetName(const string& _strName) { m_strName = _strName; }
	const string& GetName() { return m_strName; }
	void SetData(DWORD_PTR _dwData) { m_dwData = _dwData; }
	DWORD_PTR GetData() { return m_dwData; }
	void UseFrame(bool _bUse) { m_bUseFrame = _bUse; }

	void AddChild(TreeViewNode* _pChildNode) {
		m_vecChildNodes.push_back(_pChildNode);
		_pChildNode->_SetParent(this);
	}
	const vector<TreeViewNode*>& GetChildNodes() { return m_vecChildNodes; }

private:
	void _SetParent(TreeViewNode* _pParentNode) { m_pParentNode = _pParentNode; }
	void _SetOwner(TreeViewGUI* _pOwner) { m_pOwner = _pOwner; }

public:
	TreeViewNode();
	virtual ~TreeViewNode();

	friend class TreeViewGUI;
};

class TreeViewGUI : public GUI
{
private:
	TreeViewNode* m_pRootNode;		// �ֻ��� ���
	TreeViewNode* m_pSelectedNode;	// ������ ���

	bool m_bRootRender;		// �ֻ��� �θ� �������ϴ��� ����
	bool m_bFrameUse;		// Frame ��� ����
	bool m_bFrameOnlyParent;// Parent�� Frame�� ����� ������ ����

	// �ݹ��Լ�
	SELECT_CHANGE_CALLBACK	m_pSelectFunc;
	DRAG_DROP_CALLBACK		m_PDragDropFunc;

public:
	virtual void Init() override;
	virtual void Update() override;

public:
	void SetRootRender(bool _bRender) { m_bRootRender = _bRender; }
	bool IsRootRender() { return m_bRootRender; }
	TreeViewNode* AddItem(const string& _str, DWORD_PTR _dwData, TreeViewNode* _pParent);

	void SetFrameRender(bool _bRender) { m_bFrameUse = _bRender; }
	bool IsFrameRender() { return m_bFrameUse; }

	void SetFrameOnlyParent(bool _bSet) { m_bFrameOnlyParent = _bSet; }
	bool IsFrameOnlyParent() { return m_bFrameOnlyParent; }

	TreeViewNode* GetRootNode() { return m_pRootNode; }

	void Clear();

private:
	void _SetSelectedNode(TreeViewNode* _pNode) { m_pSelectedNode = _pNode; }

public:
	TreeViewGUI();
	virtual ~TreeViewGUI() override;

	friend class TreeViewNode;
};