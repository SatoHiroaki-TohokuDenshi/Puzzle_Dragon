#pragma once
#include "Engine/GameObject.h"

//ステージを管理するクラス
class Stage : public GameObject
{
    enum COLOR {
        RED = 0,
        BLUE,
        YELLOW,
        GREEN,
        PURPLE,
        HEART,
        NUM,
    };
    static const int WIDTH = 6;
    static const int HEIGHT = 5;
    COLOR field_[WIDTH][HEIGHT];
    XMFLOAT3 mousePos_;
    int selectX_, selectY_;
    int selectColor_;

    // ドット座標から3D座標に変換する関数
    XMFLOAT3 ConvDrawPos(float x, float y);

    int hPict_[COLOR::NUM];    //画像番号
public:
    //コンストラクタ
    Stage(GameObject* parent);

    //デストラクタ
    ~Stage();

    //初期化
    void Initialize() override;

    //更新
    void Update() override;

    //描画
    void Draw() override;

    //開放
    void Release() override;
};