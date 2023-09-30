// ===各種計算用関数、クラス実装===
// 3D計算に必要な関数、クラスを実装
// Author : Kimura
// ================================

// ===インクルード===
#include "Calc.h"

// ===回転マトリックスから角度を求める===
void RotationMatrixRollPitchYaw(XMFLOAT3* e, const XMFLOAT4X4& m)
{
	if (m._32 > 0.999999f && m._32 < 1.000001f) {
		e->x = (float)(M_PI / 2);
		e->y = 0.0f;
		e->z = atan2f(-m._21, m._11);
	}
	else if (m._32 < -0.999999f && m._32 > -1.000001f) {
		e->x = (float)(M_PI / -2);
		e->y = 0.0f;
		e->z = atan2f(-m._21, m._11);
	}
	else {
		e->x = -asinf(m._32);
		e->y = atan2f(m._31, m._33);
		e->z = atan2f(m._12, m._22);
	}
}

// ===渡されたベクトルの単位ベクトルを算出===
XMFLOAT3 CalcUnitVector(const XMFLOAT3& vec)
{
	XMVECTOR Unit = XMLoadFloat3(&vec);
	XMFLOAT3 ans;
	float length = sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
	Unit /= length;

	XMStoreFloat3(&ans,Unit);

	return ans;
}

// ===ベクトル正規化(三次元)===
void Normalize(XMFLOAT3* vec)
{
	XMVECTOR ans = XMLoadFloat3(vec);
	// ベクトルの大きさを求める
	float length = sqrt(vec->x * vec->x + vec->y * vec->y + vec->z * vec->z);
	ans /= length;
	XMStoreFloat3(vec, ans);
}

// ===ベクトル正規化(二次元)===
void Normalize(XMFLOAT2* vec)
{
	// ベクトルの大きさを求める
	float length = sqrt(vec->x*vec->x + vec->y*vec->y);
	vec->x = vec->x / length;
	vec->y = vec->y / length;
}

void Cross(XMFLOAT3* vOut, const XMFLOAT3& vec1, const XMFLOAT3& vec2)
{
	vOut->x = vec1.y * vec2.z - vec1.z * vec2.y;
	vOut->y = vec1.z * vec2.x - vec1.x * vec2.z;
	vOut->z = vec1.x * vec2.y - vec1.y * vec2.x;
}

// ===内積(2次元)===
void Dot(float* out, const XMFLOAT2& vec1, const XMFLOAT2& vec2)
{
	*out = vec1.x * vec2.x + vec1.y * vec2.y;
}

// ===内積(3次元)===
void Dot(float* out, const XMFLOAT3& vec1, const XMFLOAT3& vec2)
{
	*out = vec1.x * vec2.x + vec1.y * vec2.y + vec1.z * vec2.z;
}

// ===2つのベクトルからなす角度(ラジアン値)を求める===
void CalcAngle(float* angle, const XMFLOAT3& vec1, const XMFLOAT3& vec2)
{
	float dot = vec1.x * vec2.x + vec1.y * vec2.y + vec1.z * vec2.z;

	// ベクトルの大きさを求める
	float v1 = sqrtf(vec1.x*vec1.x + vec1.y*vec1.y + vec1.z*vec1.z);
	float v2 = sqrtf(vec2.x*vec2.x + vec2.y*vec2.y + vec2.z*vec2.z);

	float cosita = dot / (v1 * v2);

	*angle = -acosf(cosita);
}

void CalcAngle(float* angle, const XMFLOAT2& vec1, const XMFLOAT2& vec2)
{
	float dot = vec1.x * vec2.x + vec1.y * vec2.y;

	// ベクトルの大きさを求める
	float v1 = sqrtf(vec1.x*vec1.x + vec1.y*vec1.y);
	float v2 = sqrtf(vec2.x*vec2.x + vec2.y*vec2.y);

	float cosita = dot / (v1 * v2);
	*angle = -acosf(cosita);
}

// ===四捨五入(位指定)===
void Round(float* out, const int& num)
{
	float power = powf(10, num - 1);
	*out = *out * power;
	*out = roundf(*out);
	*out /= power;
}

// ===四捨五入(位指定)===
void Round(XMFLOAT3* out, const int& num)
{
	float power = powf(10, num - 1);
	out->x = out->x * power;
	out->x = roundf(out->x);
	out->x /= power;

	out->y = out->y * power;
	out->y = roundf(out->y);
	out->y /= power;

	out->z = out->z * power;
	out->z = roundf(out->z);
	out->z /= power;

}

// ===距離計算(3次元)===
void Length(float* out, const XMFLOAT3& vec1, const XMFLOAT3& vec2)
{
	XMFLOAT3 work;
	work.x = vec1.x - vec2.x;
	work.y = vec1.y - vec2.y;
	work.z = vec1.z - vec2.z;

	*out = sqrt(work.x * work.x + work.y * work.y + work.z * work.z);
}

// ===距離計算(2次元)===
void Length(float* out, const XMFLOAT2& vec1, const XMFLOAT2& vec2)
{
	XMFLOAT2 work;
	work.x = vec1.x - vec2.x;
	work.y = vec1.y - vec2.y;

	*out = sqrt(work.x * work.x + work.y * work.y);
}

// ===軸回転===
void TransformRotate(Transform* transform, const XMFLOAT3& axis, const float& rot)
{
	Quaternion q(transform->Pos);
	Quaternion::PositionRotate(&q, axis, rot);
	transform->Pos = XMFLOAT3(q.x, q.y, q.z);
	transform->Rot.x += fabs(axis.x) * rot;
	transform->Rot.y += fabs(axis.y) * rot;
	transform->Rot.z += fabs(axis.z) * rot;
}

// ===桁数算出(整数)===
void Digit(int* out, const int& num) {
	int count = 0;
	int work = num;
	if (work == 0) {
		++count;
		*out = count;
		return;
	}
		
	while (work != 0) {
		work /= 10;
		++count;
	}
	*out = count;
}
