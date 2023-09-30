// CPU�������Ă���l(���f���̍��W)
struct VS_IN
{
	float3 pos : POSITION0;
	float3 normal : NORMAL0;
	float2 uv : TEXCOORD0;
	float4 weight : WEIGHT0;
	uint4 index : INDEX0;
};

// �s�N�Z���V�F�[�_�[�ɓn���l
struct VS_OUT
{
	float4 pos : SV_POSITION;
	float3 normal : NORMAL0;
	float2 uv : TEXCOORD0;
	float4 wPos : TEXCOORD1;
	float4 sunPos : TEXCOORD2;				// ���z���猩���ʒu
};

// �萔�o�b�t�@
cbuffer Matrix : register(b0)
{
	float4x4 world;
	float4x4 view;
	float4x4 proj;
};

// ���z���猩�����_
cbuffer SunCamera : register(b2)
{
	float4x4 sunView;
	float4x4 sunproj;
}

// ���̒萔�o�b�t�@
cbuffer BoneMatrix : register(b3)
{
	float4x4 bone[64];
};

VS_OUT main(VS_IN vin)
{
	VS_OUT vout;
	vout.pos = float4(vin.pos, 1.0f);
	vout.normal = vin.normal;
	 //�������������W�͂����ȍ��̉e�����󂯂Ă���
	 //�����Ƃ̍s��Ɋ��������߂āA�ŏI�I�ɑS������
	float4x4 anime = bone[vin.index.x] * vin.weight.x +
		bone[vin.index.y] * vin.weight.y +
		bone[vin.index.z] * vin.weight.z +
		bone[vin.index.w] * vin.weight.w;
	vout.pos = mul(vout.pos, anime);
	vout.normal = mul(vout.normal, (float3x3)anime);

	vout.pos = mul(vout.pos, world);
	vout.wPos = vout.pos;
	vout.pos = mul(vout.pos, view);
	vout.pos = mul(vout.pos, proj);

	// ���C���J�����Ƃ͕ʂɁA���z�p�̃J�����Œ��_���v�Z
	float4 sunPos = vout.wPos;
	sunPos = mul(sunPos, sunView);
	sunPos = mul(sunPos, sunproj);

	vout.sunPos = sunPos;

	vout.uv = vin.uv;
	vout.normal = mul(vout.normal, (float3x3)world);

	return vout;
}