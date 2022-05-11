//
// Created by Gustavo on 5/10/22.
//

#include "PXMainWindow.h"
#include <QtWidgets>

PXMainWindow::PXMainWindow(QWidget *parent) : QMainWindow(parent) {

    // Resize and center GUI
    resize(QGuiApplication::primaryScreen()->availableSize() * 3 / 5);
    move(screen()->geometry().center() - frameGeometry().center());
    
    // Populate GUI with widgets
    createMenuBar();
    createCentralWidget();
}

PXMainWindow::~PXMainWindow() = default;

void PXMainWindow::createMenuBar() {
    auto *fileMenu = menuBar()->addMenu(tr("File"));
    auto *aboutMenu = menuBar()->addMenu(tr("About"));

    QAction *aboutAction = aboutMenu->addAction(tr("About"), this, &PXMainWindow::about);
    aboutAction->setStatusTip(tr("Show information about this program"));
}

void PXMainWindow::about() {
    const QString text = tr("<p>This is a technical test for <b>Pixeon</b> recruitment process.\n"
                            "For more information click "
                            "<a href='https://github.com/gustavobat/desafio-pixeon'>here</a>.</p>");

    QMessageBox::about(this, tr("About Desafio Pixeon"), text);
}

void PXMainWindow::createCentralWidget() {

    // Root horizontal root_layout, contains a scroll area and
    // a group of QGroupBox for editing and the list viewer
    scrollArea = new QScrollArea();
    scrollArea->setVisible(true);
    auto *groupBox = new QGroupBox(this);

    auto *root_layout = new QHBoxLayout;
    root_layout->addWidget(groupBox, 2);
    root_layout->addWidget(scrollArea, 8);

    // Create and set central widget from root layout
    auto *centralWidget = new QWidget();
    centralWidget->setLayout(root_layout);
    setCentralWidget(centralWidget);

    // Create vertical layout in groupbox and add list widget and a button
    auto *list = new QListWidget();
    auto *open_image_btn = new QPushButton("Open Image File", this);

    auto *groupbox_layout = new QVBoxLayout;

    groupbox_layout->addWidget(list);
    groupbox_layout->addWidget(open_image_btn);
    groupBox->setLayout(groupbox_layout);
}
