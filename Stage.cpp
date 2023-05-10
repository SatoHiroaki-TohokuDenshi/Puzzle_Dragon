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
	:GameObject(parent, "Stage"), hPict_{-1, -1, -1, -1, -1, -1},
	mousePos_(XMFLOAT3(0.0f,0.0f,0.0f)), selectX_(-1), selectY_(-1), selectColor_(-1)
{
	//field をランダムな色で埋める
	for (int h = 0; h < HEIGHT; h++)
	{
		for (int w = 0; w < WIDTH; w++)
		{
			field_[h][w] =(COLOR)(rand() % COLOR::NUM);
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
	"ball0.png" ,
	"ball1.png" ,
	"ball2.png" ,
	"ball3.png" ,
	"ball4.png" ,
	"ball5.png" ,
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
	else{
		selectY_ = -1;
	}
	//現在取得している玉の選択
	if (selectX_ >= 0 && selectX_ < WIDTH &&
		selectY_ >= 0 && selectY_ < HEIGHT) {
		selectColor_ = field_[selectY_][selectX_];
	}
}

//描画
void Stage::Draw()
{
	//玉の画像の描画
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

	//現在選択している玉の表示
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

//開放
void Stage::Release()
{
}

// ドット座標から3D座標に変換する関数
XMFLOAT3 Stage::ConvDrawPos(float x, float y)
{
	XMFLOAT3 p;
	p.x = x / 40 * 0.062f;
	p.y = -y / 40 * 0.110f;
	p.z = 0;
	return p;
}