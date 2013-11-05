/********************************************************************************
** Form generated from reading ui file 'enterSurfaceDescrption.ui'
**
** Created: Fri 18. Sep 16:35:06 2009
**      by: Qt User Interface Compiler version 4.4.1
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_ENTERSURFACEDESCRPTION_H
#define UI_ENTERSURFACEDESCRPTION_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QGridLayout>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>

QT_BEGIN_NAMESPACE

class Ui_surfaceDescriptionDialog
{
public:
    QGridLayout *gridLayout;
    QLineEdit *surfaceDescription;
    QLineEdit *surfaceName;
    QLabel *label;
    QLabel *label_2;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *surfaceDescriptionDialog)
    {
    if (surfaceDescriptionDialog->objectName().isEmpty())
        surfaceDescriptionDialog->setObjectName(QString::fromUtf8("surfaceDescriptionDialog"));
    surfaceDescriptionDialog->resize(360, 100);
    QSizePolicy sizePolicy(static_cast<QSizePolicy::Policy>(0), static_cast<QSizePolicy::Policy>(0));
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);
    sizePolicy.setHeightForWidth(surfaceDescriptionDialog->sizePolicy().hasHeightForWidth());
    surfaceDescriptionDialog->setSizePolicy(sizePolicy);
    surfaceDescriptionDialog->setMinimumSize(QSize(360, 100));
    surfaceDescriptionDialog->setMaximumSize(QSize(360, 100));
    gridLayout = new QGridLayout(surfaceDescriptionDialog);
#ifndef Q_OS_MAC
    gridLayout->setSpacing(6);
#endif
#ifndef Q_OS_MAC
    gridLayout->setMargin(9);
#endif
    gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
    surfaceDescription = new QLineEdit(surfaceDescriptionDialog);
    surfaceDescription->setObjectName(QString::fromUtf8("surfaceDescription"));

    gridLayout->addWidget(surfaceDescription, 1, 1, 1, 1);

    surfaceName = new QLineEdit(surfaceDescriptionDialog);
    surfaceName->setObjectName(QString::fromUtf8("surfaceName"));

    gridLayout->addWidget(surfaceName, 0, 1, 1, 1);

    label = new QLabel(surfaceDescriptionDialog);
    label->setObjectName(QString::fromUtf8("label"));

    gridLayout->addWidget(label, 0, 0, 1, 1);

    label_2 = new QLabel(surfaceDescriptionDialog);
    label_2->setObjectName(QString::fromUtf8("label_2"));

    gridLayout->addWidget(label_2, 1, 0, 1, 1);

    buttonBox = new QDialogButtonBox(surfaceDescriptionDialog);
    buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
    buttonBox->setOrientation(Qt::Horizontal);
    buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::NoButton|QDialogButtonBox::Ok);
    buttonBox->setCenterButtons(true);

    gridLayout->addWidget(buttonBox, 2, 0, 1, 2);

    QWidget::setTabOrder(surfaceName, surfaceDescription);
    QWidget::setTabOrder(surfaceDescription, buttonBox);

    retranslateUi(surfaceDescriptionDialog);
    QObject::connect(buttonBox, SIGNAL(accepted()), surfaceDescriptionDialog, SLOT(accept()));
    QObject::connect(buttonBox, SIGNAL(rejected()), surfaceDescriptionDialog, SLOT(reject()));

    QMetaObject::connectSlotsByName(surfaceDescriptionDialog);
    } // setupUi

    void retranslateUi(QDialog *surfaceDescriptionDialog)
    {
    surfaceDescriptionDialog->setWindowTitle(QApplication::translate("surfaceDescriptionDialog", "Enter surface description", 0, QApplication::UnicodeUTF8));
    label->setText(QApplication::translate("surfaceDescriptionDialog", "Name:", 0, QApplication::UnicodeUTF8));
    label_2->setText(QApplication::translate("surfaceDescriptionDialog", "Description:", 0, QApplication::UnicodeUTF8));
    Q_UNUSED(surfaceDescriptionDialog);
    } // retranslateUi

};

namespace Ui {
    class surfaceDescriptionDialog: public Ui_surfaceDescriptionDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ENTERSURFACEDESCRPTION_H
