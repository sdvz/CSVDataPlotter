#include "dialog.h"
#include "ui_dialog.h"
#include <QString>
#include <QFile>
#include <QDebug>
#include <QTextStream>
#include <QFileInfo>

QStringList headers;
QString backupDataFilePath;
QString myDataFilePath = "C:/Users/Sage/Downloads/mvcdata.csv";

static const float minX = -1;
static const float maxX = 32;
static const float minY = 0;
static const float maxY = 1400;

void setModelAndDataWithCSVData(QVector<double> *xData, QVector<double> *yData, QStandardItemModel *model, QString datafilepath);
void writeXYDataToFile(QVector<double> *xData, QVector<double> *yData, QString filepath);
void backupDataFile(QString datafilepath);
void configureViewWithFilePath(QString filepath);

//SLOTS
void configureViewWithFilePath(QString filepath){

}

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);

    model = new QStandardItemModel(this);
    connect(model, SIGNAL(itemChanged(QStandardItem*)), this,SLOT(dataItemChangedSlot(QStandardItem*)) );


//    backupDataFile(myDataFilePath);
//    setModelAndDataWithCSVData(&xData, &yData, model, myDataFilePath);


//    ui->tableView->setModel(model);

//    ui->plot->clearGraphs();
//    ui->plot->addGraph();
//    ui->plot->graph(0)->setData(xData,yData);
//    ui->plot->xAxis->setRange(minX,maxX);
//    ui->plot->yAxis->setRange(minY,maxY);
}

void Dialog::init(){
    model->blockSignals(1);
    backupDataFile(myDataFilePath);
    setModelAndDataWithCSVData(&xData, &yData, model, myDataFilePath);


    ui->tableView->setModel(model);

    ui->plot->clearGraphs();
    ui->plot->addGraph();
    ui->plot->graph(0)->setData(xData,yData);
    ui->plot->xAxis->setRange(minX,maxX);
    ui->plot->yAxis->setRange(minY,maxY);
    ui->plot->replot();
    model->blockSignals(0);
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
    xData->clear();
    yData->clear();

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
    //qDebug() << xData.at(row) << yData.at(row);

    ui->plot->graph(0)->clearData();
    ui->plot->graph(0)->setData(xData,yData);
    ui->plot->replot();

    qDebug() << yData.at(15);
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

void backupDataFile(QString dataFile){

    QFile file(dataFile);
    QFileInfo dataFileInfo(dataFile);
    bool copySuccess;

    QString backupDataFile = QString("backup_") + dataFileInfo.fileName();


    copySuccess = file.copy(dataFile, backupDataFile);
    if (copySuccess){
        qDebug() << "Data Backup Successful";
    }
    else {
        qDebug() << "Data Backup Failed Or Already Exists";
    }

}


void Dialog::on_saveChanges_pushButton_released()
{
    qDebug() << "save button clicked";
    writeXYDataToFile(&xData, &yData,"C:/Users/Sage/Downloads/mvcdata.csv");
}

void Dialog::on_discardChanges_pushButton_released()
{
    model->blockSignals(1);
    setModelAndDataWithCSVData(&xData, &yData, model, myDataFilePath);
    ui->tableView->reset();

    ui->plot->graph(0)->clearData();
    ui->plot->graph(0)->setData(xData,yData);
    ui->plot->replot();

//    qDebug() << ui->plot->graphCount();

    model->blockSignals(0);
}

void Dialog::on_applyPlotWindow_released()
{
    //edit plot window
    ui->plot->xAxis->setRange(ui->xMin_doubleSpinBox->value(),ui->xMax_doubleSpinBox->value());
    ui->plot->yAxis->setRange(ui->minY_doubleSpinBox->value(),ui->maxY_doubleSpinBox->value());
    ui->plot->replot();
}

void Dialog::on_browse_pushButton_released()
{
    myDataFilePath = QFileDialog::getOpenFileName(this, tr("Open File"), "C://", "Comma Seperated (*.csv);;");
    init();
}
