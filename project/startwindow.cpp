#include "startwindow.h"
#include "ui_startwindow.h"
#include <QMessageBox>

StartWindow::StartWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::StartWindow)
{
    ui->setupUi(this);
    // Инициализируем второе окно с графиками
    mainWindow = new MainWindow();
    usageWindow = new UsageWindow();
    // подключаем к слоту запуска главного окна по кнопке во втором окне
    connect(mainWindow, &MainWindow::firstWindow, this, &StartWindow::show);

    QPixmap bkgnd("C:/Users/Ludmila/Documents/technopark/stonks_project/images/dark_blue");
    //QPixmap bkgnd("C:/Users/Ludmila/Documents/technopark/stonks_project/images/light_blue.png");
       bkgnd = bkgnd.scaled(size(), Qt::IgnoreAspectRatio);
       QPalette p = palette();
       p.setBrush(QPalette::Window, bkgnd);
       setPalette(p);

       qApp->setStyleSheet("QMessageBox QPushButton { color: rgb(255, 255, 255);\nbackground-color:rgb(39, 203, 53);\nborder:5px; }"
                           "QMessageBox { color: rgb(255, 255, 255);\n background-color:rgb(35, 37, 84);\nborder:10px; }");
   //qApp->setStyleSheet("QMessageBox { color: rgb(255, 255, 255);\n background-color:rgb(35, 37, 84);\nborder:10px; }");

       ui->startButton->setStyleSheet(QString("border-radius:9px;\nbackground-color: rgb(70, 224, 130);\ncolor: rgb(255, 255, 255);\nfont: 900 14pt \"Arial Black\";"));
       ui->usage->setStyleSheet(QString("border-radius:9px;\nbackground-color: rgb(70, 224, 130);\ncolor: rgb(255, 255, 255);\nfont: 900 14pt \"Arial Black\";"));


        this->setFixedSize(800, 600);

}

StartWindow::~StartWindow()
{
    delete ui;
}

void StartWindow::resizeEvent(QResizeEvent *evt) {
    QPixmap bkgnd("C:/Users/Ludmila/Documents/technopark/stonks_project/images/dark_blue");
    //QPixmap bkgnd("C:/Users/Ludmila/Documents/technopark/stonks_project/images/light_blue.png");
       bkgnd = bkgnd.scaled(size(), Qt::IgnoreAspectRatio);
       QPalette p = palette();
       p.setBrush(QPalette::Window, bkgnd);
       setPalette(p);
       QMainWindow::resizeEvent(evt);
}

static QString getText() {
    QString usage = "This app can show you the prediction of stocks yield.\n"
                    "All you need is:\n"
                    "\t- check the yield of your stocks on the last 5 days and round up them\n"
                    "\t- tap the Start button\n"
                    "\t- enter them in the fields, one number in each field, the numbers must be integers\n"
                    "\n"
                    "That's all! You will see your prediction in a few seconds!";
    return usage;
}

void StartWindow::on_usage_clicked()
{
// справка как пользоваться в новом окне без закрытия этого
    usageWindow->show();

}


void StartWindow::on_startButton_clicked()
{
    mainWindow->show();
    this->close();
}

