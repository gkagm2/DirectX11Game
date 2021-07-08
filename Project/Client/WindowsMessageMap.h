#pragma once
class WindowsMessageMap
{
private:
	unordered_map<DWORD, string> m_umap;
public:
	WindowsMessageMap();
	virtual ~WindowsMessageMap();

	tstring operator() (DWORD message, LPARAM lp, WPARAM wp) const;
};