#pragma once
#include "CSingleton.h"
#include "CMouseEvent.h"

class CMouseEvent;
class CMouseManager : public CSingleton<CMouseManager>
{
	SINGLETON(CMouseManager)

public:
	LRESULT WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	
public:
	void Update();

private:
	std::queue<CMouseEvent> eventBuffer;
	bool leftIsDown = false;
	bool rightIsDown = false;
	bool mbuttonDown = false;
	int x = 0;
	int y = 0;

	CMouseEvent m_CurMouseEvent;

public:
	const CMouseEvent& GetMouseEvent() { return m_CurMouseEvent; }
	inline int GetPosX();
	inline int GetPosY();
	MousePoint GetPos();

private:
	inline void _OnLeftPressed(int x, int y);
	inline void _OnLeftReleased(int x, int y);
	inline void _OnRightPressed(int x, int y);
	inline void _OnRightReleased(int x, int y);
	inline void _OnMiddlePressed(int x, int y);
	inline void _OnMiddleReleased(int x, int y);
	inline void _OnWheelUp(int x, int y);
	inline void _OnWheelDown(int x, int y);
	inline void _OnMouseMove(int x, int y);
	
	inline bool _IsLeftDown();
	inline bool _IsMiddleDown();
	inline bool _IsRightDown();
	
	inline bool _EventBufferIsEmpty();
	CMouseEvent _ReadEvent();
};