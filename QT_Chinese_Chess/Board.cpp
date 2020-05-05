#include "Board.h"
#include <QPainter>
#include <QMouseEvent>
#include <QDebug>
#include<QMessageBox>
#include<QApplication>
#define GetRowCol(__row, __col, __id) \
    int __row = _s[__id]._row;\
    int __col = _s[__id]._col

Board::Board(QWidget *parent) : QFrame(parent)
{
    //棋子半径默认为20
    this->_r = 20;
    //设置最大窗口
    setMinimumSize(_r*18+1, _r*20+1);
    //默认首先给_bSide棋盘上的某一边true
    init(true);
}

Board::~Board()
{

}

//在board中初始化32个棋子的位置
void Board::init(bool bRedSide)
{
    for(int i=0; i<32; ++i)
    {
        _s[i].init(i);
    }
    //默认初始bRedSide为true
    if(bRedSide)
    {
        for(int i=0; i<32; ++i)
        {
            _s[i].rotate();
        }
    }

    _selectid = -1;
    _bRedTurn = true;//一开始红方先走
    //这两者的区别需要思考下
    _bSide = bRedSide;
    update();
}

void Board::paintEvent(QPaintEvent *)
{
    //初始赋值
    int r = height()/20;
    _r = r;
    _off = QPoint(r+1, r+1);

    QPainter p(this);
    //防止图形走样,防锯齿，棋子字上面的锯齿
    p.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
    //保存QPainter的状态
    p.save();
    //画棋盘
    drawPlate(p);
    //恢复QPainter的状态
    p.restore();

    p.save();
    //画九宫格
    drawPlace(p);
    p.restore();

    p.save();
    //drawInitPosition绘制初始位置，如炮兵初始有特殊标记
    drawInitPosition(p);
    p.restore();

    p.save();
    //画棋子
    drawStone(p);
    //p.drawText(3*_r,10.5*_r,"楚    河");
    //p.drawText(10.5*_r,10.5*_r,"漢    界");
    p.restore();




}



//用于画图当中，不再画死去的棋子对象
bool Board::isDead(int id)
{
    if(id == -1)return true;
    return _s[id]._dead;
}

//用函数重载，画32个棋子对象
void Board::drawStone(QPainter &p)
{
    for(int i=0; i<32; i++)
    {
        drawStone(p, i);
    }
}

//画单个棋子对象，参数需要棋子的id和Qpainter类，传的是引用，即对象当做参数
//使用对象引用作为函数参数不但具有用对象指针作函数参数的优点，
//而且用对象引用作函数参数将更简单、更直接
void Board::drawStone(QPainter &p, int id)
{
    //某个棋子对象被击杀后，就不再画它了
    if(isDead(id)) return;

    //关于美化棋子外表的
    QColor color;
    if(red(id)) color = Qt::red;//Qt::red 内置的宏
    else color = Qt::black;

    p.setPen(QPen(QBrush(color), 2));//改变字体的颜色

    //如果选中棋子，变成灰色，否则默认黄色
    //_selectid为选中的id
    if(id == _selectid) p.setBrush(Qt::gray);
    else p.setBrush(Qt::yellow);

    p.drawEllipse(cell(id));
    //设置字体
    p.setFont(QFont("system", _r*1.2, 700));//700貌似是线条粗细


    //QTextOption(Qt::AlignCenter)中间对齐
    //cell(id)是QRect类型,矩形rectangle
    p.drawText(cell(id), name(id), QTextOption(Qt::AlignCenter));

}

//返回棋子的中文名
QString Board::name(int id)
{
    return _s[id].name();
}

//通过id返回是哪一方，如小于16的是红色方，为true
bool Board::red(int id)
{
    return _s[id]._red;
}

