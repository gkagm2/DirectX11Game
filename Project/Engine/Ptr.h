#pragma once

class CMesh;
class CTexture;
class CGraphicsShader;
class CComputeShader;
class CMaterial;

template<typename T>
class SharedPtr {
private:
	T* m_pResource;

public:
	T* Get() { return m_pResource; }
	T* Get() const { return m_pResource; }
	T** GetAddress() { return &m_pResource; }

public:
	SharedPtr() : m_pResource(nullptr) {
	}

	SharedPtr(T* _pResource) : m_pResource(_pResource) {
		if (nullptr != m_pResource)
			m_pResource->AddRef();
	}

	SharedPtr(const SharedPtr<T>& _other) : m_pResource(_other.m_pResource) {
		if (nullptr != _other.m_pResource) {
			m_pResource->AddRef();
		}
	}

	virtual ~SharedPtr() {
		if (nullptr != m_pResource) {
			try {
				m_pResource->SubRef();
			}
			catch (std::exception e)
			{
				assert(nullptr && _T("Ptr Error"));
			}
		}
	}

public:
	T* operator ->() {
		return m_pResource;
	}

	SharedPtr& operator =(T* _pResource) {
		if (nullptr != m_pResource) // 다른것을 가리키고 있는 경우
			m_pResource->SubRef();
		m_pResource = _pResource;
		if (nullptr != m_pResource) // 새로 입력받은게 NULL이 아닌 경우
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

	bool operator == (SharedPtr<T> _ptr) {
		return m_pResource == _ptr.Get();
	}
	bool operator != (SharedPtr<T> _ptr) {
		return !(m_pResource == _ptr.Get());
	}
};

template<typename T>
bool operator == (void* _pRes, SharedPtr<T> _ptr) {
	return _pRes == _ptr.Get();
}

template<typename T>
bool operator != (void* _pRes, SharedPtr<T> _ptr) {
	return !(_pRes == _ptr);
}