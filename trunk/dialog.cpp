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
#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
   // this->setFixedSize(400,300);
    this->conMngr = new ConnectionManager;
    ui->plainTextEdit->appendPlainText("LANchat by SiavoshKC. Created under LGPLv3 2012.\nYou may get a copy of the license at <http://www.gnu.org/licenses/>\n\nEnter your  subnet IP and subnet mask in the fields below and press Connect button.");
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::OnConnect()
{
    connect(conMngr, SIGNAL(NewMessage()), this, SLOT(OnNewMessage()));
    connect(conMngr, SIGNAL(NicknamesChanged()), this, SLOT(OnNickChange()));
    conMngr->SetMyNickname(ui->lineEdit_nickname->text());
    conMngr->StartNetwork(QHostAddress(ui->lineEdit_subnet->text()), ui->lineEdit_netmask->text().toUInt() , !ui->checkBox_nobroadcast->isChecked());
    ui->pushButton_con->setEnabled(false);
}

void Dialog::OnNewMessage()
{
    ui->plainTextEdit->appendPlainText(conMngr->ReadMessage() + '\n');
    this->activateWindow();
}

void Dialog::OnSend()
{
    if(!ui->lineEdit->text().isEmpty())
    {
	QString message = ui->lineEdit->text();
	ui->plainTextEdit->appendPlainText(conMngr->GetMyNickname() + ": " + message);
        conMngr->WriteMessage(message);
        ui->lineEdit->clear();
    }
}

void Dialog::OnNickChange()
{
  QStringList nickNames;
  if(!conMngr->GetNicknames(&nickNames)) return;
  QStringListModel *model = new QStringListModel(nickNames);
  ui->listView->setModel( model);

}

void Dialog::OnDisconnect()
{
    conMngr->StopNetwork();
    ui->pushButton_con->setEnabled(true);
    ui->listView->reset();
}