//drawInitPosition绘制初始位置，如炮兵初始有特殊标记
//调用它的函数在下面
void Board::drawInitPosition(QPainter &p, int row, int col)
{
    QPoint pt = center(row, col);
    QPoint off = QPoint(_r/6, _r/6);
    int len = _r/3;

    QPoint ptStart;
    QPoint ptEnd;

    if(col != 0)
    {
        /* 左上角 */
        ptStart = QPoint(pt.x() - off.x(), pt.y() - off.y());
        ptEnd = ptStart + QPoint(-len, 0);
        p.drawLine(ptStart, ptEnd);
        ptEnd = ptStart + QPoint(0, -len);
        p.drawLine(ptStart, ptEnd);

        /* 左下角 */
        ptStart = QPoint(pt.x() - off.x(), pt.y() + off.y());
        ptEnd = ptStart + QPoint(-len, 0);
        p.drawLine(ptStart, ptEnd);
        ptEnd = ptStart + QPoint(0, +len);
        p.drawLine(ptStart, ptEnd);
    }

    if(col != 8)
    {
        /* 右下角 */
        ptStart = QPoint(pt.x() + off.x(), pt.y() + off.y());
        ptEnd = ptStart + QPoint(+len, 0);
        p.drawLine(ptStart, ptEnd);
        ptEnd = ptStart + QPoint(0, +len);
        p.drawLine(ptStart, ptEnd);

        /* 右上角 */
        ptStart = QPoint(pt.x() + off.x(), pt.y() - off.y());
        ptEnd = ptStart + QPoint(+len, 0);
        p.drawLine(ptStart, ptEnd);
        ptEnd = ptStart + QPoint(0, -len);
        p.drawLine(ptStart, ptEnd);
    }

}

//drawInitPosition绘制初始位置，如炮兵初始有特殊标记

void Board::drawInitPosition(QPainter &p)
{
    //画五个兵的特殊标记
    drawInitPosition(p, 3, 0);
    drawInitPosition(p, 3, 2);
    drawInitPosition(p, 3, 4);
    drawInitPosition(p, 3, 6);
    drawInitPosition(p, 3, 8);

    //画五个兵的特殊标记
    drawInitPosition(p, 6, 0);
    drawInitPosition(p, 6, 2);
    drawInitPosition(p, 6, 4);
    drawInitPosition(p, 6, 6);
    drawInitPosition(p, 6, 8);

    //画两个炮的标记
    drawInitPosition(p, 2, 1);
    drawInitPosition(p, 2, 7);
    //画两个炮的标记
    drawInitPosition(p, 7, 1);
    drawInitPosition(p, 7, 7);
}

//画九宫格
void Board::drawPlace(QPainter &p)
{
    p.drawLine(center(0, 3), center(2, 5));
    p.drawLine(center(2, 3), center(0, 5));

    p.drawLine(center(9, 3), center(7, 5));
    p.drawLine(center(7, 3), center(9, 5));
}

//画棋盘
void Board::drawPlate(QPainter &p)
{
    for(int i=0; i<10; ++i)
    {
        //将棋盘的边缘描黑更深
        if(i==0 || i==9)
        {
            p.setPen(QPen(Qt::black, 3, Qt::SolidLine));
        }

        else
        {
            p.setPen(QPen(Qt::black, 1, Qt::SolidLine));
        }
        p.drawLine(center(i, 0), center(i, 8));
    }

    for(int i=0; i<9; ++i)
    {
        //画出楚河汉界，且加深棋盘边界深度
        if(i==0 || i==8)
        {
            p.setPen(QPen(Qt::black, 3, Qt::SolidLine));
            p.drawLine(center(0, i), center(9, i));
        }
        else
        {
            p.setPen(QPen(Qt::black, 1, Qt::SolidLine));
            p.drawLine(center(0, i), center(4, i));
            p.drawLine(center(5, i), center(9, i));
        }
    }
}

//针对单个棋子对象，返回棋盘行列对应的像素坐标
//QPoint 类代表一个坐标点，实现在 QtCore 共享库中。
//它可以认为是一个整型的横坐标和一个整型的纵坐标的组合。

QPoint Board::center(int row, int col)
{
    QPoint pt(_r*col*2, _r*row*2);
    //_off:棋子与边界的偏移
    return pt + _off;
}

QPoint Board::center(int id)
{
    //函数重载，得到每个棋子对象在棋盘行列对应的像素坐标
    return center(_s[id]._row, _s[id]._col);
}

//关于画棋子，椭圆里面来个矩形画
QPoint Board::topLeft(int row, int col)
{
    return center(row, col) - QPoint(_r, _r);
}

