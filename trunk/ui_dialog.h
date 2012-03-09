/********************************************************************************
** Form generated from reading UI file 'dialog.ui'
**
** Created: Fri Mar 9 21:48:10 2012
**      by: Qt User Interface Compiler version 4.7.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DIALOG_H
#define UI_DIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QDialog>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QListView>
#include <QtGui/QPlainTextEdit>
#include <QtGui/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_Dialog
{
public:
    QListView *listView;
    QLineEdit *lineEdit;
    QPushButton *pushButton;
    QPlainTextEdit *plainTextEdit;
    QPushButton *pushButton_con;
    QLineEdit *lineEdit_subnet;
    QLabel *label;
    QCheckBox *checkBox_nobroadcast;
    QLineEdit *lineEdit_netmask;
    QLabel *label_2;
    QLineEdit *lineEdit_nickname;
    QPushButton *pushButton_discon;
    QLabel *label_3;

    void setupUi(QDialog *Dialog)
    {
        if (Dialog->objectName().isEmpty())
            Dialog->setObjectName(QString::fromUtf8("Dialog"));
        Dialog->setWindowModality(Qt::ApplicationModal);
        Dialog->resize(499, 358);
        QSizePolicy sizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(Dialog->sizePolicy().hasHeightForWidth());
        Dialog->setSizePolicy(sizePolicy);
        Dialog->setMinimumSize(QSize(499, 358));
        Dialog->setMaximumSize(QSize(499, 358));
        Dialog->setSizeGripEnabled(false);
        Dialog->setModal(true);
        listView = new QListView(Dialog);
        listView->setObjectName(QString::fromUtf8("listView"));
        listView->setGeometry(QRect(10, 10, 101, 251));
        listView->setEditTriggers(QAbstractItemView::NoEditTriggers);
        lineEdit = new QLineEdit(Dialog);
        lineEdit->setObjectName(QString::fromUtf8("lineEdit"));
        lineEdit->setGeometry(QRect(10, 270, 381, 21));
        pushButton = new QPushButton(Dialog);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setGeometry(QRect(400, 270, 75, 23));
        plainTextEdit = new QPlainTextEdit(Dialog);
        plainTextEdit->setObjectName(QString::fromUtf8("plainTextEdit"));
        plainTextEdit->setGeometry(QRect(120, 10, 351, 251));
        plainTextEdit->setReadOnly(true);
        plainTextEdit->setBackgroundVisible(false);
        pushButton_con = new QPushButton(Dialog);
        pushButton_con->setObjectName(QString::fromUtf8("pushButton_con"));
        pushButton_con->setGeometry(QRect(320, 330, 71, 23));
        lineEdit_subnet = new QLineEdit(Dialog);
        lineEdit_subnet->setObjectName(QString::fromUtf8("lineEdit_subnet"));
        lineEdit_subnet->setGeometry(QRect(230, 300, 101, 21));
        label = new QLabel(Dialog);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(190, 300, 41, 20));
        checkBox_nobroadcast = new QCheckBox(Dialog);
        checkBox_nobroadcast->setObjectName(QString::fromUtf8("checkBox_nobroadcast"));
        checkBox_nobroadcast->setGeometry(QRect(10, 300, 161, 17));
        lineEdit_netmask = new QLineEdit(Dialog);
        lineEdit_netmask->setObjectName(QString::fromUtf8("lineEdit_netmask"));
        lineEdit_netmask->setGeometry(QRect(350, 300, 31, 20));
        label_2 = new QLabel(Dialog);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(330, 302, 16, 16));
        lineEdit_nickname = new QLineEdit(Dialog);
        lineEdit_nickname->setObjectName(QString::fromUtf8("lineEdit_nickname"));
        lineEdit_nickname->setGeometry(QRect(70, 330, 113, 20));
        pushButton_discon = new QPushButton(Dialog);
        pushButton_discon->setObjectName(QString::fromUtf8("pushButton_discon"));
        pushButton_discon->setGeometry(QRect(400, 330, 71, 23));
        label_3 = new QLabel(Dialog);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(10, 330, 51, 20));

        retranslateUi(Dialog);
        QObject::connect(pushButton, SIGNAL(pressed()), Dialog, SLOT(OnSend()));
        QObject::connect(pushButton_con, SIGNAL(pressed()), Dialog, SLOT(OnConnect()));
        QObject::connect(pushButton_discon, SIGNAL(pressed()), Dialog, SLOT(OnDisconnect()));

        QMetaObject::connectSlotsByName(Dialog);
    } // setupUi

    void retranslateUi(QDialog *Dialog)
    {
        Dialog->setWindowTitle(QApplication::translate("Dialog", "LANchat", 0, QApplication::UnicodeUTF8));
        pushButton->setText(QApplication::translate("Dialog", "Send", 0, QApplication::UnicodeUTF8));
        pushButton_con->setText(QApplication::translate("Dialog", "Connect", 0, QApplication::UnicodeUTF8));
        lineEdit_subnet->setInputMask(QApplication::translate("Dialog", "000.000.000.000;_", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("Dialog", "Subnet:", 0, QApplication::UnicodeUTF8));
        checkBox_nobroadcast->setText(QApplication::translate("Dialog", "Do not send broadcast", 0, QApplication::UnicodeUTF8));
        lineEdit_netmask->setInputMask(QApplication::translate("Dialog", "00;_", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("Dialog", "/", 0, QApplication::UnicodeUTF8));
        lineEdit_nickname->setText(QApplication::translate("Dialog", "LANchatter", 0, QApplication::UnicodeUTF8));
        pushButton_discon->setText(QApplication::translate("Dialog", "Disconnect", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("Dialog", "Nickname:", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class Dialog: public Ui_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIALOG_H
