#ifndef PXMAINWINDOW_H
#define PXMAINWINDOW_H

#include <QMainWindow>

class PXMainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit PXMainWindow(QWidget *parent = nullptr);
    ~PXMainWindow() override;

};

#endif // PXMAINWINDOW_H