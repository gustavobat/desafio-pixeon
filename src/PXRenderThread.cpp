#include "PXRenderThread.h"
#include <QImage>
#include <QPixmap>

PXRenderThread::PXRenderThread(QObject *parent) : QThread(parent) { }

PXRenderThread::~PXRenderThread() {
    m_mutex.lock();
    m_abort = true;
    m_condition.wakeOne();
    m_mutex.unlock();
    wait();
}

void PXRenderThread::render(int brightness_factor, int contrast_factor, QSize size,
                            QPixmap *orig_pixmap) {
    
    // Set class data
    QMutexLocker locker(&m_mutex);
    this->m_brightness_factor = brightness_factor;
    this->m_contrast_factor = contrast_factor;
    this->m_result_size = size;
    this->m_original_pixmap = orig_pixmap;
    
    // Start task of restart current one
    if (!isRunning()) {
        start(LowPriority);
    } else {
        m_restart = true;
        m_condition.wakeOne();
    }
}

void PXRenderThread::run() {
    forever {
        m_mutex.lock();
        const QSize result_size = this->m_result_size;
        m_mutex.unlock();
        
        if (m_restart) break;
        if (m_abort) return;
        
        // Scale pixmap
        QPixmap processed_pixmap =
            m_original_pixmap->scaled(result_size, Qt::KeepAspectRatio, Qt::SmoothTransformation);

        // Adjust brightness/contrast if needed
        if (m_brightness_factor != 0) adjustBrightness(processed_pixmap);
        if (m_contrast_factor != 0) adjustContrast(processed_pixmap);

        // Emit signal with rendered image
        if (!m_restart) emit renderedImage(processed_pixmap);

        // Put thread to wait
        m_mutex.lock();
        if (!m_restart) m_condition.wait(&m_mutex);
        m_restart = false;
        m_mutex.unlock();
    }
}

void PXRenderThread::adjustBrightness(QPixmap& processed_pixmap) const {
    // Convert the Pixmap to QImage
    QImage tmp = processed_pixmap.toImage();
    
    // Loop all the pixels and update lightness channel
    for (int y = 0; y < tmp.height(); y++) {
        for (int x = 0; x < tmp.width(); x++) {
            const auto old_color = tmp.pixelColor(x, y).convertTo(QColor::Hsl);
            auto new_lightness = old_color.lightness() + m_brightness_factor;
            if (new_lightness > 255) new_lightness = 255;
            if (new_lightness < 0) new_lightness = 0;
            const auto new_color = QColor::fromHsl(old_color.hue(), old_color.saturation(),
                                                   new_lightness, old_color.alpha());
            tmp.setPixelColor(x, y, new_color);
        }
    }
    
    // Convert Pixmap back from QImage
    processed_pixmap = QPixmap::fromImage(tmp);
}

void PXRenderThread::adjustContrast(QPixmap& processed_pixmap) const {
    
    // Convert Pixmap QImage
    QImage tmp = processed_pixmap.toImage();
    int min_light = 10000;
    int max_light = -10000;
    
    // Loop all the pixels and retrieve min/max lightness channel values
    for (int y = 0; y < tmp.height(); y++) {
        for (int x = 0; x < tmp.width(); x++) {
            const int l_channel = tmp.pixelColor(x, y).convertTo(QColor::Hsl).lightness();
            if (l_channel > max_light) max_light = l_channel;
            if (l_channel < min_light) min_light = l_channel;
        }
    }
    
    // Loop all the pixels and update lightness channel
    for (int y = 0; y < tmp.height(); y++) {
        for (int x = 0; x < tmp.width(); x++) {
            const auto old_color = tmp.pixelColor(x, y).convertTo(QColor::Hsl);
            auto new_lightness = (255 + m_contrast_factor) *
                                 (old_color.lightness() - min_light) / (max_light - min_light);
            if (new_lightness > 255) new_lightness = 255;
            if (new_lightness < 0) new_lightness = 0;
            const auto new_color = QColor::fromHsl(old_color.hue(), old_color.saturation(),
                                                   new_lightness, old_color.alpha());
            tmp.setPixelColor(x, y, new_color);
        }
    }
    
    // Convert Pixmap back from QImage
    processed_pixmap = QPixmap::fromImage(tmp);
}
