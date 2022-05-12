#ifndef PXMAINWINDOW_H
#define PXMAINWINDOW_H

#include <QLabel>
#include <QListWidget>
#include <QMainWindow>
#include <QPushButton>
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
    void centerScrollBars();

    QLabel imageLabel;
    QPushButton *delete_image_btn = nullptr;
    QPixmap pixmap;
    double scaleFactor = 1.0;
    int contrastFactor = 0;
    int brightnessFactor = 0;
    
    QScrollArea * scrollArea = nullptr;
    QListWidget * list = nullptr;

    std::map<QString, QString> strippedToAbsoluteFileName;

private slots:
    void openImageDialog();
    void loadFile(const QString &fileName);
    void fitToScreen();
    void zoomIn();
    void zoomOut();
    void onListItemDoubleClick(QListWidgetItem * item);
    void onListItemSelected(QListWidgetItem * item);
    void deleteImages();
    void onBrightnessChange(int value);
    void onContrastChange(int value);
};

#endif // PXMAINWINDOW_H