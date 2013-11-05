/********************************************************************************
** Form generated from reading ui file 'floatParameter.ui'
**
** Created: Fri 18. Sep 16:35:07 2009
**      by: Qt User Interface Compiler version 4.4.1
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_FLOATPARAMETER_H
#define UI_FLOATPARAMETER_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDoubleSpinBox>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QLabel>
#include <QtGui/QSlider>
#include <QtGui/QSpacerItem>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_floatParameter
{
public:
    QVBoxLayout *vboxLayout;
    QGroupBox *floatParameterGroupBox;
    QVBoxLayout *vboxLayout1;
    QVBoxLayout *vboxLayout2;
    QHBoxLayout *hboxLayout;
    QLabel *label_1;
    QDoubleSpinBox *floatParameterValue;
    QSlider *floatParameterSlider;
    QHBoxLayout *hboxLayout1;
    QLabel *label_3;
    QDoubleSpinBox *floatParameterMin;
    QSpacerItem *spacerItem;
    QLabel *label_2;
    QDoubleSpinBox *floatParameterMax;

    void setupUi(QWidget *floatParameter)
    {
    if (floatParameter->objectName().isEmpty())
        floatParameter->setObjectName(QString::fromUtf8("floatParameter"));
    floatParameter->resize(240, 115);
    floatParameter->setMinimumSize(QSize(0, 0));
    vboxLayout = new QVBoxLayout(floatParameter);
#ifndef Q_OS_MAC
    vboxLayout->setSpacing(6);
#endif
    vboxLayout->setMargin(0);
    vboxLayout->setObjectName(QString::fromUtf8("vboxLayout"));
    floatParameterGroupBox = new QGroupBox(floatParameter);
    floatParameterGroupBox->setObjectName(QString::fromUtf8("floatParameterGroupBox"));
    QSizePolicy sizePolicy(static_cast<QSizePolicy::Policy>(5), static_cast<QSizePolicy::Policy>(0));
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);
    sizePolicy.setHeightForWidth(floatParameterGroupBox->sizePolicy().hasHeightForWidth());
    floatParameterGroupBox->setSizePolicy(sizePolicy);
    floatParameterGroupBox->setAutoFillBackground(false);
    floatParameterGroupBox->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);
    vboxLayout1 = new QVBoxLayout(floatParameterGroupBox);
#ifndef Q_OS_MAC
    vboxLayout1->setSpacing(6);
#endif
#ifndef Q_OS_MAC
    vboxLayout1->setMargin(9);
#endif
    vboxLayout1->setObjectName(QString::fromUtf8("vboxLayout1"));
    vboxLayout2 = new QVBoxLayout();
#ifndef Q_OS_MAC
    vboxLayout2->setSpacing(6);
#endif
#ifndef Q_OS_MAC
    vboxLayout2->setMargin(0);
#endif
    vboxLayout2->setObjectName(QString::fromUtf8("vboxLayout2"));
    hboxLayout = new QHBoxLayout();
#ifndef Q_OS_MAC
    hboxLayout->setSpacing(6);
#endif
#ifndef Q_OS_MAC
    hboxLayout->setMargin(0);
#endif
    hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
    label_1 = new QLabel(floatParameterGroupBox);
    label_1->setObjectName(QString::fromUtf8("label_1"));
    QSizePolicy sizePolicy1(static_cast<QSizePolicy::Policy>(5), static_cast<QSizePolicy::Policy>(5));
    sizePolicy1.setHorizontalStretch(0);
    sizePolicy1.setVerticalStretch(0);
    sizePolicy1.setHeightForWidth(label_1->sizePolicy().hasHeightForWidth());
    label_1->setSizePolicy(sizePolicy1);

    hboxLayout->addWidget(label_1);

    floatParameterValue = new QDoubleSpinBox(floatParameterGroupBox);
    floatParameterValue->setObjectName(QString::fromUtf8("floatParameterValue"));
    sizePolicy.setHeightForWidth(floatParameterValue->sizePolicy().hasHeightForWidth());
    floatParameterValue->setSizePolicy(sizePolicy);
    floatParameterValue->setCorrectionMode(QAbstractSpinBox::CorrectToNearestValue);
    floatParameterValue->setDecimals(10);

    hboxLayout->addWidget(floatParameterValue);


    vboxLayout2->addLayout(hboxLayout);

    floatParameterSlider = new QSlider(floatParameterGroupBox);
    floatParameterSlider->setObjectName(QString::fromUtf8("floatParameterSlider"));
    floatParameterSlider->setAutoFillBackground(false);
    floatParameterSlider->setMaximum(10000);
    floatParameterSlider->setSingleStep(1);
    floatParameterSlider->setPageStep(1000);
    floatParameterSlider->setOrientation(Qt::Horizontal);
    floatParameterSlider->setInvertedAppearance(false);
    floatParameterSlider->setTickPosition(QSlider::TicksBelow);

    vboxLayout2->addWidget(floatParameterSlider);

    hboxLayout1 = new QHBoxLayout();
#ifndef Q_OS_MAC
    hboxLayout1->setSpacing(6);
#endif
    hboxLayout1->setMargin(0);
    hboxLayout1->setObjectName(QString::fromUtf8("hboxLayout1"));
    label_3 = new QLabel(floatParameterGroupBox);
    label_3->setObjectName(QString::fromUtf8("label_3"));
    sizePolicy1.setHeightForWidth(label_3->sizePolicy().hasHeightForWidth());
    label_3->setSizePolicy(sizePolicy1);

    hboxLayout1->addWidget(label_3);

    floatParameterMin = new QDoubleSpinBox(floatParameterGroupBox);
    floatParameterMin->setObjectName(QString::fromUtf8("floatParameterMin"));
    sizePolicy.setHeightForWidth(floatParameterMin->sizePolicy().hasHeightForWidth());
    floatParameterMin->setSizePolicy(sizePolicy);
    floatParameterMin->setCorrectionMode(QAbstractSpinBox::CorrectToNearestValue);
    floatParameterMin->setDecimals(10);
    floatParameterMin->setMaximum(1e+07);
    floatParameterMin->setMinimum(-1e+07);
    floatParameterMin->setValue(0);

    hboxLayout1->addWidget(floatParameterMin);

    spacerItem = new QSpacerItem(0, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    hboxLayout1->addItem(spacerItem);

    label_2 = new QLabel(floatParameterGroupBox);
    label_2->setObjectName(QString::fromUtf8("label_2"));
    sizePolicy1.setHeightForWidth(label_2->sizePolicy().hasHeightForWidth());
    label_2->setSizePolicy(sizePolicy1);

    hboxLayout1->addWidget(label_2);

    floatParameterMax = new QDoubleSpinBox(floatParameterGroupBox);
    floatParameterMax->setObjectName(QString::fromUtf8("floatParameterMax"));
    sizePolicy.setHeightForWidth(floatParameterMax->sizePolicy().hasHeightForWidth());
    floatParameterMax->setSizePolicy(sizePolicy);
    floatParameterMax->setCorrectionMode(QAbstractSpinBox::CorrectToNearestValue);
    floatParameterMax->setDecimals(10);
    floatParameterMax->setMaximum(1e+07);
    floatParameterMax->setMinimum(-1e+07);
    floatParameterMax->setValue(0);

    hboxLayout1->addWidget(floatParameterMax);


    vboxLayout2->addLayout(hboxLayout1);


    vboxLayout1->addLayout(vboxLayout2);


    vboxLayout->addWidget(floatParameterGroupBox);

    label_1->setBuddy(floatParameterValue);
    label_3->setBuddy(floatParameterMin);
    label_2->setBuddy(floatParameterMax);

    retranslateUi(floatParameter);

    QMetaObject::connectSlotsByName(floatParameter);
    } // setupUi

    void retranslateUi(QWidget *floatParameter)
    {
    floatParameter->setWindowTitle(QApplication::translate("floatParameter", "Form", 0, QApplication::UnicodeUTF8));
    floatParameterGroupBox->setTitle(QApplication::translate("floatParameter", "Parameter", 0, QApplication::UnicodeUTF8));
    label_1->setText(QApplication::translate("floatParameter", "Value:", 0, QApplication::UnicodeUTF8));

#ifndef QT_NO_TOOLTIP
    floatParameterValue->setToolTip(QApplication::translate("floatParameter", "Set the current value of this parameter between MIN and MAX.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP


#ifndef QT_NO_TOOLTIP
    floatParameterSlider->setToolTip(QApplication::translate("floatParameter", "Set the current value of this parameter between MIN and MAX.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP

    label_3->setText(QApplication::translate("floatParameter", "Min:", 0, QApplication::UnicodeUTF8));

#ifndef QT_NO_TOOLTIP
    floatParameterMin->setToolTip(QApplication::translate("floatParameter", "Set the minimum value for this parameter.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP

    label_2->setText(QApplication::translate("floatParameter", "Max:", 0, QApplication::UnicodeUTF8));

#ifndef QT_NO_TOOLTIP
    floatParameterMax->setToolTip(QApplication::translate("floatParameter", "Set the maximum value for this parameter.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP

    Q_UNUSED(floatParameter);
    } // retranslateUi

};

namespace Ui {
    class floatParameter: public Ui_floatParameter {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FLOATPARAMETER_H
