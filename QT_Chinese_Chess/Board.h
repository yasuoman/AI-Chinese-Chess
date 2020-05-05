#ifndef BOARD_H
#define BOARD_H

#include <QFrame>
#include "Stone.h"
#include "Step.h"
#include <QVector>
//棋盘类
//QFrame是QWidgit的子类
class Board : public QFrame
{
    Q_OBJECT
public:
    explicit Board(QWidget *parent = 0);
    ~Board();

    /* game data */
    Stone _s[32];//32颗棋子，类数组
    int _r;//棋子半径
    QPoint _off;//棋子与边界的偏移

    //红边还是黑边，用于象，将、士、兵的位置限制
    //因为在不同的边上，不同的象，将、士、兵的位置限制不一样
    bool _bSide;

    QVector<Step*> _steps;//每走一步棋的记录，用于悔棋

    /* game status */
    int _selectid;//走棋选择的棋子id
    bool _bRedTurn;//true 红方走，false 黑方走
    void init(bool bRedSide);//初始化棋盘，bRedSide控制红黑棋的上下方位置

    /* draw functions */
    void paintEvent(QPaintEvent *);//绘制棋盘
    void drawPlate(QPainter& p);//
    void drawPlace(QPainter& p);//绘制九宫
    void drawInitPosition(QPainter& p);//绘制初始位置，如炮兵初始有特殊标记
    void drawInitPosition(QPainter& p, int row, int col);
    void drawStone(QPainter& p);//绘制棋子
    void drawStone(QPainter& p, int id);

    /* function for coordinate */
    //坐标转化。
    //center()将棋盘上的位置转换到窗口的像素位置
    QPoint center(int row, int col);
    QPoint center(int id);
    QPoint topLeft(int row, int col);
    QPoint topLeft(int id);
    QRect cell(int row, int col);
    QRect cell(int id);

    //通过鼠标指定的点获取窗口的位置，转换到棋盘上的位置。
    //与center相对应
    bool getClickRowCol(QPoint pt, int& row, int& col);

    /* help function */
    QString name(int id);//id转化为名字
    bool red(int id);
    bool sameColor(int id1, int id2);
    int getStoneId(int row, int col);
    void killStone(int id);
    void reliveStone(int id);
    void moveStone(int moveid, int row, int col);
    bool isDead(int id);

    /* move stone */
    //mouseReleaseEvent是在鼠标松开的时候，mousePressEvent是在鼠标点击的时候
    void mouseReleaseEvent(QMouseEvent *);
    void click(QPoint pt);
    virtual void click(int id, int row, int col);//派生类用来重写父类的
    void trySelectStone(int id);
    void tryMoveStone(int killid, int row, int col);
    void moveStone(int moveid, int killid, int row, int col);
    void saveStep(int moveid, int killid, int row, int col, QVector<Step*>& steps);
    void backOne();
    void back(Step* step);
    virtual void back();

    /* rule */
    bool canMove(int moveid, int killid, int row, int col);
    bool canMoveChe(int moveid, int killid, int row, int col);
    bool canMoveMa(int moveid, int killid, int row, int col);
    bool canMovePao(int moveid, int killid, int row, int col);
    bool canMoveBing(int moveid, int killid, int row, int col);
    bool canMoveJiang(int moveid, int killid, int row, int col);
    bool canMoveShi(int moveid, int killid, int row, int col);
    bool canMoveXiang(int moveid, int killid, int row, int col);

    bool canSelect(int id);

    /* rule helper */
    int getStoneCountAtLine(int row1, int col1, int row2, int col2);
    int relation(int row1, int col1, int row, int col);
    bool isBottomSide(int id);
    //@wzy 5.4
    void Victory();

signals:

public slots:
    void slotBack();

};

#endif // BOARD_H
