/********************************************************************************
** Form generated from reading ui file 'surfaceEquationEditor.ui'
**
** Created: Fri 18. Sep 16:35:06 2009
**      by: Qt User Interface Compiler version 4.4.1
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_SURFACEEQUATIONEDITOR_H
#define UI_SURFACEEQUATIONEDITOR_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QToolButton>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_surfaceEquationEditor
{
public:
    QVBoxLayout *vboxLayout;
    QGroupBox *groupBox;
    QHBoxLayout *hboxLayout;
    QComboBox *surfaceEquation;
    QLabel *label;
    QPushButton *surfaceEquationOkButton;
    QToolButton *helpButton;

    void setupUi(QWidget *surfaceEquationEditor)
    {
    if (surfaceEquationEditor->objectName().isEmpty())
        surfaceEquationEditor->setObjectName(QString::fromUtf8("surfaceEquationEditor"));
    surfaceEquationEditor->resize(350, 54);
    vboxLayout = new QVBoxLayout(surfaceEquationEditor);
#ifndef Q_OS_MAC
    vboxLayout->setSpacing(6);
#endif
    vboxLayout->setMargin(0);
    vboxLayout->setObjectName(QString::fromUtf8("vboxLayout"));
    groupBox = new QGroupBox(surfaceEquationEditor);
    groupBox->setObjectName(QString::fromUtf8("groupBox"));
    QSizePolicy sizePolicy(static_cast<QSizePolicy::Policy>(13), static_cast<QSizePolicy::Policy>(4));
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);
    sizePolicy.setHeightForWidth(groupBox->sizePolicy().hasHeightForWidth());
    groupBox->setSizePolicy(sizePolicy);
    groupBox->setLayoutDirection(Qt::LeftToRight);
    groupBox->setFlat(false);
    groupBox->setCheckable(false);
    hboxLayout = new QHBoxLayout(groupBox);
#ifndef Q_OS_MAC
    hboxLayout->setSpacing(6);
#endif
    hboxLayout->setMargin(6);
    hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
    surfaceEquation = new QComboBox(groupBox);
    surfaceEquation->setObjectName(QString::fromUtf8("surfaceEquation"));
    QSizePolicy sizePolicy1(static_cast<QSizePolicy::Policy>(7), static_cast<QSizePolicy::Policy>(0));
    sizePolicy1.setHorizontalStretch(0);
    sizePolicy1.setVerticalStretch(0);
    sizePolicy1.setHeightForWidth(surfaceEquation->sizePolicy().hasHeightForWidth());
    surfaceEquation->setSizePolicy(sizePolicy1);
    surfaceEquation->setEditable(true);

    hboxLayout->addWidget(surfaceEquation);

    label = new QLabel(groupBox);
    label->setObjectName(QString::fromUtf8("label"));
    QSizePolicy sizePolicy2(static_cast<QSizePolicy::Policy>(5), static_cast<QSizePolicy::Policy>(0));
    sizePolicy2.setHorizontalStretch(0);
    sizePolicy2.setVerticalStretch(0);
    sizePolicy2.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
    label->setSizePolicy(sizePolicy2);
    label->setMaximumSize(QSize(16777215, 61));

    hboxLayout->addWidget(label);

    surfaceEquationOkButton = new QPushButton(groupBox);
    surfaceEquationOkButton->setObjectName(QString::fromUtf8("surfaceEquationOkButton"));

    hboxLayout->addWidget(surfaceEquationOkButton);

    helpButton = new QToolButton(groupBox);
    helpButton->setObjectName(QString::fromUtf8("helpButton"));
    const QIcon icon = QIcon(QString::fromUtf8(":/resources/help.png"));
    helpButton->setIcon(icon);
    helpButton->setIconSize(QSize(16, 16));
    helpButton->setAutoRaise(true);

    hboxLayout->addWidget(helpButton);


    vboxLayout->addWidget(groupBox);

    QWidget::setTabOrder(surfaceEquation, surfaceEquationOkButton);

    retranslateUi(surfaceEquationEditor);
    QObject::connect(surfaceEquation, SIGNAL(activated(int)), surfaceEquationOkButton, SLOT(animateClick()));

    QMetaObject::connectSlotsByName(surfaceEquationEditor);
    } // setupUi

    void retranslateUi(QWidget *surfaceEquationEditor)
    {
    surfaceEquationEditor->setWindowTitle(QApplication::translate("surfaceEquationEditor", "Form", 0, QApplication::UnicodeUTF8));

#ifndef QT_NO_TOOLTIP
    groupBox->setToolTip(QString());
#endif // QT_NO_TOOLTIP

    groupBox->setTitle(QApplication::translate("surfaceEquationEditor", "Edit Algebraic Surface Equation", 0, QApplication::UnicodeUTF8));
    label->setText(QApplication::translate("surfaceEquationEditor", "= 0", 0, QApplication::UnicodeUTF8));

#ifndef QT_NO_TOOLTIP
    surfaceEquationOkButton->setToolTip(QApplication::translate("surfaceEquationEditor", "Process this surface.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP

    surfaceEquationOkButton->setText(QApplication::translate("surfaceEquationEditor", "Go!", 0, QApplication::UnicodeUTF8));

#ifndef QT_NO_TOOLTIP
    helpButton->setToolTip(QApplication::translate("surfaceEquationEditor", "Display help. (F1)", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP

    helpButton->setText(QApplication::translate("surfaceEquationEditor", "...", 0, QApplication::UnicodeUTF8));
    Q_UNUSED(surfaceEquationEditor);
    } // retranslateUi

};

namespace Ui {
    class surfaceEquationEditor: public Ui_surfaceEquationEditor {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SURFACEEQUATIONEDITOR_H
