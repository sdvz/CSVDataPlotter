#include "dialog.h"
#include "ui_dialog.h"
#include <QString>
#include <QFile>
#include <QDebug>
#include <QTextStream>
#include <QFileInfo>

static const float minX = -1;
static const float maxX = 32;
static const float minY = 0;
static const float maxY = 1400;

void setModelAndDataWithCSVData(QVector<double> *xData, QVector<double> *yData, QStandardItemModel *model, QString datafilepath);
void writeXYDataToFile(QVector<double> *xData, QVector<double> *yData, QString filepath);
void backupDataFile(QString datafilepath);

//SLOTS
void itemchangedslot(QStandardItem *item);


Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    QString myDataFilePath = "C:/Users/Sage/Downloads/mvcdata.csv";
    model = new QStandardItemModel(this);
    setModelAndDataWithCSVData(&xData, &yData, model, myDataFilePath);
    backupDataFile(myDataFilePath);

    ui->tableView->setModel(model);

    connect(model, SIGNAL(itemChanged(QStandardItem*)), this,SLOT(itemchangedslot(QStandardItem*)) );

    ui->plot->clearGraphs();
    ui->plot->addGraph();
    ui->plot->graph(0)->setData(xData,yData);
    ui->plot->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle));
    ui->plot->xAxis->setRange(minX,maxX);
    ui->plot->yAxis->setRange(minY,maxY);
}

Dialog::~Dialog()
{
    delete ui;
}

void setModelWithCSVData(QStandardItemModel *model, QString datafilepath){


    QString fileData;
    QStringList dataFromRow;
    QStringList rowsOfData;
    QFile stream(datafilepath);

    fileData.clear();
    dataFromRow.clear();
    rowsOfData.clear();

    if (!stream.open(QFile::ReadOnly | QFile::WriteOnly)){

        qDebug() << "Could not open file. Make sure file has R/W Capabilities and retry.";
        return;
    }

    fileData = stream.readAll();
    rowsOfData = fileData.split('\n');
    stream.close();


    QStringList headers = rowsOfData.at(0).split(',');;

    model->setHorizontalHeaderLabels(headers);



    for (int row = 1; row < (rowsOfData.size()); row++){

        if(!rowsOfData.at(row).isEmpty()){
        dataFromRow = rowsOfData.at(row).split(',');

        for (int col = 0; col < dataFromRow.size(); col++)
            {
                //set model to data
                QStandardItem *item = new QStandardItem(dataFromRow.at(col));
                model->setItem(row-1, col, item);
            }
        }
    }

}

void setModelAndDataWithCSVData(QVector<double> *xData, QVector<double> *yData, QStandardItemModel *model, QString datafilepath){


    QString fileData;
    QStringList dataFromRow;
    QStringList rowsOfData;
    QFile stream(datafilepath);


    fileData.clear();
    dataFromRow.clear();
    rowsOfData.clear();

    if (!stream.open(QFile::ReadOnly | QFile::WriteOnly)){

        qDebug() << "Could not open file. Make sure file has R/W Capabilities and retry.";
        return;
    }

    fileData = stream.readAll();
    rowsOfData = fileData.split('\n');
    stream.close();


    QStringList headers = rowsOfData.at(0).split(',');;

    model->setHorizontalHeaderLabels(headers);



    for (int row = 1; row < (rowsOfData.size()); row++){

        if(!rowsOfData.at(row).isEmpty()){
        dataFromRow = rowsOfData.at(row).split(',');

//        qDebug() << dataFromRow.at(0).toDouble() << dataFromRow.at(1).toDouble();

        xData->append(dataFromRow.at(0).toDouble());
        yData->append(dataFromRow.at(1).toDouble());

//        qDebug() << xData.at(row - 1) << yData.at(row - 1);

        for (int col = 0; col < dataFromRow.size(); col++)
            {
                //set model to data
                QStandardItem *item = new QStandardItem(dataFromRow.at(col));
                model->setItem(row-1, col, item);
            }
        }
    }

}

void Dialog::itemchangedslot(QStandardItem *item){
    qDebug() << item->row() << item->column();
    int column = item->column();
    int row = item->row();
    if(column == 1){
        yData.replace(row, item->text().toDouble());
    }
    else{
        xData.replace(row, item->text().toDouble());
    }

    ui->plot->graph(0)->setData(xData,yData);
    ui->plot->replot();
}

void writeXYDataToFile(QVector<double> *xData, QVector<double> *yData, QString filepath){
    //write to file

    for (int row = 0; row < xData->size(); row++){

    }
}

void backupDataFile(QString datafilepath){

    QFile stream(datafilepath);
    QFileInfo fileInfo(datafilepath);
    bool copySuccess;

    QString newName = QString("backup_") + fileInfo.fileName();


    copySuccess = stream.copy(datafilepath, newName);
    if (copySuccess){
        qDebug() << "Data Backup Successful";
    }
    else {
        qDebug() << "Data Backup Failed";
    }
}

