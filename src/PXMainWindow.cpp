#include "PXMainWindow.h"
#include <QtWidgets>

PXMainWindow::PXMainWindow(QWidget *parent) : QMainWindow(parent), m_render_thread(this) {

    // Resize and center GUI
    resize(QGuiApplication::primaryScreen()->availableSize() * 3 / 5);
    move(QGuiApplication::primaryScreen()->geometry().center() - frameGeometry().center());
    
    // Set center alignment of m_image_label that will display the images
    m_image_label.setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    
    // Populate GUI with widgets
    createMenuBar();
    createCentralWidget();

    // Create signal/slot connection to rendering thread
    connect(&m_render_thread, &PXRenderThread::renderedImage, this, &PXMainWindow::updatePixmap);
}

PXMainWindow::~PXMainWindow() = default;

void PXMainWindow::createMenuBar() {
    auto *file_menu = menuBar()->addMenu(tr("&File"));

    auto *open_action =
        file_menu->addAction(tr("&Open Image File"), this, &PXMainWindow::openImageDialog);
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
    
    // This method creates the main/central widget and its layout
    // The root layout contains a scroll area and a QGroupBox
    // with the m_list viewer and the control panel
    auto *root_layout = new QHBoxLayout;
    
    // Create scroll area
    m_scroll_area = new QScrollArea();
    m_scroll_area->setVisible(true);
    m_scroll_area->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    m_scroll_area->setWidget(&m_image_label);
    
    // Create group box
    auto *group_box = new QGroupBox(this);
    createGroupBoxWidgets(group_box);
    
    // Add widgets to layout
    root_layout->addWidget(group_box, 2);
    root_layout->addWidget(m_scroll_area, 8);

    // Create and set central widget from root layout
    auto *central_widget = new QWidget();
    central_widget->setLayout(root_layout);
    setCentralWidget(central_widget);

}

void PXMainWindow::createGroupBoxWidgets(QGroupBox *group_box) {
    // Create vertical layout in groupbox and add m_list widget and buttons/sliders
    auto *groupbox_layout = new QVBoxLayout;
    
    // Create m_list
    m_list = new QListWidget();
    groupbox_layout->addWidget(m_list);
    
    // Create open/delete image label/buttons
    auto *open_delete_label = new QLabel("Open/Delete Image:");
    auto *open_image_btn = new QPushButton("Open Image File", this);
    m_delete_image_btn = new QPushButton("Delete Image File", this);
    
    // Create zoom related label/buttons
    m_zoom_label = new QLabel("Zoom options:");
    m_increase_zoom_btn = new QPushButton("Zoom In", this);
    m_decrease_zoom_btn = new QPushButton("Zoom Out", this);
    m_fit_to_screen_btn = new QPushButton("Fit to screen", this);
   
    // Create brightness/contrast related labels/sliders
    m_brightness_label = new QLabel("Brightness adjustment:");
    m_contrast_label = new QLabel("Contrast adjustment:");
    
    const auto create_slider = [group_box](int min, int max, int initial_val) {
      auto *slider = new QSlider(Qt::Horizontal, group_box);
      slider->setMinimum(min);
      slider->setMaximum(max);
      slider->setValue(initial_val);
      return slider;
    };

    m_brightness_slider = create_slider(-100, 100, 0);
    m_contrast_slider = create_slider(-100, 100, 0);
    
    // Connect widgets signals to slots
    connect(m_list, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this,
            SLOT(onListItemDoubleClick(QListWidgetItem*)));
    connect(open_image_btn, SIGNAL(clicked()), this, SLOT(openImageDialog()));
    connect(m_delete_image_btn, SIGNAL(clicked()), this, SLOT(deleteImages()));
    connect(m_increase_zoom_btn, SIGNAL(clicked()), this, SLOT(zoomIn()));
    connect(m_decrease_zoom_btn, SIGNAL(clicked()), this, SLOT(zoomOut()));
    connect(m_fit_to_screen_btn, SIGNAL(clicked()), this, SLOT(fitToScreen()));
    connect(m_brightness_slider, SIGNAL(valueChanged(int)), this, SLOT(onBrightnessChange(int)));
    connect(m_contrast_slider, SIGNAL(valueChanged(int)), this, SLOT(onContrastChange(int)));
    
    // Add widgets to layout
    groupbox_layout->addWidget(open_delete_label);
    groupbox_layout->addWidget(open_image_btn);
    groupbox_layout->addWidget(m_delete_image_btn);
    groupbox_layout->addWidget(m_zoom_label);
    groupbox_layout->addWidget(m_increase_zoom_btn);
    groupbox_layout->addWidget(m_decrease_zoom_btn);
    groupbox_layout->addWidget(m_fit_to_screen_btn);
    groupbox_layout->addWidget(m_brightness_label);
    groupbox_layout->addWidget(m_brightness_slider);
    groupbox_layout->addWidget(m_contrast_label);
    groupbox_layout->addWidget(m_contrast_slider);

    disableControls();

    // Set groupbox layout
    group_box->setLayout(groupbox_layout);
}

