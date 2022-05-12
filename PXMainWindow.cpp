//
// Created by Gustavo on 5/10/22.
//

#include "PXMainWindow.h"
#include <QtWidgets>

PXMainWindow::PXMainWindow(QWidget *parent) : QMainWindow(parent) {

    // Resize and center GUI
    resize(QGuiApplication::primaryScreen()->availableSize() * 3 / 5);
    move(screen()->geometry().center() - frameGeometry().center());

    // Populate GUI with widgets
    createMenuBar();
    createCentralWidget();
}

PXMainWindow::~PXMainWindow() = default;

void PXMainWindow::createMenuBar() {
    auto *fileMenu = menuBar()->addMenu(tr("File"));
    auto *aboutMenu = menuBar()->addMenu(tr("About"));

    QAction *openAction =
        fileMenu->addAction(tr("Open Image"), this, &PXMainWindow::openImageDialog);
    openAction->setStatusTip(tr("Open an image file"));
    openAction->setShortcut(QKeySequence::Open);
    
    QAction *exitAct = fileMenu->addAction(tr("Exit"), this, &QWidget::close);
    exitAct->setShortcut(QKeySequence::Close);

    QAction *aboutAction = aboutMenu->addAction(tr("About"), this, &PXMainWindow::about);
    aboutAction->setStatusTip(tr("Show information about this program"));
}

void PXMainWindow::about() {
    const QString text = tr("<p>This is a technical test for <b>Pixeon</b> recruitment process.\n"
                            "For more information click "
                            "<a href='https://github.com/gustavobat/desafio-pixeon'>here</a>.</p>");

    QMessageBox::about(this, tr("About Desafio Pixeon"), text);
}

void PXMainWindow::createCentralWidget() {
    
    imageLabel.setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    // Root horizontal root_layout, contains a scroll area and
    // a group of QGroupBox for editing and the list viewer
    scrollArea = new QScrollArea();
    scrollArea->setVisible(true);
    scrollArea->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    auto *groupBox = new QGroupBox(this);
    
    auto *root_layout = new QHBoxLayout;
    root_layout->addWidget(groupBox, 2);
    root_layout->addWidget(scrollArea, 8);

    // Create and set central widget from root layout
    auto *centralWidget = new QWidget();
    centralWidget->setLayout(root_layout);
    setCentralWidget(centralWidget);

    // Create vertical layout in groupbox and add list widget and a button
    auto *groupbox_layout = new QVBoxLayout;
    list = new QListWidget();
    groupbox_layout->addWidget(list);
    connect(list, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this,
            SLOT(onListItemDoubleClick(QListWidgetItem*)));
    
    connect(list, SIGNAL(itemPressed(QListWidgetItem*)), this,
            SLOT(onListItemSelected(QListWidgetItem*)));
    
    auto *open_delete_label = new QLabel("Open/Delete Image:");
    auto *open_image_btn = new QPushButton("Open Image File", this);
    connect(open_image_btn, SIGNAL(clicked()), this, SLOT(openImageDialog()));
    
    delete_image_btn = new QPushButton("Delete Image File", this);
    delete_image_btn->setDisabled(true);
    connect(delete_image_btn, SIGNAL(clicked()), this, SLOT(deleteImages()));

    groupbox_layout->addWidget(open_delete_label);
    groupbox_layout->addWidget(open_image_btn);
    groupbox_layout->addWidget(delete_image_btn);

    auto *zoom_label = new QLabel("Zoom options:");
    auto *increase_zoom_btn = new QPushButton("Increase Zoom", this);
    connect(increase_zoom_btn, SIGNAL(clicked()), this, SLOT(zoomIn()));
    auto *decrease_zoom_btn = new QPushButton("Decrease Zoom", this);
    connect(decrease_zoom_btn, SIGNAL(clicked()), this, SLOT(zoomOut()));
    auto *fit_to_screen_btn = new QPushButton("Fit to screen", this);
    connect(fit_to_screen_btn, SIGNAL(clicked()), this, SLOT(fitToScreen()));

    groupbox_layout->addWidget(zoom_label);
    groupbox_layout->addWidget(increase_zoom_btn);
    groupbox_layout->addWidget(decrease_zoom_btn);
    groupbox_layout->addWidget(fit_to_screen_btn);
    
    auto *brightness_label = new QLabel("Brightness adjustment:");
    auto *contrast_label = new QLabel("Constrast adjustment:");
    
    auto * brightness_slider = new QSlider(Qt::Orientation::Horizontal, groupBox);
    auto * contrast_slider = new QSlider(Qt::Orientation::Horizontal, groupBox);
    brightness_slider->setMinimum(-100);
    brightness_slider->setMaximum(100);
    brightness_slider->setValue(0);
    contrast_slider->setMinimum(-100);
    contrast_slider->setMaximum(100);
    contrast_slider->setValue(0);
    connect(brightness_slider, SIGNAL(valueChanged(int)), this, SLOT(onBrightnessChange(int)));
    connect(contrast_slider, SIGNAL(valueChanged(int)), this, SLOT(onContrastChange(int)));
    
    groupbox_layout->addWidget(brightness_label);
    groupbox_layout->addWidget(brightness_slider);
    groupbox_layout->addWidget(contrast_label);
    groupbox_layout->addWidget(contrast_slider);
    
    groupBox->setLayout(groupbox_layout);
}

