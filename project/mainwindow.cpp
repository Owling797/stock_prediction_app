
#include <QMessageBox>
#include <QPixmap>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QChartView>
#include <QLabel>
#include <QDateTimeAxis>
#include <QValueAxis>
#include <QIcon>

#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <iostream>

#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include <vector>
#include <string>
#include <algorithm>
#include <iterator>
#include <cstring>
#include <stdlib.h>
#include <QtCore/QDir>
#include <QtQuick/QQuickView>
#include <QtQml/QQmlEngine>
#include <QDateTime>
#include <QTextBrowser>

#include "mainwindow.h"
#include "ui_mainwindow.h"

using namespace QtCharts;
using namespace std;
using namespace boost::asio;
using ip::tcp;
using std::string;
using std::cout;
using std::endl;

#define pressedButtonDesign "QPushButton{background-color: rgb(7, 200, 209);color: rgb(255, 255, 255);font: 900 10pt \"Arial Black\";border-radius:10px;}"
#define basicButtonDesign   "QPushButton{color: rgb(255, 255, 255);font: 900 10pt \"Arial Black\";border-radius:10px;}"

#define INP_SIZE 5
#define OUTP_SIZE (7 + INP_SIZE)

#define MAIN_CHART_NAME     "Predicted values"
#define DIVIDEND_CHART_NAME "Expected dividend yield"
#define PS_CHART_NAME       "P/S"


/*static int localMinUtil(int arr[], int low, int high, int n)
{
    // Find index of middle element
    int mid = low + (high - low)/2;

    // Compare middle element with its neighbours
    // (if neighbours exist)
    if ((mid == 0 || arr[mid-1] > arr[mid]) &&
            (mid == n-1 || arr[mid+1] > arr[mid]))
        return mid;

    // If middle element is not minima and its left
    // neighbour is smaller than it, then left half
    // must have a local minima.
    else if (mid > 0 && arr[mid-1] < arr[mid])
        return localMinUtil(arr, low, (mid -1), n);

    // If middle element is not minima and its right
    // neighbour is smaller than it, then right half
    // must have a local minima.
    return localMinUtil(arr, (mid + 1), high, n);
}

// A wrapper over recursive function localMinUtil()
int localMin(int arr[], int n)
{
    return localMinUtil(arr, 0, n-1, n);
}*/


static void clearLayout(QLayout* layout, bool deleteWidgets = true)
{
    while (QLayoutItem* item = layout->takeAt(0))
    {
        if (deleteWidgets)
        {
            if (QWidget* widget = item->widget())
                widget->deleteLater();
        }
        if (QLayout* childLayout = item->layout())
            clearLayout(childLayout, deleteWidgets);
        delete item;
    }
}


static QtCharts::QChartView *create_chart(double *predicted_values, string name){
    QtCharts::QChart::ChartTheme theme = static_cast<QtCharts::QChart::ChartTheme>(1);
    QtCharts::QLineSeries *series = new QtCharts::QLineSeries();
    QtCharts::QLineSeries *input_series = new QtCharts::QLineSeries();
    for(int i=0; i<5; i++){
        input_series->append(i+1, predicted_values[i]);
    }
    for(int i=4; i<OUTP_SIZE; i++){
        series->append(i+1, predicted_values[i]);
    }

    QtCharts::QChart *chart = new QtCharts::QChart();
    chart->addSeries(series);
    chart->addSeries(input_series);
    chart->legend()->hide();
    chart->setTitle(name.c_str());
    chart->setAnimationOptions(QChart::SeriesAnimations);
    chart->setAnimationDuration(200);
    chart->createDefaultAxes();

    QtCharts::QChartView *chartView = new QtCharts::QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->chart()->setTheme(theme); // новая тема для графиков
    chartView->setRubberBand(QChartView::HorizontalRubberBand);

   return chartView;
}