QPoint Board::topLeft(int id)
{
    return center(id) - QPoint(_r, _r);
}

QRect Board::cell(int row, int col)
{
    return QRect(topLeft(row, col), QSize(_r*2-1, _r*2-1));
}

//画矩形，将棋子中文名字填充进矩形
QRect Board::cell(int id)
{
    //选择的第二个重载
    return QRect(topLeft(id), QSize(_r*2-1, _r*2-1));

}

//通过鼠标指定的点获取窗口的位置，转换到棋盘上的位置。
//与center相反，bool类型是为了判断是否点击到窗口外部
//传入的pt为鼠标在窗口的位置
bool Board::getClickRowCol(QPoint pt, int &row, int &col)
{   //这是最简单的方法，但是效率比较低。
    //对于90个点，找到距离最近行列的位置

    /*for(row=0; row<=9; ++row)
    {
        for(col=0; col<=8; ++col)
        {
            //通过center()将行列转化为窗口位置
            //判断点击的这个地方与其行列所在的位置的距离
            //若是小于棋子的半径，就归属于这个行列
            //从而间接找到了点击的哪个棋子。
            QPoint distance = center(row, col) - pt;
            if(distance.x() * distance.x() + distance.y() * distance.y() < _r* _r)
                return true;
        }
    }
    return false;*/
    //新的算法找到点击的位置转化成棋盘所在的行列
    //@wzy   5.4
    //目前已完成功能，未发现明显bug
    QPoint newpt;
    //减去棋盘与窗口的偏移量
    newpt= pt - _off;
    //点击的像素点在棋盘内

    //计算得到点击像素点所在的行列
    col = (int(newpt.rx()/_r)+1)/2;
    row = (int(newpt.ry()/_r)+1)/2;
    //在棋盘内，
    if(col < 9 && col >-1 && row>-1 && row<10)
    return true;

    else
    {
        return false;
    }

}

//用来点击之后的走棋
//QMouseEvent *ev带有鼠标点击的位置信息
void Board::mouseReleaseEvent(QMouseEvent *ev)
{
    //要用左键来点击
    if(ev->button() != Qt::LeftButton)
    {
        return;
    }

    click(ev->pos());   //ev->pos()为鼠标点击的位置
}

//判断是不是某人的回合
bool Board::canSelect(int id)
{
    //一开始是红方先走，因此是true==true，可以选择
    return _bRedTurn == _s[id]._red;
}

//尝试选则棋子
void Board::trySelectStone(int id)
{
    if(id == -1)
        return;
    //还要看是不是某一方的回合。
    if(!canSelect(id)) return;
    //这里将选则正确的id赋值给了_selectid
    _selectid = id;
    //重画PaintEvent，这里就是让棋子的颜色改变了，因为在
    //PaintEvent里面设置了，若id==_select
    update();
}

bool Board::sameColor(int id1, int id2)
{
    //点击到两个棋盘上的空点
    if(id1 == -1 || id2 == -1) return false;
    //red()通过id返回是哪一方，如小于16的是红色方
    return red(id1) == red(id2);
}

void Board::tryMoveStone(int killid, int row, int col)
{
    //未选择走棋盘上的某个位置，且要杀的与自己的棋子同一类，则重新选择棋子来杀
    if(killid != -1 && sameColor(killid, _selectid))
    {
        //重新将你点击要killed的id赋给_selectedid，
        //且更新棋盘表示你选中了新的自己的棋子
        //重新进行之前的点击过程,尝试再点击另一个棋子，或者再次重复这个过程。
        trySelectStone(killid);
        return;
    }
    //根据棋子的行走规则，这里包括击杀和行走的规则，其实质是一样的
    //棋子能行走的地方，他都可以去击杀那些地方上的点
    bool ret = canMove(_selectid, killid, row, col);
    if(ret)
    {
        //移动棋子，并将走棋权利给另一方
        moveStone(_selectid, killid, row, col);
        //将_selectid置为-1，等待下一轮的选择
        _selectid = -1;
        //更新棋盘
        update();
    }
}

