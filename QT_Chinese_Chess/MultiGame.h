#ifndef MULTIGAME_H
#define MULTIGAME_H

#include "Board.h"
//跟棋盘类几乎没有区别
class MultiGame : public Board
{
    Q_OBJECT
public:
    explicit MultiGame(QWidget *parent = 0);
    ~MultiGame();

signals:

public slots:
};

#endif // MULTIGAME_H
