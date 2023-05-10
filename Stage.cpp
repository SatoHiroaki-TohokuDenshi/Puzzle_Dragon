#include "Stage.h"
#include "Engine/Image.h"
#include "Engine/Input.h"

namespace {
	static const int SIZE = 40;	//�{�[���̒��a
	static const int LEFT = 60;	//�Ֆʂ̍��ӂ̈ʒu
	static const int TOP = 80;	//�Ֆʂ̏�ӂ̈ʒu
};



//�R���X�g���N�^
Stage::Stage(GameObject* parent)
	:GameObject(parent, "Stage"), hPict_{-1, -1, -1, -1, -1, -1},
	mousePos_(XMFLOAT3(0.0f,0.0f,0.0f)), selectX_(-1), selectY_(-1), selectColor_(-1)
{
	//field �������_���ȐF�Ŗ��߂�
	for (int h = 0; h < HEIGHT; h++)
	{
		for (int w = 0; w < WIDTH; w++)
		{
			field_[h][w] =(COLOR)(rand() % COLOR::NUM);
		}
	}
}

//�f�X�g���N�^
Stage::~Stage()
{
}

//������
void Stage::Initialize()
{
	// �t�@�C�����̔z��
	const char* fileName[] = {
	"ball0.png" ,
	"ball1.png" ,
	"ball2.png" ,
	"ball3.png" ,
	"ball4.png" ,
	"ball5.png" ,
	};

	//�摜�f�[�^�̃��[�h
	for (int type = 0; type < COLOR::NUM; type++)
	{
		hPict_[type] = Image::Load(fileName[type]);
		assert(hPict_[type] >= 0);
	}
}

//�X�V
void Stage::Update()
{
	//�}�E�X�̈ʒu�̎擾
	mousePos_ = Input::GetMousePosition();
	mousePos_.x -= 608;
	mousePos_.y -= 330;

	//X���W�̔���
	if (mousePos_.x >= 0) {
		selectX_ = (int)(mousePos_.x / 40);
	}
	else {
		selectX_ = -1;
	}
	//Y���W�̔���
	if (mousePos_.y >= 0) {
		selectY_ = (int)(mousePos_.y / 40);
	}
	else{
		selectY_ = -1;
	}
	//���ݎ擾���Ă���ʂ̑I��
	if (selectX_ >= 0 && selectX_ < WIDTH &&
		selectY_ >= 0 && selectY_ < HEIGHT) {
		selectColor_ = field_[selectY_][selectX_];
	}
}

//�`��
void Stage::Draw()
{
	//�ʂ̉摜�̕`��
	{
		Transform t;
		t.position_ = XMFLOAT3(0.0f, 0.0f, 0.0f);
		int color;
		for (int h = 0; h < HEIGHT; h++)
		{
			for (int w = 0; w < WIDTH; w++)
			{
				//t.position_ = XMFLOAT3(0.062f * w, 0.110f * h, 0);
				t.position_ = ConvDrawPos(w * 40, h * 40);
				color = field_[h][w];
				if (w == selectX_ && h == selectY_) {
					t.scale_ = XMFLOAT3(1.2f, 1.2f, 0.0f);
				}
				else {
					t.scale_ = XMFLOAT3(1.0f, 1.0f, 0.0f);
				}
				Image::SetTransform(hPict_[color], t);
				Image::Draw(hPict_[color]);
			}
		}
	}

	//���ݑI�����Ă���ʂ̕\��
	{
		Transform t;
		t.position_ = ConvDrawPos(mousePos_.x - 20, mousePos_.y - 20);
		if (selectX_ >= 0 && selectX_ < WIDTH &&
			selectY_ >= 0 && selectY_ < HEIGHT) {
			Image::SetTransform(hPict_[selectColor_], t);
			Image::Draw(hPict_[selectColor_]);
		}
	}
}

//�J��
void Stage::Release()
{
}

// �h�b�g���W����3D���W�ɕϊ�����֐�
XMFLOAT3 Stage::ConvDrawPos(float x, float y)
{
	XMFLOAT3 p;
	p.x = x / 40 * 0.062f;
	p.y = -y / 40 * 0.110f;
	p.z = 0;
	return p;
}