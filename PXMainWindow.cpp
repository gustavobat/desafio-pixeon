//
// Created by Gustavo on 5/10/22.
//

#include "PXMainWindow.h"
#include <QtWidgets>

PXMainWindow::PXMainWindow(QWidget *parent) : QMainWindow(parent) {

    // Resize and center GUI
    resize(QGuiApplication::primaryScreen()->availableSize() * 3 / 5);
    move(screen()->geometry().center() - frameGeometry().center());

    createMenuBar();
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
