#pragma once

template<typename T>
class CSingleton
{
private:
	static T* g_pInstance;
public:
	static T* GetInstance() {
		if (nullptr == g_pInstance) {
			g_pInstance = new T;
			atexit(&CSingleton<T>::Destroy); // ���� �� ȣ��
		}
		return g_pInstance;
	}

public:
	static void Destroy() {
		if (nullptr != g_pInstance) {
			delete g_pInstance;
			g_pInstance = nullptr;
		}
	}

protected:
	CSingleton() {}
	virtual ~CSingleton() {}
};

template<typename T>
T* CSingleton<T>::g_pInstance = nullptr;