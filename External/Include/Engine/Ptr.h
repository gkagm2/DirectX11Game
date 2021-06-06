#pragma once
template<typename T>
class Ptr {
private:
	T* m_pRes;

public:

public:
	Ptr() :
		m_pRes(nullptr) {

	}

	Ptr(T* _pRes) : m_pRes(_pRes) {
		if (nullptr != m_pRes)
			m_pRes->AddRef();
	}

public:
	T* operator ->() {
		return m_pRes;
	}

};