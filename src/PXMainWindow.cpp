//
// Created by Gustavo on 5/10/22.
//

#include "PXMainWindow.h"
#include <QtWidgets>

PXMainWindow::PXMainWindow(QWidget *parent) : QMainWindow(parent), render_thread(this) {

    // Resize and center GUI
    resize(QGuiApplication::primaryScreen()->availableSize() * 3 / 5);
    move(screen()->geometry().center() - frameGeometry().center());

    // Populate GUI with widgets
    createMenuBar();
    createCentralWidget();

    connect(&render_thread, &PXRenderThread::renderedImage, this, &PXMainWindow::updatePixmap);
}

PXMainWindow::~PXMainWindow() = default;

void PXMainWindow::createMenuBar() {
    auto *file_menu = menuBar()->addMenu(tr("&File"));

    auto *open_action =
        file_menu->addAction(tr("&Open Image"), this, &PXMainWindow::openImageDialog);
    open_action->setStatusTip(tr("Open an image file"));
    open_action->setShortcut(QKeySequence::Open);

    auto *exit_action = file_menu->addAction(tr("E&xit"), this, &QWidget::close);
    exit_action->setStatusTip(tr("Close this program"));
    exit_action->setShortcut(QKeySequence::Close);

    auto *about_action = menuBar()->addAction(tr("&About"), this, &PXMainWindow::about);
    about_action->setStatusTip(tr("Show information about this program"));
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
    scrollArea->setWidget(&imageLabel);
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

    populateGroupBoxLayout(groupBox, groupbox_layout);

    groupBox->setLayout(groupbox_layout);
}

void PXMainWindow::populateGroupBoxLayout(QGroupBox *groupBox, QVBoxLayout *groupbox_layout) {
    
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
    auto *contrast_label = new QLabel("Contrast adjustment:");

    const auto create_slider = [groupBox](int min, int max, int initial_val) {
        auto *slider = new QSlider(Qt::Horizontal, groupBox);
        slider->setMinimum(min);
        slider->setMaximum(max);
        slider->setValue(initial_val);
        return slider;
    };

    auto *brightness_slider = create_slider(-100, 100, 0);
    auto *contrast_slider = create_slider(-100, 100, 0);
    connect(brightness_slider, SIGNAL(valueChanged(int)), this, SLOT(onBrightnessChange(int)));
    connect(contrast_slider, SIGNAL(valueChanged(int)), this, SLOT(onContrastChange(int)));

    groupbox_layout->addWidget(brightness_label);
    groupbox_layout->addWidget(brightness_slider);
    groupbox_layout->addWidget(contrast_label);
    groupbox_layout->addWidget(contrast_slider);
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
    const QSize new_size = scaleFactor * scrollArea->maximumViewportSize();
    imageLabel.resize(new_size);
    render_thread.render(brightnessFactor, contrastFactor, new_size, &original_pixmap);
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
    brightnessFactor = value;
    drawImage();
}

void PXMainWindow::onContrastChange(int value) {
    contrastFactor = value;
    drawImage();
}

void PXMainWindow::updatePixmap(const QPixmap &pixmap) {
    imageLabel.setPixmap(pixmap);
    centerScrollBars();
    update();
}
