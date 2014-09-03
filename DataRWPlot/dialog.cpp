#include "dialog.h"
#include "ui_dialog.h"
#include <QString>
#include <QFile>
#include <QDebug>
#include <QTextStream>

void setModelWithCSVData(QStandardItemModel *model, QString datafilepath);

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    QString myDataFilePath = "C:/Users/Sage/Downloads/mvcdata.csv";
    model = new QStandardItemModel(this);
    setModelWithCSVData(model, myDataFilePath);
    ui->tableView->setModel(model);
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


