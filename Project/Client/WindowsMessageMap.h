#pragma once
class WindowsMessageMap
{
private:
	unordered_map<DWORD, string> m_umap;
public:
	WindowsMessageMap();
	virtual ~WindowsMessageMap();

	string operator() (DWORD message, LPARAM lp, WPARAM wp) const;
};