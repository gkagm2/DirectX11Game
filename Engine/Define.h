#pragma once

#define SINGLETON(type)	private:\
						type();\
						virtual ~type();\
						friend class CSingleton<type>;