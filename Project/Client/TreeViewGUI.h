#pragma once
#include "GUI.h"

class TreeViewGUI;
class TreeViewNode;

// SEL_CHANGE : Select Change (���õȰ� ����Ǿ��� ����� �ݹ� �Լ�)
typedef void (GUI::* SEL_CHANGE_CALLBACK)(TreeViewNode* _pNode);
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
	ImVec4 m_vTextColor;

public:
	void Update();

public:
	void SetName(const string& _strName) { m_strName = _strName; }
	const string& GetName() { return m_strName; }
	void SetData(DWORD_PTR _dwData) { m_dwData = _dwData; }
	DWORD_PTR GetData() { return m_dwData; }
	void UseFrame(bool _bUse) { m_bUseFrame = _bUse; }
	void SetTextColor(const ImVec4 _vColor) { m_vTextColor = _vColor; }
	const ImVec4& GetTextColor() { return m_vTextColor; }


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

	TreeViewNode* m_pDraggedNode;	// �巡�� ������ ���
	TreeViewNode* m_pDropTargetNode;// �巡�� ������ ���

	bool m_bRootRender;		// �ֻ��� �θ� �������ϴ��� ����
	bool m_bFrameUse;		// Frame ��� ����
	bool m_bFrameOnlyParent;// Parent�� Frame�� ����� ������ ����

	// �ݹ��Լ�
	// Ŭ�� �� �ݹ�
	SEL_CHANGE_CALLBACK		m_pSelectFunc;
	GUI*					m_pSelectInst;

	// �巡�� �� ��ӽ� �ݹ�
	DRAG_DROP_CALLBACK		m_pDragDropFunc;
	GUI*					m_pDragDropInst;

	// ���� Ŭ�� �� �ݹ�
	GUI_CALLBACK	m_pDBCallBack; // DBC :: Double Click
	GUI*			m_pDBCInst;
	GLOBAL_CALLBACK m_pGDBCCallBack; // GDBC : global double click

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

public:
	void SetSelectCallBack(GUI* _pInst, SEL_CHANGE_CALLBACK _pMemFunc) {
		m_pSelectInst = _pInst;
		m_pSelectFunc = _pMemFunc;
	}
	void SetDragDropCallBack(GUI* _pInst, DRAG_DROP_CALLBACK _pMemFunc) {
		m_pDragDropInst = _pInst;
		m_pDragDropFunc = _pMemFunc;
	}

	void SetDoubleClickCallBack(GUI* _pGUI, GUI_CALLBACK _pFunc) {
		m_pDBCInst = _pGUI; m_pDBCallBack = _pFunc;
	}
	void SetDoubleClickCallBack(GLOBAL_CALLBACK _pFunc) {
		m_pGDBCCallBack = _pFunc;
	}

private:
	void _SetSelectedNode(TreeViewNode* _pNode) { m_pSelectedNode = _pNode; }
	void _SetDragStartNode(TreeViewNode* _pNode) { m_pDraggedNode = _pNode; }
	void _SetDropTargetNode(TreeViewNode* _pNode) { m_pDropTargetNode = _pNode; }

public:
	TreeViewGUI();
	virtual ~TreeViewGUI() override;

	friend class TreeViewNode;
};