#include <QApplication>
#include "ChooseDlg.h"
#include "MainWnd.h"
#include <QTime>
int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    QTime t = QTime::currentTime();
    qsrand(t.msec()+t.second()*1000);

    //对话框
    ChooseDlg dlg;

    //对话框结束
    if(dlg.exec() != QDialog::Accepted)
        return 0;
    //创建继承了QWidget的子类MainWind,参数为之前选择的对话框中的选择
    MainWnd wnd(dlg._selected);
    wnd.show();




    return app.exec();
}
