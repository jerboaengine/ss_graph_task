#ifndef GRAPHWINDOW_H
#define GRAPHWINDOW_H

#include <QMainWindow>
#include "asyncnumbergenerator.h"
#include "qcustomplot.h"

class GraphWindow : public QMainWindow
{
    Q_OBJECT

public:
    GraphWindow(QWidget *parent = nullptr);
    ~GraphWindow();

public slots:
    void handleNewPoint(QPointF point);
    void clearGraph();

private:
    void constructUI();

    AsyncNumberGenerator asyncNumberGenerator;
    QCustomPlot *graphWidget;
};
#endif // GRAPHWINDOW_H
