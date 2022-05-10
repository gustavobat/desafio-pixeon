//
// Created by Gustavo Batistela on 5/10/22.
//

#include <QApplication>
#include <QPushButton>

int main(int argc, char *argv[]) {

    QApplication::setApplicationName("Desafio Pixeon");
    QApplication a(argc, argv);
    QPushButton button("Hello world!", nullptr);
    button.resize(200, 100);
    button.show();
    return QApplication::exec();
}
