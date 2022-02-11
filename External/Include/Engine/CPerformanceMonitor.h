#pragma once

#ifdef _DEBUG
#define PM_START(strLog) CPerformanceMonitor::TimeStart(strLog)
#define PM_END() CPerformanceMonitor::TimeEnd()
#else
#define PM_START(strLog) 
#define PM_END() 
#endif

class CPerformanceMonitor
{
private:
	static clock_t m_start;
	static clock_t m_end;
	static double m_result;
	static tstring m_strLog;
public:
	static void TimeStart(const tstring& _log);
	static void TimeEnd();
};