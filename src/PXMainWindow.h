#ifndef PXMAINWINDOW_H
#define PXMAINWINDOW_H

#include <QGroupBox>
#include <QLabel>
#include <QListWidget>
#include <QMainWindow>
#include <QPushButton>
#include <QScrollArea>
#include <QVBoxLayout>

#include "PXRenderThread.h"

class PXMainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit PXMainWindow(QWidget *parent = nullptr);
    ~PXMainWindow() override;

private:
    void createMenuBar();
    void createCentralWidget();
    void populateGroupBoxLayout(QGroupBox *groupBox, QVBoxLayout *groupbox_layout);
    void about();
    static QString strippedName(const QString &fullFileName);
    void setCurrentFile(const QString &fullFileName);
    void drawImage();
    void scaleImage(double factor);
    void centerScrollBars();

    QLabel imageLabel;
    QPushButton *delete_image_btn = nullptr;
    QPixmap original_pixmap;
    double scaleFactor = 1.0;
    int contrastFactor = 0;
    int brightnessFactor = 0;
    
    PXRenderThread render_thread;
    
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
    void updatePixmap(const QPixmap &pixmap);
};

#endif // PXMAINWINDOW_H