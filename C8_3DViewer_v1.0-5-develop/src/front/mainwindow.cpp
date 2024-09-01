#include "mainwindow.h"

#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  setWindowTitle("3DViewer_v1.0");
  sliderSetup();
  connectSetup();
  defaultSettings();

  // gif saving
  connect(ui->gif_button, &QPushButton::clicked, this,
          &MainWindow::on_gif_button_clicked);
  connect(&timer_for_gif, &QTimer::timeout, this, &MainWindow::create_screen);
}

MainWindow::~MainWindow() {
  saveSettings();
  delete ui;
}

/* ------------------ SETUP ------------------ */

void MainWindow::sliderSetup() {
  // Scale
  ui->scale_slider->setRange(1, maxScale);
  ui->scale_slider->setSingleStep(1);

  // Edge thickness
  ui->slider_edge_thickness->setRange(1, MAX_THICKNESS);
  ui->slider_edge_thickness->setSingleStep(1);

  // Verticle size
  ui->slider_vertice_size->setRange(1, MAX_THICKNESS);
  ui->slider_vertice_size->setSingleStep(1);

  // Rotation
  ui->slider_x_rotation->setRange(0, FULL_CIRCLE * rotation_factor);
  ui->slider_x_rotation->setSingleStep(rotation_factor);
  ui->slider_x_rotation->setPageStep(15 * rotation_factor);

  ui->slider_y_rotation->setRange(0, FULL_CIRCLE * rotation_factor);
  ui->slider_y_rotation->setSingleStep(rotation_factor);
  ui->slider_y_rotation->setPageStep(15 * rotation_factor);

  ui->slider_z_rotation->setRange(0, FULL_CIRCLE * rotation_factor);
  ui->slider_z_rotation->setSingleStep(rotation_factor);
  ui->slider_z_rotation->setPageStep(15 * rotation_factor);

  // Moving
  ui->slider_x_moving->setRange(0, MOVING_CONST_SLIDER);
  ui->slider_x_moving->setSingleStep(1);

  ui->slider_y_moving->setRange(0, MOVING_CONST_SLIDER);
  ui->slider_y_moving->setSingleStep(1);

  ui->slider_z_moving->setRange(0, MOVING_CONST_SLIDER);
  ui->slider_z_moving->setSingleStep(1);
}

void MainWindow::movingConnectSetup() {
  // Move X
  connect(ui->slider_x_moving, &QScrollBar::valueChanged, ui->glwidget,
          &GLWidget::setXMoving);
  connect(ui->moving_x_value, SIGNAL(editingFinished()), (this),
          SLOT(xMovingTextEdit()));
  // Move Y
  connect(ui->slider_y_moving, &QScrollBar::valueChanged, ui->glwidget,
          &GLWidget::setYMoving);
  connect(ui->moving_y_value, SIGNAL(editingFinished()), (this),
          SLOT(yMovingTextEdit()));
  // Move Z
  connect(ui->slider_z_moving, &QScrollBar::valueChanged, ui->glwidget,
          &GLWidget::setZMoving);
  connect(ui->moving_z_value, SIGNAL(editingFinished()), (this),
          SLOT(zMovingTextEdit()));

  // Connecting slider moving to text
  connect(ui->slider_x_moving, &QScrollBar::valueChanged, (this),
          &MainWindow::xMovingSliderValueChanged);
  connect(ui->slider_y_moving, &QScrollBar::valueChanged, (this),
          &MainWindow::yMovingSliderValueChanged);
  connect(ui->slider_z_moving, &QScrollBar::valueChanged, (this),
          &MainWindow::zMovingSliderValueChanged);

  // Connecting mouse moving to slider
  connect(ui->glwidget, &GLWidget::xMovingChanged, this,
          &MainWindow::updateXMoving);
  connect(ui->glwidget, &GLWidget::yMovingChanged, this,
          &MainWindow::updateYMoving);
}

