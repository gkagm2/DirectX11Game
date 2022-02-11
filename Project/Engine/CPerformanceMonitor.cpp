#include "pch.h"
#include "CPerformanceMonitor.h"
clock_t CPerformanceMonitor::m_start = {};
clock_t CPerformanceMonitor::m_end= {};
double CPerformanceMonitor::m_result= {};
tstring CPerformanceMonitor::m_strLog = {};

#define CONSOLE_COLOR_RED 4
#define CONSOLE_COLOR_GREEN 2
#define CONSOLE_COLOR_WHITE 7

void CPerformanceMonitor::TimeStart(const tstring& _log)
{
	m_strLog = _log;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), CONSOLE_COLOR_GREEN);
	_tcprintf_s(_T("[performance monitoring start] %s\n"), m_strLog.c_str());
	m_start = clock();
	
}

void CPerformanceMonitor::TimeEnd()
{
	m_end = clock();
	// ms
	m_result = (double)(m_end - m_start);
	// sec
	double timeInSecond = (double)(m_result / CLOCKS_PER_SEC);
	_tcprintf_s(_T("[delay time] "));
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), CONSOLE_COLOR_RED);
	_tcprintf_s(_T("%lf\n"), timeInSecond);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), CONSOLE_COLOR_WHITE);
}