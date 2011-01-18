/********************************************************************************
** Form generated from reading UI file 'ntrain.ui'
**
** Created: Fri Dec 10 14:56:40 2010
**      by: Qt User Interface Compiler version 4.6.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_NTRAIN_H
#define UI_NTRAIN_H

#include <QtCore/QLocale>
#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QProgressBar>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QSpinBox>
#include <QtGui/QTextEdit>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_nTrainClass
{
public:
    QWidget *layoutWidget;
    QVBoxLayout *verticalLayout_3;
    QHBoxLayout *horizontalLayout_2;
    QVBoxLayout *verticalLayout;
    QLabel *label;
    QPushButton *pbConvert;
    QHBoxLayout *horizontalLayout;
    QLabel *label_2;
    QSpinBox *sbEpochs;
    QSpacerItem *horizontalSpacer;
    QPushButton *pbTest;
    QLabel *label_3;
    QPushButton *pbSave;
    QVBoxLayout *verticalLayout_2;
    QLineEdit *leTrainPath;
    QPushButton *pbBrowseDir;
    QPushButton *pbTrain;
    QPushButton *pbStop;
    QLineEdit *leOpenPath;
    QPushButton *pbBrowseFile;
    QTextEdit *teLog;
    QProgressBar *pbProgress;

    void setupUi(QDialog *nTrainClass)
    {
        if (nTrainClass->objectName().isEmpty())
            nTrainClass->setObjectName(QString::fromUtf8("nTrainClass"));
        nTrainClass->resize(327, 419);
        nTrainClass->setLocale(QLocale(QLocale::English, QLocale::UnitedStates));
        layoutWidget = new QWidget(nTrainClass);
        layoutWidget->setObjectName(QString::fromUtf8("layoutWidget"));
        layoutWidget->setGeometry(QRect(10, 5, 314, 411));
        verticalLayout_3 = new QVBoxLayout(layoutWidget);
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setContentsMargins(11, 11, 11, 11);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        verticalLayout_3->setContentsMargins(0, 0, 0, 0);
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        label = new QLabel(layoutWidget);
        label->setObjectName(QString::fromUtf8("label"));

        verticalLayout->addWidget(label);

        pbConvert = new QPushButton(layoutWidget);
        pbConvert->setObjectName(QString::fromUtf8("pbConvert"));

        verticalLayout->addWidget(pbConvert);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        label_2 = new QLabel(layoutWidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        horizontalLayout->addWidget(label_2);

        sbEpochs = new QSpinBox(layoutWidget);
        sbEpochs->setObjectName(QString::fromUtf8("sbEpochs"));
        sbEpochs->setMinimum(1);

        horizontalLayout->addWidget(sbEpochs);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);


        verticalLayout->addLayout(horizontalLayout);

        pbTest = new QPushButton(layoutWidget);
        pbTest->setObjectName(QString::fromUtf8("pbTest"));

        verticalLayout->addWidget(pbTest);

        label_3 = new QLabel(layoutWidget);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        verticalLayout->addWidget(label_3);

        pbSave = new QPushButton(layoutWidget);
        pbSave->setObjectName(QString::fromUtf8("pbSave"));

        verticalLayout->addWidget(pbSave);


        horizontalLayout_2->addLayout(verticalLayout);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        leTrainPath = new QLineEdit(layoutWidget);
        leTrainPath->setObjectName(QString::fromUtf8("leTrainPath"));
        leTrainPath->setEnabled(true);
        leTrainPath->setReadOnly(true);

        verticalLayout_2->addWidget(leTrainPath);

        pbBrowseDir = new QPushButton(layoutWidget);
        pbBrowseDir->setObjectName(QString::fromUtf8("pbBrowseDir"));

        verticalLayout_2->addWidget(pbBrowseDir);

        pbTrain = new QPushButton(layoutWidget);
        pbTrain->setObjectName(QString::fromUtf8("pbTrain"));
        pbTrain->setLocale(QLocale(QLocale::English, QLocale::UnitedStates));

        verticalLayout_2->addWidget(pbTrain);

        pbStop = new QPushButton(layoutWidget);
        pbStop->setObjectName(QString::fromUtf8("pbStop"));

        verticalLayout_2->addWidget(pbStop);

        leOpenPath = new QLineEdit(layoutWidget);
        leOpenPath->setObjectName(QString::fromUtf8("leOpenPath"));
        leOpenPath->setReadOnly(true);

        verticalLayout_2->addWidget(leOpenPath);

        pbBrowseFile = new QPushButton(layoutWidget);
        pbBrowseFile->setObjectName(QString::fromUtf8("pbBrowseFile"));

        verticalLayout_2->addWidget(pbBrowseFile);


        horizontalLayout_2->addLayout(verticalLayout_2);


        verticalLayout_3->addLayout(horizontalLayout_2);

        teLog = new QTextEdit(layoutWidget);
        teLog->setObjectName(QString::fromUtf8("teLog"));
        teLog->setEnabled(true);
        teLog->setReadOnly(true);

        verticalLayout_3->addWidget(teLog);

        pbProgress = new QProgressBar(layoutWidget);
        pbProgress->setObjectName(QString::fromUtf8("pbProgress"));
        pbProgress->setValue(0);

        verticalLayout_3->addWidget(pbProgress);

        QWidget::setTabOrder(leTrainPath, pbConvert);
        QWidget::setTabOrder(pbConvert, pbBrowseDir);
        QWidget::setTabOrder(pbBrowseDir, sbEpochs);
        QWidget::setTabOrder(sbEpochs, pbTrain);
        QWidget::setTabOrder(pbTrain, pbTest);
        QWidget::setTabOrder(pbTest, pbStop);
        QWidget::setTabOrder(pbStop, leOpenPath);
        QWidget::setTabOrder(leOpenPath, pbSave);
        QWidget::setTabOrder(pbSave, pbBrowseFile);
        QWidget::setTabOrder(pbBrowseFile, teLog);

        retranslateUi(nTrainClass);

        QMetaObject::connectSlotsByName(nTrainClass);
    } // setupUi

    void retranslateUi(QDialog *nTrainClass)
    {
        nTrainClass->setWindowTitle(QApplication::translate("nTrainClass", "neuroTrainer", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("nTrainClass", "Train directory", 0, QApplication::UnicodeUTF8));
        pbConvert->setText(QApplication::translate("nTrainClass", "Convert train files", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("nTrainClass", "Epochs", 0, QApplication::UnicodeUTF8));
        pbTest->setText(QApplication::translate("nTrainClass", "Test", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("nTrainClass", "Save path", 0, QApplication::UnicodeUTF8));
        pbSave->setText(QApplication::translate("nTrainClass", "Save", 0, QApplication::UnicodeUTF8));
        pbBrowseDir->setText(QApplication::translate("nTrainClass", "Browse Train Folder", 0, QApplication::UnicodeUTF8));
        pbTrain->setText(QApplication::translate("nTrainClass", "Train", 0, QApplication::UnicodeUTF8));
        pbStop->setText(QApplication::translate("nTrainClass", "Stop", 0, QApplication::UnicodeUTF8));
        leOpenPath->setText(QApplication::translate("nTrainClass", "./bpn1.bpn", 0, QApplication::UnicodeUTF8));
        pbBrowseFile->setText(QApplication::translate("nTrainClass", "Load BPN", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class nTrainClass: public Ui_nTrainClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_NTRAIN_H