void MainWindow::rotationConnectSetup() {
  // Rotate X
  connect(ui->slider_x_rotation, &QScrollBar::valueChanged, ui->glwidget,
          &GLWidget::setXRotation);
  connect(ui->rotating_x_value, SIGNAL(editingFinished()), (this),
          SLOT(xRotationTextEdit()));
  // Rotate Y
  connect(ui->slider_y_rotation, &QScrollBar::valueChanged, ui->glwidget,
          &GLWidget::setYRotation);
  connect(ui->rotating_y_value, SIGNAL(editingFinished()), (this),
          SLOT(yRotationTextEdit()));
  // Rotate Z
  connect(ui->slider_z_rotation, &QScrollBar::valueChanged, ui->glwidget,
          &GLWidget::setZRotation);
  connect(ui->rotating_z_value, SIGNAL(editingFinished()), (this),
          SLOT(zRotationTextEdit()));

  // Connecting slider rotation to text
  connect(ui->slider_x_rotation, &QScrollBar::valueChanged, (this),
          &MainWindow::xRotationValueChanged);
  connect(ui->slider_y_rotation, &QScrollBar::valueChanged, (this),
          &MainWindow::yRotationValueChanged);
  connect(ui->slider_z_rotation, &QScrollBar::valueChanged, (this),
          &MainWindow::zRotationValueChanged);

  // Connecting mouse rotation to slider
  connect(ui->glwidget, &GLWidget::xRotationChanged, this,
          &MainWindow::updateXRotation);
  connect(ui->glwidget, &GLWidget::yRotationChanged, this,
          &MainWindow::updateYRotation);
}

void MainWindow::connectSetup() {
  // Scale
  connect(ui->scale_slider, &QScrollBar::valueChanged, ui->glwidget,
          &GLWidget::setScale);
  connect(ui->scale_text, SIGNAL(editingFinished()), (this),
          SLOT(scaleTextEdit()));
  // Connecting slider scale to text
  connect(ui->scale_slider, &QScrollBar::valueChanged, (this),
          &MainWindow::scaleSliderValueChanged);
  // Connecting wheel scale to slider
  connect(ui->glwidget, &GLWidget::scaleChanged, this,
          &MainWindow::updateScaleSlider);

  // Edge parameters
  connect(ui->slider_edge_thickness, &QScrollBar::valueChanged, (this),
          &MainWindow::linesSizeSliderChanged);

  // Verticle parameters
  connect(ui->slider_vertice_size, &QScrollBar::valueChanged, (this),
          &MainWindow::vertexSizeSliderChanged);

  movingConnectSetup();
  rotationConnectSetup();
}

void MainWindow::defaultSliders() {
  // Scale
  ui->scale_slider->setValue(defaultScale);
  ui->scale_text->setText(QString::number(defaultScale));

  // Moving
  ui->slider_x_moving->setValue(MOVING_CONST);
  ui->slider_y_moving->setValue(MOVING_CONST);
  ui->slider_z_moving->setValue(MOVING_CONST);
  ui->moving_x_value->setText(QString::number(0));
  ui->moving_y_value->setText(QString::number(0));
  ui->moving_z_value->setText(QString::number(0));

  // Rotation
  ui->slider_x_rotation->setValue(FULL_ROTATION_ANGLE);
  ui->slider_y_rotation->setValue(FULL_ROTATION_ANGLE);
  ui->slider_z_rotation->setValue(FULL_ROTATION_ANGLE);
  ui->rotating_x_value->setText(QString::number(0));
  ui->rotating_y_value->setText(QString::number(0));
  ui->rotating_z_value->setText(QString::number(0));
}

void MainWindow::defaultColors() {
  // Edges
  ui->glwidget->edgeColor = QColor(0, 0, 0);
  QColor edgeDefaultColor = ui->glwidget->edgeColor;
  QString edgeQSS =
      QString("background-color: %1;\nborder:1px solid;\n border-color: black;")
          .arg(edgeDefaultColor.name());
  ui->graphics_color_edges->setStyleSheet(edgeQSS);

  // Verticles
  ui->glwidget->verticleColor = QColor(255, 0, 0);
  QColor verticleDefaultColor = ui->glwidget->verticleColor;
  QString verticleQSS =
      QString("background-color: %1;\nborder:1px solid;\n border-color: black;")
          .arg(verticleDefaultColor.name());
  ui->graphics_verticies_color->setStyleSheet(verticleQSS);

  // Background
  ui->glwidget->backgroundColor = QColor(255, 255, 255);
  QColor backgroundDefaultColor = ui->glwidget->backgroundColor;
  QString backgroundQSS =
      QString("background-color: %1;\nborder:1px solid;\n border-color: black;")
          .arg(backgroundDefaultColor.name());
  ui->graphics_background_color->setStyleSheet(backgroundQSS);
}