void PXMainWindow::openImageDialog() {
    QString fileName = QFileDialog::getOpenFileName(
        this, tr("Open Image File"), "",
        tr("Image Files (*.png *.jpg *.jpeg  *.bmp)"));
    if (!fileName.isEmpty()) loadFile(fileName);
}

void PXMainWindow::loadFile(const QString &fileName) {

    const auto stripped_name = strippedName(fileName);
    strippedToAbsoluteFileName.insert({stripped_name, fileName});

    list->addItem(stripped_name);
    setCurrentFile(fileName);
}

void PXMainWindow::setCurrentFile(const QString &fullFileName) {
    original_pixmap = QPixmap(fullFileName);
    drawImage();
}

void PXMainWindow::drawImage() {
    imageLabel.resize(scaleFactor * scrollArea->maximumViewportSize());
    processed_pixmap =
        original_pixmap.scaled(imageLabel.size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    imageLabel.setPixmap(processed_pixmap);
    scrollArea->setWidget(&imageLabel);
    centerScrollBars();
}

QString PXMainWindow::strippedName(const QString &fullFileName) {
    return QFileInfo(fullFileName).fileName();
}

void PXMainWindow::fitToScreen() {
    scaleFactor = 1.0;
    drawImage();
}

void PXMainWindow::scaleImage(const double factor) {
    scaleFactor *= factor;
    drawImage();
}

void PXMainWindow::zoomIn() { scaleImage(1.25); }

void PXMainWindow::zoomOut() { scaleImage(0.8); }

void PXMainWindow::centerScrollBars() {
    std::array<QScrollBar *, 2> bars = {scrollArea->horizontalScrollBar(),
                                        scrollArea->verticalScrollBar()};
    std::for_each(bars.begin(), bars.end(), [](QScrollBar *bar) {
        const auto range = bar->maximum() - bar->minimum() + bar->pageStep();
        bar->setValue(int((range - bar->pageStep()) / 2));
    });
}

void PXMainWindow::onListItemDoubleClick(QListWidgetItem *item) {
    const auto text = item->text();
    if (strippedToAbsoluteFileName.find(text) != strippedToAbsoluteFileName.end()) {
        const auto file_path = strippedToAbsoluteFileName.find(text)->second;
        setCurrentFile(file_path);
    }
}

void PXMainWindow::onListItemSelected(QListWidgetItem *item) {
    delete_image_btn->setEnabled(true);
}

void PXMainWindow::deleteImages() {
    QModelIndexList selectedList = list->selectionModel()->selectedIndexes();
    for(const QModelIndex& singleIndex : selectedList) {
        list->model()->removeRow(singleIndex.row());
    }
    if (list->selectionModel()->selectedIndexes().isEmpty()) {
        delete_image_btn->setDisabled(true);
        imageLabel.clear();
    } else {
        onListItemDoubleClick(list->currentItem());
    }
}

void PXMainWindow::onBrightnessChange(int value) {
    // Convert the pixmap to QImage
    QImage tmp = scaled_pixmap.toImage();
    // Loop all the pixels
    for (int y = 0; y < tmp.height(); y++) {
        for (int x = 0; x < tmp.width(); x++) {
            const auto old_color = tmp.pixelColor(x, y).convertTo(QColor::Hsl);
            auto new_lightness = old_color.lightness() + value;
            if (new_lightness > 255) new_lightness = 255;
            if (new_lightness < 0) new_lightness = 0;
            const auto new_color = QColor::fromHsl(old_color.hue(), old_color.saturation(),
                                                   new_lightness, old_color.alpha());
            tmp.setPixelColor(x, y, new_color);
        }
    }

    // Get the coloured pixmap
    imageLabel.setPixmap(QPixmap::fromImage(tmp));
    scrollArea->setWidget(&imageLabel);
    centerScrollBars();
}

void PXMainWindow::onContrastChange(int value) {
    
    // Convert the pixmap to QImage
    QImage tmp = scaled_pixmap.toImage();
    int min_light = 1000;
    int max_light = -1000;
    
    // Loop all the pixels
    for (int y = 0; y < tmp.height(); y++) {
        for (int x = 0; x < tmp.width(); x++) {
            const int l_channel = tmp.pixelColor(x, y).convertTo(QColor::Hsl).lightness();
            if (l_channel > max_light) max_light = l_channel;
            if (l_channel < min_light) min_light = l_channel;
        }
    }
    
    // Loop all the pixels
    for (int y = 0; y < tmp.height(); y++) {
        for (int x = 0; x < tmp.width(); x++) {
            const auto old_color = tmp.pixelColor(x, y).convertTo(QColor::Hsl);
            auto new_lightness =
                (255 + value) * (old_color.lightness() - min_light) / (max_light - min_light);
            if (new_lightness > 255) new_lightness = 255;
            if (new_lightness < 0) new_lightness = 0;
            const auto new_color = QColor::fromHsl(old_color.hue(), old_color.saturation(),
                                                   new_lightness, old_color.alpha());
            tmp.setPixelColor(x, y, new_color);
        }
    }
    
    // Get the coloured pixmap
    imageLabel.setPixmap(QPixmap::fromImage(tmp));
    scrollArea->setWidget(&imageLabel);
    centerScrollBars();
}