//跟车的移动规则有关。获取两个棋子间的象棋个数
int Board::getStoneCountAtLine(int row1, int col1, int row2, int col2)
{
    int ret = 0;
    //不在同一行列
    if(row1 != row2 && col1 != col2)
        return -1;
    //这两个点重合了
    if(row1 == row2 && col1 == col2)
        return -1;

    //在同一行
    if(row1 == row2)
    {
        //得到col1和col2中的最小值
        int min = col1 < col2 ? col1 : col2;
        //得到col1和col2中的最大值
        int max = col1 < col2 ? col2 : col1;
        //遍历两个点之间的其他棋子间的个数
        for(int col = min+1; col<max; ++col)
        {
            if(getStoneId(row1, col) != -1) ++ret;
        }
    }
    //同上
    else
    {
        int min = row1 < row2 ? row1 : row2;
        int max = row1 < row2 ? row2 : row1;
        for(int row = min+1; row<max; ++row)
        {
            if(getStoneId(row, col1) != -1) ++ret;
        }
    }

    return ret;
}

//用于巧妙计算每个棋类的移动坐标
int Board::relation(int row1, int col1, int row, int col)
{
    //qAbs()为求绝对值
    //将是10、1；马是21、12；象是22；兵是1、10
    return qAbs(row1-row)*10+qAbs(col1-col);
}

bool Board::canMoveChe(int moveid, int, int row, int col)
{
    GetRowCol(row1, col1, moveid);
    //getStoneCountAtLine获取这两个点之前的象棋个数
    int ret = getStoneCountAtLine(row1, col1, row, col);
    //如果这两个点之间没有其他棋子，车可以走，包括吃掉那个棋子
    if(ret == 0)
        return true;
    return false;
}

bool Board::canMoveMa(int moveid, int, int row, int col)
{
    GetRowCol(row1, col1, moveid);
    int r = relation(row1, col1, row, col);
    if(r != 12 && r != 21)
        return false;



    //找蹩脚马
    if(r == 12)
    {
        //列相加除以2,行不变，所在的位置就是马腿
        //若马腿位置上有棋子，则不能走。
        if(getStoneId(row1, (col+col1)/2) != -1)
            return false;
    }
    else
    {
        //行相加除以2,行不变，所在的位置就是马腿
        //若马腿位置上有棋子，则不能走。
        if(getStoneId((row+row1)/2, col1) != -1)
            return false;
    }

    return true;
}

bool Board::canMovePao(int moveid, int killid, int row, int col)
{
    GetRowCol(row1, col1, moveid);
    //getStoneCountAtLine获取这两个点之前的象棋个数
    int ret = getStoneCountAtLine(row, col, row1, col1);
    //炮要吃别人的棋子
    if(killid != -1)
    {
        //且在这两个点之间只有一个棋子才能吃别人
        if(ret == 1) return true;
    }
    //如果不吃，像车一样移动
    else
    {
        //且移动的两个点之间没得棋子
        if(ret == 0) return true;
    }
    return false;
}

bool Board::canMoveBing(int moveid, int, int row, int col)
{
    GetRowCol(row1, col1, moveid);
    int r = relation(row1, col1, row, col);
    if(r != 1 && r != 10) return false;

    //在红色方，即下面
    if(isBottomSide(moveid))
    {
        //row是目标行，row1是原始行
        //目标行大于原始行则是在后退，不行
        //注意坐标是从上到下
        if(row > row1) return false;
        //还未过河，横着走，不行
        if(row1 >= 5 && row == row1) return false;
    }
    else
    {
        if(row < row1) return false;
        if(row1 <= 4 && row == row1) return false;
    }

    return true;
}

//将，帅的移动规则 1、首先目标位置在九宫格，移动的步长是一个格子
//两个将领照面可以吃,这里实现了
bool Board::canMoveJiang(int moveid, int killid, int row, int col)
{
    //killid != -1 选中了某个要杀的棋子id，
    //并且你要吃的是对面的老将（帅）,可以直接按照车的方式进行，巧妙！
    if(killid != -1 && _s[killid]._type == Stone::JIANG)
        return canMoveChe(moveid, killid, row, col);

    //将_s[moveid].row赋给row1，colt同理
    //这是个宏定义函数，用来记录每个棋子到哪里去了
    GetRowCol(row1, col1, moveid);
    //计算坐标关系。将需要为1或者10
    int r = relation(row1, col1, row, col);
    if(r != 1 && r != 10) return false;

    //只能在第3列到第5列移动（从0开始数）
    if(col < 3 || col > 5) return false;

    if(isBottomSide(moveid))
    {
        //红方的行限制，注意默认棋盘下，没在网络端
        //红方的_red是true
        if(row < 7) return false;
    }
    else
    {
        //黑方的行限制
        if(row > 2) return false;
    }
    return true;
}

