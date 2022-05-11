#ifndef PXMAINWINDOW_H
#define PXMAINWINDOW_H

#include <QLabel>
#include <QMainWindow>
#include <QScrollArea>

class PXMainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit PXMainWindow(QWidget *parent = nullptr);
    ~PXMainWindow() override;

private:
    void createMenuBar();
    void createCentralWidget();
    void about();
    static QString strippedName(const QString &fullFileName);
    
    QScrollArea * scrollArea = nullptr;
    QListWidget * list = nullptr;
};

#endif // PXMAINWINDOW_H