#pragma once
template<typename T>
class SharedPtr {
private:
	T* m_pResource;

public:
	SharedPtr() : m_pResource(nullptr) {
	}

	SharedPtr(T* _pResource) : m_pResource(_pResource) {
		if (nullptr != m_pResource)
			m_pResource->AddRef();
	}

	SharedPtr(const SharedPtr<T>& _other) {
		if (nullptr != _other.m_pResource)
			m_pResource->AddRef();
	}

	virtual ~SharedPtr() {
		if (nullptr != m_pResource) {
			m_pResource->SubRef();
		}
	}

public:
	T* operator ->() {
		return m_pResource;
	}

	SharedPtr& operator =(T* _pResource) {
		if (nullptr != m_pResource) // �ٸ����� ����Ű�� �ִ� ���
			m_pResource->SubRef();
		m_pResource = _pResource;
		if (nullptr != m_pResource) // ���� �Է¹����� NULL�� �ƴ� ���
			m_pResource->AddRef();

		return *this;
	}

	SharedPtr& operator =(const SharedPtr<T>& _ptr) {
		if (nullptr != m_pResource)
			m_pResource->SubRef();
		m_pResource = _ptr.m_pResource;
		if (nullptr != m_pResource)
			m_pResource->AddRef();
		return *this;
	}

	bool operator == (T* _pResource) {
		return m_pResource == _pResource;
	}
	bool operator != (T* _pResource) {
		return !((*this) == _pResource);
	}
};
//
//template<typename T>
//bool operator(void* t, T*) {
//	if (t == T)
//		return true;
//	else
//		return false;
//}