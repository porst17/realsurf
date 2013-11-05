/********************************************************************************
** Form generated from reading ui file 'savedSurfaces.ui'
**
** Created: Fri 18. Sep 16:35:07 2009
**      by: Qt User Interface Compiler version 4.4.1
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_SAVEDSURFACES_H
#define UI_SAVEDSURFACES_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QGridLayout>
#include <QtGui/QLabel>
#include <QtGui/QListWidget>
#include <QtGui/QPushButton>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_savedSurfaces
{
public:
    QGridLayout *gridLayout;
    QListWidget *savedSurfacesList;
    QLabel *label;
    QPushButton *saveButton;
    QPushButton *removeFromListButton;
    QPushButton *addToListButton;

    void setupUi(QWidget *savedSurfaces)
    {
    if (savedSurfaces->objectName().isEmpty())
        savedSurfaces->setObjectName(QString::fromUtf8("savedSurfaces"));
    savedSurfaces->resize(237, 364);
    gridLayout = new QGridLayout(savedSurfaces);
#ifndef Q_OS_MAC
    gridLayout->setSpacing(6);
#endif
#ifndef Q_OS_MAC
    gridLayout->setMargin(9);
#endif
    gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
    savedSurfacesList = new QListWidget(savedSurfaces);
    savedSurfacesList->setObjectName(QString::fromUtf8("savedSurfacesList"));
    savedSurfacesList->setIconSize(QSize(64, 64));
    savedSurfacesList->setLayoutMode(QListView::Batched);
    savedSurfacesList->setUniformItemSizes(true);
    savedSurfacesList->setWordWrap(true);

    gridLayout->addWidget(savedSurfacesList, 0, 0, 1, 2);

    label = new QLabel(savedSurfaces);
    label->setObjectName(QString::fromUtf8("label"));
    label->setAlignment(Qt::AlignCenter);

    gridLayout->addWidget(label, 1, 0, 1, 2);

    saveButton = new QPushButton(savedSurfaces);
    saveButton->setObjectName(QString::fromUtf8("saveButton"));

    gridLayout->addWidget(saveButton, 3, 0, 1, 2);

    removeFromListButton = new QPushButton(savedSurfaces);
    removeFromListButton->setObjectName(QString::fromUtf8("removeFromListButton"));

    gridLayout->addWidget(removeFromListButton, 2, 1, 1, 1);

    addToListButton = new QPushButton(savedSurfaces);
    addToListButton->setObjectName(QString::fromUtf8("addToListButton"));

    gridLayout->addWidget(addToListButton, 2, 0, 1, 1);


    retranslateUi(savedSurfaces);

    QMetaObject::connectSlotsByName(savedSurfaces);
    } // setupUi

    void retranslateUi(QWidget *savedSurfaces)
    {
    savedSurfaces->setWindowTitle(QApplication::translate("savedSurfaces", "Form", 0, QApplication::UnicodeUTF8));
    label->setText(QApplication::translate("savedSurfaces", "Double-click on a surface to load it.", 0, QApplication::UnicodeUTF8));
    saveButton->setText(QApplication::translate("savedSurfaces", "Save current Surface to File", 0, QApplication::UnicodeUTF8));
    removeFromListButton->setText(QApplication::translate("savedSurfaces", "Remove Surface from List", 0, QApplication::UnicodeUTF8));
    addToListButton->setText(QApplication::translate("savedSurfaces", "Add Files to List", 0, QApplication::UnicodeUTF8));
    Q_UNUSED(savedSurfaces);
    } // retranslateUi

};

namespace Ui {
    class savedSurfaces: public Ui_savedSurfaces {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SAVEDSURFACES_H
