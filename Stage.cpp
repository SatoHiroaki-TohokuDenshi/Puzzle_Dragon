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
	mousePos_(Input::GetMousePosition()), selectX_(-1), selectY_(-1), selectColor_(NOCOLOR)
{
	//field �������_���ȐF�Ŗ��߂�
	for (int h = 0; h < HEIGHT; h++) {
		for (int w = 0; w < WIDTH; w++) {
			auto& set = field_[h][w];
			set.color =(COLOR)(rand() % COLOR::NUM);
			set.x = w * 40;
			set.y = h * 40;
#if 0
			//���炩�ɓ��������@�@
			set.counter = 0;
#else
			//���炩�ɓ��������@�A
			set.bx = w * 40;
			set.by = h * 40;
			set.rate = 1.0f;
#endif
			set.doErase = 0;
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
	"ball0.png" , "ball1.png" , "ball2.png" ,
	"ball3.png" , "ball4.png" , "ball5.png" ,
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
			else if (field_[h][w].doErase > 0) {
				t.scale_ = XMFLOAT3(1.5f, 1.5f, 0.0f);
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
		std::swap(field_[selectY_][selectX_], field_[lastY][lastX]);

		auto& s = field_[selectY_][selectX_];
		auto& la = field_[lastY][lastX];
#if 0
		//���@�@
		s.counter = 10;
		la.counter = 10;
#else
		//���@�A

		s.bx = s.x;
		s.by = s.y;
		s.rate = 0.0f;
		la.bx = la.x;
		la.by = la.y;
		la.rate = 0.0f;
#endif
	}
	//���炩�ɓ�����
	for (int h = 0; h < HEIGHT; h++) {
		for (int w = 0; w < WIDTH; w++) {
			auto& b = field_[h][w];
#if 0
			//���@�@
			if (b.counter > 0) {
				b.x += (w * 40 - b.x) / b.counter;
				b.y += (h * 40 - b.y) / b.counter;
				b.counter--;
			}
#else
			//���@�A
			if (b.rate < 1.0f) {
				b.rate += 0.1f;
				if (b.rate > 1.0f)
					b.rate = 1.0f;
				b.x = GetRateValue(b.bx, w * 40, b.rate);
				b.y = GetRateValue(b.by, h * 40, b.rate);
			}
#endif
		}
	}

	if (Input::IsMouseButtonUp(0)) {
		selectColor_ = COLOR::NOCOLOR;
		if (CheckErase()) {
			state_ = STATE::S_ERASE;
		}
		else {
			state_ = STATE::S_IDLE;
		}
	}
}
void Stage::UpdateErase() {

}
void Stage::UpdateFall() {

}
void Stage::UpdateAttack() {

}

// �h�b�g���W����3D���W�ɕϊ�����֐�
XMFLOAT3 Stage::ConvDrawPos(float x, float y) {
	XMFLOAT3 p;
	p.x = x / 40 * 0.062f;
	p.y =- y / 40 * 0.110f;
	p.z = 0;
	return p;
}

//�I�����Ă���ʂ��Z�o
void Stage::CalcMouseSelect() {
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

//���炩�ɓ��������߂̌v�Z
float Stage::GetRateValue(float begin, float end, float rate) {
	return (float)((end - begin) * rate + begin);
}

//�����邩�ǂ������`�F�b�N
bool Stage::CheckErase() {
	bool IsErase = false;
	//���̊m�F
	for (int h = 0; h < HEIGHT; h++) {
		for (int w = 0; w < WIDTH - 3; w++) {
			if (field_[h][w].color == field_[h][w + 1].color &&
				field_[h][w].color == field_[h][w + 2].color) {
				field_[h][w].doErase = 1;
				field_[h][w + 1].doErase = 1;
				field_[h][w + 2].doErase = 1;
				IsErase = true;
			}
		}
	}
	//�c�̊m�F
	for (int w = 0; w < WIDTH; w++) {
		for (int h = 0; h < HEIGHT - 3; h++) {
			if (field_[h][w].color == field_[h + 1][w].color &&
				field_[h][w].color == field_[h + 2][w].color) {
				field_[h][w].doErase = 1;
				field_[h + 1][w].doErase = 1;
				field_[h + 2][w].doErase = 1;
				IsErase = true;
			}
		}
	}
	return IsErase;
}
