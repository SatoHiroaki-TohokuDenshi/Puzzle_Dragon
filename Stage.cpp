#include "Stage.h"
#include "Engine/Image.h"
#include "Engine/Input.h"

namespace {
	static const int SIZE = 40;	//ボールの直径
	static const int LEFT = 60;	//盤面の左辺の位置
	static const int TOP = 80;	//盤面の上辺の位置
};



//コンストラクタ
Stage::Stage(GameObject* parent)
	:GameObject(parent, "Stage"), hPict_{-1, -1, -1, -1, -1, -1},state_(S_IDLE),
	mousePos_(Input::GetMousePosition()), selectX_(-1), selectY_(-1), selectColor_(NOCOLOR),
	eraseTime_(0)
{
	//field をランダムな色で埋める
	for (int h = 0; h < HEIGHT; h++) {
		for (int w = 0; w < WIDTH; w++) {
			auto& set = field_[h][w];
			set.color =(COLOR)(rand() % COLOR::NUM);
			set.x = (float)(w * 40);
			set.y = (float)(h * 40);

			//滑らかに動かす変数
			set.bx = (float)(w * 40);
			set.by = (float)(h * 40);
			set.rate = 1.0f;

			//消すための変数
			set.doErase = 0;
		}
	}
}

//デストラクタ
Stage::~Stage()
{
}

//初期化
void Stage::Initialize()
{
	// ファイル名の配列
	const char* fileName[] = {
	"ball0.png" , "ball1.png" , "ball2.png" ,
	"ball3.png" , "ball4.png" , "ball5.png" ,
	};

	//画像データのロード
	for (int type = 0; type < COLOR::NUM; type++)
	{
		hPict_[type] = Image::Load(fileName[type]);
		assert(hPict_[type] >= 0);
	}
}

//更新
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

//描画
void Stage::Draw()
{
	//玉の画像の描画

	for (int h = 0; h < HEIGHT; h++) {
		for (int w = 0; w < WIDTH; w++) {
			int color = field_[h][w].color;
			Transform t;
			t.position_ = ConvDrawPos(field_[h][w].x, field_[h][w].y);
			if (field_[h][w].doErase > 0) {
				t.scale_ = XMFLOAT3(0.5f, 0.5f, 0.0f);
			}
			else if (w == selectX_ && h == selectY_) {
				t.scale_ = XMFLOAT3(1.2f, 1.2f, 0.0f);
			}
			else {
				t.scale_ = XMFLOAT3(1.0f, 1.0f, 0.0f);
			}
			Image::SetTransform(hPict_[color], t);
			Image::Draw(hPict_[color]);
		}
	}

	//現在選択している玉の表示
	if (selectColor_ != COLOR::NOCOLOR) {
		Transform t;
		t.position_ = ConvDrawPos(mousePos_.x - 20, mousePos_.y - 20);
		Image::SetTransform(hPict_[selectColor_], t);
		Image::Draw(hPict_[selectColor_]);
	}
}

//開放
void Stage::Release()
{
}

