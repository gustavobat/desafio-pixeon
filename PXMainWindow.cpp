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
}
