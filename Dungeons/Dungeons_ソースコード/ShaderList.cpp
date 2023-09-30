#include "ShaderList.h"

const char* VSResouceFile[] = {
	"data/shader/LineTestVS.cso",
	"data/shader/AssimpVertex.cso",
	"data/shader/SkinMeshVS.cso",
	"data/shader/InstancingVS.cso",
	"data/shader/OutLineVS.cso",
	"data/shader/ObjectVS.cso",
	"data/shader/DepthWriteVS.cso",
	"data/shader/ShadowVS.cso",
};

const char* PSResouceFile[] = {
	"data/shader/LineTestPS.cso",
	"data/shader/AssimpPixel.cso",
	"data/shader/LambertPS.cso",
	"data/shader/OutLinePS.cso",
	"data/shader/ToonPS.cso",
	"data/shader/TexturePS.cso",
	"data/shader/PhongPS.cso",
	"data/shader/DepthWritePS.cso",
	"data/shader/DepthShadowPS.cso",
	"data/shader/ObjectMask.cso",
	"data/shader/Behind.cso",
};

VertexShader* g_VSList[VS_MAX];
PixelShader* g_PSList[PS_MAX];

void InitShaderList()
{
	for (int i = 0; i < VS_MAX; ++i)
	{
		g_VSList[i] = new VertexShader();
		if (FAILED(g_VSList[i]->Load(VSResouceFile[i])))
			MessageBoxW(0, L"hlslì«Ç›çûÇ›é∏îs", nullptr, MB_OK);
	}

	for (int i = 0; i < PS_MAX; ++i)
	{
		g_PSList[i] = new PixelShader();
		if (FAILED(g_PSList[i]->Load(PSResouceFile[i])))
			MessageBoxW(0, L"hlslì«Ç›çûÇ›é∏îs", nullptr, MB_OK);
	}
}

void UninitShaderList()
{
	for (int i = 0; i < VS_MAX; ++i)
		delete g_VSList[i];

	for (int i = 0; i < PS_MAX; ++i)
		delete g_PSList[i];
}

VertexShader* GetVS(VS_KIND vskind)
{
	return g_VSList[vskind];
}

PixelShader* GetPS(PS_KIND pskind)
{
	return g_PSList[pskind];
}