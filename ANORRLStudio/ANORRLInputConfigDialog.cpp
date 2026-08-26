/**
 * ANORRLInputConfigDialog.cpp
 * Copyright (c) 2013 ROBLOX Corp. All Rights Reserved.
 */

#include "stdafx.h"
#include "ANORRLInputConfigDialog.h"

#include <QDialogButtonBox>
#include <QGroupBox>
#include <QKeyEvent>
#include <QLineEdit>
#include <QMap>
#include <QMessageBox>
#include <QPushButton>
#include <QSettings>
#include <QStyledItemDelegate>
#include <QTreeView>

#include "ANORRLMainWindow.h"
#include "ANORRLKeyboardConfig.h"
#include "ANORRLMouseConfig.h"


ANORRLInputConfigDialog::ANORRLInputConfigDialog(ANORRLMainWindow& MainWindow)
    : QDialog(&MainWindow,Qt::WindowTitleHint | Qt::WindowSystemMenuHint)
    , m_mainWindow(MainWindow)
    , m_dataChanged(false)
{
    setAttribute(Qt::WA_DeleteOnClose,false);
    setWindowTitle("Keyboard and Mouse");

    resize(700, 500);

    initialize();

    // Restore window state.
    QSettings settings;
    restoreGeometry(settings.value(windowTitle() + "/Geometry").toByteArray());
}

ANORRLInputConfigDialog::~ANORRLInputConfigDialog()
{
    // Save window state.
    QSettings settings;
    settings.setValue(windowTitle() + "/Geometry",saveGeometry());
}

void ANORRLInputConfigDialog::initialize()
{
    ARLASSERT(!layout());

    ARL::BaldPtr<QLayout> layout = new QVBoxLayout;
    layout->setSpacing(18);
    
    m_dialogButtonBox = new QDialogButtonBox(QDialogButtonBox::RestoreDefaults
                                      | QDialogButtonBox::Ok
                                      | QDialogButtonBox::Cancel);
    
    connect(m_dialogButtonBox, SIGNAL(accepted()),
            this,              SLOT(accept()));

    connect(m_dialogButtonBox, SIGNAL(rejected()),
            this,              SLOT(cancel()));
    
    connect(m_dialogButtonBox, SIGNAL(clicked(QAbstractButton*)),
            this,              SLOT(clicked(QAbstractButton*)));

    QGroupBox* keyboardGroup = new QGroupBox("Keyboard");
    QVBoxLayout* keyboardGroupLayout = new QVBoxLayout();
    
    m_keyboardConfigWidget = new ANORRLKeyboardConfigWidget(m_mainWindow, this);

    connect(m_keyboardConfigWidget, SIGNAL(dataChanged()),
            this,                   SLOT(dataChanged()));
    
    keyboardGroupLayout->addWidget(m_keyboardConfigWidget);
    keyboardGroup->setLayout(keyboardGroupLayout);
    layout->addWidget(keyboardGroup);

    QGroupBox* mouseGroup = new QGroupBox("Mouse");
    QVBoxLayout* mouseGroupLayout = new QVBoxLayout();
    mouseGroup->setLayout(mouseGroupLayout);
    
    m_mouseConfigWidget = new ANORRLMouseConfigWidget(this);

    connect(m_mouseConfigWidget, SIGNAL(dataChanged()),
            this,                SLOT(dataChanged()));

    mouseGroupLayout->addWidget(m_mouseConfigWidget);
    layout->addWidget(mouseGroup);
    
    layout->addWidget(m_dialogButtonBox);
    setLayout(layout);
    
    setWindowModality(Qt::ApplicationModal);
}

void ANORRLInputConfigDialog::clicked(QAbstractButton* button)
{
    if (m_dialogButtonBox->button(QDialogButtonBox::RestoreDefaults) == button)
    {
        restoreAllDefaults();
    }
}

void ANORRLInputConfigDialog::closeEvent(QCloseEvent *event)
{
    // When a user uses the close button.  Prompt them if they'd like
    // to commit their changes.
    
    if (m_dataChanged)
    {
        QMessageBox msgBox;
        msgBox.setText("You have made changes to your settings.");
        msgBox.setInformativeText("Do you want to keep your changes?");
        msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Discard);
        msgBox.setDefaultButton(QMessageBox::Ok);
        int ret = msgBox.exec();
        
        if (ret == QMessageBox::Ok)
        {
            m_keyboardConfigWidget->accept();
            m_mouseConfigWidget->accept();
        }
        else
        {
            m_keyboardConfigWidget->cancel();
            m_mouseConfigWidget->cancel();
        }
    }
    
    m_dataChanged = false;
}

void ANORRLInputConfigDialog::cancel()
{
    m_keyboardConfigWidget->cancel();
    m_mouseConfigWidget->cancel();
    m_dataChanged = false;
    close();
}

void ANORRLInputConfigDialog::accept()
{
    m_keyboardConfigWidget->accept();
    m_mouseConfigWidget->accept();
    m_dataChanged = false;
    close();
}

void ANORRLInputConfigDialog::restoreAllDefaults()
{
    m_keyboardConfigWidget->restoreAllDefaults();
    m_mouseConfigWidget->restoreAllDefaults();
    m_dataChanged = false;
}

void ANORRLInputConfigDialog::dataChanged()
{
    m_dataChanged = true;
}
