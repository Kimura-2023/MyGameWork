#pragma once
// ===各種計算用関数、クラス定義===
// 3D計算に必要な関数、クラスを定義
// Author : Kimura
// ================================

// ===インクルード===
#include "main.h"

// ===座標、拡縮、回転をまとめたもの===
struct Transform {
	XMFLOAT3 Pos;		// 座標
	XMFLOAT3 Scale;		// 拡縮
	XMFLOAT3 Rot;		// 回転
	Transform()
	{
		Pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
		Scale = XMFLOAT3(1.0f, 1.0f, 1.0f);
		Rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
	}

	Transform& operator =(const Transform &t)
	{
		Pos = t.Pos;
		Scale = t.Scale;
		Rot = t.Rot;
		return *this;
	}
};

// ===クォータニオンクラス定義===
class Quaternion
{
public:

	// ===クォータニオン初期化===
	Quaternion()
	{
		x = y = z = w = 0;
	}

	Quaternion(float ax, float ay, float az, float aw)
	{
		x = ax;
		y = ay;
		z = az;
		w = aw;
	}

	Quaternion(XMFLOAT3 vec)
	{
		x = vec.x;
		y = vec.y;
		z = vec.z;
		w = 0.0f;
	}

	Quaternion(XMFLOAT3 vec,float angle)
	{
		float rad = angle * (float)M_PI / 180.0f;
		x = vec.x * sinf(rad / 2);
		y = vec.y * sinf(rad / 2);
		z = vec.z * sinf(rad / 2);
		w = cosf(rad / 2);
	}

	Quaternion(XMFLOAT4 q)
	{
		x = q.x;
		y = q.y;
		z = q.z;
		w = q.y;
	}

	// ===クォータニオン乗算===
	Quaternion operator *(const Quaternion &q)
	{
		Quaternion ans;
		ans.w = this->w * q.w - this->x * q.x - this->y * q.y - this->z * q.z;
		ans.x = this->w * q.x + q.w * this->x + this->y * q.z - this->z * q.y;
		ans.y = this->w * q.y + q.w * this->y + this->z * q.x - this->x * q.z;
		ans.z = this->w * q.z + q.w * this->z + this->x * q.y - this->y * q.x;
		return ans;
	}

	// ===クォータニオン加算===
	Quaternion operator +(const Quaternion &q)
	{
		Quaternion ans;
		ans.w = this->w + q.w;
		ans.x = this->x + q.x; 
		ans.y = this->y + q.y;
		ans.z = this->z + q.z;
		return ans;
	}

	// ===共役四元数を算出===
	Quaternion operator -()
	{
		Quaternion ans;
		ans.w = w;
		ans.x = -this->x;
		ans.y = -this->y;
		ans.z = -this->z;
		return ans;
	}

	// ===クォータニオン代入===
	Quaternion& operator =(const Quaternion &q)
	{
		x = q.x;
		y = q.y;
		z = q.z;
		w = q.w;
		return *this;
	}

	// ===関数定義===
	static Quaternion Identity();																							// 初期化
	static Quaternion EulerQuaternion(const float& x, const float& y, const float& z);										// オイラー角から算出
	static void LookAt(Quaternion* qOut, const XMFLOAT3& vec, const XMFLOAT3& target);										// 対象のベクトルに移動させる
	static void LookRotation(Quaternion* qOut, const XMFLOAT3& target, const XMFLOAT3& up = XMFLOAT3(0.0f, 1.0f, 0.0f));	// XZ平面上であるベクトルに回転するクォータニオンを求める
	static void AxisRotate(Quaternion* qOut, const XMFLOAT3&, float);														// 軸回転させるクォータニオン生成
	static void PositionRotate(Quaternion* qOut, const XMFLOAT3& axis, float angle);										// 回転させた座標を取得
	static void SLerp(Quaternion* qOut, const Quaternion& start, const Quaternion& end, float rate);						// 球面線形補間
	static void GetRotateMatrix(XMFLOAT4X4* matrix,const Quaternion& q);													// クォータニオンから回転マトリックス取得(XMFLOAT4X4型
	static void GetRotateMatrix(XMMATRIX* matrix, const Quaternion& q);														// クォータニオンから回転マトリックス取得(XMMATRIX

	float x, y, z, w;
};

// ===回転行列→オイラー角===
void RotationMatrixRollPitchYaw(XMFLOAT3* e, const XMFLOAT4X4& m);

// ===単位ベクトルを算出===
XMFLOAT3 CalcUnitVector(const XMFLOAT3&);
void Normalize(XMFLOAT3* vec);
void Normalize(XMFLOAT2* vec);

// ===外積===
void Cross(XMFLOAT3* vOut, const XMFLOAT3& vec1, const XMFLOAT3& vec2);

// ===内積===
void Dot(float* out, const XMFLOAT2& vec1, const XMFLOAT2& vec2);
void Dot(float* out, const XMFLOAT3& vec1, const XMFLOAT3& vec2);

// ===2つのベクトルからラジアン値を算出===
void CalcAngle(float* angle,const XMFLOAT3& vec1,const XMFLOAT3& vec2);
void CalcAngle(float* angle, const XMFLOAT2& vec1, const XMFLOAT2& vec2);

