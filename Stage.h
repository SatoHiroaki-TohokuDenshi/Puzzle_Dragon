#pragma once
#include "Engine/GameObject.h"

//�X�e�[�W���Ǘ�����N���X
class Stage : public GameObject
{
    // ���
    enum STATE {
        S_IDLE = 0,     //�ҋ@���
        S_MOVE,         //������
        S_ERASE,        //�������
        S_FALL,         //�������
        S_ATTACK,       //�U�����
    };
    STATE state_;

    void UpdateIdle();
    void UpdateMove();
    void UpdateErase();
    void UpdateFall();
    void UpdateAttack();

    //�ʂ̐F
    enum COLOR {
        NOCOLOR = -1,
        RED = 0,    //��
        BLUE,       //��
        YELLOW,     //��
        GREEN,      //��
        PURPLE,     //��
        HEART,      //��
        NUM,        //�ő�l
    };

    //�ʂ̏���ێ�����\����
    struct BALLINFO {
        COLOR color;        //�F���
        float x, y;         //���݂̈ʒu���
#if 0
        int counter;
#else
        float bx, by;       //���炩�ɓ��������߂̈ʒu���
        float rate;         //���炩�ɓ��������߂̐i�����
#endif
        int doErase;        //�����鐔
    };

    static const int WIDTH = 6;     //�t�B�[���h�̕�
    static const int HEIGHT = 5;    //�t�B�[���h�̍���
    BALLINFO field_[HEIGHT][WIDTH]; //�t�B�[���h�̃f�[�^
    XMFLOAT3 mousePos_;             //�}�E�X�̈ʒu
    int selectX_, selectY_;         //���ݑI�����Ă���ʂ̍��W
    int selectColor_;               //���ݑI�����Ă���ʂ̃f�[�^
    int hPict_[COLOR::NUM];         //�摜�ԍ�
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

    // �h�b�g���W����3D���W�ɕϊ�����֐�
    XMFLOAT3 ConvDrawPos(float x, float y);

    //�I�����Ă���ʂ��Z�o
    void CalcMouseSelect();

    //���炩�ɓ��������߂̌v�Z
    float GetRateValue(float begin, float end, float rate);

    //�����邩�ǂ������`�F�b�N
    bool CheckErase();
};