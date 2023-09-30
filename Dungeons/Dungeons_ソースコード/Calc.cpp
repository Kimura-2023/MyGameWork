// ===�e��v�Z�p�֐��A�N���X����===
// 3D�v�Z�ɕK�v�Ȋ֐��A�N���X������
// Author : Kimura
// ================================

// ===�C���N���[�h===
#include "Calc.h"

// ===��]�}�g���b�N�X����p�x�����߂�===
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

// ===�n���ꂽ�x�N�g���̒P�ʃx�N�g�����Z�o===
XMFLOAT3 CalcUnitVector(const XMFLOAT3& vec)
{
	XMVECTOR Unit = XMLoadFloat3(&vec);
	XMFLOAT3 ans;
	float length = sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
	Unit /= length;

	XMStoreFloat3(&ans,Unit);

	return ans;
}

// ===�x�N�g�����K��(�O����)===
void Normalize(XMFLOAT3* vec)
{
	XMVECTOR ans = XMLoadFloat3(vec);
	// �x�N�g���̑傫�������߂�
	float length = sqrt(vec->x * vec->x + vec->y * vec->y + vec->z * vec->z);
	ans /= length;
	XMStoreFloat3(vec, ans);
}

// ===�x�N�g�����K��(�񎟌�)===
void Normalize(XMFLOAT2* vec)
{
	// �x�N�g���̑傫�������߂�
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

// ===����(2����)===
void Dot(float* out, const XMFLOAT2& vec1, const XMFLOAT2& vec2)
{
	*out = vec1.x * vec2.x + vec1.y * vec2.y;
}

// ===����(3����)===
void Dot(float* out, const XMFLOAT3& vec1, const XMFLOAT3& vec2)
{
	*out = vec1.x * vec2.x + vec1.y * vec2.y + vec1.z * vec2.z;
}

// ===2�̃x�N�g������Ȃ��p�x(���W�A���l)�����߂�===
void CalcAngle(float* angle, const XMFLOAT3& vec1, const XMFLOAT3& vec2)
{
	float dot = vec1.x * vec2.x + vec1.y * vec2.y + vec1.z * vec2.z;

	// �x�N�g���̑傫�������߂�
	float v1 = sqrtf(vec1.x*vec1.x + vec1.y*vec1.y + vec1.z*vec1.z);
	float v2 = sqrtf(vec2.x*vec2.x + vec2.y*vec2.y + vec2.z*vec2.z);

	float cosita = dot / (v1 * v2);

	*angle = -acosf(cosita);
}

void CalcAngle(float* angle, const XMFLOAT2& vec1, const XMFLOAT2& vec2)
{
	float dot = vec1.x * vec2.x + vec1.y * vec2.y;

	// �x�N�g���̑傫�������߂�
	float v1 = sqrtf(vec1.x*vec1.x + vec1.y*vec1.y);
	float v2 = sqrtf(vec2.x*vec2.x + vec2.y*vec2.y);

	float cosita = dot / (v1 * v2);
	*angle = -acosf(cosita);
}

// ===�l�̌ܓ�(�ʎw��)===
void Round(float* out, const int& num)
{
	float power = powf(10, num - 1);
	*out = *out * power;
	*out = roundf(*out);
	*out /= power;
}

// ===�l�̌ܓ�(�ʎw��)===
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

// ===�����v�Z(3����)===
void Length(float* out, const XMFLOAT3& vec1, const XMFLOAT3& vec2)
{
	XMFLOAT3 work;
	work.x = vec1.x - vec2.x;
	work.y = vec1.y - vec2.y;
	work.z = vec1.z - vec2.z;

	*out = sqrt(work.x * work.x + work.y * work.y + work.z * work.z);
}

// ===�����v�Z(2����)===
void Length(float* out, const XMFLOAT2& vec1, const XMFLOAT2& vec2)
{
	XMFLOAT2 work;
	work.x = vec1.x - vec2.x;
	work.y = vec1.y - vec2.y;

	*out = sqrt(work.x * work.x + work.y * work.y);
}

// ===����]===
void TransformRotate(Transform* transform, const XMFLOAT3& axis, const float& rot)
{
	Quaternion q(transform->Pos);
	Quaternion::PositionRotate(&q, axis, rot);
	transform->Pos = XMFLOAT3(q.x, q.y, q.z);
	transform->Rot.x += fabs(axis.x) * rot;
	transform->Rot.y += fabs(axis.y) * rot;
	transform->Rot.z += fabs(axis.z) * rot;
}

// ===�����Z�o(����)===
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
