#include "graphwindow.h"

static constexpr QSize WINDOW_SIZE(640, 480);
static constexpr QSize BUTTON_SIZE(60, 60);
static constexpr int FONT_SIZE = 28;

GraphWindow::GraphWindow(QWidget *parent)
    : QMainWindow(parent)
{
    constructUI();
}

GraphWindow::~GraphWindow()
{
}

void GraphWindow::constructUI()
{
    resize(WINDOW_SIZE);

    QWidget *centralWidget = new QWidget;
    setCentralWidget(centralWidget);
    QVBoxLayout *mainLayout = new QVBoxLayout;
    centralWidget->setLayout(mainLayout);

    graphWidget = new QCustomPlot;
    graphWidget->addGraph();
    graphWidget->graph(0)->setLineStyle(QCPGraph::lsNone);
    QCPScatterStyle style(QCPScatterStyle::ssCircle, 6);
    style.setBrush(QBrush(QColor(113, 164, 217)));
    graphWidget->graph(0)->setScatterStyle(style);
    graphWidget->xAxis->setRange(0, X_MAX_GRAPH);
    graphWidget->yAxis->setRange(0, Y_MAX_GRAPH);

    mainLayout->addWidget(graphWidget);

    QHBoxLayout *butonsLayout = new QHBoxLayout;
    butonsLayout->setAlignment(Qt::AlignLeft);

    mainLayout->addLayout(butonsLayout);

    QFont butsFont;
    butsFont.setPixelSize(FONT_SIZE);

    QPushButton *butRun = new QPushButton;
    butRun->setFixedSize(BUTTON_SIZE);
    butRun->setText("\u25b6");
    butRun->setFont(butsFont);

    QPushButton * butPause = new QPushButton;
    butPause->setFixedSize(BUTTON_SIZE);
    butPause->setText("\u23F8");
    butPause->setFont(butsFont);

    QPushButton *butStop = new QPushButton;
    butStop->setFixedSize(BUTTON_SIZE);
    butStop->setText("\u23F9");
    butStop->setFont(butsFont);

    butonsLayout->addWidget(butRun);
    butonsLayout->addWidget(butPause);
    butonsLayout->addWidget(butStop);

    connect(butRun, &QPushButton::clicked, &asyncNumberGenerator, &AsyncNumberGenerator::run);
    connect(butPause, &QPushButton::clicked, &asyncNumberGenerator, &AsyncNumberGenerator::pause);
    connect(butStop, &QPushButton::clicked, &asyncNumberGenerator, &AsyncNumberGenerator::stop);

    connect(&asyncNumberGenerator, &AsyncNumberGenerator::newPoint, this, &GraphWindow::handleNewPoint);
    connect(&asyncNumberGenerator, &AsyncNumberGenerator::stopped, this, &GraphWindow::clearGraph, Qt::DirectConnection);
}

void GraphWindow::handleNewPoint(QPointF point)
{
    graphWidget->graph(0)->addData(point.x(), point.y());
    graphWidget->replot();
}

void GraphWindow::clearGraph() {
    graphWidget->graph(0)->data()->clear();
    graphWidget->replot();
}
