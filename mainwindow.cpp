#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "parser.h"
#include <QFileDialog>
#include <QDebug>
#include <algorithm>
#include "requester.h"
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QUrl>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    InitCustomPlot();
    Rec.initRequester("localhost", 8000, nullptr);
    //Rec.sendRequest("/login",);
    //Rec.sendMulishGetRequest();
}

void Requester_example(const QJsonObject &){

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_action_triggered()
{
    QString filename =  QFileDialog::getOpenFileName(
                this,
                "Открыть файл",
                QDir::currentPath(),
                "MIG files (*.mig)");
    qDebug() << filename.toUtf8();
    p.SetSourceFileName(filename);
    p.Data();
    CreateWidgetItems(p.GetNamesPar(), p.GetSubheadersSize());
    qDebug() << "test";
}

void MainWindow::CreateWidgetItems(namespar* subheaders, const size_t& subheaders_size){
    for (auto i = 0; i < subheaders_size; ++i){
        widgets_names.push_back(new QListWidgetItem);
        char name[7];
        strncpy_s(name, subheaders[i].NamePar, 6);
        widgets_names[i]->setText(name);
        ui->listWidget->addItem(widgets_names[i]);
    }
}

void MainWindow::on_pushButton_clicked()
{
    auto filter = ui->lineEdit->text();
    if (filter != ""){
        for(int i=0; i < ui->listWidget->count(); i++)
        {
            QListWidgetItem* item = ui->listWidget->item(i);
            qDebug() << filter << " " << item->text().trimmed();
            item->setHidden(false);
            if (filter != item->text().trimmed()) {
                item->setHidden(true);
            }
        }
    }
    else {
        for(int i=0; i < ui->listWidget->count(); i++)
        {
            QListWidgetItem* item = ui->listWidget->item(i);
            item->setHidden(false);
        }
    }
}

void MainWindow::InitCustomPlot()
{
    QSharedPointer<QCPAxisTickerFixed> fixedTicker(new QCPAxisTickerFixed);

    fixedTicker->setTickStep(1.0); // tick step shall be 1.0
    fixedTicker->setScaleStrategy(QCPAxisTickerFixed::ssNone); // and no scaling of the tickstep (like multiples or powers) is allowed

    ui->customPlot->setInteraction(QCP::iRangeZoom,true);   // Включаем взаимодействие удаления/приближения
    ui->customPlot->setInteraction(QCP::iRangeDrag, true);  // Включаем взаимодействие перетаскивания графика
    ui->customPlot->axisRect()->setRangeDrag(Qt::Horizontal | Qt::Vertical);   // Включаем перетаскивание по горизонтальной и вертикальной оси
    ui->customPlot->axisRect()->setRangeZoom(Qt::Horizontal | Qt::Vertical);   // Включаем удаление/приближение по горизонтальной и вертикальной оси

        // Настраиваем шрифт по осям координат
    ui->customPlot->xAxis->setTickLabelFont(QFont(QFont().family(), 8));
    ui->customPlot->yAxis->setTickLabelFont(QFont(QFont().family(), 8));

        /* Делаем видимыми оси X и Y по верхней и правой границам графика,
         * но отключаем на них тики и подписи координат
         * */
    ui->customPlot->xAxis2->setVisible(true);
    ui->customPlot->yAxis2->setVisible(true);
    ui->customPlot->xAxis2->setTicks(false);
    ui->customPlot->yAxis2->setTicks(false);
    ui->customPlot->xAxis2->setTickLabels(false);
    ui->customPlot->yAxis2->setTickLabels(false);

    ui->customPlot->yAxis->setTickLabelColor(QColor(Qt::red)); // Красный цвет подписей тиков по Оси Y
    ui->customPlot->legend->setVisible(true);   //Включаем Легенду графика
        // Устанавливаем Легенду в левый верхний угол графика
    ui->customPlot->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignLeft|Qt::AlignTop);

}

void MainWindow::on_pushButton_2_clicked()
{
    auto current_item = ui->listWidget->currentItem();
    SetDataPlot(current_item);
}

