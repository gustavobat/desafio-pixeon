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

    // Root horizontal root_layout, contains a scroll area and
    // a group of QGroupBox for editing and the list viewer
    scrollArea = new QScrollArea();
    scrollArea->setVisible(true);
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

    auto *open_delete_label = new QLabel("Open/Delete Image:");
    auto *open_image_btn = new QPushButton("Open Image File", this);
    auto *delete_image_btn = new QPushButton("Delete Image File", this);
    connect(open_image_btn, SIGNAL(clicked()), this, SLOT(openImageDialog()));

    groupbox_layout->addWidget(open_delete_label);
    groupbox_layout->addWidget(open_image_btn);
    groupbox_layout->addWidget(delete_image_btn);

    auto *zoom_label = new QLabel("Zoom options:");
    auto *increase_zoom_btn = new QPushButton("Increase Zoom", this);
    auto *decrease_zoom_btn = new QPushButton("Decrease Zoom", this);
    auto *fit_to_screen = new QPushButton("Fit to screen", this);

    groupbox_layout->addWidget(zoom_label);
    groupbox_layout->addWidget(increase_zoom_btn);
    groupbox_layout->addWidget(decrease_zoom_btn);
    groupbox_layout->addWidget(fit_to_screen);

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
    pixmap = QPixmap(fullFileName);
    drawImage();
}

void PXMainWindow::drawImage() {
    imageLabel.resize(scaleFactor * scrollArea->maximumViewportSize() );
    imageLabel.setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    QPixmap scaled_pixmap =
        pixmap.scaled(imageLabel.size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    imageLabel.setPixmap(scaled_pixmap);

    scrollArea->setWidget(&imageLabel);
}

QString PXMainWindow::strippedName(const QString &fullFileName) {
    return QFileInfo(fullFileName).fileName();
}