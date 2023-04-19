#pragma once
#include "Engine/GameObject.h"

//�X�e�[�W���Ǘ�����N���X
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

    // �h�b�g���W����3D���W�ɕϊ�����֐�
    XMFLOAT3 ConvDrawPos(float x, float y);

    int hPict_[COLOR::NUM];    //�摜�ԍ�
public:
    //�R���X�g���N�^
    Stage(GameObject* parent);

    //�f�X�g���N�^
    ~Stage();

    //������
    void Initialize() override;

    //�X�V
    void Update() override;

    //�`��
    void Draw() override;

    //�J��
    void Release() override;
};