// ===四捨五入===
void Round(float* out, const int& num);
void Round(XMFLOAT3*, const int&);

// ===距離算出===
void Length(float* out, const XMFLOAT3& vec1, const XMFLOAT3& vec2);
void Length(float* out, const XMFLOAT2& vec1, const XMFLOAT2& vec2);

// ===座標を軸回転させる===
void TransformRotate(Transform* transform, const XMFLOAT3& axis, const float& rot);

// ===整数桁数を算出===
void Digit(int* out, const int& num);

// ===初期クォータニオン===
inline Quaternion Quaternion::Identity()
{
	return Quaternion(0, 0, 0, 1);
}

// ===オイラー角からクォータニオンを求める===
inline Quaternion Quaternion::EulerQuaternion(const float& rx, const float& ry, const float& rz)
{
	float x, y, z;
	x = rx * (float)M_PI / 180.0f;
	y = ry * (float)M_PI / 180.0f;
	z = rz * (float)M_PI / 180.0f;

	x *= 0.5f;
	y *= 0.5f;
	z *= 0.5f;

	float sinx = sinf(x);
	float siny = sinf(y);
	float sinz = sinf(z);
	float cosx = cosf(x);
	float cosy = cosf(y);
	float cosz = cosf(z);

	Quaternion q;
	q.w = cosx * cosy * cosz + sinx * siny * sinz;
	q.x = sinx * cosy * cosz + cosx * siny * sinz;
	q.y = cosx * siny * cosz - sinx * cosy * sinz;
	q.z = cosx * cosy * sinz - sinx * siny * cosz;
	
	return q;
}

// ===あるベクトルを対象のベクトルに移動させる(第一引数のx,y,zに移動後のベクトルが求められる===
inline void Quaternion::LookAt(Quaternion* qOut, const XMFLOAT3& vec,const XMFLOAT3& target)
{
	float angle;
	XMFLOAT3 vec1 = XMFLOAT3(vec.x, vec.y, vec.z);
	XMFLOAT3 vec2 = XMFLOAT3(target.x, target.y, target.z);
	Normalize(&vec1);
	Normalize(&vec2);
	CalcAngle(&angle, vec1, vec2);			// 二つのベクトルがなす角(cosθ)を求める

	XMFLOAT3 axis;
	Cross(&axis, vec1, vec2);

	Quaternion R, P, Q;
	P = Quaternion(vec);
	Q = Quaternion(axis.x * sinf(angle / 2.0f), axis.y * sinf(angle / 2.0f), axis.z * sinf(angle / 2.0f), cosf(angle / 2.0f));
	R = -Q;

	*qOut = R * P * Q;
}

// ===回転行列からクォータニオンを求める===
inline Quaternion GetRotation(const XMFLOAT4X4& matrix)
{
	float elem[4];
	elem[0] = matrix._11 - matrix._22 - matrix._33 + 1.0f;
	elem[1] = -matrix._11 + matrix._22 - matrix._33 + 1.0f;
	elem[2] = -matrix._11 - matrix._22 + matrix._33 + 1.0f;
	elem[3] = matrix._11 + matrix._22 + matrix._33 + 1.0f;

	int idx = 0;
	for (int i = 1; i < 4; ++i)
	{
		if (elem[i] > elem[idx])
		{
			idx = i;
		}
	}

	if (elem[idx] < 0)
	{
		return Quaternion::Identity();
	}

	float q[4];
	float v = sqrtf(elem[idx]) * 0.5f;
	q[idx] = v;
	float mult = 0.25f / v;

	switch (idx)
	{
	case 0:
		q[1] = (matrix._12 + matrix._21) * mult;
		q[2] = (matrix._31 + matrix._13) * mult;
		q[3] = (matrix._23 - matrix._32) * mult;
		break;
	case 1:
		q[0] = (matrix._12 + matrix._21) * mult;
		q[2] = (matrix._23 + matrix._32) * mult;
		q[3] = (matrix._31 - matrix._13) * mult;
		break;
	case 2:
		q[0] = (matrix._31 + matrix._13) * mult;
		q[1] = (matrix._23 + matrix._32) * mult;
		q[3] = (matrix._12 - matrix._21) * mult;
		break;
	case 3:
		q[0] = (matrix._23 - matrix._32) * mult;
		q[1] = (matrix._31 - matrix._13) * mult;
		q[2] = (matrix._12 - matrix._21) * mult;
		break;
	}

	return Quaternion(q[0], q[1], q[2], q[3]);
}

