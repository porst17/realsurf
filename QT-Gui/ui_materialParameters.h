/********************************************************************************
** Form generated from reading ui file 'materialParameters.ui'
**
** Created: Fri 18. Sep 16:35:07 2009
**      by: Qt User Interface Compiler version 4.4.1
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_MATERIALPARAMETERS_H
#define UI_MATERIALPARAMETERS_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDoubleSpinBox>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QLabel>
#include <QtGui/QWidget>
#include "ColorPicker.h"
#include "PredefinedMaterials.h"

QT_BEGIN_NAMESPACE

class Ui_materialParameters
{
public:
    QHBoxLayout *hboxLayout;
    QGroupBox *groupBox;
    QHBoxLayout *hboxLayout1;
    QGridLayout *gridLayout;
    PredefinedMaterials *predefinedMaterials;
    QDoubleSpinBox *transparency;
    ColorPicker *diffuse;
    ColorPicker *emission;
    QLabel *transparencyLabel;
    QLabel *label_1;
    ColorPicker *specular;
    QDoubleSpinBox *shininess;
    ColorPicker *ambient;

    void setupUi(QWidget *materialParameters)
    {
    if (materialParameters->objectName().isEmpty())
        materialParameters->setObjectName(QString::fromUtf8("materialParameters"));
    materialParameters->resize(196, 170);
    hboxLayout = new QHBoxLayout(materialParameters);
    hboxLayout->setSpacing(0);
    hboxLayout->setMargin(0);
    hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
    groupBox = new QGroupBox(materialParameters);
    groupBox->setObjectName(QString::fromUtf8("groupBox"));
    QSizePolicy sizePolicy(static_cast<QSizePolicy::Policy>(5), static_cast<QSizePolicy::Policy>(0));
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);
    sizePolicy.setHeightForWidth(groupBox->sizePolicy().hasHeightForWidth());
    groupBox->setSizePolicy(sizePolicy);
    groupBox->setCheckable(false);
    hboxLayout1 = new QHBoxLayout(groupBox);
#ifndef Q_OS_MAC
    hboxLayout1->setSpacing(6);
#endif
#ifndef Q_OS_MAC
    hboxLayout1->setMargin(9);
#endif
    hboxLayout1->setObjectName(QString::fromUtf8("hboxLayout1"));
    gridLayout = new QGridLayout();
#ifndef Q_OS_MAC
    gridLayout->setSpacing(6);
#endif
#ifndef Q_OS_MAC
    gridLayout->setMargin(0);
#endif
    gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
    predefinedMaterials = new PredefinedMaterials(groupBox);
    predefinedMaterials->setObjectName(QString::fromUtf8("predefinedMaterials"));
    predefinedMaterials->setEditable(false);

    gridLayout->addWidget(predefinedMaterials, 0, 0, 1, 2);

    transparency = new QDoubleSpinBox(groupBox);
    transparency->setObjectName(QString::fromUtf8("transparency"));
    transparency->setEnabled(true);
    transparency->setMaximum(1);
    transparency->setSingleStep(0.05);

    gridLayout->addWidget(transparency, 4, 1, 1, 1);

    diffuse = new ColorPicker(groupBox);
    diffuse->setObjectName(QString::fromUtf8("diffuse"));

    gridLayout->addWidget(diffuse, 1, 1, 1, 1);

    emission = new ColorPicker(groupBox);
    emission->setObjectName(QString::fromUtf8("emission"));

    gridLayout->addWidget(emission, 1, 0, 1, 1);

    transparencyLabel = new QLabel(groupBox);
    transparencyLabel->setObjectName(QString::fromUtf8("transparencyLabel"));
    transparencyLabel->setEnabled(true);

    gridLayout->addWidget(transparencyLabel, 4, 0, 1, 1);

    label_1 = new QLabel(groupBox);
    label_1->setObjectName(QString::fromUtf8("label_1"));

    gridLayout->addWidget(label_1, 3, 0, 1, 1);

    specular = new ColorPicker(groupBox);
    specular->setObjectName(QString::fromUtf8("specular"));

    gridLayout->addWidget(specular, 2, 1, 1, 1);

    shininess = new QDoubleSpinBox(groupBox);
    shininess->setObjectName(QString::fromUtf8("shininess"));
    shininess->setMaximum(128);
    shininess->setSingleStep(0.5);

    gridLayout->addWidget(shininess, 3, 1, 1, 1);

    ambient = new ColorPicker(groupBox);
    ambient->setObjectName(QString::fromUtf8("ambient"));

    gridLayout->addWidget(ambient, 2, 0, 1, 1);


    hboxLayout1->addLayout(gridLayout);


    hboxLayout->addWidget(groupBox);


    retranslateUi(materialParameters);

    QMetaObject::connectSlotsByName(materialParameters);
    } // setupUi

    void retranslateUi(QWidget *materialParameters)
    {
    materialParameters->setWindowTitle(QApplication::translate("materialParameters", "Form", 0, QApplication::UnicodeUTF8));
    groupBox->setTitle(QApplication::translate("materialParameters", "Material", 0, QApplication::UnicodeUTF8));

#ifndef QT_NO_TOOLTIP
    predefinedMaterials->setToolTip(QApplication::translate("materialParameters", "Choose a predefined material.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP


#ifndef QT_NO_TOOLTIP
    transparency->setToolTip(QApplication::translate("materialParameters", "Set the transparency of the surface (0.0=opaque, 1.0=transparent). If this is enabled, you should use the same materials for front and back side.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP


#ifndef QT_NO_TOOLTIP
    diffuse->setToolTip(QApplication::translate("materialParameters", "Set the diffuse color.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP

    diffuse->setText(QApplication::translate("materialParameters", " Diffuse", 0, QApplication::UnicodeUTF8));

#ifndef QT_NO_TOOLTIP
    emission->setToolTip(QApplication::translate("materialParameters", "Set the emission/glow color. ", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP

    emission->setText(QApplication::translate("materialParameters", " Emission", 0, QApplication::UnicodeUTF8));
    transparencyLabel->setText(QApplication::translate("materialParameters", "Transparency:", 0, QApplication::UnicodeUTF8));
    label_1->setText(QApplication::translate("materialParameters", "Shininess:", 0, QApplication::UnicodeUTF8));

#ifndef QT_NO_TOOLTIP
    specular->setToolTip(QApplication::translate("materialParameters", "Set the color of specular highlights.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP

    specular->setText(QApplication::translate("materialParameters", " Specular", 0, QApplication::UnicodeUTF8));

#ifndef QT_NO_TOOLTIP
    shininess->setToolTip(QApplication::translate("materialParameters", "Set the strength of the specular reflections.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP


#ifndef QT_NO_TOOLTIP
    ambient->setToolTip(QApplication::translate("materialParameters", "Set the ambient color.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP

    ambient->setText(QApplication::translate("materialParameters", " Ambient", 0, QApplication::UnicodeUTF8));
    Q_UNUSED(materialParameters);
    } // retranslateUi

};

namespace Ui {
    class materialParameters: public Ui_materialParameters {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MATERIALPARAMETERS_H