void PXMainWindow::disableControls() {
    m_delete_image_btn->setDisabled(true);
    m_zoom_label->setDisabled(true);
    m_increase_zoom_btn->setDisabled(true);
    m_decrease_zoom_btn->setDisabled(true);
    m_fit_to_screen_btn->setDisabled(true);
    m_brightness_label->setDisabled(true);
    m_brightness_slider->setDisabled(true);
    m_contrast_label->setDisabled(true);
    m_contrast_slider->setDisabled(true);
}

void PXMainWindow::enableControls() {
    m_delete_image_btn->setEnabled(true);
    m_zoom_label->setEnabled(true);
    m_increase_zoom_btn->setEnabled(true);
    m_decrease_zoom_btn->setEnabled(true);
    m_fit_to_screen_btn->setEnabled(true);
    m_brightness_label->setEnabled(true);
    m_brightness_slider->setEnabled(true);
    m_contrast_label->setEnabled(true);
    m_contrast_slider->setEnabled(true);
}

void PXMainWindow::openImageDialog() {
    QString file_name = QFileDialog::getOpenFileName(
        this, tr("Open Image File"), "",
        tr("Image Files (*.png *.jpg *.jpeg  *.bmp)"));
    if (!file_name.isEmpty()) loadFile(file_name);
}

void PXMainWindow::loadFile(const QString &file_name) {

    const auto stripped_name = strippedName(file_name);
    
    if (m_stripped_to_absolute_file_name.find(stripped_name) !=
        m_stripped_to_absolute_file_name.end()) {
        QMessageBox msg_box;
        msg_box.setText("Please select a file with a different name.");
        msg_box.exec();
        return;
    }

    m_stripped_to_absolute_file_name.insert({stripped_name, file_name});

    m_list->addItem(stripped_name);
    setCurrentFile(file_name);
    enableControls();
}

void PXMainWindow::setCurrentFile(const QString &full_file_name) {
    m_original_pixmap = QPixmap(full_file_name);
    m_scale_factor = 1.0;
    m_brightness_factor = 0.0;
    m_contrast_factor = 0.0;
    drawImage();
}

void PXMainWindow::drawImage() {
    const QSize new_size = m_scale_factor * m_scroll_area->maximumViewportSize();
    m_image_label.resize(new_size);
    m_render_thread.render(m_brightness_factor, m_contrast_factor, new_size, &m_original_pixmap);
}

QString PXMainWindow::strippedName(const QString &full_file_name) {
    return QFileInfo(full_file_name).fileName();
}

void PXMainWindow::fitToScreen() {
    m_scale_factor = 1.0;
    drawImage();
}

void PXMainWindow::scaleImage(const double factor) {
    m_scale_factor *= factor;
    drawImage();
}

void PXMainWindow::zoomIn() { scaleImage(1.25); }

void PXMainWindow::zoomOut() { scaleImage(0.8); }

void PXMainWindow::centerScrollBars() {
    std::array<QScrollBar *, 2> bars = {m_scroll_area->horizontalScrollBar(),
                                        m_scroll_area->verticalScrollBar()};
    std::for_each(bars.begin(), bars.end(), [](QScrollBar *bar) {
        const auto range = bar->maximum() - bar->minimum() + bar->pageStep();
        bar->setValue(int((range - bar->pageStep()) / 2));
    });
}

void PXMainWindow::onListItemDoubleClick(QListWidgetItem *item) {
    const auto text = item->text();
    if (m_stripped_to_absolute_file_name.find(text) != m_stripped_to_absolute_file_name.end()) {
        const auto file_path = m_stripped_to_absolute_file_name.find(text)->second;
        setCurrentFile(file_path);
    }
}

void PXMainWindow::deleteImages() {
    QList<QListWidgetItem*> items = m_list->selectedItems();
    foreach(QListWidgetItem * item, items) {
        const QString text = item->text();
        delete m_list->takeItem(m_list->row(item));
        m_stripped_to_absolute_file_name.erase(text);
    }
    
    if (m_list->selectionModel()->selectedIndexes().isEmpty()) {
        m_stripped_to_absolute_file_name.clear();
        m_list->clear();
        m_list->setCurrentItem(nullptr);
        disableControls();
        m_image_label.clear();
    } else {
        onListItemDoubleClick(m_list->currentItem());
    }
    m_brightness_slider->setValue(0);
    m_brightness_factor = 0.;
    m_contrast_slider->setValue(0);
    m_contrast_factor = 0.;
    m_scale_factor = 1.;
}

void PXMainWindow::onBrightnessChange(int value) {
    m_brightness_factor = value;
    drawImage();
}

void PXMainWindow::onContrastChange(int value) {
    m_contrast_factor = value;
    drawImage();
}

void PXMainWindow::updatePixmap(const QPixmap &pixmap) {
    m_image_label.setPixmap(pixmap);
    centerScrollBars();
    update();
}