void MainWindow::defaultSettings() {
  defaultSliders();
  defaultColors();
  ui->glwidget->initSettings();

  // possibly somewhat is also needed here

  // projection modes
  ui->comboBox_projection->activated(CENTRAL);
  int resetProjectionIndex = ui->glwidget->projectionMode;
  ui->comboBox_projection->setCurrentIndex(resetProjectionIndex);

  // edge modes
  ui->comboBox_typeLine->activated(SOLID);
  int resetedgeIndex = ui->glwidget->edgeMode;
  ui->comboBox_typeLine->setCurrentIndex(resetedgeIndex);
  // thickness
  ui->slider_edge_thickness->setValue(1);

  // verticle modes
  ui->comboBox_typeVertex->activated(CIRCLE);
  int resetDotIndex = ui->glwidget->verticleMode;
  ui->comboBox_typeVertex->setCurrentIndex(resetDotIndex);
  // size
  ui->slider_vertice_size->setValue(5);

  QString settingsPath =
      QCoreApplication::applicationDirPath() + "/" + settingsFile;
  if (QFile::exists(settingsPath)) {
    loadSettings();
  }
}

/* ---------------------  settings  --------------------- */

void MainWindow::saveSettings() {
  QString settingsPath =
      QCoreApplication::applicationDirPath() + "/" + settingsFile;
  QSettings settings(settingsPath, QSettings::IniFormat);

  if (QFile::exists(settingsFile)) {
    QFile::remove(settingsFile);
  }

  // Save combo box indexes
  settings.setValue("projectionIndex", ui->comboBox_projection->currentIndex());
  // qDebug() << ui->comboBox_projection->currentIndex();
  settings.setValue("edgeIndex", ui->comboBox_typeLine->currentIndex());
  settings.setValue("verticleIndex", ui->comboBox_typeVertex->currentIndex());

  // Save slider values
  settings.setValue("edgeThickness", ui->slider_edge_thickness->value());
  settings.setValue("vertexThickness", ui->slider_vertice_size->value());

  // Save colors
  settings.setValue(
      "backgroundColor",
      ui->graphics_background_color->palette().color(QPalette::Window));
  settings.setValue(
      "edgeColor", ui->graphics_color_edges->palette().color(QPalette::Window));
  settings.setValue(
      "vertexColor",
      ui->graphics_verticies_color->palette().color(QPalette::Window));
}

void MainWindow::loadSettings() {
  QString settingsPath =
      QCoreApplication::applicationDirPath() + "/" + settingsFile;
  QSettings settings(settingsPath, QSettings::IniFormat);

  /* ---------  load combo box values  --------- */
  int projectionIndex = settings.value("projectionIndex", 0).toInt();
  int edgeIndex = settings.value("edgeIndex", 0).toInt();
  int verticleIndex = settings.value("verticleIndex", 0).toInt();

  ui->comboBox_projection->setCurrentIndex(projectionIndex);
  ui->comboBox_typeLine->setCurrentIndex(edgeIndex);
  ui->comboBox_typeVertex->setCurrentIndex(verticleIndex);

  ui->glwidget->projectionMode = projectionIndex;
  ui->glwidget->edgeMode = edgeIndex;
  ui->glwidget->verticleMode = verticleIndex;

  /* ---------   load slider values   --------- */
  ui->slider_edge_thickness->setValue(
      settings.value("edgeThickness", 1).toInt());
  ui->slider_vertice_size->setValue(
      settings.value("vertexThickness", 1).toInt());

  /* ------------   load colors   ------------ */
  QColor bgColor = settings.value("backgroundColor").value<QColor>();
  QColor edgeColor = settings.value("edgeColor").value<QColor>();
  QColor verticleColor = settings.value("vertexColor").value<QColor>();

  ui->graphics_background_color->setPalette(QPalette(bgColor));
  ui->graphics_color_edges->setPalette(QPalette(edgeColor));
  ui->graphics_verticies_color->setPalette(QPalette(verticleColor));

  QString backgroundQSS =
      QString("background-color: %1;\nborder:1px solid;\n border-color: black;")
          .arg(bgColor.name());
  ui->graphics_background_color->setStyleSheet(backgroundQSS);

  QString edgeQSS =
      QString("background-color: %1;\nborder:1px solid;\n border-color: black;")
          .arg(edgeColor.name());
  ui->graphics_color_edges->setStyleSheet(edgeQSS);

  QString verticleQSS =
      QString("background-color: %1;\nborder:1px solid;\n border-color: black;")
          .arg(verticleColor.name());
  ui->graphics_verticies_color->setStyleSheet(verticleQSS);

  ui->glwidget->backgroundColor = bgColor;
  ui->glwidget->edgeColor = edgeColor;
  ui->glwidget->verticleColor = verticleColor;

  ui->glwidget->update();
}

/* ------------------ END OF SETUP ------------------ */