bool Board::canMoveShi(int moveid, int, int row, int col)
{
    GetRowCol(row1, col1, moveid);
    int r = relation(row1, col1, row, col);
    if(r != 11) return false;//士的r要等于11

    if(col < 3 || col > 5) return false;
    //红方的行限制，注意默认棋盘下，没在网络端
    //红方的_red是true
    if(isBottomSide(moveid))

    {
        if(row < 7) return false;
    }
    else
    {
        if(row > 2) return false;
    }
    return true;
}

//这里第二个参数应该是killed,但这里是哑元
//有的时候用得到，有的时候用不到
//加上的话，有时候编译系统会警告参数没用到
//一个函数的参数 只有类型 没有名字 则这个参数称之为哑元。
//炮和将有killed参数，其余没得
//因为炮和将都有判断是否要吃棋子的操作，且这个操作与
//不吃棋子之间的方式有区别
bool Board::canMoveXiang(int moveid, int, int row, int col)
{

    GetRowCol(row1, col1, moveid);
    int r = relation(row1, col1, row, col);
    if(r != 22) return false;

    //找象眼睛
    //row1是象这种棋子所在的行
    //row是你要点击行走的行
    int rEye = (row+row1)/2;
    int cEye = (col+col1)/2;
    //判断象心上有没有棋子，有则返回这个棋子的id,没有则返回-1
    if(getStoneId(rEye, cEye) != -1)
        return false;

    //限制红色方（未网络对战）
    if(isBottomSide(moveid))
    {
        if(row < 4) return false;
    }
    else
    {
        if(row > 5) return false;
    }
    return true;
}

//是否可以走，需要移动的id,是否需要击杀，如炮的击杀和移动不一样，
//row,col是你要走哪里去
bool Board::canMove(int moveid, int killid, int row, int col)
{
    //killded为-1代表棋盘上的某个位置，否则就是要击杀的棋子的id
    //如果要移动的两个棋子的颜色都是一样的，就选中到第二次点击的那个棋子
    //这句话是不是可以不要啊？因为前面已经判断了他们俩不是同一个颜色
    //@wyz 5.4,目前删除之后未发现异常，在singlegame中也是先前判断了
    //sameColor(moveid, killid)


    //if(sameColor(moveid, killid)) return false;


    //每一个id就是一个棋子对象，都有唯一的类型，如车，马等
    switch (_s[moveid]._type)
    {
    case Stone::CHE:
        return canMoveChe(moveid, killid, row, col);

    case Stone::MA:
        return canMoveMa(moveid, killid, row, col);

    case Stone::PAO:
        return canMovePao(moveid, killid, row, col);

    case Stone::BING:
        return canMoveBing(moveid, killid, row, col);

    case Stone::JIANG:
        return canMoveJiang(moveid, killid, row, col);

    case Stone::SHI:
        return canMoveShi(moveid, killid, row, col);

    case Stone::XIANG:
        return canMoveXiang(moveid, killid, row, col);

    }
    return false;
}

//用于悔棋或者用于人机当中的unfakemove()，将之前棋子死掉的变成没死
void Board::reliveStone(int id)
{
    if(id==-1) return;
    _s[id]._dead = false;
}

