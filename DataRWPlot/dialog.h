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
    ~Dialog();

private slots:
    void itemchangedslot(QStandardItem *item);
private:
    Ui::Dialog *ui;
    QStandardItemModel *model;
    QVector<double> xData, yData;
};

#endif // DIALOG_H
