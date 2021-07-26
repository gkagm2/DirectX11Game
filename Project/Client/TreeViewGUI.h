#pragma once
#include "GUI.h"


class TreeViewNode
{
private:
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

	void AddChild(TreeViewNode* _pChildNode) { m_vecChildNodes.push_back(_pChildNode); }
	const vector<TreeViewNode*>& GetChildNodes() { return m_vecChildNodes; }

public:
	TreeViewNode();
	virtual ~TreeViewNode();
};


class TreeViewGUI : public GUI
{
private:
	TreeViewNode* m_pRoot;
	bool m_bRootRender;		// 최상위 부모를 렌더링하는지 여부
public:
	virtual void Init() override;
	virtual void Update() override;

public:
	void SetRootRender(bool _bRender) { m_bRootRender = _bRender; }
	TreeViewNode* AddItem(const string& _str, DWORD_PTR _dwData, TreeViewNode* _pParent);
	TreeViewNode* AddItem(const string& _str, DWORD_PTR _dwData, bool UseFrame, TreeViewNode* _pParent);
	void Clear();

public:
	TreeViewGUI();
	virtual ~TreeViewGUI() override;
};