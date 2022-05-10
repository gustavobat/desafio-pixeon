//
// Created by Gustavo on 5/10/22.
//

#include "PXMainWindow.h"
#include <QtWidgets>

PXMainWindow::PXMainWindow(QWidget *parent) : QMainWindow(parent) {

    // Resize and center GUI
    this->resize(800, 600);
    move(screen()->geometry().center() - frameGeometry().center());
}

PXMainWindow::~PXMainWindow() = default;
