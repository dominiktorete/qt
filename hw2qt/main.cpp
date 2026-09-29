#include <iostream>
#include <QAbstractSocket>
#include <QSqlDatabase>

int main()
{
    QAbstractSocket A (QAbstractSocket::SocketType::TcpSocket, nullptr);
    QSqlDatabase data;
    A.close();
    return 0;
}
