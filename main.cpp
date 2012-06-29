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
#include <QtGui/QApplication>
#include "dialog.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Dialog chatInterface;
    chatInterface.show();
    return a.exec();

}
