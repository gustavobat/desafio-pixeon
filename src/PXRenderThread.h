#ifndef PXRENDERTHREAD_H
#define PXRENDERTHREAD_H

#include <QMutex>
#include <QSize>
#include <QThread>
#include <QWaitCondition>

/**
 * \brief PXRenderThread class: responsible for the applying scaling, b
 * rightness and contrast transformations to a QPixmap
 */
class PXRenderThread : public QThread {
    Q_OBJECT

public:
    /** Public methods section */
    
    /** \brief Constructor */
    explicit PXRenderThread(QObject *parent = nullptr);
    /** \brief Destructor */
    ~PXRenderThread() override;
    
    /** \brief Process a QPixmap given brightness, contrast and result size params */
    void render(int brightness_factor, int contrast_factor, QSize size, QPixmap * orig_pixmap);

signals:
    /** \brief Signal to be emitted when a rendering result is finished */
    void renderedImage(const QPixmap& result_pixmap);

protected:
    /** \brief Perform image processing */
    void run() override;

private:
    /** Private methods section */
    /** \brief Edit the brightness of an image */
    void adjustBrightness(QPixmap &processed_pixmap) const;
    /** \brief Edit the contrast of an image */
    void adjustContrast(QPixmap &processed_pixmap) const;
    
    /** Private member variables */
    /** \brief Pointer to the original QPixmap obj */
    QPixmap *m_original_pixmap = nullptr;
    /** \brief Size to scale the image */
    QSize m_result_size;
    /** \brief Store how much should the brightness be increased/decreased */
    int m_brightness_factor = 0.0;
    /** \brief Store how much should the contrast be increased/decreased */
    int m_contrast_factor = 0.0;
    
    /** \brief Thread controlling variables: mutex, wait condition and restart/abort flags */
    QMutex m_mutex;
    QWaitCondition m_condition;
    bool m_restart = false;
    bool m_abort = false;
};

#endif // PXRENDERTHREAD_H
