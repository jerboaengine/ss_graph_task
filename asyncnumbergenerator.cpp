#include "asyncnumbergenerator.h"

Generator::Generator(QObject *parent)
    : QObject{parent}
{

}

void Generator::run()
{
    QMutexLocker locker(&mutex);
    state = Run;
    emit resume();
}

void Generator::stop()
{
    QMutexLocker locker(&mutex);
    state = Stop;
}

void Generator::pause()
{
    QMutexLocker locker(&mutex);
    state = Pause;
}

void Generator::waitingResumeEvent()
{
    QEventLoop el;
    connect(this, &Generator::resume, &el, &QEventLoop::quit);
    el.exec();
}

void Generator::doGenerate()
{
    QRandomGenerator *rg = QRandomGenerator::global();
    while(getState() != Stop) {
        if (getState() == Run) {
            QThread::msleep(300); // work simulation
            emit newPoint(QPointF(rg->bounded(X_MAX_GRAPH), rg->bounded(Y_MAX_GRAPH)));
        } else { // if "getState() == Pause" do nothing
            waitingResumeEvent();
        }
    }
}

Generator::State Generator::getState()
{
    QMutexLocker locker(&mutex);
    return state;
}

AsyncNumberGenerator::AsyncNumberGenerator(QObject *parent)
    : QObject{parent}
{
    generator.moveToThread(&thread);
    connect(&thread, &QThread::started, &generator, &Generator::doGenerate);
    connect(&thread, &QThread::finished, this, &AsyncNumberGenerator::stopped);
    connect(&generator, &Generator::newPoint, this, &AsyncNumberGenerator::newPoint);
}

AsyncNumberGenerator::~AsyncNumberGenerator()
{
    generator.stop();
    thread.quit();
    thread.wait();
}

void AsyncNumberGenerator::run()
{
    generator.run();
    if (!thread.isRunning()) {
        thread.start();
    }
}

void AsyncNumberGenerator::stop()
{
    if (!thread.isFinished()) {
        generator.stop();
        thread.quit();
    }
}

void AsyncNumberGenerator::pause()
{
    generator.pause();
}
