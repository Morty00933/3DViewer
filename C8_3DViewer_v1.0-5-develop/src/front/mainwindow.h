#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QColor>
#include <QColorDialog>
#include <QFileDialog>
#include <QGraphicsView>
#include <QLineEdit>
#include <QMainWindow>
#include <QOpenGLWidget>
#include <QScreen>
#include <QSettings>
#include <QTimer>

#include "QtGifImage/src/gifimage/qgifimage.h"
#include "glwidget.h"
// #include "qgifimage.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT

 public slots
     :  // connecting with GLWidget: wheelEvent, rotation and translation
  void updateScaleSlider(int newScale);
  void updateXRotation(int angle);
  void updateYRotation(int angle);
  void updateXMoving(int value);
  void updateYMoving(int value);

 public:
  MainWindow(QWidget* parent = nullptr);
  ~MainWindow();

  void saveSettings();
  void loadSettings();

 private slots:
  /* buttons */
  void on_open_button_clicked();
  /* menu */
  void on_color_edges_button_clicked();
  void on_color_verticies_button_clicked();
  void on_background_color_button_clicked();
  void selectAndSetColor(QColor& targetColor, QGraphicsView* targetLabel);

  void on_comboBox_projection_activated(int index);
  void on_comboBox_typeLine_activated(int index);
  void on_comboBox_typeVertex_activated(int index);

  void on_bmp_button_clicked();
  void on_jpeg_button_clicked();

  void on_gif_button_clicked();
  void create_screen();
  void save_gif();

  void on_reset_values_button_clicked();  // reset button

  /* functions */
  void scaleTextEdit();
  void scaleSliderValueChanged(int value);
  void linesSizeSliderChanged(int value);
  void vertexSizeSliderChanged(int value);

  void xMovingSliderValueChanged(int value);
  void yMovingSliderValueChanged(int value);
  void zMovingSliderValueChanged(int value);
  void xMovingTextEdit();
  void yMovingTextEdit();
  void zMovingTextEdit();
  void movingTextEdit(QScrollBar* slider, QLineEdit* ui_value, int axis);

  void xRotationValueChanged(int value);
  void yRotationValueChanged(int value);
  void zRotationValueChanged(int value);
  void xRotationTextEdit();
  void yRotationTextEdit();
  void zRotationTextEdit();
  void rotateTextEdit(QScrollBar* slider, QLineEdit* ui_value, int axis);

 private:
  Ui::MainWindow* ui;
  QString settingsFile = "settings.conf";
  /* --- SETUP --- */
  void movingConnectSetup();
  void rotationConnectSetup();
  void connectSetup();

  void sliderSetup();

  void defaultSliders();
  void defaultColors();
  void defaultSettings();

  /* --- CONST --- */
  int defaultScale = 200;
  int maxScale = defaultScale * 5;

  int MAX_THICKNESS = 20;

  int FULL_CIRCLE = 360;
  int HALF_CIRCLE = FULL_CIRCLE / 2;
  int FULL_ROTATION_ANGLE = FULL_CIRCLE * 8;
  int rotation_factor = 16;

  int MOVING_CONST = 100;
  int MOVING_CONST_SLIDER = MOVING_CONST * 2;
  // axises declared in the backend
  // modes
  int CENTRAL = 0;  // projection
  int PARALLEL = 1;

  int SOLID = 0;  // edges
  int DASHED = 1;

  int NO_VERTEX = 0;  // verticles
  int CIRCLE = 1;
  int SQUARE = 2;

  /* ---  GIF  --- */
  int flag_record = 0;  // запись (1 - включена, 0 - выключена)
  double elapsedTime = 0;
  QVector<QImage> mkr_image;  // The vector for storing frames
  QTimer timer_for_gif;       // Таймер
};

#endif  // MAINWINDOW_H
