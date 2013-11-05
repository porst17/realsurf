/********************************************************************************
** Form generated from reading ui file 'lightParameters.ui'
**
** Created: Fri 18. Sep 16:35:07 2009
**      by: Qt User Interface Compiler version 4.4.1
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_LIGHTPARAMETERS_H
#define UI_LIGHTPARAMETERS_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDoubleSpinBox>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QLabel>
#include <QtGui/QSpacerItem>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>
#include "ColorPicker.h"

QT_BEGIN_NAMESPACE

class Ui_lightParameters
{
public:
    QVBoxLayout *vboxLayout;
    QGroupBox *groupBox;
    QVBoxLayout *vboxLayout1;
    QVBoxLayout *vboxLayout2;
    ColorPicker *ambient;
    ColorPicker *diffuse;
    ColorPicker *specular;
    QGridLayout *gridLayout;
    QLabel *label_3;
    QDoubleSpinBox *spotCutoff;
    QLabel *label_2;
    QLabel *label_1;
    QDoubleSpinBox *spotCutoffExponent;
    QDoubleSpinBox *spotDirY;
    QDoubleSpinBox *linearAttenuation;
    QDoubleSpinBox *posZ;
    QLabel *label_5;
    QDoubleSpinBox *quadraticAttenuation;
    QDoubleSpinBox *constantAttenuation;
    QDoubleSpinBox *spotDirX;
    QDoubleSpinBox *posY;
    QLabel *label_4;
    QDoubleSpinBox *spotDirZ;
    QDoubleSpinBox *posX;
    QSpacerItem *spacerItem;

    void setupUi(QWidget *lightParameters)
    {
    if (lightParameters->objectName().isEmpty())
        lightParameters->setObjectName(QString::fromUtf8("lightParameters"));
    lightParameters->resize(285, 284);
    vboxLayout = new QVBoxLayout(lightParameters);
#ifndef Q_OS_MAC
    vboxLayout->setSpacing(6);
#endif
#ifndef Q_OS_MAC
    vboxLayout->setMargin(9);
#endif
    vboxLayout->setObjectName(QString::fromUtf8("vboxLayout"));
    groupBox = new QGroupBox(lightParameters);
    groupBox->setObjectName(QString::fromUtf8("groupBox"));
    QSizePolicy sizePolicy(static_cast<QSizePolicy::Policy>(5), static_cast<QSizePolicy::Policy>(0));
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);
    sizePolicy.setHeightForWidth(groupBox->sizePolicy().hasHeightForWidth());
    groupBox->setSizePolicy(sizePolicy);
    groupBox->setCheckable(true);
    groupBox->setChecked(true);
    vboxLayout1 = new QVBoxLayout(groupBox);
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
    ambient = new ColorPicker(groupBox);
    ambient->setObjectName(QString::fromUtf8("ambient"));
    sizePolicy.setHeightForWidth(ambient->sizePolicy().hasHeightForWidth());
    ambient->setSizePolicy(sizePolicy);

    vboxLayout2->addWidget(ambient);

    diffuse = new ColorPicker(groupBox);
    diffuse->setObjectName(QString::fromUtf8("diffuse"));
    sizePolicy.setHeightForWidth(diffuse->sizePolicy().hasHeightForWidth());
    diffuse->setSizePolicy(sizePolicy);
    diffuse->setCheckable(false);

    vboxLayout2->addWidget(diffuse);

    specular = new ColorPicker(groupBox);
    specular->setObjectName(QString::fromUtf8("specular"));
    sizePolicy.setHeightForWidth(specular->sizePolicy().hasHeightForWidth());
    specular->setSizePolicy(sizePolicy);

    vboxLayout2->addWidget(specular);


    vboxLayout1->addLayout(vboxLayout2);

    gridLayout = new QGridLayout();
#ifndef Q_OS_MAC
    gridLayout->setSpacing(6);
#endif
    gridLayout->setMargin(0);
    gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
    label_3 = new QLabel(groupBox);
    label_3->setObjectName(QString::fromUtf8("label_3"));

    gridLayout->addWidget(label_3, 3, 0, 1, 2);

    spotCutoff = new QDoubleSpinBox(groupBox);
    spotCutoff->setObjectName(QString::fromUtf8("spotCutoff"));
    sizePolicy.setHeightForWidth(spotCutoff->sizePolicy().hasHeightForWidth());
    spotCutoff->setSizePolicy(sizePolicy);
    spotCutoff->setMaximum(180);
    spotCutoff->setSingleStep(0.5);
    spotCutoff->setValue(180);

    gridLayout->addWidget(spotCutoff, 3, 3, 1, 1);

    label_2 = new QLabel(groupBox);
    label_2->setObjectName(QString::fromUtf8("label_2"));

    gridLayout->addWidget(label_2, 2, 0, 1, 1);

    label_1 = new QLabel(groupBox);
    label_1->setObjectName(QString::fromUtf8("label_1"));

    gridLayout->addWidget(label_1, 0, 0, 1, 1);

    spotCutoffExponent = new QDoubleSpinBox(groupBox);
    spotCutoffExponent->setObjectName(QString::fromUtf8("spotCutoffExponent"));
    sizePolicy.setHeightForWidth(spotCutoffExponent->sizePolicy().hasHeightForWidth());
    spotCutoffExponent->setSizePolicy(sizePolicy);
    spotCutoffExponent->setMaximum(128);
    spotCutoffExponent->setSingleStep(0.5);

    gridLayout->addWidget(spotCutoffExponent, 4, 3, 1, 1);

    spotDirY = new QDoubleSpinBox(groupBox);
    spotDirY->setObjectName(QString::fromUtf8("spotDirY"));
    sizePolicy.setHeightForWidth(spotDirY->sizePolicy().hasHeightForWidth());
    spotDirY->setSizePolicy(sizePolicy);
    spotDirY->setMaximum(100);
    spotDirY->setMinimum(-100);
    spotDirY->setSingleStep(0.05);
    spotDirY->setValue(0);

    gridLayout->addWidget(spotDirY, 2, 2, 1, 1);

    linearAttenuation = new QDoubleSpinBox(groupBox);
    linearAttenuation->setObjectName(QString::fromUtf8("linearAttenuation"));
    sizePolicy.setHeightForWidth(linearAttenuation->sizePolicy().hasHeightForWidth());
    linearAttenuation->setSizePolicy(sizePolicy);
    linearAttenuation->setMaximum(100000);
    linearAttenuation->setSingleStep(0.5);

    gridLayout->addWidget(linearAttenuation, 1, 2, 1, 1);

    posZ = new QDoubleSpinBox(groupBox);
    posZ->setObjectName(QString::fromUtf8("posZ"));
    sizePolicy.setHeightForWidth(posZ->sizePolicy().hasHeightForWidth());
    posZ->setSizePolicy(sizePolicy);
    posZ->setMaximum(100);
    posZ->setMinimum(-100);
    posZ->setSingleStep(0.05);
    posZ->setValue(-1);

    gridLayout->addWidget(posZ, 0, 3, 1, 1);

    label_5 = new QLabel(groupBox);
    label_5->setObjectName(QString::fromUtf8("label_5"));

    gridLayout->addWidget(label_5, 1, 0, 1, 1);

    quadraticAttenuation = new QDoubleSpinBox(groupBox);
    quadraticAttenuation->setObjectName(QString::fromUtf8("quadraticAttenuation"));
    sizePolicy.setHeightForWidth(quadraticAttenuation->sizePolicy().hasHeightForWidth());
    quadraticAttenuation->setSizePolicy(sizePolicy);
    quadraticAttenuation->setMaximum(100000);
    quadraticAttenuation->setSingleStep(0.5);

    gridLayout->addWidget(quadraticAttenuation, 1, 3, 1, 1);

    constantAttenuation = new QDoubleSpinBox(groupBox);
    constantAttenuation->setObjectName(QString::fromUtf8("constantAttenuation"));
    sizePolicy.setHeightForWidth(constantAttenuation->sizePolicy().hasHeightForWidth());
    constantAttenuation->setSizePolicy(sizePolicy);
    constantAttenuation->setMaximum(100000);
    constantAttenuation->setSingleStep(0.5);
    constantAttenuation->setValue(1);

    gridLayout->addWidget(constantAttenuation, 1, 1, 1, 1);

    spotDirX = new QDoubleSpinBox(groupBox);
    spotDirX->setObjectName(QString::fromUtf8("spotDirX"));
    sizePolicy.setHeightForWidth(spotDirX->sizePolicy().hasHeightForWidth());
    spotDirX->setSizePolicy(sizePolicy);
    spotDirX->setMaximum(100);
    spotDirX->setMinimum(-100);
    spotDirX->setSingleStep(0.05);
    spotDirX->setValue(0);

    gridLayout->addWidget(spotDirX, 2, 1, 1, 1);

    posY = new QDoubleSpinBox(groupBox);
    posY->setObjectName(QString::fromUtf8("posY"));
    sizePolicy.setHeightForWidth(posY->sizePolicy().hasHeightForWidth());
    posY->setSizePolicy(sizePolicy);
    posY->setMaximum(100);
    posY->setMinimum(-100);
    posY->setSingleStep(0.05);

    gridLayout->addWidget(posY, 0, 2, 1, 1);

    label_4 = new QLabel(groupBox);
    label_4->setObjectName(QString::fromUtf8("label_4"));

    gridLayout->addWidget(label_4, 4, 0, 1, 2);

    spotDirZ = new QDoubleSpinBox(groupBox);
    spotDirZ->setObjectName(QString::fromUtf8("spotDirZ"));
    sizePolicy.setHeightForWidth(spotDirZ->sizePolicy().hasHeightForWidth());
    spotDirZ->setSizePolicy(sizePolicy);
    spotDirZ->setMaximum(100);
    spotDirZ->setMinimum(-100);
    spotDirZ->setSingleStep(0.05);
    spotDirZ->setValue(-1);

    gridLayout->addWidget(spotDirZ, 2, 3, 1, 1);

    posX = new QDoubleSpinBox(groupBox);
    posX->setObjectName(QString::fromUtf8("posX"));
    sizePolicy.setHeightForWidth(posX->sizePolicy().hasHeightForWidth());
    posX->setSizePolicy(sizePolicy);
    posX->setMaximum(100);
    posX->setMinimum(-100);
    posX->setSingleStep(0.05);

    gridLayout->addWidget(posX, 0, 1, 1, 1);


    vboxLayout1->addLayout(gridLayout);


    vboxLayout->addWidget(groupBox);

    spacerItem = new QSpacerItem(20, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);

    vboxLayout->addItem(spacerItem);


    retranslateUi(lightParameters);

    QMetaObject::connectSlotsByName(lightParameters);
    } // setupUi

    void retranslateUi(QWidget *lightParameters)
    {
    lightParameters->setWindowTitle(QApplication::translate("lightParameters", "Form", 0, QApplication::UnicodeUTF8));

#ifndef QT_NO_TOOLTIP
    groupBox->setToolTip(QApplication::translate("lightParameters", "Allows setting of parameters of a light source. A light may also be disabled.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP

    groupBox->setTitle(QApplication::translate("lightParameters", "enabled", 0, QApplication::UnicodeUTF8));

#ifndef QT_NO_TOOLTIP
    ambient->setToolTip(QApplication::translate("lightParameters", "Set the ambient color of the light source.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP

    ambient->setText(QApplication::translate("lightParameters", " Ambient", 0, QApplication::UnicodeUTF8));

#ifndef QT_NO_TOOLTIP
    diffuse->setToolTip(QApplication::translate("lightParameters", "Set the diffuse color of the light source.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP

    diffuse->setText(QApplication::translate("lightParameters", " Diffuse", 0, QApplication::UnicodeUTF8));

#ifndef QT_NO_TOOLTIP
    specular->setToolTip(QApplication::translate("lightParameters", "Set the ambient specular of the light source.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP

    specular->setText(QApplication::translate("lightParameters", " Specular", 0, QApplication::UnicodeUTF8));

#ifndef QT_NO_TOOLTIP
    label_3->setToolTip(QApplication::translate("lightParameters", "Set the cutoff angle of the spot of the light source. This value must lie between 0.0 and 90.0. The special value 180.0 means, that this light has no spot (i.e. point light).", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP

    label_3->setText(QApplication::translate("lightParameters", "Spot Cutoff Angle:", 0, QApplication::UnicodeUTF8));

#ifndef QT_NO_TOOLTIP
    spotCutoff->setToolTip(QApplication::translate("lightParameters", "Set the cutoff angle of the spot of the light source. This value must lie between 0.0 and 90.0. The special value 180.0 means, that this light has no spot (i.e. point light).", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP


#ifndef QT_NO_TOOLTIP
    label_2->setToolTip(QApplication::translate("lightParameters", "Set the direction of the spot of the light source.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP

    label_2->setText(QApplication::translate("lightParameters", "Spot Direction:", 0, QApplication::UnicodeUTF8));

#ifndef QT_NO_TOOLTIP
    label_1->setToolTip(QApplication::translate("lightParameters", "Set the position of the light source.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP

    label_1->setText(QApplication::translate("lightParameters", "Position:", 0, QApplication::UnicodeUTF8));

#ifndef QT_NO_TOOLTIP
    spotCutoffExponent->setToolTip(QApplication::translate("lightParameters", "Set the cutoff exponent of the spot of the light source. This value specifies the intensity distribution of the light.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP


#ifndef QT_NO_TOOLTIP
    spotDirY->setToolTip(QApplication::translate("lightParameters", "Set the y direction of the spot of the light source.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP


#ifndef QT_NO_TOOLTIP
    linearAttenuation->setToolTip(QApplication::translate("lightParameters", "Set the linear attenuation of the light source.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP


#ifndef QT_NO_TOOLTIP
    posZ->setToolTip(QApplication::translate("lightParameters", "Set the z position of the light source.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP


#ifndef QT_NO_TOOLTIP
    label_5->setToolTip(QApplication::translate("lightParameters", "Set the attenuation of the light source.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP

    label_5->setText(QApplication::translate("lightParameters", "Attenuation:", 0, QApplication::UnicodeUTF8));

#ifndef QT_NO_TOOLTIP
    quadraticAttenuation->setToolTip(QApplication::translate("lightParameters", "Set the quadratic attenuation of the light source.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP


#ifndef QT_NO_TOOLTIP
    constantAttenuation->setToolTip(QApplication::translate("lightParameters", "Set the contant attenuation of the light source.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP


#ifndef QT_NO_TOOLTIP
    spotDirX->setToolTip(QApplication::translate("lightParameters", "Set the x direction of the spot of the light source.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP


#ifndef QT_NO_TOOLTIP
    posY->setToolTip(QApplication::translate("lightParameters", "Set the y position of the light source.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP


#ifndef QT_NO_TOOLTIP
    label_4->setToolTip(QApplication::translate("lightParameters", "Set the cutoff exponent of the spot of the light source. This value specifies the intensity distribution of the light.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP

    label_4->setText(QApplication::translate("lightParameters", "Spot Cutoff Exponent: ", 0, QApplication::UnicodeUTF8));

#ifndef QT_NO_TOOLTIP
    spotDirZ->setToolTip(QApplication::translate("lightParameters", "Set the z direction of the spot of the light source.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP


#ifndef QT_NO_TOOLTIP
    posX->setToolTip(QApplication::translate("lightParameters", "Set the x position of the light source.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP

    Q_UNUSED(lightParameters);
    } // retranslateUi

};

namespace Ui {
    class lightParameters: public Ui_lightParameters {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LIGHTPARAMETERS_H
