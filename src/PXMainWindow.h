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

/**
 * \brief PXMainWindow class: responsible for the coordinating GUI inputs and
 * displaying a list of images and the widgets that control them.
 */
class PXMainWindow : public QMainWindow {
    Q_OBJECT

public:
    /** Public methods section */

    /** \brief Constructor */
    explicit PXMainWindow(QWidget *parent = nullptr);
    /** \brief Destructor */
    ~PXMainWindow() override;

private:
    /** Private member methods section */
    
    /** \brief Method to create the components of the top menu bar */
    void createMenuBar();
    /** \brief Method to create the central components */
    void createCentralWidget();
    /** \brief Populate the QGroupBox widget that is displayed at the left part of the GUI */
    void createGroupBoxWidgets(QGroupBox *group_box);
    /** \brief Callback function to display a message box with info about this program */
    void about();
    /** \brief Disable image processing controls. Called if no image is loaded */
    void disableControls();
    /** \brief Enable image processing controls. Called when an image is loaded */
    void enableControls();
    /** \brief Given a file path, retrieve the stripped file name to be inserted in the list view */
    static QString strippedName(const QString &full_file_name);
    /** \brief Set the file of the current image to be displayed */
    void setCurrentFile(const QString &full_file_name);
    /** \brief Render image on screen using a worker thread */
    void drawImage();
    /** \brief Scale image size given a multiplying factor.
     * This is called by both zoomIn and zoomOut slots
     */
    void scaleImage(double factor);
    /** \brief Center scroll bars once the image is rescaled */
    void centerScrollBars();

    /** Private member variables section */

    /** \brief Image label that is shown in scroll area */
    QLabel m_image_label;
    /** \brief Pixmap object that is used to cache a non-modified version of the image */
    QPixmap m_original_pixmap;
    /** \brief Store the factor used to scale the image */
    double m_scale_factor = 1.0;
    /** \brief Store how much should the brightness be increased/decreased */
    int m_brightness_factor = 0;
    /** \brief Store how much should the contrast be increased/decreased */
    int m_contrast_factor = 0;
    
    /** \brief Rendering thread object */
    PXRenderThread m_render_thread;
    
    /** \brief Pointers to different GUI elements to allow for access after creation */
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
    
    /** \brief Data structure that maps the stripped to the absolute path of images */
    std::map<QString, QString> m_stripped_to_absolute_file_name{};

private slots:
    /** Slots section */
    
    /** \brief Trigger image file opening dialog */
    void openImageDialog();
    /** \brief Load an image file given its path */
    void loadFile(const QString &file_name);
    /** \brief Rescale image so it fits the entire screen */
    void fitToScreen();
    /** \brief Increase image zoom */
    void zoomIn();
    /** \brief Decrease image zoom */
    void zoomOut();
    /** \brief Update displayed image upon double-clicking its name in the list */
    void onListItemDoubleClick(QListWidgetItem * item);
    /** \brief Remove an image if selected from list or multiple images if none is selected */
    void deleteImages();
    /** \brief Handle change on brightness slider */
    void onBrightnessChange(int value);
    /** \brief Handle change on contrast slider */
    void onContrastChange(int value);
    /** \brief Slot to receive a new rendered Pixmap from the rendering thread */
    void updatePixmap(const QPixmap &pixmap);
};

#endif // PXMAINWINDOW_H