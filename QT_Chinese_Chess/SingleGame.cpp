#include "SingleGame.h"
#include <QTimer>
#include <QDebug>
SingleGame::SingleGame(QWidget *parent) : Board(parent)
{
    _level = 4;
}

SingleGame::~SingleGame()
{

}

void SingleGame::back()
{
    if(_bRedTurn)
    {
        backOne();
        backOne();
    }
}

//重写父类虚函数click(int id, int row, int col)
void SingleGame::click(int id, int row, int col)
{
    if(_bRedTurn)
    {
        //如果是红方，即玩家走
        Board::click(id, row, col);//执行完之后会将_bRedTurn置为false
        //默认人是红方，人先走。

        if(!_bRedTurn)
        {
            //电脑走
            //启动0.1秒计时器，在0.1秒后电脑思考。使得用户交互体验好
            // 单singleShot，表示它只会触发一次，发出一次信号，然后来执行槽函数。

            QTimer::singleShot(100, this, SLOT(computerMove()));
        }
    }
}

void SingleGame::computerMove()
{

    //得到最好的步骤方式
    Step* step = getBestMove();
    //然后执行这个步骤
    moveStone(step->_moveid, step->_killid, step->_rowTo, step->_colTo);
    delete step;
    update();
}

//得到最好的步骤
Step* SingleGame::getBestMove()
{


    /*
     * 就像模拟人的思考一样
     * 1、看看有哪些步骤可以走
     * 2、试着走一下
     * 3、评估走的结果
     * 4、取最好的结果
*/
    //初始化
    Step* ret = NULL;
    QVector<Step*> steps;
    getAllPossibleMove(steps);
    //越小越好，后面才有可能增大
    int maxInAllMinScore = -300000;

    //元素个数
    while(steps.count())
    {
        //取容器内最后一个元素
        Step* step = steps.last();
        //删除最后一个元素
        steps.removeLast();

        fakeMove(step);
        //fakeMove()已经走了一步，因此_level要减去1
        int minScore = getMinScore(this->_level-1, maxInAllMinScore);
        unfakeMove(step);

        //取得最小值中的最大值（最小值最大值算法）
        if(minScore > maxInAllMinScore)
        {
            //如果之前的步骤不为空，删除这段内存
            if(ret) delete ret;
            //保存最好的步骤
            ret = step;
            maxInAllMinScore = minScore;
        }
        else
        {
            delete step;
        }
    }
    return ret;
}

//评判局面分
int SingleGame::score()
{
    //enum TYPE{CHE, MA, PAO, BING, JIANG, SHI, XIANG};
  //各个类型的分数
    //static int s[] = {1000, 499, 501, 200, 15000, 100, 100};
  //评判：黑棋分的总数 - 红棋分的总数（视频上）

    //@wzy 5.4个人看法
    static int s[] = {1000, 499, 501, 200, 15000, 100, 100};
    //黑棋的总分
    int scoreBlack = 0;
    //红棋的总分
    int scoreRed = 0;
    for(int i=0; i<16; ++i)
    {
        //累加红棋的分数
        //红棋死掉就不计分数
        if(_s[i]._dead) continue;
        scoreRed += s[_s[i]._type];
    }
    for(int i=16; i<32; ++i)
    {
         //累加黑棋的分数
        if(_s[i]._dead) continue;
        scoreBlack += s[_s[i]._type];
    }
    return scoreBlack - scoreRed;
}

//第二个参数用于剪枝优化
int SingleGame::getMinScore(int level, int curMin)
{
    //递归的终结条件
    if(level == 0)
        return score();

    QVector<Step*> steps;
    //人的所有可能走的步骤
    getAllPossibleMove(steps);
    int minInAllMaxScore = 300000;

    while(steps.count())
    {
        Step* step = steps.last();
        steps.removeLast();

        fakeMove(step);
        int maxScore = getMaxScore(level-1, minInAllMaxScore);
        unfakeMove(step);
        //释放内存
        delete step;

        if(maxScore <= curMin)
        {
            while(steps.count())
            {
                Step* step = steps.last();
                steps.removeLast();
                delete step;
            }
            return maxScore;
        }

        if(maxScore < minInAllMaxScore)
        {
            minInAllMaxScore = maxScore;
        }


    }
    return minInAllMaxScore;
}
//第二个参数用于剪枝优化
int SingleGame::getMaxScore(int level, int curMax)
{
    //递归的终结条件
    //返回一般的局面分
    if(level == 0)
        return score();

    QVector<Step*> steps;
    getAllPossibleMove(steps);
    int maxInAllMinScore = -300000;

    while(steps.count())
    {
        Step* step = steps.last();
        steps.removeLast();

        fakeMove(step);
        int minScore = getMinScore(level-1, maxInAllMinScore);
        unfakeMove(step);
        delete step;
        //加上等于，剪枝优化会裁掉更多的分支
        //在走棋的时候，很多时候没有吃棋子
        //局面分没有变化
        if(minScore >= curMax)
        {
            while(steps.count())
            {
                Step* step = steps.last();
                steps.removeLast();
                delete step;
            }
            return minScore;
        }
        if(minScore > maxInAllMinScore)
        {
            maxInAllMinScore = minScore;
        }


    }
    return maxInAllMinScore;
}


//尝试着走一步棋。就像街头老人一样，手尝试着将棋子放到某个地方
void SingleGame::fakeMove(Step *step)
{
    //killed是0-31某个id,就将其_dead置为true,killed是-1（棋盘上），则返回
    //这里虽然将某个棋子置为了死去，但是没有更新在界面上.
    //加上了update()也没反应，因为马上又执行reliveStone了
    killStone(step->_killid);
    //将棋子移动过去
    moveStone(step->_moveid, step->_rowTo, step->_colTo);
}

//像街头老人一样将棋子放回来。
void SingleGame::unfakeMove(Step *step)
{
    //将之前棋子死掉的变成没死,killed为-1则返回
    reliveStone(step->_killid);
    //把棋子移回来，重写父类的函数
    moveStone(step->_moveid, step->_rowFrom, step->_colFrom);
}

//获取所有可能走的步骤
void SingleGame::getAllPossibleMove(QVector<Step *> &steps)
{
    int min, max;
    if(this->_bRedTurn)
    {
        //人走的话，遍历人的可能步骤
        min = 0, max = 16;
    }
    else
    {
        //机器走的话，遍历机器可能的步骤
        min = 16, max = 32;
    }

    //
    for(int i=min;i<max; i++)
    {
        //要把死去的棋子从棋盘中扣掉，死掉的棋子不能有可能走的步骤
        if(this->_s[i]._dead) continue;
        //看棋子能不能走到这些行列
        for(int row = 0; row<=9; ++row)
        {
            for(int col=0; col<=8; ++col)
            {
                //目标行列上有没有棋子
                int killid = this->getStoneId(row, col);
                //不杀自己人
                if(sameColor(i, killid)) continue;
                 //根据不同棋子的移动规则，如果可以移动，合法移动
                if(canMove(i, killid, row, col))
                {
                    //保存所有可以走的步骤，用于后续的评分
                    saveStep(i, killid, row, col, steps);
                }
            }
        }
    }
}
