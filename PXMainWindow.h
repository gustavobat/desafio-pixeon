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

    QLabel imageLabel;
    QPixmap pixmap;
    
    QScrollArea * scrollArea = nullptr;
    QListWidget * list = nullptr;
    
private slots:
    void openImageDialog();
    void loadFile(const QString &fileName);
};

#endif // PXMAINWINDOW_H