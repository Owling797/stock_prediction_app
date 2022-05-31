#include "aboutwindow.h"
#include "ui_aboutwindow.h"

AboutWindow::AboutWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::AboutWindow)
{
    ui->setupUi(this);
    this->setFixedSize(800, 600);
}

AboutWindow::~AboutWindow()
{
    delete ui;
}