void Stage::UpdateIdle() {
	//マウスの左クリックしたら
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
	
	//範囲外の処理
	if (selectY_ < 0) selectY_ = 0;
	if (selectX_ < 0) selectX_ = 0;
	if (selectY_ >= HEIGHT) selectY_ = HEIGHT - 1;
	if (selectX_ >= WIDTH) selectX_ = WIDTH - 1;


	//移動したかどうかを判定
	if (selectX_ != lastX || selectY_ != lastY) {
		std::swap(field_[selectY_][selectX_], field_[lastY][lastX]);

		auto& s = field_[selectY_][selectX_];
		auto& la = field_[lastY][lastX];
		
		//滑らかに動かす準備
#if 0
		//方法①
		s.counter = 10;
		la.counter = 10;
#else
		//方法②
		s.bx = s.x;
		s.by = s.y;
		s.rate = 0.0f;
		la.bx = la.x;
		la.by = la.y;
		la.rate = 0.0f;
#endif
	}

	//滑らかに動かす演出
	for (int h = 0; h < HEIGHT; h++) {
		for (int w = 0; w < WIDTH; w++) {
			auto& b = field_[h][w];
			if (b.rate < 1.0f) {
				b.rate += 0.1f;
				if (b.rate > 1.0f)
					b.rate = 1.0f;
				b.x = GetRateValue(b.bx, (float)(w * 40), b.rate);
				b.y = GetRateValue(b.by, (float)(h * 40), b.rate);
			}
		}
	}

	//左クリックを離したら
	if (Input::IsMouseButtonUp(0)) {
		eraseTime_ = 30;
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
	PrepareFall();
	eraseTime_--;
	if (eraseTime_ <= 0) {
		state_ = STATE::S_FALL;
	}
}

void Stage::UpdateFall() {
	for (int h = 0; h < HEIGHT; h++) {
		for (int w = 0; w < WIDTH; w++) {
			auto& b = field_[h][w];
			if (b.rate < 1.0f) {
				b.rate += 0.1f;
				if (b.rate > 1.0f)
					b.rate = 1.0f;
				b.x = GetRateValue(b.bx, (float)(w * 40), b.rate);
				b.y = GetRateValue(b.by, (float)(h * 40), b.rate);
			}
		}
	}
}

void Stage::UpdateAttack() {

}

// ドット座標から3D座標に変換する関数
XMFLOAT3 Stage::ConvDrawPos(float x, float y) {
	XMFLOAT3 p;
	p.x = x / 40 * 0.062f;
	p.y =- y / 40 * 0.110f;
	p.z = 0;
	return p;
}

//選択している玉を算出
void Stage::CalcMouseSelect() {
	//マウスの位置の取得
	mousePos_ = Input::GetMousePosition();
	mousePos_.x -= 608;
	mousePos_.y -= 330;

	//X座標の判定
	if (mousePos_.x >= 0) {
		selectX_ = (int)(mousePos_.x / 40);
	}
	else {
		selectX_ = -1;
	}
	//Y座標の判定
	if (mousePos_.y >= 0) {
		selectY_ = (int)(mousePos_.y / 40);
	}
	else {
		selectY_ = -1;
	}
	//現在取得している玉の選択
	if (selectX_ >= 0 && selectX_ < WIDTH &&
		selectY_ >= 0 && selectY_ < HEIGHT) {
		selectColor_ = field_[selectY_][selectX_].color;
	}
}

//滑らかに動かすための計算
float Stage::GetRateValue(float begin, float end, float rate) {
	return (float)((end - begin) * rate + begin);
}

//消えるかどうかをチェック
bool Stage::CheckErase() {
	bool IsErase = false;
	//横の確認
	for (int h = 0; h < HEIGHT; h++) {
		for (int w = 0; w < WIDTH - 2; w++) {
			if (field_[h][w].color == field_[h][w + 1].color &&
				field_[h][w].color == field_[h][w + 2].color) {
				field_[h][w].doErase = 1;
				field_[h][w + 1].doErase = 1;
				field_[h][w + 2].doErase = 1;
				IsErase = true;
			}
		}
	}
	//縦の確認
	for (int w = 0; w < WIDTH; w++) {
		for (int h = 0; h < HEIGHT - 2; h++) {
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

//落ちるための準備
void Stage::PrepareFall() {
	for (int w = 0; w < WIDTH; w++) {
		int hole = 0;
		for (int h = HEIGHT - 1; h >= 0; h--) {
			if (field_[h][w].doErase > 0) {
				hole += 1;
			}
			else {
				//field_[h][w]をhole分だけ落下させる
				field_[h + hole][w] = field_[h][w];
				field_[h + hole][w].by = field_[h][w].y;
				field_[h + hole][w].bx = field_[h][w].x;
				field_[h + hole][w].rate = 0.0f;
			}
		}
		//玉を補充する
		for (int h = 0; h < hole; h++) {
			auto& set = field_[h][w];
			set.color = (COLOR)(rand() % COLOR::NUM);
		}
	}
}
