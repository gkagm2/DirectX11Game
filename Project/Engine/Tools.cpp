#include "pch.h"
#include "Tools.h"
/* SimpleMath 대체
///////////////// Matrix ////////////////

// 단위행렬
void SetIdentityMatrix4x4(Matrix4x4& m) {
	memset(m.m_afElements, 0, sizeof(m.m_afElements));
	m(0, 0) = m(1, 1) = m(2, 2) = m(3, 3) = 1.0f;
}

// 행렬 덧셈
void MatrixPlus(Matrix4x4& q, const Matrix4x4& a, const Matrix4x4& b) {
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			q(i, j) = a(i, j) + b(i, j);
		}
	}
}


// 행렬 뺄셈
void MatrixMinus(Matrix4x4& q, const Matrix4x4& a, const Matrix4x4& b) {
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			q(i, j) = a(i, j) - b(i, j);
		}
	}
}

// 행렬 곱셈
void MatrixMultiply(Matrix4x4& q, const Matrix4x4& a, const Matrix4x4& b) {
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			for (int k = 0; k < 4; ++k) {
				q(i, j) += a(i, k) * b(k, j);
			}
		}
	}
}

// 이동 행렬
inline void SetTranslateMatrix(Matrix4x4& m, float tx, float ty, float tz) {
	SetIdentityMatrix4x4(m);
	m(3, 0) = tx;
	m(3, 1) = ty;
	m(3, 2) = tz;
}

// 확대,축소 행렬
inline void SetScaleMatrix(Matrix4x4& m, float sx, float sy, float sz) {
	SetIdentityMatrix4x4(m);
	m(0, 0) = sx;
	m(1, 1) = sy;
	m(2, 2) = sz;
}

// 회전 행렬 X
void SetRotateXMatrix(Matrix4x4& m, float fRads) {
	SetIdentityMatrix4x4(m);
	m(1, 1) = cos(fRads);
	m(1, 2) = sin(fRads);
	m(2, 1) = -sin(fRads);
	m(2, 2) = cos(fRads);
}

// 회전 행렬 Y
void SetRotateYMatrix(Matrix4x4& m, float fRads) {
	SetIdentityMatrix4x4(m);
	m(0, 0) = cosf(fRads);
	m(0, 2) = -sinf(fRads);
	m(2, 0) = sinf(fRads);
	m(2, 2) = cos(fRads);
}

// 회전 행렬 Z
void SetRotateZMatrix(Matrix4x4& m, float fRads) {
	SetIdentityMatrix4x4(m);
	m(0, 0) = cosf(fRads);
	m(0, 1) = sinf(fRads);
	m(1, 0) = -sinf(fRads);
	m(1, 1) = cos(fRads);
}

// 프로젝션
void SetProjection(Matrix4x4& m, float d) {
	m.SetZero();
	m(0, 0) = d;
	m(1, 1) = d;
	m(2, 2) = 1;
	m(3, 3) = d;
}

// 전치 행렬
void TransposedMatrix(Matrix4x4& m) {
	float t;
	t = m(0, 1);
	m(0, 1) = m(1, 0);
	m(1, 0) = t;
	for (int y = 0; y < 4; ++y) {
		for (int x = 0; x < 4; ++x) {
			t = m(y, x);
			m(y, x) = m(x, y);
			m(x, y) = t;
		}
	}
}

// TODO (Sagacity Jang)
// 역행렬

// 행렬 내적

// 행렬 외적
*/

// TODO :  공식으로 추려야한다.! (성능 엄청안좋음)
// 비율을 유지하면서 원하는 사이즈로 맞춘다.
// 맞출 사이즈 : _fitWidth, _fitHeight, 원본 크기 : _width, _height
Vector2 ResizeScaleMaintainRatio(float _fitWidth, float _fitHeight, float _width, float _height)
{
	float maxValue = _width > _height ? _width : _height;
	float x = _width / maxValue;
	float y = _height / maxValue;
	float i = _fitWidth > _fitHeight ? _fitWidth : _fitHeight;
	while (i > 0) {
		if (x * i <= _fitWidth && y * i <= _fitHeight) {
			x = x * i;
			y = y * i;
			break;
		}
		i -= 0.1f;
	}

	return Vector2(x, y);
}


