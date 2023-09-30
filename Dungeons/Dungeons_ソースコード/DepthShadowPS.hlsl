struct PS_IN
{
	// �X�N���[���̍��W-1�`1��w�����Ŋ���Ɛ������v�Z�ł���
	// x' = x/w, y' = y/w, z' = z/w
	// �uSV_POSITION�̃p�����[�^�v�͒��_�V�F�[�_�\����
	// �s�N�Z���V�F�[�_�[�ɓn�����r���ŏ���Ɍv�Z����Ă���
	float4 pos : SV_POSITION;
	float3 normal : NORMAL0;
	float2 uv : TEXCOORD0;
	float4 wPos : TEXCOORD1;
	float4 sunPos : TEXCOORD2;
};

// �e�N�X�`��
// register(t0)�̓��f���ɒ���t����f�t�H���g�̃e�N�X�`��
Texture2D sunViewTex : register(t2);
SamplerState samp : register(s0);

float4 main(PS_IN pin) : SV_TARGET
{
	// ���W��w�Ŋ���
	float2 sunUV = pin.sunPos.xy / pin.sunPos.w;
	// ��ʂ̍��W-1�`1���e�N�X�`���̍��W0�`1�ɕϊ�
	sunUV = sunUV * 0.5f + 0.5f;
	// ��ʂ�Y���W�͉������Ɍ������đ����邪�A
	// �e�N�X�`����Y���W�͏ォ�牺�Ɍ������đ�����̂Ŕ��]
	sunUV.y = 1.0f - sunUV.y;


	// ���s�����������񂾃e�N�X�`������A�������擾
	float sunDepth = sunViewTex.Sample(samp, sunUV).r;

	// �V���h�E�A�N�l(�}�b�n�o���h)�΍�
	// �`�悷��s�N�Z���̉��s�ƁA�e�N�X�`���̉��s����v���Ă���ꍇ�A
	// �e�N�X�`���ɏ������܂ꂽ�f�[�^�Ƃ̍��ق��������ĉe���ł��Ȃ�������
	// �e���ł��Ă��܂��B�e�N�X�`���ɏ������܂ꂽ�����͎��ۂ͏��������A�ƒu�����Ƃŉ���ł���B
	sunDepth += 0.001f;		// �����͓K�X����

	// ���C���J�����ŕ\�����悤�Ƃ��Ă���s�N�Z���̑��z���猩���������擾
	float pixelDepth = pin.sunPos.z / pin.sunPos.w;


	float4 color = float4(1.0f, 1.0f, 1.0f, 1.0f);
	// ���݂̃s�N�Z���̉��s�����e�N�X�`���ɏ������܂�Ă��鉜�s�����������ꍇ�A�e�Ɣ��肷��B
	// ���s�N�Z���V�F�[�_�[�ł̓s�N�Z�����ƂɌv�Z����̂ŁA���܂�if���͎g��Ȃ������悢
	if (sunDepth < pixelDepth)
	{
		color.rgb = 0.2f;	// �e�F
	}

	return color;
}