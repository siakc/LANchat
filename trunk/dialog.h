#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <conmngr.h>
#include <QStringListModel>
namespace Ui {
    class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();

private:
    Ui::Dialog *ui;
    ConnectionManager *conMngr;

public Q_SLOTS:
    void OnSend();
    void OnConnect();
    void OnDisconnect();
    void OnNewMessage();
    void OnNickChange();
};

#endif // DIALOG_H
