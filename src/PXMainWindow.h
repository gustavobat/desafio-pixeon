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
    void createGroupBoxWidgets(QGroupBox *groupBox);
    void about();
    void disableControls();
    void enableControls();
    static QString strippedName(const QString &fullFileName);
    void setCurrentFile(const QString &fullFileName);
    void drawImage();
    void scaleImage(double factor);
    void centerScrollBars();

    QLabel m_image_label;
    QPixmap m_original_pixmap;
    double m_scale_factor = 1.0;
    int m_contrast_factor = 0;
    int m_brightness_factor = 0;
    
    PXRenderThread m_render_thread;
    
    QLabel *m_zoom_label = nullptr;
    QLabel *m_brightness_label = nullptr;
    QLabel *m_contrast_label = nullptr;
    QPushButton *m_delete_image_btn = nullptr;
    QPushButton *m_increase_zoom_btn = nullptr;
    QPushButton *m_decrease_zoom_btn = nullptr;
    QPushButton *m_fit_to_screen_btn = nullptr;
    QSlider *m_brightness_slider = nullptr;
    QSlider *m_contrast_slider = nullptr;
    
    QScrollArea *m_scroll_area = nullptr;
    QListWidget *m_list = nullptr;

    std::map<QString, QString> m_stripped_to_absolute_file_name{};

private slots:
    void openImageDialog();
    void loadFile(const QString &fileName);
    void fitToScreen();
    void zoomIn();
    void zoomOut();
    void onListItemDoubleClick(QListWidgetItem * item);
    void deleteImages();
    void onBrightnessChange(int value);
    void onContrastChange(int value);
    void updatePixmap(const QPixmap &pixmap);
};

#endif // PXMAINWINDOW_H