void MainWindow::on_open_button_clicked() {
  QString filePath = QFileDialog::getOpenFileName(
      this, ("Select Model"), "../models/", "3D Image Files (*.obj)");
  if (filePath == "") {
    ui->statusBar->showMessage("failed to open a file");
    return;
  }

  remove_data(&ui->glwidget->data);
  char* path = (filePath.toLocal8Bit()).data();
  file_parsing(&ui->glwidget->data, path);

  ui->glwidget->setScale(defaultScale);
  ui->statusBar->showMessage("path: " + filePath);
  ui->number_of_vertices->setText(
      QString::number(ui->glwidget->data.count_of_verticles));
  ui->number_of_facets->setText(
      QString::number(ui->glwidget->data.count_of_facets));

  update();
}

/* ------------------  menu  ------------------ */

void MainWindow::on_comboBox_projection_activated(
    int index) {  // projection mode changing
  if (index == CENTRAL) {
    ui->glwidget->projectionMode = CENTRAL;
    ui->glwidget->update();
  } else if (index == PARALLEL) {
    ui->glwidget->projectionMode = PARALLEL;
    ui->glwidget->update();
  }
}

void MainWindow::on_comboBox_typeLine_activated(
    int index) {  // edge mode changing
  if (index == SOLID) {
    ui->glwidget->edgeMode = SOLID;
    ui->glwidget->update();
  } else if (index == DASHED) {
    ui->glwidget->edgeMode = DASHED;
    ui->glwidget->update();
  }
}

void MainWindow::on_comboBox_typeVertex_activated(
    int index) {  // verticle mode changing
  if (index == NO_VERTEX) {
    ui->glwidget->verticleMode = NO_VERTEX;
    ui->glwidget->update();
  } else if (index == CIRCLE) {
    ui->glwidget->verticleMode = CIRCLE;
    ui->glwidget->update();
  } else if (index == SQUARE) {
    ui->glwidget->verticleMode = SQUARE;
    ui->glwidget->update();
  }
}

void MainWindow::selectAndSetColor(QColor& targetColor,
                                   QGraphicsView* targetLabel) {
  QColorDialog dialog;
  dialog.setOption(QColorDialog::ShowAlphaChannel);

  if (dialog.exec() == QDialog::Accepted) {
    QColor color = dialog.selectedColor();
    if (color.isValid()) {
      targetColor = color;
      QString qss =
          QString(
              "background-color: %1;\nborder:1px solid;\n border-color: black;")
              .arg(color.name());
      targetLabel->setStyleSheet(qss);
      ui->glwidget->update();
    }
  }
}
void MainWindow::on_color_edges_button_clicked() {
  selectAndSetColor(ui->glwidget->edgeColor, ui->graphics_color_edges);
}
void MainWindow::on_color_verticies_button_clicked() {
  selectAndSetColor(ui->glwidget->verticleColor, ui->graphics_verticies_color);
}
void MainWindow::on_background_color_button_clicked() {
  selectAndSetColor(ui->glwidget->backgroundColor,
                    ui->graphics_background_color);
}

void MainWindow::on_reset_values_button_clicked() {
  if (QFile::exists(settingsFile)) {
    QFile::remove(settingsFile);
  }
  defaultSettings();
  ui->glwidget->update();
}

/* ------------------ functions ------------------ */

// Scale
void MainWindow::scaleSliderValueChanged(int value) {
  ui->scale_text->setText(QString::number(value));
}
void MainWindow::scaleTextEdit() {
  int value = ui->scale_text->text().toInt();
  ui->scale_slider->setValue(value);
}
void MainWindow::updateScaleSlider(int newScale) {
  ui->scale_slider->setValue(newScale);
}

// Edge thickness
void MainWindow::linesSizeSliderChanged(int value) {
  ui->glwidget->edgeThickness = value;
  ui->glwidget->update();
}

// Vertex size
void MainWindow::vertexSizeSliderChanged(int value) {
  ui->glwidget->verticleSize = value;
  ui->glwidget->update();
}

// Moving
void MainWindow::xMovingSliderValueChanged(int value) {
  ui->moving_x_value->setText(QString::number(value - MOVING_CONST));
}
void MainWindow::yMovingSliderValueChanged(int value) {
  ui->moving_y_value->setText(QString::number(value - MOVING_CONST));
}
void MainWindow::zMovingSliderValueChanged(int value) {
  ui->moving_z_value->setText(QString::number(value - MOVING_CONST));
}
void MainWindow::movingTextEdit(QScrollBar* slider, QLineEdit* ui_value,
                                int axis) {
  int value = ui_value->text().toInt();
  value += MOVING_CONST;
  slider->setValue(value);

  if (axis == X) {
    ui->glwidget->setXMoving(value);
  } else if (axis == Y) {
    ui->glwidget->setYMoving(value);
  } else if (axis == Z) {
    ui->glwidget->setZMoving(value);
  }
}
void MainWindow::xMovingTextEdit() {
  movingTextEdit(ui->slider_x_moving, ui->moving_x_value, X);
}
void MainWindow::yMovingTextEdit() {
  movingTextEdit(ui->slider_y_moving, ui->moving_y_value, Y);
}
void MainWindow::zMovingTextEdit() {
  movingTextEdit(ui->slider_z_moving, ui->moving_z_value, Z);
}
void MainWindow::updateXMoving(int value) {
  ui->slider_x_moving->setValue(value);
}
void MainWindow::updateYMoving(int value) {
  ui->slider_y_moving->setValue(value);
}

