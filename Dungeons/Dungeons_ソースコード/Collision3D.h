#pragma once

// ===�����蔻��N���X��`===
// �����蔻��N���X���`
// Author:	Kimura
// ==============================

// ===�C���N���[�h===
#include "main.h"
#include "Object3DBase.h"
#include "Object2DBase.h"
#include "CollisionBase.h"
#include <vector>

class CollisionBase;				// �N���X�����邱�Ƃ��`

// ===�萔�E�}�N����`===
using COLDIT = std::vector<CollisionBase*>::iterator;

// ===�N���X��`===
class Collision3D
{
	// ---�����o�֐�---
public:
	Collision3D(Object*);												// �R���X�g���N�^
	~Collision3D();														// �f�X�g���N�^

	void Update();														// �X�V����
	void UpdateMatrix();												// �ϊ��s��X�V
	void Draw();														// �f�o�b�O�\��
	virtual void OnCollision(Object*) = 0;								// �����������̏���(�����蔻�肪�K�v�ȃI�u�W�F�N�g���ƂŌp�����A�֐����쐬����)
	virtual void ReflectMovement() { ; }								// �߂��̌v�Z

	// ---�����蔻��Z�b�g�֐�---
	void AddCollisionAABB();												// �����蔻�胊�X�g��AABB�ǉ�
	void AddCollisionAABB(const XMFLOAT3& size, const XMFLOAT3& offset);	// �����蔻�胊�X�g��AABB�ǉ�(�����ŃT�C�Y�A���S���W����̂�����w��)
	void AddCollisionLine();												// �����蔻��������X�g�ɓ����蔻�������ǉ�
	void AddCollisionLine(const XMFLOAT3&, const XMFLOAT3&);				// �����蔻��������X�g�ɓ����蔻�������ǉ�(�����Ő����̏I�_�A���S���W����̂�����w��)
	void AddCollisionSphere();												// �����蔻�苅���X�g�ɓ����蔻�苅��ǉ�
	void AddCollisionSphere(const XMFLOAT3&, const float&);					// �����蔻�苅���X�g�ɓ����蔻�苅��ǉ�(�����Œ��S���W����̂���A���a���w��)
	void AddCollisionOBB();													// �����蔻�胊�X�g��OBB�ǉ�
	void AddCollisionOBB(const XMFLOAT3& offset, const XMFLOAT3& size);		// �����蔻�胊�X�g��OBB�ǉ�(�����ŃT�C�Y�A���S���W����̂�����w��)

	void SwitchVisible();											// �f�o�b�O�\���L���؂�ւ�
	void SetCollision(bool);										// �����蔻��`�F�b�N�L�����Z�b�g
	void SetCollider(bool);											// �߂荞�ݗʂ�߂����ǂ�����ݒ�

//	void SetNode(DWORD* pnode) { m_pNodeindex = *pnode; }			// ������ԃm�[�h���Z�b�g

	// ---�擾�֐�---
	Object* GetFollowObject() { return m_pObj; }								// �Q�ƃI�u�W�F�N�g�擾
	std::vector<CollisionBase*> GetCollisionData() { return m_pColList; }		// �����蔻�胊�X�g���擾
	bool CheckCollision(Collision3D*);								// �q�b�g�m�F

//	DWORD* GetCurrentNode() { return &m_pNodeindex; }				// ���݂̏�����ԃm�[�h���擾

	XMFLOAT3& GetCenter() { return m_vCenter; }							// ���S���W
	XMFLOAT3& GetSize() { return m_vSize; }
	XMFLOAT4X4& GetWorld() { return m_mWorld; }							// ���[���h�ϊ��s��
	bool& GetEnable() { return m_bEnableflg; }							// �����蔻�����邩�ǂ����̃t���O�擾
	bool& GetCollider() { return m_bCollider; }							// �߂��̌v�Z�����邩�ǂ����擾

	void CalcMovement(XMFLOAT3* movement);								// �߂荞�݂�߂��ʂ��v�Z
	XMFLOAT3& GetMovement();											// �߂��ʂ��擾
	void SetHitPos(XMFLOAT3* hitpos);									// �Փ˂������W��ݒ�
	XMFLOAT3& GetHitPos();												// �Փ˂������W���擾
protected:

	// ---�����o�ϐ�---
private:
	std::vector<CollisionBase*> m_pColList;								// �����蔻�胊�X�g(�x�[�X����h�������Đ�������)
	XMFLOAT3 m_vCenter;													// ���S���W
	XMFLOAT3 m_vSize;													// �����蔻��T�C�Y
	XMFLOAT3 m_vRot;													// ��]��
	XMFLOAT4X4 m_mWorld;												// ���[���h�}�g���b�N�X
	XMFLOAT3 m_Movement;												// �Փ˂��Ă߂荞�񂾍ۂɖ߂���
	XMFLOAT3 m_HitPos;													// �Փ˂����ʒu

	bool m_bEnableflg;													// �����蔻�����邩���Ȃ����̃t���O(m_bEnable�ɂ��悤�Ǝv���Ă�����p�����ăI�u�W�F�N�g�𐶐�����֌W��Object3Dbase�œ������̂��`���Ă�������m_bEnableflg��
	bool m_bCollider;													// �߂��v�Z���s��Ȃ����̂�true

	Object* m_pObj;														// �Q�Ƃ��Ă���I�u�W�F�N�g
	//DWORD m_pNodeindex;												// �������Ă���m�[�h(�I�N�c���[�X�V�̂��߂ɂǂ̃m�[�h�ɏ������Ă��邩��\��)
};