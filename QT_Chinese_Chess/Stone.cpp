#include "Stone.h"
#include <QDebug>

Stone::Stone()
{

}

Stone::~Stone()
{

}

QString Stone::name()
{
    /*根据红色方与黑色方
    的不同，设置不同棋子的中文名字
    @wzy  2020.5.3*/
    if(_red){
    switch(this->_type)
    {

    case CHE:
        return "車";
    case MA:
        return "馬";
    case PAO:
        return "炮";
    case BING:
        return "兵";
    case JIANG:
        return "帥";
    case SHI:
        return "仕";
    case XIANG:
        return "相";
    }
    }
    else {
        switch(this->_type)
        {

        case CHE:
            return "車";
        case MA:
            return "馬";
        case PAO:
            return "炮";
        case BING:
            return "卒";
        case JIANG:
            return "将";
        case SHI:
            return "士";
        case XIANG:
            return "象";


    }
        }
    return "错误";
}

//初始化32个棋子的位置
void Stone::init(int id)
{
    struct {
        int row, col;
        Stone::TYPE type;
    } pos[16] = {
    {0, 0, Stone::CHE},
    {0, 1, Stone::MA},
    {0, 2, Stone::XIANG},
    {0, 3, Stone::SHI},
    {0, 4, Stone::JIANG},
    {0, 5, Stone::SHI},
    {0, 6, Stone::XIANG},
    {0, 7, Stone::MA},
    {0, 8, Stone::CHE},

    {2, 1, Stone::PAO},
    {2, 7, Stone::PAO},
    {3, 0, Stone::BING},
    {3, 2, Stone::BING},
    {3, 4, Stone::BING},
    {3, 6, Stone::BING},
    {3, 8, Stone::BING},
    };

    if(id < 16)
    {
        //黑棋的初始化位置
        this->_col = pos[id].col;
        this->_row = pos[id].row;
        this->_type = pos[id].type;
    }
    else
    {
        //红棋的初始化位置，好像比较巧妙，
        //因为象棋是对称的，这里是斜对着相对应，
        //如左上角的红车，对应右下角的黑车这种方式
        //其他类似存在，row相加为8，col相加为8
        this->_col = 8-pos[id-16].col;
        this->_row = 9-pos[id-16].row;
        this->_type = pos[id-16].type;
    }

    this->_dead = false;//初始化都没死
    this->_red = id<16;//id小于16的规定为黑棋，黑棋都为true
}

//这里比较复杂，为什么一开始要转化红棋黑棋的位置，那所以一开始的话
//id<16的还是红色了，懂了
//最后是id<16，_red=true,且在棋盘下方
void Stone::rotate()
{
    this->_col = 8-this->_col;
    this->_row = 9-this->_row;
}