//用于杀棋子，将棋子的_dead置为true
void Board::killStone(int id)
{
    if(id==-1) return;
    _s[id]._dead = true;
    //Victory();
}
void Board::Victory()
{
    if(_s[4]._dead)
    {
//       QDialog blackDialog;
//       blackDialog.setWindowTitle("Black Win");
//       blackDialog.setWindowIconText("Black Win");
//       blackDialog.resize(QSize(300,300));
//       blackDialog.exec();
//       QMessageBox msgBox;
//       msgBox.setText("<font size='20' color='red'>黑方胜</font>");
//       msgBox.exec();
        //目前不适用与人机对战，原因找到了
        //人机对战中，killStone(int id)会在fakemove()
        //里面执行，但是是假的执行。因此要将Victory（）函数放到另外的地方
       if (!(QMessageBox::information(this,tr("结束"),tr("黑方胜！"),tr("退出"))))
            {
                  QApplication* app;
                  app->exit(0);
            }


    }
    if(_s[20]._dead)
    {
//        QDialog redDialog ;
//        redDialog.setWindowTitle("Red Win");
//        redDialog.setWindowIconText("red Win");

//        redDialog.resize(QSize(300,300));
//        redDialog.exec();
//        QMessageBox msgBox;
//        msgBox.setText("<font size='20' color='red'>红方胜</font>");
//        msgBox.exec();
        if (!(QMessageBox::information(this,tr("结束"),tr("红方胜！"),tr("退出"))))
             {
                   QApplication* app;
                   app->exit(0);
             }



  }

}

//将棋子对象的颜色属性赋给棋盘的哪一边属性，表示在哪一边
bool Board::isBottomSide(int id)
{
    //_bSide默认是true,这里比较复杂
    //
    return _bSide == _s[id]._red;
}

void Board::moveStone(int moveid, int row, int col)
{
    //将目标的位置赋给之前选中的棋子位置
    _s[moveid]._row = row;
    _s[moveid]._col = col;

    //某一方走了之后，_bRedTurn 将权利给另一方
    _bRedTurn = !_bRedTurn;
}

//保存杀棋，或者走棋步骤
void Board::saveStep(int moveid, int killid, int row, int col, QVector<Step*>& steps)
{
    GetRowCol(row1, col1, moveid);
    Step* step = new Step;
    step->_colFrom = col1;
    step->_colTo = col;
    step->_rowFrom = row1;
    step->_rowTo = row;
    step->_moveid = moveid;
    step->_killid = killid;

    steps.append(step);
}

//允许杀棋子或者允许走棋子到棋盘上之后
void Board::moveStone(int moveid, int killid, int row, int col)
{
    //保留移动记录
    saveStep(moveid, killid, row, col, _steps);

    killStone(killid);
    moveStone(moveid, row, col);
    Victory();
}

void Board::click(int id, int row, int col)
{
    if(this->_selectid == -1)

    {
        //以下函数，先判断是不是你的回合
        //然后若是你的回合，将你选中的棋子id赋给_selectedid
        //准备进行点击下一个目标：对面的棋子，或者棋盘上的某个位置

        trySelectStone(id);
    }
    else
    {

        //这是进行点击下一个目标：对面的棋子，或者棋盘上的某个位置
        tryMoveStone(id, row, col);
    }
}

//点击某个位置后，返回
void Board::click(QPoint pt)
{

    int row, col;
    //点击某个位置后，返回这个位置是否在棋盘上
    //牛皮，这里传的是row,col的引用，
    //getClickRowCol()中找到的row，col都会被保存下来。
    bool bClicked = getClickRowCol(pt, row, col);
    //在棋盘外，返回，重新点击某个位置
    if(!bClicked) return;

    //返回了点击的这个位置上是否有棋子的id
    //没有则返回-1

    int id = getStoneId(row, col);
    //以下下这个函数是个纯虚函数
    click(id, row, col);
}

int Board::getStoneId(int row, int col)
{
    //判断点击到的窗口上有没有这个棋子，有则返回这个棋子的id,没有则返回-1
    //通过每个棋子的行列是否相等，以及棋子是否死亡来看。
    for(int i=0; i<32; ++i)
    {
        if(_s[i]._row == row && _s[i]._col == col && !isDead(i))
            return i;
    }
    return -1;
}

//悔棋
void Board::back(Step *step)
{
    reliveStone(step->_killid);
    moveStone(step->_moveid, step->_rowFrom, step->_colFrom);
}

void Board::backOne()
{
    if(this->_steps.size() == 0) return;

    Step* step = this->_steps.last();
    _steps.removeLast();
    back(step);

    update();
    delete step;
}

void Board::back()
{
    backOne();
}

void Board::slotBack()
{
    back();
}
