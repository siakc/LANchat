#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
   // this->setFixedSize(400,300);
    this->conMngr = new ConnectionManager;
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