// Rotation
void MainWindow::xRotationValueChanged(int value) {
  ui->rotating_x_value->setText(
      QString::number(-HALF_CIRCLE + value / rotation_factor));
}
void MainWindow::yRotationValueChanged(int value) {
  ui->rotating_y_value->setText(
      QString::number(-HALF_CIRCLE + value / rotation_factor));
}
void MainWindow::zRotationValueChanged(int value) {
  ui->rotating_z_value->setText(
      QString::number(-HALF_CIRCLE + value / rotation_factor));
}
void MainWindow::rotateTextEdit(QScrollBar* slider, QLineEdit* ui_value,
                                int axis) {
  int value = ui_value->text().toInt();
  value += HALF_CIRCLE;
  slider->setValue(value * rotation_factor);
  value = normalize_angle(value);

  if (axis == X) {
    ui->glwidget->setXRotation(value * rotation_factor);
  } else if (axis == Y) {
    ui->glwidget->setYRotation(value * rotation_factor);
  } else if (axis == Z) {
    ui->glwidget->setZRotation(value * rotation_factor);
  }
}
void MainWindow::xRotationTextEdit() {
  rotateTextEdit(ui->slider_x_rotation, ui->rotating_x_value, X);
}
void MainWindow::yRotationTextEdit() {
  rotateTextEdit(ui->slider_y_rotation, ui->rotating_y_value, Y);
}
void MainWindow::zRotationTextEdit() {
  rotateTextEdit(ui->slider_z_rotation, ui->rotating_z_value, Z);
}
void MainWindow::updateXRotation(int angle) {
  ui->slider_x_rotation->setValue(angle);
}
void MainWindow::updateYRotation(int angle) {
  ui->slider_y_rotation->setValue(angle);
}

/* ------------------  menu / 2 bonus ------------------ */

void MainWindow::on_bmp_button_clicked() {
  QString savePath = QFileDialog::getSaveFileName(
      this, tr("Save Image"), "", tr("BMP Image (*.bmp);;All Files (*)"));
  if (!savePath.isEmpty()) {
    QPixmap pixmap = ui->glwidget->grab();
    pixmap.save(savePath, "BMP");
  }
}

void MainWindow::on_jpeg_button_clicked() {
  QString savePath = QFileDialog::getSaveFileName(
      this, tr("Save Image"), "",
      tr("JPEG Image (*.jpeg *.jpg);;All Files (*)"));
  if (!savePath.isEmpty()) {
    QPixmap pixmap = ui->glwidget->grab();
    QImage img = pixmap.toImage();
    img.save(savePath, "JPEG", 100);
  }
}

void MainWindow::on_gif_button_clicked() {
  flag_record = 1;
  ui->rec_button->setStyleSheet("background-color:#FF0000");

  timer_for_gif.start(1000);  // 1 секунда
  create_screen();
}

void MainWindow::create_screen() {
  if (flag_record == 0) {
    return;
  }

  QPixmap pixmap = ui->glwidget->grab();  // скриншот
  QImage image = pixmap.toImage();

  mkr_image.push_back(image);  // Добавляем скриншот в вектор кадров

  elapsedTime += 1;

  if (elapsedTime >= 5.0) {
    save_gif();
    timer_for_gif.stop();
  }
}

void MainWindow::save_gif() {
  QString gifFilePath = QFileDialog::getSaveFileName(this, tr("Save GIF"), "",
                                                     tr("GIF Files (*.gif)"));

  if (!gifFilePath.isEmpty()) {
    QGifImage gif(QSize(640, 480));  // Создаем объект GIF анимации

    // Добавляем кадры из вектора mkr_image
    for (const QImage& image : mkr_image) {
      gif.addFrame(image);
    }

    gif.save(gifFilePath);

    mkr_image.clear();  // Очищаем вектор кадров
    // Сбрасываем:
    flag_record = 0;
    elapsedTime = 0;
    ui->rec_button->setStyleSheet("");  // reset
  }
}