static double* client(int num1, int num2, int num3, int num4, int num5){
    string str1 = to_string(num1);
    string str2 = to_string(num2);
    string str3 = to_string(num3);
    string str4 = to_string(num4);
    string str5 = to_string(num5);

    const string string_to_server = str1 + " " + str2 + " " + str3 + " " + str4 + " " + str5;
    double* predicted_values = new double[OUTP_SIZE];
    predicted_values[0] = num1;
    predicted_values[1] = num2;
    predicted_values[2] = num3;
    predicted_values[3] = num4;
    predicted_values[4] = num5;
    boost::asio::io_service io_service;

    //socket creation
    tcp::socket socket(io_service);

    //connection
    socket.connect(tcp::endpoint(boost::asio::ip::address::from_string("127.0.0.1"), 8000));

    // request/message from client
    boost::system::error_code error;

    //boost::asio::write(socket, boost::asio::buffer(msg), error);
    boost::asio::write(socket, boost::asio::buffer(string_to_server), error);

    // getting response from server
    boost::asio::streambuf receive_buffer;
    boost::asio::read(socket, receive_buffer, boost::asio::transfer_all(), error);

    if (!(error && error != boost::asio::error::eof)) {
        const char* data = boost::asio::buffer_cast<const char*>(receive_buffer.data());
        size_t i = INP_SIZE;
        char delims[] = " ";
        char* tok;
        char* data_copy = strdup(data);
        tok = strtok(data_copy, delims);
        while (tok) {
            double x = atof(tok);
            predicted_values[i] = x;
            i++;
            tok = strtok(NULL, delims);
        }
    }
    return predicted_values;
}

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
   /* ui->aboutButton->setStyleSheet(basicButtonDesign
                              "QPushButton:hover{color: rgb(255, 255, 255);font: 900 10pt \"Arial Black\";border-radius:10px;}");
    ui->reportButton->setStyleSheet(basicButtonDesign
                              "QPushButton:hover{color: rgb(255, 255, 255);font: 900 10pt \"Arial Black\";border-radius:10px;}");
    ui->pushButton->show();
    //ui->back->show();
    ui->reportButton->show();*/
    // Инициализируем второе окно с графиками
    aboutWindow = new AboutWindow();
       /*QMainWindow window;
       window.resize(1000, 600);
       window.setFixedSize(window.sizeHint());
       window.show();*/

    this->setFixedSize(1000, 650);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete []predicted_values;
}

/*void MainWindow::resizeEvent(QResizeEvent *evt) {
    QPixmap bkgnd("C:/Users/Ludmila/Documents/technopark/stonks_project/images/dark_blue");
    //QPixmap bkgnd("C:/Users/Ludmila/Documents/technopark/stonks_project/images/light_blue.png");
       bkgnd = bkgnd.scaled(size(), Qt::IgnoreAspectRatio);
       QPalette p = palette();
       p.setBrush(QPalette::Window, bkgnd);
       setPalette(p);
       QMainWindow::resizeEvent(evt);
}*/

void MainWindow::on_pushButton_clicked()
{
   // ui->pushButton->setText("Pushed");
    int num1 = ui->spinBox->text().toInt();
    int num2 = ui->spinBox_2->text().toInt();
    int num3 = ui->spinBox_3->text().toInt();
    int num4 = ui->spinBox_4->text().toInt();
    int num5 = ui->spinBox_5->text().toInt();


//![1]///////////////////////client_part
    // обращение на сервер за новыми значениями
    predicted_values = client(num1, num2, num3, num4, num5);
    if (!predicted_values) {
       // cout<<"server error"<<endl;
        return;
    }
    for(size_t i=0;i<OUTP_SIZE;i++)
    std::cout<<predicted_values[i]<< ", ";
    std::cout<<std::endl;

    clearLayout(ui->firstAddVerticalLayout);
    clearLayout(ui->secondAddVerticalLayout);
    clearLayout(ui->mainChartVerticalLayout);

    ui->mainChartButton->setStyleSheet(basicButtonDesign);
    ui->otherChartButton->setStyleSheet(basicButtonDesign);
    ui->summaryButton->setStyleSheet(pressedButtonDesign);
    ui->PSChartButton->setStyleSheet(basicButtonDesign);

//![2]/////////////////////// main_chart
    was_prediction=1;
    m_charts << create_chart(predicted_values, MAIN_CHART_NAME);
    ui->mainChartVerticalLayout->addWidget(create_chart(predicted_values, MAIN_CHART_NAME));

//![3]/////////////////////////// функция дивидендной доходности
    double yield[OUTP_SIZE];
    for (size_t j = 0; j < OUTP_SIZE; j++) {
        yield[j] = 8/predicted_values[j]*100;
    }
    m_charts << create_chart(yield, DIVIDEND_CHART_NAME);
    ui->firstAddVerticalLayout->addWidget(create_chart(yield, DIVIDEND_CHART_NAME));

    double psMultiplier[OUTP_SIZE];
    for (size_t j = 0; j < OUTP_SIZE; j++){
        psMultiplier[j] = predicted_values[j] / 54.84;
    }
    m_charts << create_chart(psMultiplier, PS_CHART_NAME);
    ui->secondAddVerticalLayout->addWidget(create_chart(psMultiplier, PS_CHART_NAME));

std::cout<<"good"<<std::endl;
}


