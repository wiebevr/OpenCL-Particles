#include "main_window.h"
#include "simulator.h"

#include <QVBoxLayout>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    _glWidget = new GLWidget(this);

    this->centralWidget()->setLayout(new QVBoxLayout());
    this->centralWidget()->layout()->addWidget(_glWidget);

}

MainWindow::~MainWindow()
{

}
