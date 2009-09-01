#ifndef NTRAIN_H
#define NTRAIN_H

#include <QtGui/QDialog>
#include <QTextStream>
#include "bpn.h"

namespace Ui
{
    class nTrainClass;
}

class nTrain : public QDialog
{
    Q_OBJECT

public:
    nTrain(QWidget *parent = 0);
    ~nTrain();

private:
    Ui::nTrainClass *ui;
    BPN *bpn0;

    void ConstructBPN();
    void ConvertLog(QString);
    void FENtoInput(QString, QTextStream &);
    bool TrainOverFile(QString);
    bool TrainOverRawFile(QString);
    double TestOverFile(QString, int&);
    double TestOverRawFile(QString, int&);

private slots:
    void on_pbTest_clicked();
    void on_pbStop_clicked();
    void on_pbConvert_clicked();
    void on_pbSave_clicked();
    void on_pbBrowseFile_clicked();
    void on_pbBrowseDir_clicked();
    void on_pbTrain_clicked();
};

#endif // NTRAIN_H
