/*
    This file is part of LANchat.

    LANchat is free software: you can redistribute it and/or modify
    it under the terms of the GNU LESSER GENERAL PUBLIC LICENSE
    Version 3 as published by the Free Software Foundation.

    LANchat is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with LANchat.  If not, see <http://www.gnu.org/licenses/>.


  */
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
