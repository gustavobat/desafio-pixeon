#ifndef PXMAINWINDOW_H
#define PXMAINWINDOW_H

#include <QLabel>
#include <QListWidget>
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
    void setCurrentFile(const QString &fullFileName);
    void drawImage();
    void scaleImage(double factor);

    QLabel imageLabel;
    QPixmap pixmap;
    double scaleFactor = 1.0;
    
    QScrollArea * scrollArea = nullptr;
    QListWidget * list = nullptr;

    std::map<QString, QString> strippedToAbsoluteFileName;

private slots:
    void openImageDialog();
    void loadFile(const QString &fileName);
    void fitToScreen();
    void zoomIn();
    void zoomOut();
};

#endif // PXMAINWINDOW_H