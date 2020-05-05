#ifndef STEP_H
#define STEP_H

#include <QObject>
//走棋的记录，用于悔棋。用于人工智能最优走法。
class Step : public QObject
{
    Q_OBJECT
public:
    explicit Step(QObject *parent = 0);
    ~Step();

    int _moveid;
    int _killid;
    int _rowFrom;
    int _colFrom;
    int _rowTo;
    int _colTo;

signals:

public slots:
};

#endif // STEP_H
