#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QMainWindow>

#include "ui_main_window.h"
#include "gl_widget.h"

class MainWindow
    : public QMainWindow
{
    Q_OBJECT

    public:
        MainWindow(QWidget *parent = 0);
        virtual ~MainWindow();

    private:
        Ui::MainWindow ui;

        GLWidget *_glWidget;



};
#endif // MAIN_WINDOW_H
