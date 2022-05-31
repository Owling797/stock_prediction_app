#ifndef STARTWINDOW_H
#define STARTWINDOW_H

#include <QMainWindow>
#include <mainwindow.h>
#include <usagewindow.h>

namespace Ui {
    class StartWindow;
}

class StartWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit StartWindow(QWidget *parent = nullptr);
    ~StartWindow();
    void StartWindow::resizeEvent(QResizeEvent *evt);

private slots:
    void on_usage_clicked();

    void on_startButton_clicked();

private:
    Ui::StartWindow *ui;
    MainWindow *mainWindow; // второстепенное по сути
    UsageWindow *usageWindow;
};

#endif // STARTWINDOW_H
