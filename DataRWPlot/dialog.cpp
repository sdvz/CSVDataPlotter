#include "dialog.h"
#include "ui_dialog.h"
#include <QString>
#include <QFile>
#include <QDebug>
#include <QTextStream>
#include <QFileInfo>

QStringList headers;

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

    writeXYDataToFile(&xData, &yData, myDataFilePath);

    ui->tableView->setModel(model);

    connect(model, SIGNAL(itemChanged(QStandardItem*)), this,SLOT(dataItemChangedSlot(QStandardItem*)) );

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

void setModelAndDataWithCSVData(QVector<double> *xData, QVector<double> *yData, QStandardItemModel *model, QString datafilepath){


    QString fileData;
    QStringList dataFromRow;
    QStringList rowsOfData;
    QFile file(datafilepath);


    fileData.clear();
    dataFromRow.clear();
    rowsOfData.clear();

    if (file.open(QFile::ReadWrite)){
        QTextStream stream(&file);
        fileData = stream.readAll();
        rowsOfData = fileData.split('\n');
        file.close();

        headers = rowsOfData.at(0).split(',');
        model->setHorizontalHeaderLabels(headers);

        for (int row = 1; row < (rowsOfData.size()); row++){

            if(!rowsOfData.at(row).isEmpty()){
            dataFromRow = rowsOfData.at(row).split(',');
            xData->append(dataFromRow.at(0).toDouble());
            yData->append(dataFromRow.at(1).toDouble());

            for (int col = 0; col < dataFromRow.size(); col++)
                {
                    //set model to data
                    QStandardItem *item = new QStandardItem(dataFromRow.at(col));
                    model->setItem(row-1, col, item);
                }
            }
        }
    }
    else {
        //
        qDebug() << "Could not open file. Make sure file has R/W Capabilities and retry.";
        return;
    }
}

void Dialog::dataItemChangedSlot(QStandardItem *item){
    qDebug() << item->row() << item->column();
    int column = item->column();
    int row = item->row();
    if(column == 1){
        yData.replace(row, item->text().toDouble());
    }
    else{
        xData.replace(row, item->text().toDouble());
    }
    qDebug() << xData.at(row) << yData.at(row);

    ui->plot->graph(0)->setData(xData,yData);
    ui->plot->replot();
}

void writeXYDataToFile(QVector<double> *xData, QVector<double> *yData, QString filepath){
    //write to file
    QString dataFromRow;
    QString data;
    QFile file(filepath);

    file.resize(0);

    if (file.open(QIODevice::ReadWrite)) {
        QTextStream stream(&file);
        data = headers.at(0) + "," + headers.at(1) + "\n";
        int size = xData->size();

        for (int row = 0; row < size; row++){
            dataFromRow = QString::number(xData->at(row)) + "," + QString::number(yData->at(row)) + "\n";
            data += dataFromRow;
        }
        stream << data;
        }
    else{
        qDebug() << "File could not be opened.";
    }
}

void backupDataFile(QString datafilepath){

    QFile file(datafilepath);
    QFileInfo fileInfo(datafilepath);
    bool copySuccess;

    QString newName = QString("backup_") + fileInfo.fileName();


    copySuccess = file.copy(datafilepath, newName);
    if (copySuccess){
        qDebug() << "Data Backup Successful";
    }
    else {
        qDebug() << "Data Backup Failed";
    }
}


void Dialog::on_saveChanges_pushButton_clicked()
{
    qDebug() << "save button clicked";
    writeXYDataToFile(&xData, &yData,"C:/Users/Sage/Downloads/mvcdata.csv");
}
