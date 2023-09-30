#pragma once
// ===2D�A�j���[�V�����x�[�X�N���X��`===
// 2D�A�j���[�V�����x�[�X�N���X���`
// Author:	Kimura
// ==============================

// ===�C���N���[�h===
#include "Object2DBase.h"

// ===�N���X��`===
class Animation2DBase : public Object2DBase
{
	// ---�����o�֐�---
public:
	Animation2DBase();
	~Animation2DBase();

	virtual void Update();		// �X�V

	bool GetEnd();				// �Đ��I���������ǂ���
	int GetFrame();				// ���݃t���[���擾
	void SetSpeed(float);		// �X�s�[�h�ύX
	void SetMaxAnim(int);		// �A�j������
	void SetStartFrame(int);	// �A�j���[�V�����̊J�n�t���[��
	void SetnEnd();				// �A�j���[�V�����I��

	// ---�����o�ϐ�---
protected:
	int m_nFrame;		// �Đ����t���[����
	int m_nOld;			// �O�t���[��
	int m_nStart;		// �A�j���[�V�����̊J�nuv
	int m_nAnimCnt;		// �A�j���[�V�������閇��
	float m_fSpd;		// �Đ��X�s�[�h(�v�Z�p)
	float m_fdefault;	// �X�s�[�h���Z�b�g�p

	bool m_Loop;		// �Đ���������
	bool m_bEnd;		// �Đ��I���t���O
	bool m_bDestroy;	// �Đ��I���Ŕj�����邩�ǂ���
};
