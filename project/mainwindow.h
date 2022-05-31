#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets/QWidget>
#include <QtCharts/QChartGlobal>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QGridLayout>
#include <aboutwindow.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    //void MainWindow::resizeEvent(QResizeEvent *evt);
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QGridLayout *gridLayout;

signals:
    void firstWindow();// Сигнал для первого окна на открытие

private slots:
    void on_pushButton_clicked();
    void on_back_clicked();

    void on_aboutButton_clicked();
   // void reportBox();



    void on_mainChartButton_clicked();

    void on_summaryButton_clicked();

    void on_otherChartButton_clicked();

    void on_PSChartButton_clicked();

private:
   // QtCharts::QChart *createLineChart(char* filename) const;
    Ui::MainWindow *ui;
    AboutWindow *aboutWindow;
    double *predicted_values;
    int was_prediction = 0;

    QtCharts::QChartView *mainChart;
    QtCharts::QChartView *secondChart;
    QtCharts::QChartView *thirdChart;

    QList<QtCharts::QChartView *> m_charts;
};
#endif // MAINWINDOW_H
