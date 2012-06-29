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
#ifndef BUFFER_H
#define BUFFER_H
class ReadBuffer :public QObject
{
    QByteArray buffer;
public:
    QByteArray Read(); //Cleans the buffer
    QByteArray Look(); //Does not clean the buffer


};

class WriteBuffer:public QObject
{
    QByteArray buffer;

public:
    void Append();
    void Clear();

};

#endif // BUFFER_H