static double get_max_min(double *arr, int choise = 1) {
    const size_t N = OUTP_SIZE-INP_SIZE;
    double result = arr[0];
    for (size_t j = 0; j < N; ++j) {
        if ((choise == 1 && arr[j] > result) || (choise == -1 && arr[j] < result)) {
            result = arr[j];
        }
    }
    return result;
}

static double get_median(double *arr){
    const size_t N = OUTP_SIZE-INP_SIZE;
    double array[N];
    for (size_t i = 0; i< N; i++) {
        array[i] = arr[i];
    }

      // sort
    double temp;
    for (size_t i = 0; i < N-1; i++){
        for (size_t j = i+1; j < N; j++)
        {
            if (array[i] > array[j]) {
                temp = array[i];
                array[i] = array[j];
                array[j] = temp;
            }
        }
    }

    return (array[N/2] + array[N/2-1]) / 2;
}

static string get_report_by_chart(double *arr, string dimension) {
    double max = get_max_min(arr, 1);
    double min = get_max_min(arr, -1);
    double med = get_median(arr);

    string report = "Minimum: " + to_string(min) + " " + dimension +
                    "\nMaximum: " + to_string(max) + " " + dimension +
                    "\nMedian: " + to_string(med) + " " + dimension;
    return report;
}

static string get_html_for_report(string report) {
    string html_report = "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\"><html><head><meta name=\"qrichtext\" content=\"1\" /><meta charset=\"utf-8\" /><style type=\"text/css\">p, li { white-space: pre-wrap; }</style></head><body style=\" font-family:'Arial Black'; font-size:9pt; font-weight:400; font-style:normal;\">";
    html_report += "<p style=\" margin-top:12px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:10pt;\">";
    html_report+= report;
    html_report +="</span></p>";
    html_report += "</body></html>";
    return html_report;
}

static string get_html_for_chart(double *arr, string dimension) {
    double arr1[OUTP_SIZE-INP_SIZE];
    for(int i = INP_SIZE; i < OUTP_SIZE; i++) {
        arr1[i-INP_SIZE] = arr[i];
    }
    double max1 = get_max_min(arr1, 1);
    double min1 = get_max_min(arr1, -1);
    double med1 = get_median(arr1);


    string html_report = "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\"><html><head><meta name=\"qrichtext\" content=\"1\" /><meta charset=\"utf-8\" /><style type=\"text/css\">p, li { white-space: pre-wrap; }</style></head><body style=\" font-family:'Arial'; font-size:9pt; font-weight:400; font-style:normal;\">";

    html_report += "<p style=\" margin-top:12px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:12pt;\">";
    html_report+= "Minimum: " + to_string(min1) + " " + dimension;
    html_report +="</span></p>";
    html_report +="<p></p>";
    html_report += "<p style=\" margin-top:12px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:12pt;\">";
    html_report+= "Maximum: " + to_string(max1) + " " + dimension;
    html_report +="</span></p>";
    html_report +="<p></p>";
    html_report += "<p style=\" margin-top:12px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:12pt;\">";
    html_report+= "Median: " + to_string(med1) + " " + dimension;
    html_report +="</span></p>";
    html_report +="<p></p>";

    html_report += "</body></html>";
    return html_report;
}

void MainWindow::on_back_clicked() {
    this->close();      // Закрываем окно
    emit firstWindow(); // И вызываем сигнал на открытие главного окна
}

void MainWindow::on_aboutButton_clicked() {
    ui->aboutButton->setStyleSheet(pressedButtonDesign);
    aboutWindow->show();

}

void MainWindow::on_mainChartButton_clicked() {
    if (was_prediction) {
        ui->mainChartButton->setStyleSheet(pressedButtonDesign);
        ui->otherChartButton->setStyleSheet(basicButtonDesign);
        ui->summaryButton->setStyleSheet(basicButtonDesign);
        ui->PSChartButton->setStyleSheet(basicButtonDesign);

        clearLayout(ui->firstAddVerticalLayout);
        clearLayout(ui->secondAddVerticalLayout);
        clearLayout(ui->mainChartVerticalLayout);

        ui->mainChartVerticalLayout->addWidget(create_chart(predicted_values, MAIN_CHART_NAME));

        string report = get_report_by_chart(predicted_values, "RUB");
        string html_code = get_html_for_chart(predicted_values, "RUB");
        QTextBrowser *textBrowser = new QTextBrowser();
        textBrowser->setStyleSheet(QString("color:white; border:none;"));

        textBrowser->setMarkdown(QString(report.c_str()));
        textBrowser->setHtml(QString(html_code.c_str()));
        ui->firstAddVerticalLayout->addWidget(textBrowser);
    }
}