// 벡터 회전 2D Rotate
Vector3 Rotate(const Vector3& _vVec, float _fDegree)
{
	// degree + : 시계방향
	// degree - : 반시계방향
	float fRadian = CMyMath::DegreeToRadian(_fDegree);

	// 회전 
	return 	Vector3(
		_vVec.x * cosf(fRadian) - _vVec.y * sinf(fRadian),
		_vVec.x * sinf(fRadian) + _vVec.y * cosf(fRadian),
		0.0f
	);
}

Vector3 Rotate_Radian(const Vector3& _vVec, float _radian)
{
	// 회전 
	return Vector3(
		_vVec.x * cosf(_radian) - _vVec.y * sinf(_radian),
		_vVec.x * sinf(_radian) + _vVec.y * cosf(_radian),
		0.0f
	);
}

Vector2 Rotate(const Vector2& _vVec, float _fDegree)
{
	// degree + : 시계방향
	// degree - : 반시계방향
	float fRadian = CMyMath::DegreeToRadian(_fDegree);

	// 회전 
	return Vector2(
		_vVec.x * cosf(fRadian) - _vVec.y * sinf(fRadian),
		_vVec.x * sinf(fRadian) + _vVec.y * cosf(fRadian)
	);
}

Vector2 Rotate_Radian(const Vector2& _vVec, float _radian)
{
	// 회전 
	return Vector2(
		_vVec.x * cosf(_radian) - _vVec.y * sinf(_radian),
		_vVec.x * sinf(_radian) + _vVec.y * cosf(_radian)
	);
}



Vector3 Rotate3D(E_AngleType _eAngleType, const Vector3& _vAxis, float _fDegree) {
	float fRadian = CMyMath::DegreeToRadian(_fDegree);
	if (_eAngleType == E_AngleType::pitch) {
		return Vector3(
			_vAxis.x,
			_vAxis.y * cosf(fRadian) - _vAxis.z * sinf(fRadian),
			_vAxis.y * sinf(fRadian) + _vAxis.z * cosf(fRadian)
		);
	}
	else if (_eAngleType == E_AngleType::yaw) {
		return Vector3(
			_vAxis.z * sinf(fRadian) + _vAxis.x * cosf(fRadian),
			_vAxis.y,
			_vAxis.z * cosf(fRadian) - _vAxis.x * sinf(fRadian)
		);
	}
	else if (_eAngleType == E_AngleType::roll) {
		return Vector3(
			_vAxis.x * cosf(fRadian) - _vAxis.y * sinf(fRadian),
			_vAxis.x * sinf(fRadian) + _vAxis.y * cosf(fRadian),
			_vAxis.z
		);
	}
	assert(nullptr);
}

Vector3 Rotate3D_Radian(E_AngleType _eAngleType, const Vector3& _vAxis, float _fRadian) {
	if (_eAngleType == E_AngleType::pitch) {
		return Vector3(
			_vAxis.x,
			_vAxis.y * cosf(_fRadian) - _vAxis.z * sinf(_fRadian),
			_vAxis.y * sinf(_fRadian) + _vAxis.z * cosf(_fRadian)
		);
	}
	else if (_eAngleType == E_AngleType::yaw) {
		return Vector3(
			_vAxis.z * sinf(_fRadian) + _vAxis.x * cosf(_fRadian),
			_vAxis.y,
			_vAxis.z * cosf(_fRadian) - _vAxis.x * sinf(_fRadian)
		);
	}
	else if (_eAngleType == E_AngleType::roll) {
		return Vector3(
			_vAxis.x * cosf(_fRadian) - _vAxis.y * sinf(_fRadian),
			_vAxis.x * sinf(_fRadian) + _vAxis.y * cosf(_fRadian),
			_vAxis.z
		);
	}
	assert(nullptr);
}


