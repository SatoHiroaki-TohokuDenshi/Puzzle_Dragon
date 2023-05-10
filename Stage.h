#pragma once
#include "Engine/GameObject.h"

//ステージを管理するクラス
class Stage : public GameObject
{
    // 状態
    enum STATE {
        S_IDLE = 0,     //待機状態
        S_MOVE,         //操作状態
        S_ERASE,        //消去状態
        S_FALL,         //落下状態
        S_ATTACK,       //攻撃状態
    };
    STATE state_;

    void UpdateIdle();
    void UpdateMove();
    void UpdateErase();
    void UpdateFall();
    void UpdateAttack();

    //玉の色
    enum COLOR {
        NOCOLOR = -1,
        RED = 0,    //赤
        BLUE,       //青
        YELLOW,     //黄
        GREEN,      //緑
        PURPLE,     //紫
        HEART,      //桃
        NUM,        //最大値
    };

    //玉の情報を保持する構造体
    struct BALLINFO {
        COLOR color;        //色情報
        float x, y;         //現在の位置情報
#if 0
        int counter;
#else
        float bx, by;       //滑らかに動かすための位置情報
        float rate;         //滑らかに動かすための進捗情報
#endif
        int doErase;        //消える数
    };

    static const int WIDTH = 6;     //フィールドの幅
    static const int HEIGHT = 5;    //フィールドの高さ
    BALLINFO field_[HEIGHT][WIDTH]; //フィールドのデータ
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

    //選択している玉を算出
    void CalcMouseSelect();

    //滑らかに動かすための計算
    float GetRateValue(float begin, float end, float rate);

    //消えるかどうかをチェック
    bool CheckErase();
};