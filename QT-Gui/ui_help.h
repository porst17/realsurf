/********************************************************************************
** Form generated from reading ui file 'help.ui'
**
** Created: Fri 18. Sep 16:35:07 2009
**      by: Qt User Interface Compiler version 4.4.1
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_HELP_H
#define UI_HELP_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QLabel>
#include <QtGui/QTextBrowser>
#include <QtGui/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_helpDialog
{
public:
    QVBoxLayout *vboxLayout;
    QLabel *label;
    QTextBrowser *helpBrowser;
    QDialogButtonBox *button_55;

    void setupUi(QDialog *helpDialog)
    {
    if (helpDialog->objectName().isEmpty())
        helpDialog->setObjectName(QString::fromUtf8("helpDialog"));
    helpDialog->resize(375, 500);
    const QIcon icon = QIcon(QString::fromUtf8(":/resources/help.png"));
    helpDialog->setWindowIcon(icon);
    vboxLayout = new QVBoxLayout(helpDialog);
#ifndef Q_OS_MAC
    vboxLayout->setSpacing(6);
#endif
#ifndef Q_OS_MAC
    vboxLayout->setMargin(9);
#endif
    vboxLayout->setObjectName(QString::fromUtf8("vboxLayout"));
    label = new QLabel(helpDialog);
    label->setObjectName(QString::fromUtf8("label"));
    label->setAlignment(Qt::AlignCenter);

    vboxLayout->addWidget(label);

    helpBrowser = new QTextBrowser(helpDialog);
    helpBrowser->setObjectName(QString::fromUtf8("helpBrowser"));
    QPalette palette;
    QBrush brush(QColor(255, 255, 190, 255));
    brush.setStyle(Qt::SolidPattern);
    palette.setBrush(QPalette::Active, QPalette::Base, brush);
    palette.setBrush(QPalette::Inactive, QPalette::Base, brush);
    QBrush brush1(QColor(212, 208, 200, 255));
    brush1.setStyle(Qt::SolidPattern);
    palette.setBrush(QPalette::Disabled, QPalette::Base, brush1);
    helpBrowser->setPalette(palette);
    helpBrowser->setTabStopWidth(0);
    helpBrowser->setOpenExternalLinks(true);

    vboxLayout->addWidget(helpBrowser);

    button_55 = new QDialogButtonBox(helpDialog);
    button_55->setObjectName(QString::fromUtf8("button_55"));
    button_55->setOrientation(Qt::Horizontal);
    button_55->setStandardButtons(QDialogButtonBox::Ok);
    button_55->setCenterButtons(true);

    vboxLayout->addWidget(button_55);


    retranslateUi(helpDialog);
    QObject::connect(button_55, SIGNAL(accepted()), helpDialog, SLOT(accept()));
    QObject::connect(button_55, SIGNAL(rejected()), helpDialog, SLOT(reject()));

    QMetaObject::connectSlotsByName(helpDialog);
    } // setupUi

    void retranslateUi(QDialog *helpDialog)
    {
    helpDialog->setWindowTitle(QApplication::translate("helpDialog", "RealSurf Help", 0, QApplication::UnicodeUTF8));
    label->setText(QApplication::translate("helpDialog", "<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8.25pt; font-weight:400; font-style:normal; text-decoration:none;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:12pt; font-weight:600;\">RealSurf Help</span></p></body></html>", 0, QApplication::UnicodeUTF8));
    Q_UNUSED(helpDialog);
    } // retranslateUi

};

namespace Ui {
    class helpDialog: public Ui_helpDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_HELP_H