Vector2 GetScreenPosFromCenter(Vector2 _vTargetPos, Vector2 _vCenterPos, float _fDegree)
{
	float fRadian = _fDegree * CMyMath::Deg2Rad();
	// degree가 증가되면 시계 반대방향으로 회전됨
	return Vector2 {
		_vCenterPos.x + (_vCenterPos.x - _vTargetPos.x) * cosf(fRadian) + (_vCenterPos.y - _vTargetPos.y) * sinf(fRadian),
		_vCenterPos.y - (_vCenterPos.x - _vTargetPos.x) * sinf(fRadian) + (_vCenterPos.y - _vTargetPos.y) * cosf(fRadian)
	};
}

Matrix GetRotationMatrix(Vector3 _vLookAt)
{
	// Right Vector 구하기	
	_vLookAt.Normalize();
	Vector3 vRight = Vector3(0.f, 1.f, 0.f).Cross(_vLookAt);
	vRight.Normalize();

	Vector3 vUp = _vLookAt.Cross(vRight);
	vUp.Normalize();

	Matrix matRot = XMMatrixIdentity();
	matRot._11 = vRight.x; matRot._12 = vRight.y; matRot._13 = vRight.z;
	matRot._21 = vUp.x; matRot._22 = vUp.y; matRot._23 = vUp.z;
	matRot._31 = _vLookAt.x; matRot._32 = _vLookAt.y; matRot._33 = _vLookAt.z;

	return matRot;
}

bool IsEqual_Float(const float& a, const float& b, const float& epsilon)
{
	return (epsilon > std::abs(a - b));
}

Vector3 DecomposeRotMat(const Matrix& _matRot)
{
	// _mat 을 분해 후 다시 행렬 만들기	
	Vector4 vMat[4];

	vMat[0] = Vector4(_matRot._11, _matRot._12, _matRot._13, _matRot._14);
	vMat[1] = Vector4(_matRot._21, _matRot._22, _matRot._23, _matRot._24);
	vMat[2] = Vector4(_matRot._31, _matRot._32, _matRot._33, _matRot._34);
	vMat[3] = Vector4(_matRot._41, _matRot._42, _matRot._43, _matRot._44);

	/*XMStoreFloat4(&vMat[0], _matRot._11);
	XMStoreFloat4(&vMat[1], _matRot.r[1]);
	XMStoreFloat4(&vMat[2], _matRot.r[2]);
	XMStoreFloat4(&vMat[3], _matRot.r[3]);*/

	Vector3 vNewRot;
	if (IsEqual_Float(vMat[0].z, -1.0f)) {
		float x = 0; //gimbal lock, value of x doesn't matter
		float y = XM_PI / 2;
		float z = x + atan2(vMat[1].x, vMat[2].x);
		vNewRot = Vector3{ x, y, z };
	}
	else if (IsEqual_Float(vMat[0].z, 1.0f)) {
		float x = 0;
		float y = -XM_PI / 2;
		float z = -x + atan2(-vMat[1].x, -vMat[2].x);
		vNewRot = Vector3{ x, y, z };
	}
	else { //two solutions exist
		float y1 = -asin(vMat[0].z);
		float y2 = XM_PI - y1;

		float x1 = atan2f(vMat[1].z / cos(y1), vMat[2].z / cos(y1));
		float x2 = atan2f(vMat[1].z / cos(y2), vMat[2].z / cos(y2));

		float z1 = atan2f(vMat[0].y / cos(y1), vMat[0].x / cos(y1));
		float z2 = atan2f(vMat[0].y / cos(y2), vMat[0].x / cos(y2));

		//choose one solution to return
		//for example the "shortest" rotation
		if ((std::abs(x1) + std::abs(y1) + std::abs(z1)) <= (std::abs(x2) + std::abs(y2) + std::abs(z2)))
		{
			vNewRot = Vector3{ x1, y1, z1 };
		}
		else {
			vNewRot = Vector3{ x2, y2, z2 };
		}
	}
	return vNewRot;
}