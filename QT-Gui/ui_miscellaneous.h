/********************************************************************************
** Form generated from reading ui file 'miscellaneous.ui'
**
** Created: Fri 18. Sep 16:35:06 2009
**      by: Qt User Interface Compiler version 4.4.1
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_MISCELLANEOUS_H
#define UI_MISCELLANEOUS_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QFrame>
#include <QtGui/QGridLayout>
#include <QtGui/QLabel>
#include <QtGui/QSpacerItem>
#include <QtGui/QToolButton>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>
#include "ColorPicker.h"

QT_BEGIN_NAMESPACE

class Ui_miscellaneous
{
public:
    QVBoxLayout *vboxLayout;
    QLabel *label_10;
    QComboBox *polyCoeffMethod;
    QComboBox *rootFinderMethod;
    QFrame *line_4;
    QGridLayout *gridLayout;
    QToolButton *resetLightConfig;
    QLabel *label_7;
    QToolButton *clipToCube;
    QToolButton *showHideBoundingObject;
    QToolButton *resetSurfaceTransformation;
    QToolButton *screenshot;
    QLabel *label_8;
    QLabel *label_4;
    QSpacerItem *spacerItem;
    QFrame *line_3;
    QLabel *label_2;
    QToolButton *clipToSphere;
    QLabel *label;
    ColorPicker *bgColor;
    QLabel *label_3;
    QFrame *line_2;
    QFrame *line;
    QToolButton *about;
    QLabel *label_9;
    QLabel *label_5;
    QSpacerItem *spacerItem1;

    void setupUi(QWidget *miscellaneous)
    {
    if (miscellaneous->objectName().isEmpty())
        miscellaneous->setObjectName(QString::fromUtf8("miscellaneous"));
    miscellaneous->resize(260, 462);
    vboxLayout = new QVBoxLayout(miscellaneous);
#ifndef Q_OS_MAC
    vboxLayout->setSpacing(6);
#endif
#ifndef Q_OS_MAC
    vboxLayout->setMargin(9);
#endif
    vboxLayout->setObjectName(QString::fromUtf8("vboxLayout"));
    label_10 = new QLabel(miscellaneous);
    label_10->setObjectName(QString::fromUtf8("label_10"));

    vboxLayout->addWidget(label_10);

    polyCoeffMethod = new QComboBox(miscellaneous);
    polyCoeffMethod->setObjectName(QString::fromUtf8("polyCoeffMethod"));

    vboxLayout->addWidget(polyCoeffMethod);

    rootFinderMethod = new QComboBox(miscellaneous);
    rootFinderMethod->setObjectName(QString::fromUtf8("rootFinderMethod"));

    vboxLayout->addWidget(rootFinderMethod);

    line_4 = new QFrame(miscellaneous);
    line_4->setObjectName(QString::fromUtf8("line_4"));
    line_4->setFrameShape(QFrame::HLine);
    line_4->setFrameShadow(QFrame::Sunken);

    vboxLayout->addWidget(line_4);

    gridLayout = new QGridLayout();
#ifndef Q_OS_MAC
    gridLayout->setSpacing(6);
#endif
    gridLayout->setMargin(0);
    gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
    resetLightConfig = new QToolButton(miscellaneous);
    resetLightConfig->setObjectName(QString::fromUtf8("resetLightConfig"));
    const QIcon icon = QIcon(QString::fromUtf8(":/resources/reset_light.png"));
    resetLightConfig->setIcon(icon);
    resetLightConfig->setIconSize(QSize(32, 32));

    gridLayout->addWidget(resetLightConfig, 8, 0, 1, 1);

    label_7 = new QLabel(miscellaneous);
    label_7->setObjectName(QString::fromUtf8("label_7"));

    gridLayout->addWidget(label_7, 10, 1, 1, 1);

    clipToCube = new QToolButton(miscellaneous);
    clipToCube->setObjectName(QString::fromUtf8("clipToCube"));
    const QIcon icon1 = QIcon(QString::fromUtf8(":/resources/cube_clipping.png"));
    clipToCube->setIcon(icon1);
    clipToCube->setIconSize(QSize(32, 32));
    clipToCube->setCheckable(true);
    clipToCube->setAutoExclusive(true);

    gridLayout->addWidget(clipToCube, 4, 0, 1, 1);

    showHideBoundingObject = new QToolButton(miscellaneous);
    showHideBoundingObject->setObjectName(QString::fromUtf8("showHideBoundingObject"));
    showHideBoundingObject->setFocusPolicy(Qt::StrongFocus);
    const QIcon icon2 = QIcon(QString::fromUtf8(":/resources/bounding_object_off.png"));
    showHideBoundingObject->setIcon(icon2);
    showHideBoundingObject->setIconSize(QSize(32, 32));
    showHideBoundingObject->setCheckable(true);

    gridLayout->addWidget(showHideBoundingObject, 5, 0, 1, 1);

    resetSurfaceTransformation = new QToolButton(miscellaneous);
    resetSurfaceTransformation->setObjectName(QString::fromUtf8("resetSurfaceTransformation"));
    const QIcon icon3 = QIcon(QString::fromUtf8(":/resources/reset_transform.png"));
    resetSurfaceTransformation->setIcon(icon3);
    resetSurfaceTransformation->setIconSize(QSize(32, 32));

    gridLayout->addWidget(resetSurfaceTransformation, 7, 0, 1, 1);

    screenshot = new QToolButton(miscellaneous);
    screenshot->setObjectName(QString::fromUtf8("screenshot"));
    screenshot->setFocusPolicy(Qt::StrongFocus);
    const QIcon icon4 = QIcon(QString::fromUtf8(":/resources/screenshot.png"));
    screenshot->setIcon(icon4);
    screenshot->setIconSize(QSize(32, 32));
    screenshot->setCheckable(false);

    gridLayout->addWidget(screenshot, 0, 0, 1, 1);

    label_8 = new QLabel(miscellaneous);
    label_8->setObjectName(QString::fromUtf8("label_8"));

    gridLayout->addWidget(label_8, 1, 1, 1, 1);

    label_4 = new QLabel(miscellaneous);
    label_4->setObjectName(QString::fromUtf8("label_4"));

    gridLayout->addWidget(label_4, 7, 1, 1, 1);

    spacerItem = new QSpacerItem(75, 16, QSizePolicy::Minimum, QSizePolicy::Expanding);

    gridLayout->addItem(spacerItem, 11, 0, 1, 1);

    line_3 = new QFrame(miscellaneous);
    line_3->setObjectName(QString::fromUtf8("line_3"));
    line_3->setFrameShape(QFrame::HLine);
    line_3->setFrameShadow(QFrame::Sunken);

    gridLayout->addWidget(line_3, 2, 0, 1, 2);

    label_2 = new QLabel(miscellaneous);
    label_2->setObjectName(QString::fromUtf8("label_2"));

    gridLayout->addWidget(label_2, 3, 1, 1, 1);

    clipToSphere = new QToolButton(miscellaneous);
    clipToSphere->setObjectName(QString::fromUtf8("clipToSphere"));
    const QIcon icon5 = QIcon(QString::fromUtf8(":/resources/sphere_clipping.png"));
    clipToSphere->setIcon(icon5);
    clipToSphere->setIconSize(QSize(32, 32));
    clipToSphere->setCheckable(true);
    clipToSphere->setChecked(true);
    clipToSphere->setAutoExclusive(true);

    gridLayout->addWidget(clipToSphere, 3, 0, 1, 1);

    label = new QLabel(miscellaneous);
    label->setObjectName(QString::fromUtf8("label"));

    gridLayout->addWidget(label, 4, 1, 1, 1);

    bgColor = new ColorPicker(miscellaneous);
    bgColor->setObjectName(QString::fromUtf8("bgColor"));
    QSizePolicy sizePolicy(static_cast<QSizePolicy::Policy>(0), static_cast<QSizePolicy::Policy>(0));
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);
    sizePolicy.setHeightForWidth(bgColor->sizePolicy().hasHeightForWidth());
    bgColor->setSizePolicy(sizePolicy);
    bgColor->setMinimumSize(QSize(43, 44));
    bgColor->setMaximumSize(QSize(43, 44));
    bgColor->setIconSize(QSize(32, 32));

    gridLayout->addWidget(bgColor, 1, 0, 1, 1);

    label_3 = new QLabel(miscellaneous);
    label_3->setObjectName(QString::fromUtf8("label_3"));

    gridLayout->addWidget(label_3, 5, 1, 1, 1);

    line_2 = new QFrame(miscellaneous);
    line_2->setObjectName(QString::fromUtf8("line_2"));
    line_2->setFrameShape(QFrame::HLine);
    line_2->setFrameShadow(QFrame::Sunken);

    gridLayout->addWidget(line_2, 6, 0, 1, 2);

    line = new QFrame(miscellaneous);
    line->setObjectName(QString::fromUtf8("line"));
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);

    gridLayout->addWidget(line, 9, 0, 1, 2);

    about = new QToolButton(miscellaneous);
    about->setObjectName(QString::fromUtf8("about"));
    const QIcon icon6 = QIcon(QString::fromUtf8(":/resources/about.png"));
    about->setIcon(icon6);
    about->setIconSize(QSize(32, 32));

    gridLayout->addWidget(about, 10, 0, 1, 1);

    label_9 = new QLabel(miscellaneous);
    label_9->setObjectName(QString::fromUtf8("label_9"));

    gridLayout->addWidget(label_9, 0, 1, 1, 1);

    label_5 = new QLabel(miscellaneous);
    label_5->setObjectName(QString::fromUtf8("label_5"));

    gridLayout->addWidget(label_5, 8, 1, 1, 1);

    spacerItem1 = new QSpacerItem(159, 16, QSizePolicy::Minimum, QSizePolicy::Expanding);

    gridLayout->addItem(spacerItem1, 11, 1, 1, 1);


    vboxLayout->addLayout(gridLayout);


    retranslateUi(miscellaneous);

    QMetaObject::connectSlotsByName(miscellaneous);
    } // setupUi

    void retranslateUi(QWidget *miscellaneous)
    {
    miscellaneous->setWindowTitle(QApplication::translate("miscellaneous", "Form", 0, QApplication::UnicodeUTF8));
    label_10->setText(QApplication::translate("miscellaneous", "Algorithms:", 0, QApplication::UnicodeUTF8));

#ifndef QT_NO_TOOLTIP
    polyCoeffMethod->setToolTip(QApplication::translate("miscellaneous", "Method for calculation of polynomial coefficients.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP


#ifndef QT_NO_TOOLTIP
    rootFinderMethod->setToolTip(QApplication::translate("miscellaneous", "Method for calculation of polynomial roots.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP


#ifndef QT_NO_TOOLTIP
    resetLightConfig->setToolTip(QApplication::translate("miscellaneous", "Reset all lights to the default values.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP

    resetLightConfig->setText(QApplication::translate("miscellaneous", "...", 0, QApplication::UnicodeUTF8));
    label_7->setText(QApplication::translate("miscellaneous", "About", 0, QApplication::UnicodeUTF8));

#ifndef QT_NO_TOOLTIP
    clipToCube->setToolTip(QApplication::translate("miscellaneous", "Clip the algebraic surface to a cube.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP

    clipToCube->setText(QApplication::translate("miscellaneous", "...", 0, QApplication::UnicodeUTF8));

#ifndef QT_NO_TOOLTIP
    showHideBoundingObject->setToolTip(QApplication::translate("miscellaneous", "Show or hide the mesh of the object, which is used to clip the surface (i.e. cube or sphere).", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP

    showHideBoundingObject->setText(QApplication::translate("miscellaneous", "...", 0, QApplication::UnicodeUTF8));

#ifndef QT_NO_TOOLTIP
    resetSurfaceTransformation->setToolTip(QApplication::translate("miscellaneous", "Reset the translation, rotation and scale of the surface to it's defaults.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP

    resetSurfaceTransformation->setText(QApplication::translate("miscellaneous", "...", 0, QApplication::UnicodeUTF8));

#ifndef QT_NO_TOOLTIP
    screenshot->setToolTip(QApplication::translate("miscellaneous", "Make a screenshot und save it as a PNG file.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP

    screenshot->setText(QApplication::translate("miscellaneous", "...", 0, QApplication::UnicodeUTF8));
    label_8->setText(QApplication::translate("miscellaneous", "Background color", 0, QApplication::UnicodeUTF8));
    label_4->setText(QApplication::translate("miscellaneous", "Reset surface transformation (R)", 0, QApplication::UnicodeUTF8));
    label_2->setText(QApplication::translate("miscellaneous", "Clip surface to sphere (C)", 0, QApplication::UnicodeUTF8));

#ifndef QT_NO_TOOLTIP
    clipToSphere->setToolTip(QApplication::translate("miscellaneous", "Clip the algebraic surface to a sphere.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP

    clipToSphere->setText(QApplication::translate("miscellaneous", "...", 0, QApplication::UnicodeUTF8));
    label->setText(QApplication::translate("miscellaneous", "Clip surface to cube (C)", 0, QApplication::UnicodeUTF8));

#ifndef QT_NO_TOOLTIP
    bgColor->setToolTip(QApplication::translate("miscellaneous", "Choose the background color of the surface render area.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP

    bgColor->setText(QString());
    bgColor->setShortcut(QString());
    label_3->setText(QApplication::translate("miscellaneous", "Show/Hide bounding object (B)", 0, QApplication::UnicodeUTF8));

#ifndef QT_NO_TOOLTIP
    about->setToolTip(QApplication::translate("miscellaneous", "About RealSurf and the author.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP

    about->setText(QApplication::translate("miscellaneous", "...", 0, QApplication::UnicodeUTF8));
    label_9->setText(QApplication::translate("miscellaneous", "Save screenshot (F6)", 0, QApplication::UnicodeUTF8));
    label_5->setText(QApplication::translate("miscellaneous", "Reset light configuration", 0, QApplication::UnicodeUTF8));
    Q_UNUSED(miscellaneous);
    } // retranslateUi

};

namespace Ui {
    class miscellaneous: public Ui_miscellaneous {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MISCELLANEOUS_H