void MainWindow::on_summaryButton_clicked() {
    //on_pushButton_clicked();
    if (was_prediction) {
        ui->mainChartButton->setStyleSheet(basicButtonDesign);
        ui->otherChartButton->setStyleSheet(basicButtonDesign);
        ui->summaryButton->setStyleSheet(pressedButtonDesign);
        ui->PSChartButton->setStyleSheet(basicButtonDesign);
        clearLayout(ui->firstAddVerticalLayout);
        clearLayout(ui->secondAddVerticalLayout);
        clearLayout(ui->mainChartVerticalLayout);

        ui->mainChartVerticalLayout->addWidget(create_chart(predicted_values, MAIN_CHART_NAME));

        double yield[OUTP_SIZE];// = new double[100];
        for (size_t j = 0; j < OUTP_SIZE; j++){
            yield[j] = 8/predicted_values[j]*100;
        }
        ui->firstAddVerticalLayout->addWidget(create_chart(yield, DIVIDEND_CHART_NAME));


        double psMultiplier[OUTP_SIZE];
        for (size_t j = 0; j < OUTP_SIZE; j++){
            psMultiplier[j] = predicted_values[j] / 54.84;
        }
        ui->secondAddVerticalLayout->addWidget(create_chart(psMultiplier, PS_CHART_NAME));
    }
}


void MainWindow::on_otherChartButton_clicked() {
    if(was_prediction) {
        ui->otherChartButton->setStyleSheet(pressedButtonDesign);
        ui->mainChartButton->setStyleSheet(basicButtonDesign);
        ui->summaryButton->setStyleSheet(basicButtonDesign);
        ui->PSChartButton->setStyleSheet(basicButtonDesign);

        double yield[OUTP_SIZE];
        for (size_t j = 0; j < OUTP_SIZE; j++){
            yield[j] = 8/predicted_values[j]*100;
        }
        clearLayout(ui->firstAddVerticalLayout);
        clearLayout(ui->secondAddVerticalLayout);
        clearLayout(ui->mainChartVerticalLayout);

        for(size_t i=0;i<OUTP_SIZE;i++)
        std::cout<<predicted_values[i]<< ", ";
        std::cout<<std::endl;
        ui->mainChartVerticalLayout->addWidget(create_chart(yield, DIVIDEND_CHART_NAME));

        string report = get_report_by_chart(yield, "%");
        string html_code = get_html_for_chart(yield, "%");
        QTextBrowser *textBrowser = new QTextBrowser();
        textBrowser->setStyleSheet(QString("color:white; border:none;"));

        textBrowser->setMarkdown(QString(report.c_str()));
        textBrowser->setHtml(QString(html_code.c_str()));
        ui->firstAddVerticalLayout->addWidget(textBrowser);
    }
}


void MainWindow::on_PSChartButton_clicked() {
    if(was_prediction) {
        ui->PSChartButton->setStyleSheet(pressedButtonDesign);
        ui->otherChartButton->setStyleSheet(basicButtonDesign);
        ui->mainChartButton->setStyleSheet(basicButtonDesign);
        ui->summaryButton->setStyleSheet(basicButtonDesign);

        double psMultiplier[OUTP_SIZE];
        for (size_t j = 0; j < OUTP_SIZE; j++){
            psMultiplier[j] = predicted_values[j] / 54.84;
        }
        clearLayout(ui->firstAddVerticalLayout);
        clearLayout(ui->secondAddVerticalLayout);
        clearLayout(ui->mainChartVerticalLayout);

        for(size_t i=0;i<OUTP_SIZE;i++)
        std::cout<<predicted_values[i]<< ", ";
        std::cout<<std::endl;
        ui->mainChartVerticalLayout->addWidget(create_chart(psMultiplier, PS_CHART_NAME));

        string report = get_report_by_chart(psMultiplier, "");
        string html_code = get_html_for_chart(psMultiplier, "");
        QTextBrowser *textBrowser = new QTextBrowser();
        textBrowser->setStyleSheet(QString("color:white; border:none;"));

        textBrowser->setMarkdown(QString(report.c_str()));
        textBrowser->setHtml(QString(html_code.c_str()));
        ui->firstAddVerticalLayout->addWidget(textBrowser);
    }
}
