#include "usagewindow.h"
#include "ui_usagewindow.h"

UsageWindow::UsageWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::UsageWindow)
{
    ui->setupUi(this);
    this->setFixedSize(800, 600);
}

UsageWindow::~UsageWindow()
{
    delete ui;
}
