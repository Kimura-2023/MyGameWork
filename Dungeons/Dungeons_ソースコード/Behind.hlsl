struct PS_IN
{
	float4 pos : SV_POSITION;
	float3 normal : NORMAL0;
	float2 uv : TEXCOORD0;
	float4 wPos : TEXCOORD1;
	float cPos : TEXCOORD3;
};

cbuffer Line : register(b2)
{
	float4 work;
};

SamplerState samp : register(s0);
Texture2D modelTex : register(t0);
Texture2D maskTex : register(t3);		// ���߂��邽�߂̃e�N�X�`��

// ---�f�B�U�p�^�[��---
static const int pattern[4][4] = {
	{ 0,32, 8,40},
	{48,16,56,24},
	{12,44, 4,36},
	{60,28,52,20},
};

float4 main(PS_IN pin) : SV_TARGET
{
	float4 color = work;

	float2 texuv = pin.pos.xy;
	texuv.x /= 1280.0f;
	texuv.y /= 720.0f;

	int x = (int)fmod(pin.pos.x, 4.0f);
	int y = (int)fmod(pin.pos.y, 4.0f);
	int dither = pattern[y][x];

	clip(dither - maskTex.Sample(samp, texuv).r * 30);			// �ړ��I�u�W�F�N�g���d�Ȃ��Ă�����f�B�U�p�^�[���œ��߂���
	color.rgb += maskTex.Sample(samp, texuv).r;
	return color;
}