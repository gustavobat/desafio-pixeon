#ifndef PXRENDERTHREAD_H
#define PXRENDERTHREAD_H

#include <QMutex>
#include <QSize>
#include <QThread>
#include <QWaitCondition>

class PXRenderThread : public QThread {
    Q_OBJECT

public:
    explicit PXRenderThread(QObject *parent = nullptr);
    ~PXRenderThread() override;
    
    void render(int brightness_factor, int contrast_factor, QSize size, QPixmap * orig_pixmap);

signals:
    void renderedImage(const QPixmap& result_pixmap);

protected:
    [[noreturn]] void run() override;

private:
    void adjustBrightness(QPixmap &processed_pixmap) const;
    void adjustContrast(QPixmap &processed_pixmap) const;
    
    QPixmap *m_original_pixmap = nullptr;
    QSize m_result_size;
    int m_brightness_factor = 0.0;
    int m_contrast_factor = 0.0;
    
    QMutex m_mutex;
    QWaitCondition m_condition;
    bool m_restart = false;
};

#endif // PXRENDERTHREAD_H
