#pragma once
#include "main.h"
#include "Model.h"
#include "ConstantBuffer.h"
#include "Calc.h"

class TestModel {
public:
	TestModel();
	~TestModel();

	void Update();
	void Draw();
private:
	XMFLOAT3 m_vPos;
	XMFLOAT3 m_vScale;
	XMFLOAT3 m_vRotate;
	XMFLOAT3 m_vFront;
	Quaternion m_qRot;

	XMFLOAT4X4 m_mWorld;

	Model* m_pModel;
	ConstantBuffer* m_pCBuffer;
	ConstantBuffer* m_pCBlight;

	int mTime;
};