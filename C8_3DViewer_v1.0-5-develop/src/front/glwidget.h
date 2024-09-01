#ifndef GLWIDGET_H
#define GLWIDGET_H

#define GL_SILENCE_DEPRECATION

#include <QColor>
#include <QMouseEvent>
#include <QOpenGLWidget>
#include <QSettings>
// #include <QTimer>
#include <algorithm>
#include <cstring>

// #include "qgifimage.h"

using namespace std;
extern "C" {
#include "../back/backend.h"
}

class GLWidget : public QOpenGLWidget {
  Q_OBJECT

 signals:  // connecting with mainwindow ui sliders
  void scaleChanged(int newScale);
  void xRotationChanged(int current_angle);
  void yRotationChanged(int current_angle);
  void xMovingChanged(int value);
  void yMovingChanged(int value);

 public:
  GLWidget(QWidget *parent = nullptr);
  ~GLWidget();
  QColor backgroundColor;
  QColor edgeColor;
  QColor verticleColor;
  QPoint lastPos;

  int projectionMode;
  int verticleMode;
  int verticleSize;
  int edgeThickness;
  int edgeMode;

  data_t data = {0, 0, NULL, 0.0, {0, 0, 0}, NULL, NULL, 0, NULL};

  char model_file_name[S_SIZE] = {0};

  void initSettings();

  // QString nameGif;
  // void save_record();
  // QGifImage *gif;
  // QImage *frame;
  // QTimer *timer = NULL;
  int time_pr = 1;

 public slots:
  void setXMoving(int step);
  void setYMoving(int step);
  void setZMoving(int step);
  void moveByAxis(int step, int &movingValue, int &previousValue, int axis);

  void setXRotation(int current_angle);
  void setYRotation(int current_angle);
  void setZRotation(int current_angle);
  void rotateByAxis(int current_angle, int &rotationValue, int &previousValue,
                    int axis);

  void setScale(int scale);

  // void saveGIF_GL();

 private:
  int rotation_factor = 8;
  int FULL_ROTATION_ANGLE_GL = 360 * rotation_factor;

  int moveX = 0;
  int moveY = 0;
  int moveZ = 0;
  int rotateX = FULL_ROTATION_ANGLE_GL;
  int rotateY = FULL_ROTATION_ANGLE_GL;
  int rotateZ = FULL_ROTATION_ANGLE_GL;

  int previous_position_x = 0;
  int previous_position_y = 0;
  int previous_position_z = 0;
  int previous_angle_x = FULL_ROTATION_ANGLE_GL;
  int previous_angle_y = FULL_ROTATION_ANGLE_GL;
  int previous_angle_z = FULL_ROTATION_ANGLE_GL;

  int currentScale = 1;
  int previous_scale = 1;

  int timeIndex;
  // QGifImage *gifImage = NULL;

  void setupPerspective();
  void pointDrawing();
  void configureLineRendering();
  void enableLineStipple();
  void disableLineStipple();
  void lineDrawing();

 protected:
  void initializeGL() override;
  void resizeGL(int w, int h) override;
  void paintGL() override;

  void mousePressEvent(QMouseEvent *event) override;
  void mouseMoveEvent(QMouseEvent *event) override;
  void wheelEvent(QWheelEvent *event) override;

  // signals:
  // void stopRecord();
};

#endif
