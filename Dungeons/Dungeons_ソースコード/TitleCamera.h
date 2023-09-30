// ===ゲームカメラクラス===
#pragma once
#include "Camera.h"
#include "Object.h"

class TitleCamera : public CameraBase
{
public:
	TitleCamera();
	void Init();
	void Update();
private:
};