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
