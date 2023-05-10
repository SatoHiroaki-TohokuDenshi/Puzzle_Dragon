#pragma once
#include "Engine/GameObject.h"

//ステージを管理するクラス
class Stage : public GameObject
{
    // 状態
    enum STATE {
        S_IDLE = 0,
        S_MOVE,
        S_ERASE,
        S_FALL,
        S_ATTACK,
    };
    STATE state_;

    void UpdateIdle();
    void UpdateMove();
    void UpdateErase();
    void UpdateFall();
    void UpdateAttack();

    //玉の色
    enum COLOR {
        RED = 0,    //赤
        BLUE,       //青
        YELLOW,     //黄
        GREEN,      //緑
        PURPLE,     //紫
        HEART,      //桃
        NUM,        //最大値
    };

    static const int WIDTH = 6;     //フィールドの幅
    static const int HEIGHT = 5;    //フィールドの高さ
    COLOR field_[WIDTH][HEIGHT];    //フィールドのデータ
    XMFLOAT3 mousePos_;             //マウスの位置
    int selectX_, selectY_;         //現在選択している玉の座標
    int selectColor_;               //現在選択している玉のデータ
    int hPict_[COLOR::NUM];         //画像番号
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

    // ドット座標から3D座標に変換する関数
    XMFLOAT3 ConvDrawPos(float x, float y);

    void CalcMouseSelect();
};