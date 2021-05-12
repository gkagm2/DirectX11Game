#pragma once
#include "CSingleton.h"
class CDevice : public CSingleton<CDevice>
{
	SINGLETON(CDevice)
public:
};