#ifndef ASYNCNUMBERGENERATOR_H
#define ASYNCNUMBERGENERATOR_H

#include <QThread>
#include <QPointF>
#include <QMutex>
#include <QRandomGenerator>
#include <QEventLoop>

constexpr double X_MAX_GRAPH = 100;
constexpr double Y_MAX_GRAPH = 60;

class Generator : public QObject
{
    Q_OBJECT
public:
    explicit Generator(QObject *parent = nullptr);

    void stop();
    void pause();

    enum State {Run, Stop, Pause};

public slots:
    void doGenerate();
    void run();

signals:
    void newPoint(QPointF point);
    void resume();

private:
    State getState();

    void waitingResumeEvent();
    State state = Stop;
    QMutex mutex;
};

class AsyncNumberGenerator : public QObject
{
    Q_OBJECT
public:
    explicit AsyncNumberGenerator(QObject *parent = nullptr);
    ~AsyncNumberGenerator();

public slots:
    void run();
    void stop();
    void pause();

signals:
    void startGenerator();
    void newPoint(QPointF point);
    void stopped();

private:
    QThread thread;
    Generator generator;
};

#endif // ASYNCNUMBERGENERATOR_H
