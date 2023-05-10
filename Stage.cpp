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
	:GameObject(parent, "Stage"), hPict_{-1, -1, -1, -1, -1, -1},state_(S_IDLE),
	mousePos_(XMFLOAT3(0.0f,0.0f,0.0f)), selectX_(-1), selectY_(-1), selectColor_(NOCOLOR)
{
	//field �������_���ȐF�Ŗ��߂�
	for (int h = 0; h < HEIGHT; h++) {
		for (int w = 0; w < WIDTH; w++) {
			field_[h][w].color =(COLOR)(rand() % COLOR::NUM);
			field_[h][w].x = w * 40;
			field_[h][w].y = h * 40;
#if 0
			//���炩�ɓ��������@�@
			field_[selectY_][selectX_].counter = 0;
#else
			//���炩�ɓ��������@�A
			field_[h][w].bx = w * 40;
			field_[h][w].by = h * 40;
			field_[h][w].rate = 1.0f;
#endif
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
	switch (state_) {
	case STATE::S_IDLE:
		UpdateIdle();
		break;
	case STATE::S_MOVE:
		UpdateMove();
		break;
	case STATE::S_ERASE:
		UpdateErase();
		break;
	case STATE::S_FALL:
		UpdateFall();
		break;
	case STATE::S_ATTACK:
		UpdateAttack();
		break;
	default:
		break;
	}
}

//�`��
void Stage::Draw()
{
	//�ʂ̉摜�̕`��

	for (int h = 0; h < HEIGHT; h++) {
		for (int w = 0; w < WIDTH; w++) {
			int color = field_[h][w].color;
			Transform t;
			t.position_ = ConvDrawPos(field_[h][w].x, field_[h][w].y);
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

	//���ݑI�����Ă���ʂ̕\��
	if (selectColor_ != COLOR::NOCOLOR) {
		Transform t;
		t.position_ = ConvDrawPos(mousePos_.x - 20, mousePos_.y - 20);
		Image::SetTransform(hPict_[selectColor_], t);
		Image::Draw(hPict_[selectColor_]);
	}
}

//�J��
void Stage::Release()
{
}

void Stage::UpdateIdle() {
	//�}�E�X�̍��N���b�N������
	if (Input::IsMouseButtonDown(0)) {
		CalcMouseSelect();
		if (selectX_ >= 0 && selectX_ < WIDTH &&
			selectY_ >= 0 && selectY_ < HEIGHT) {
			state_ = STATE::S_MOVE;
		}
	}
}

void Stage::UpdateMove() {
	int lastX = selectX_;
	int lastY = selectY_;

	CalcMouseSelect();
	
	if (selectY_ < 0) selectY_ = 0;
	if (selectX_ < 0) selectX_ = 0;
	if (selectY_ >= HEIGHT) selectY_ = HEIGHT - 1;
	if (selectX_ >= WIDTH) selectX_ = WIDTH - 1;

	if (selectX_ != lastX || selectY_ != lastY) {
		auto tmp = field_[selectY_][selectX_];
		field_[selectY_][selectX_] = field_[lastY][lastX];
		field_[lastY][lastX] = tmp;
#if 0
		field_[selectY_][selectX_].counter = 10;
		field_[lastY][lastX].counter = 10;
#else
		field_[selectY_][selectX_].bx = field_[selectY_][selectX_].x;
		field_[selectY_][selectX_].by = field_[selectY_][selectX_].y;
		field_[selectY_][selectX_].rate = 0.0f;
		field_[lastY][lastX].bx = field_[lastY][lastX].x;
		field_[lastY][lastX].by = field_[lastY][lastX].y;
		field_[lastY][lastX].rate = 0.0f;
#endif
	}
	//���炩�ɓ�����
	for (int h = 0; h < HEIGHT; h++) {
		for (int w = 0; w < WIDTH; w++) {
#if 0
			//���@�@
			if (field_[h][w].counter > 0) {
				field_[h][w].x += (w * 40 - field_[h][w].x) / field_[h][w].counter;
				field_[h][w].y += (h * 40 - field_[h][w].y) / field_[h][w].counter;
				field_[h][w].counter--;
			}
#else
			//���@�A
			if (field_[h][w].rate < 1.0f) {
				field_[h][w].rate += 0.1f;
				if (field_[h][w].rate > 1.0f)
					field_[h][w].rate = 1.0f;
				field_[h][w].x = (w * 40 - field_[h][w].bx) *
					field_[h][w].rate + field_[h][w].bx;
				field_[h][w].y = (h * 40 - field_[h][w].by) *
					field_[h][w].rate + field_[h][w].by;
			}
#endif
		}
	}

	if (Input::IsMouseButtonUp(0)) {
//		state_ = STATE::S_ERASE;
		state_ = STATE::S_IDLE;
	}
}
void Stage::UpdateErase() {

}
void Stage::UpdateFall() {

}
void Stage::UpdateAttack() {

}

// �h�b�g���W����3D���W�ɕϊ�����֐�
XMFLOAT3 Stage::ConvDrawPos(float x, float y)
{
	XMFLOAT3 p;
	p.x = x / 40 * 0.062f;
	p.y =- y / 40 * 0.110f;
	p.z = 0;
	return p;
}

void Stage::CalcMouseSelect()
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
	else {
		selectY_ = -1;
	}
	//���ݎ擾���Ă���ʂ̑I��
	if (selectX_ >= 0 && selectX_ < WIDTH &&
		selectY_ >= 0 && selectY_ < HEIGHT) {
		selectColor_ = field_[selectY_][selectX_].color;
	}
}