// ===クォータニオンから回転マトリックスを算出===
inline void Quaternion::GetRotateMatrix(XMFLOAT4X4* matrix, const Quaternion& q)
{
	matrix->_11 = 1 - 2 * (q.y * q.y + q.z * q.z);
	matrix->_12 = 2 * (q.x * q.y - q.w * q.z);
	matrix->_13 = 2 * (q.x * q.z + q.w * q.y);
	matrix->_14 = 0.0f;
	matrix->_21 = 2 * (q.x * q.y + q.w * q.z);
	matrix->_22 = 1 - 2 * (q.x * q.x + q.z * q.z);
	matrix->_23 = 2 * (q.y * q.z - q.w * q.x);
	matrix->_24 = 0.0f;
	matrix->_31 = 2 * (q.x * q.z - q.w * q.y);
	matrix->_32 = 2 * (q.y * q.z + q.w * q.x);
	matrix->_33 = 1 - 2 * (q.x * q.x + q.y * q.y);
	matrix->_34 = 0.0f;
	matrix->_41 = 0.0f;
	matrix->_42 = 0.0f;
	matrix->_43 = 0.0f;
	matrix->_44 = 1.0f;
}

// ===クォータニオンから回転マトリックスを算出===
inline void Quaternion::GetRotateMatrix(XMMATRIX* mat, const Quaternion& q)
{
	XMFLOAT4X4 matrix;
	matrix._11 = 1 - 2 * (q.y * q.y + q.z * q.z);
	matrix._12 = 2 * (q.x * q.y - q.w * q.z);
	matrix._13 = 2 * (q.x * q.z + q.w * q.y);
	matrix._14 = 0.0f;
	matrix._21 = 2 * (q.x * q.y + q.w * q.z);
	matrix._22 = 1 - 2 * (q.x * q.x + q.z * q.z);
	matrix._23 = 2 * (q.y * q.z - q.w * q.x);
	matrix._24 = 0.0f;
	matrix._31 = 2 * (q.x * q.z - q.w * q.y);
	matrix._32 = 2 * (q.y * q.z + q.w * q.x);
	matrix._33 = 1 - 2 * (q.x * q.x + q.y * q.y);
	matrix._34 = 0.0f;
	matrix._41 = 0.0f;
	matrix._42 = 0.0f;
	matrix._43 = 0.0f;
	matrix._44 = 1.0f;

	*mat = XMLoadFloat4x4(&matrix);
}

// ===球面線形補間===
inline void Quaternion::SLerp(Quaternion* qOut, const Quaternion& pstart, const Quaternion& pend, float rate)
{
	float cosom = pstart.x * pend.x + pstart.y * pend.y + pstart.z * pend.z + pstart.w * pend.w;
	Quaternion end = pend;
	if (cosom < 0.0f)
	{
		cosom = -cosom;
		end.x = -end.x;
		end.y = -end.y;
		end.z = -end.z;
		end.w = -end.w;
	}

	float sclp, sclq;
	if (1.0f - cosom > 0.0001f)
	{
		float omega, sinom;
		omega = acosf(cosom);
		sinom = sinf(omega);
		sclp = sinf((1.0f - rate) * omega) / sinom;
		sclq = sinf(rate * omega) / sinom;

	}
	else
	{
		sclp = 1.0f - rate;
		sclq = rate;
	}

	qOut->x = sclp * pstart.x + sclq * end.x;
	qOut->y = sclp * pstart.y + sclq * end.y;
	qOut->z = sclp * pstart.z + sclq * end.z;
	qOut->w = sclp * pstart.w + sclq * end.w;
}

// ===XZ平面上であるベクトルに回転するクォータニオンを求める===
inline void Quaternion::LookRotation(Quaternion* qOut, const XMFLOAT3& target, const XMFLOAT3& up)
{
	XMFLOAT3 Z = target;
	XMFLOAT3 Y = up;
	Normalize(&Z);
	Normalize(&Y);
	XMFLOAT3 X;
	Cross(&X, Y, Z);
	Cross(&Y, Z, X);
	Normalize(&Y);

	XMFLOAT4X4 m;
	XMStoreFloat4x4(&m, XMMatrixIdentity());
	m._11 = X.x; m._12 = Y.x; m._13 = Z.x;
	m._21 = X.y; m._22 = Y.y; m._23 = Z.y;
	m._31 = X.z; m._32 = Y.z; m._33 = Z.z;

	Quaternion rot = GetRotation(m);
	*qOut = rot;
}

// ===軸回転クォータニオン生成===
inline void Quaternion::AxisRotate(Quaternion* qOut, const XMFLOAT3& axis, float angle)
{
	float rad = angle * (float)M_PI / 180.0f;
	*qOut = Quaternion(axis.x * sinf(rad / 2.0f), axis.y * sinf(rad / 2.0f), axis.z * sinf(rad / 2.0f), cosf(rad / 2.0f));
}

// ===回転させた座標を取得===
inline void Quaternion::PositionRotate(Quaternion* qOut,const XMFLOAT3& axis, float angle)
{
	Quaternion P, R, Q;
	P = *qOut;
	Q = Quaternion(axis, angle);
	R = -Q;
	*qOut = R * P * Q;
}

template <class T>
inline T Random(const float& min, const float& max)
{
	return (T)(rand() % (int)(max - min) + min);
}

// ===線形補間===
inline void Vector3Lerp(XMFLOAT3* vOut, const XMFLOAT3 & start, const XMFLOAT3 & end,float rate)
{
	vOut->x = start.x * (1 - rate) + end.x * rate;
	vOut->y = start.y * (1 - rate) + end.y * rate;
	vOut->z = start.z * (1 - rate) + end.z * rate;
}