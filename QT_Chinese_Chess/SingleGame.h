#ifndef SINGLEGAME_H
#define SINGLEGAME_H

#include "Board.h"
//SingleGame加入一些人工智能的方法

/*可以优化的方向：
 * 1、可以对getallpossiblemove()排个序，如一般都是走棋力更大的棋子
 *2、局面分score(),相同棋子在不同位置上的分值是不同的，如兵到底了九宫格，威力更大。
 *3、数据结构:
 * 4、棋盘的canmove()函数可以优化，如这个棋子是一颗兵，就不用遍历整个棋盘它可能走的位置
 * 一般感觉车、炮就遍历它的横向和竖向、马最多就8个位置，兵最多三个位置
 * 士最多四个，将最多四个，象最多两个，感觉这个可以优化
 *
 */
class SingleGame : public Board
{
    //用于槽函数
    Q_OBJECT
public:
    explicit SingleGame(QWidget *parent = 0);
    ~SingleGame();

    int _level;//考虑的层次

    void back();
    void click(int id, int row, int col);

    Step* getBestMove();
    void getAllPossibleMove(QVector<Step*>& steps);
    int getMinScore(int level, int curMin);
    int getMaxScore(int level, int curMax);
    int score();

    void fakeMove(Step* step);
    void unfakeMove(Step* step);

signals:

 //槽函数
public slots:
    void computerMove();
};

#endif // SINGLEGAME_H
