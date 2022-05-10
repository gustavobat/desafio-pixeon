//
// Created by Gustavo Batistela on 5/10/22.
//

#include "PXMainWindow.h"
#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    QApplication::setApplicationDisplayName("Desafio Pixeon");
    
    PXMainWindow mainWindow;
    mainWindow.show();
    
    return QApplication::exec();
}
