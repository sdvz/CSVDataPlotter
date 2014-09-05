#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QtCore>
#include <QtGui>


namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    QStandardItemModel *model;
    QVector<double> xData, yData;
    void configureViewWithFilePath(QString filepath);
    ~Dialog();

private slots:
    void dataItemChangedSlot(QStandardItem *item);
    void on_saveChanges_pushButton_released();
    void on_discardChanges_pushButton_released();
    void on_applyPlotWindow_released();

    void on_browse_pushButton_released();

private:
    Ui::Dialog *ui;

};

#endif // DIALOG_H
