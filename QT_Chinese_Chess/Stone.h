#ifndef STONE_H
#define STONE_H

#include <QRect>
#include <QPainter>
//封装的是象棋
class Stone
{
public:
    Stone();
    ~Stone();

    //枚举类型
    enum TYPE{CHE, MA, PAO, BING, JIANG, SHI, XIANG};

    void init(int id);

    int _row;//象棋子在棋盘上的哪一行
    int _col;
    TYPE _type;
    bool _dead;//棋子有没有被杀
    bool _red;//棋子的颜色
    int _id;//棋子的id

    QString name();//返回象棋的中文名字，QString见名知意

    void rotate();//网络端的旋转黑红双方
};

#endif // STONE_H
