#pragma once
// ===配置3Dオブジェクトベースクラス Object3DBase.h
// ---Authot kimura

// ===インクルード===
#include "main.h"
#include "Object.h"
#include "Model.h"
#include "Calc.h"
#include "ConstantBuffer.h"
#include "Shader.h"
#include <memory>

// ===クラス定義===
class Object3DBase : public Object
{
	// ---メンバ関数---
public:
	Object3DBase();				// コンストラクタ
	virtual ~Object3DBase();	// デストラクタ
	virtual void Update();		// 更新
	virtual void Draw();		// 描画

	void DrawShadow();			// 影の描画

	void SetRot(const XMFLOAT3& vRot);					// 回転()
	void SetRot(const Quaternion& qRot);				// 回転クォータニオンをセット
	void SetFront(const XMFLOAT3& front);				// オブジェクトの正面方向をセット
	XMFLOAT3 GetFront();								// 向いている方向を取得
	void SetPS(PixelShader* ps) { m_pPS = ps; }			// ピクセルシェーダーをセット
	void SetVS(VertexShader* vs) { m_pVS = vs; }		// 頂点シェーダ―をセット

	void virtual SetResource(std::string);				// リソースのパスをセット(シーン情報保存のため)

	Model* GetModel() { return m_Model; }				// 参照しているモデルのポインタを返す
	void UpdateMatrix();								// ワールドマトリクス更新

	virtual void ImGuiWindow() { ; }

protected:
	Model* m_Model;										// モデルのポインタ
	XMFLOAT3 m_vFront;									// 正面方向
	VertexShader* m_pVS;								// 頂点シェーダ―
	PixelShader* m_pPS;									// ピクセルシェーダー

	static std::shared_ptr<ConstantBuffer> m_pWVPBuffer;				// WVP行列用定数バッファ
	static int m_nObjCnt;
private:
	float CalcObjSize(XMFLOAT3*);
};