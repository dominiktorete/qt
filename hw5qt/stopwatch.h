#ifndef STOPWATCH_H
#define STOPWATCH_H
#include <QObject>
class Stopwatch : public QObject
{

    Q_OBJECT

public:

    explicit Stopwatch(QObject* parent = nullptr);



signals:

    void sig_Start();
    void sig_Stop();
    void sig_Clear();
    void sig_Round();
};

#endif // STOPWATCH_H
