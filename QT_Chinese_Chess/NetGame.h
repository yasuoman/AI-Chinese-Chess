#ifndef NETGAME_H
#define NETGAME_H

#include "Board.h"
//网络相关的头文件
#include <QTcpServer>
#include <QTcpSocket>

/*
 * 报文格式定义
 * 1.初始化    两个字节 主要是执红方还是黑方，由服务器发出，客户端接收
 * 第一个字节固定为1，第二个字节是1或者0，1表示接收方走红棋
 * 2.鼠标点击   四个字节
 * 第一个字节固定为2，第二个字节是坐标行，第三个字节是坐标列，第四个字节是
 * 点击棋子的id(可以为-1)
 * 3.悔棋      一个字节
 *
*/
//加入了一些网络功能。
class NetGame : public Board
{
    Q_OBJECT
public:
    explicit NetGame(bool server, QWidget *parent = 0);
    ~NetGame();

    bool _bServer;
    QTcpServer* _server;
    QTcpSocket* _socket;

    void back();
    void click(int id, int row, int col);

    void backFromNetwork(QByteArray buf);
    void clickFromNetwork(QByteArray buf);
    void initFromNetwork(QByteArray buf);

signals:

public slots:
    void slotNewConnection();
    void slotDataArrive();
};

#endif // NETGAME_H
