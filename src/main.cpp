//
// Created by Gustavo Batistela on 5/10/22.
//

#include "PXMainWindow.h"
#include <QApplication>

// This is the entry point of the program. The main function prepares the QApplication and
// initializes/shows an instance of PXMainWindow class
int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    QApplication::setApplicationDisplayName("Desafio Pixeon");

    PXMainWindow mainWindow;
    mainWindow.show();

    return QApplication::exec();
}
