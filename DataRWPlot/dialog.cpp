#include "dialog.h"
#include "ui_dialog.h"
#include <QString>
#include <QFile>
#include <QDebug>
#include <QTextStream>
#include <QFileInfo>

QStringList headers;
QString myDataFilePath;

//Default Plot Window Values
static const float minX = -1;
static const float maxX = 32;
static const float minY = 0;
static const float maxY = 1400;

//CSV Data Functions
void setModelAndDataVectorsWithCSV(QVector<double> *xData, QVector<double> *yData, QStandardItemModel *model, QString datafilepath);
void writeXYDataToFile(QVector<double> *xData, QVector<double> *yData, QString filepath);
void backupDataFile(QString datafilepath);
void configureViewWithFilePath(QString filepath);

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);

    model = new QStandardItemModel(this);
    connect(model, SIGNAL(itemChanged(QStandardItem*)), this,SLOT(dataItemChangedSlot(QStandardItem*)) );
}

void Dialog::initView(){

    //Block signals to avoid multiple itemChanged signals
    model->blockSignals(1);


    backupDataFile(myDataFilePath);
    setModelAndDataVectorsWithCSV(&xData, &yData, model, myDataFilePath);

    ui->tableView->setModel(model);

    //Add Graph and Data
    ui->plot->clearGraphs();
    ui->plot->addGraph();
    ui->plot->graph(0)->setData(xData,yData);

    //Set Axes
    ui->plot->xAxis->setRange(minX,maxX);
    ui->plot->yAxis->setRange(minY,maxY);
    ui->plot->xAxis->setLabel(headers.at(0));
    ui->plot->yAxis->setLabel(headers.at(1));

    ui->plot->replot();

    ui->CSVFileLabel->setText(myDataFilePath);

    model->blockSignals(0);
}

Dialog::~Dialog()
{
    delete ui;
}


// SLOT FUNCTIONS /////////////


void Dialog::on_saveChanges_pushButton_released()
{
    qDebug() << "Saved";
    writeXYDataToFile(&xData, &yData,"C:/Users/Sage/Downloads/mvcdata.csv");
}

void Dialog::on_discardChanges_pushButton_released()
{
    model->blockSignals(1);
    setModelAndDataVectorsWithCSV(&xData, &yData, model, myDataFilePath);
    ui->tableView->reset();

    ui->plot->graph(0)->clearData();
    ui->plot->graph(0)->setData(xData,yData);
    ui->plot->replot();

    model->blockSignals(0);
}

void Dialog::on_applyPlotWindow_released()
{
    //edit plot window based on UI values
    ui->plot->xAxis->setRange(ui->xMin_doubleSpinBox->value(),ui->xMax_doubleSpinBox->value());
    ui->plot->yAxis->setRange(ui->minY_doubleSpinBox->value(),ui->maxY_doubleSpinBox->value());
    ui->plot->replot();
}

void Dialog::on_browse_pushButton_released()
{
    myDataFilePath = QFileDialog::getOpenFileName(this, tr("Open File"), "C://", "Comma Seperated (*.csv);;");
    initView();
}

void Dialog::dataItemChangedSlot(QStandardItem *item){
    int column = item->column();
    int row = item->row();
    if(column == 1){
        yData.replace(row, item->text().toDouble());
    }
    else{
        xData.replace(row, item->text().toDouble());
    }

    ui->plot->graph(0)->clearData();
    ui->plot->graph(0)->setData(xData,yData);
    ui->plot->replot();

}


// DATA FUNCTIONS /////////////


void setModelAndDataVectorsWithCSV(QVector<double> *xData, QVector<double> *yData, QStandardItemModel *model, QString datafilepath){

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
                    //populate model with CSV Data
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


void writeXYDataToFile(QVector<double> *xData, QVector<double> *yData, QString filepath){

    QString dataFromRow;
    QString data;
    QFile file(filepath);

    file.resize(0); //clears file

    //read data from vectors into string
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
        qDebug() << "Succesfully backed up CSV Data";
    }
    else {
        qDebug() << "Failed to back up CSV Data. Check to see if already exists";
    }

}



