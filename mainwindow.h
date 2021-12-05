#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidget>
#include "parser.h"
#include "qcustomplot.h"
#include "requester.h"

#include <vector>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void CreateWidgetItems(namespar* subheaders, const size_t& subheaders_size);

private slots:
    void on_action_triggered();
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_change_x_clicked();

    void on_pushButton_change_y_clicked();

private:
    Ui::MainWindow *ui;
    parser p;
    std::vector <QListWidgetItem*> widgets_names;
    void InitCustomPlot();
    void SetDataPlot(QListWidgetItem* list_item);
    Requester Rec;
};

#endif // MAINWINDOW_H
