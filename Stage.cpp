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
	mousePos_(XMFLOAT3(0.0f,0.0f,0.0f)), selectX_(-1), selectY_(-1)
{
	//field を赤で埋める
	for (int h = 0; h < HEIGHT; h++)
	{
		for (int w = 0; w < WIDTH; w++)
		{
			field_[h][w] = COLOR::RED;
		}
	}
	field_[2][3] = COLOR::BLUE;
}

//デストラクタ
Stage::~Stage()
{
}

//初期化
void Stage::Initialize()
{
	const char* fileName[] = {
	"ball0.png" ,
	"ball1.png" ,
	"ball2.png" ,
	"ball3.png" ,
	"ball4.png" ,
	"ball5.png" ,
	};
	//画像データのロード
	//モデルデータのロード
	for (int type = 0; type < COLOR::NUM; type++)
	{
		hPict_[type] = Image::Load(fileName[type]);
		assert(hPict_[type] >= 0);
	}
}

//更新
void Stage::Update()
{
	mousePos_ = Input::GetMousePosition();
	mousePos_.x -= 608;
	mousePos_.y -= 330;

	if (mousePos_.x >= 0) {
		selectX_ = (int)(mousePos_.x / 40);
	}
	else {
		selectX_ = -1;
	}
	if (mousePos_.y >= 0) {
		selectY_ = (int)(mousePos_.y / 40);
	}
	else{
		selectY_ = -1;
	}
	
}

//描画
void Stage::Draw()
{
	//画像の描画
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

	Transform t;
	t.position_ = ConvDrawPos(mousePos_.x - 20, mousePos_.y - 20);
	Image::SetTransform(hPict_[COLOR::HEART], t);
	Image::Draw(hPict_[COLOR::HEART]);
}

//開放
void Stage::Release()
{
}

XMFLOAT3 Stage::ConvDrawPos(float x, float y)
{
	XMFLOAT3 p;
	p.x = x / 40 * 0.062f;
	p.y = -y / 40 * 0.110f;
	p.z = 0;
	return p;
}