void MainWindow::SetDataPlot(QListWidgetItem* list_item)
{
    auto t = ui->listWidget->item(0);
    std::vector<std::any>* time_data = p.GetSubheaderByName(t->text());
    qDebug() << time_data->size();
    std::vector<std::any>* subheader = p.GetSubheaderByName(list_item->text());
    qDebug() << subheader->size();
    QVector <double> time;
    QVector <double> data_graphs;
    for (auto i = 0; i < time_data->size(); ++i)
    {
        if (long* l_type = std::any_cast<long>(&(*subheader)[i])) {
               data_graphs.push_back(static_cast<double>(std::any_cast<long>((*subheader)[i])));
            } else if (double* d = std::any_cast<double>(&(*subheader)[i])) {
               data_graphs.push_back(std::any_cast<double>((*subheader)[i]));
            } else if (float* f = std::any_cast<float>(&(*subheader)[i])) {
               data_graphs.push_back(static_cast<double>(std::any_cast<float>((*subheader)[i])));
            } else if (int* in = std::any_cast<int>(&(*subheader)[i])) {
                data_graphs.push_back(static_cast<double>(std::any_cast<int>((*subheader)[i])));
            } else if (short* s = std::any_cast<short>(&(*subheader)[i])) {
                data_graphs.push_back(static_cast<double>(std::any_cast<short>((*subheader)[i])));
            } else {
              std::cerr << "subheader is another type or unset\n";
            }

        if (long* int_type = std::any_cast<long>(&(*time_data)[i])) {
               time.push_back(static_cast<double>(std::any_cast<long>((*time_data)[i])));
            } else if (double* d = std::any_cast<double>(&(*time_data)[i])) {
               time.push_back(std::any_cast<double>((*time_data)[i]));
            } else if (float* f = std::any_cast<float>(&(*time_data)[i])) {
               time.push_back(static_cast<double>(std::any_cast<float>((*time_data)[i])));
            } else if (int* in = std::any_cast<int>(&(*time_data)[i])) {
               time.push_back(static_cast<double>(std::any_cast<int>((*time_data)[i])));
            } else if (short* s = std::any_cast<short>(&(*time_data)[i])) {
               time.push_back(static_cast<double>(std::any_cast<short>((*time_data)[i])));
            } else {
               std::cerr << "time_data is another type or unset\n";
            }
        qDebug() << data_graphs[i];
        qDebug() << time[i];
    }

    qDebug() << "test again";
    ui->customPlot->clearPlottables();
    ui->customPlot->addGraph();
    ui->customPlot->graph(0)->setName(list_item->text());
    ui->customPlot->graph(0)->setData(data_graphs, time);
    ui->customPlot->xAxis->setLabel("time");
    ui->customPlot->yAxis->setLabel(list_item->text());
    ui->customPlot->replot();
}

void MainWindow::on_pushButton_change_x_clicked()
{
    auto str = ui->lineEdit_min_x->text();
    bool ok, ok2;
    int min_x = str.toInt(&ok);
    auto str2 = ui->lineEdit_max_x->text();
    int max_x = str2.toInt(&ok2);
    if ((!ok) || (!ok2))
    {
        std::cerr << "Incorrect input";
        return;
    }
    ui->customPlot->xAxis->setRange(min_x, max_x);
    ui->customPlot->replot();
    qDebug() << min_x;
    qDebug() << max_x;
}


void MainWindow::on_pushButton_change_y_clicked()
{
    auto str = ui->lineEdit_min_y->text();
    bool ok, ok2;
    int min_y = str.toInt(&ok);
    auto str2 = ui->lineEdit_max_y->text();
    int max_y = str2.toInt(&ok2);
    if ((!ok) || (!ok2))
    {
        std::cerr << "Incorrect input";
        return;
    }
    ui->customPlot->yAxis->setRange(min_y, max_y);
    ui->customPlot->replot();
    qDebug() << min_y;
    qDebug() << max_y;
}

