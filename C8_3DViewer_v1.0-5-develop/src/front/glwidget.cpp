#include "glwidget.h"

GLWidget::GLWidget(QWidget *parent) : QOpenGLWidget(parent) { initSettings(); }

GLWidget::~GLWidget() { remove_data(&data); }

void GLWidget::initSettings() {
  projectionMode = 0;
  verticleSize = 5;
  verticleMode = 1;
  edgeThickness = 1;
  edgeMode = 0;
  backgroundColor = QColor(255, 255, 255);
  edgeColor = QColor(0, 0, 0);
  verticleColor = QColor(255, 0, 0);
}

void GLWidget::initializeGL() {
  glClearColor(255, 255, 255, 1.0);
  glClearDepth(1.0);
  glEnable(GL_DEPTH_TEST);
  setupPerspective();
}

void GLWidget::resizeGL(int w, int h) { glViewport(0, 0, w, h); }

void GLWidget::paintGL() {
  if (!data.array) {
    remove_data(&data);
    return;
  }

  glClearColor(backgroundColor.redF(), backgroundColor.greenF(),
               backgroundColor.blueF(), backgroundColor.alphaF());
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  setupPerspective();

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  glVertexPointer(3, GL_DOUBLE, 0, data.array);
  glEnableClientState(GL_VERTEX_ARRAY);
  pointDrawing();
  lineDrawing();
  glDisableClientState(GL_VERTEX_ARRAY);
}

void GLWidget::pointDrawing() {
  if (verticleMode != 0) {
    glColor3f(verticleColor.redF(), verticleColor.greenF(),
              verticleColor.blueF());
    glPointSize(verticleSize);

    if (verticleMode == 1) {
      glEnable(GL_POINT_SMOOTH);
    }  // maybe split here <

    glDrawArrays(GL_POINTS, 1, data.count_of_verticles);

    if (verticleMode == 1) {  // maybe split here >
      glDisable(GL_POINT_SMOOTH);
    }
  }
}

void GLWidget::lineDrawing() {
  configureLineRendering();

  glDrawElements(GL_LINES, data.size_polygons, GL_UNSIGNED_INT,
                 data.array_polygons);

  disableLineStipple();
}
void GLWidget::configureLineRendering() {
  glColor3f(edgeColor.redF(), edgeColor.greenF(), edgeColor.blueF());
  glLineWidth(edgeThickness);

  if (edgeMode == 1) {
    enableLineStipple();
  }
}
void GLWidget::enableLineStipple() {
  glEnable(GL_LINE_STIPPLE);
  glLineStipple(2, 0x00F0);
}
void GLWidget::disableLineStipple() {
  if (edgeMode == 1) {
    glDisable(GL_LINE_STIPPLE);
  }
}

void GLWidget::setupPerspective() {
  // -fW, fW, -fH, fH, zNear, zFar
  if (projectionMode == 1) {  // parallel
    glOrtho(-5, 8.3, -5, 5, -100, 100);
    glTranslated(2, 0, -10);
  } else if (projectionMode == 0) {  // central
    glFrustum(-1, 1, -1, 1, 1, 15);
    glTranslated(0, 0, -10);
  }
}

void GLWidget::setScale(int scale) {
  const float SCALE_FACTOR = 1000.0f;

  if (scale != currentScale) {
    currentScale = scale;
    scale_array(&data, fabs(currentScale / SCALE_FACTOR),
                fabs(previous_scale / SCALE_FACTOR));
    previous_scale = currentScale;
    update();
  }
  emit scaleChanged(scale);  // connecting with ui->scale_slider
}

void GLWidget::moveByAxis(int step, int &movingValue, int &previousValue,
                          int axis) {
  const int MOVING_CONST = 200;
  const int MOVING_CONST_SLIDER = MOVING_CONST * 2;
  const float movingFactor = -0.05;  // positive is inverted excluding y axis

  if (step > MOVING_CONST_SLIDER) {  // +MOVING_CONST
    step = MOVING_CONST_SLIDER;
  } else if (step < 0) {  // -MOVING_CONST
    step = 0;
  }

  if (step != movingValue) {
    movingValue = step;
    float translation = movingFactor * (MOVING_CONST - movingValue);
    float prevTranslation = movingFactor * (MOVING_CONST - previousValue);
    if (axis == Qt::XAxis) {
      move_x_axis(&data, translation, prevTranslation);
      previousValue = movingValue;
    } else if (axis == Qt::YAxis) {
      move_y_axis(&data, -translation, -prevTranslation);
      previousValue = movingValue;
    } else if (axis == Qt::ZAxis) {
      move_z_axis(&data, translation, prevTranslation);
      previousValue = movingValue;
    }
    update();
  }
}
void GLWidget::setXMoving(int step) {
  moveByAxis(step, moveX, previous_position_x, Qt::XAxis);
  emit xMovingChanged(step);  // connecting with ui->slider_x_moving
}
void GLWidget::setYMoving(int step) {
  moveByAxis(step, moveY, previous_position_y, Qt::YAxis);
  emit yMovingChanged(step);  // connecting with ui->slider_y_moving
}
void GLWidget::setZMoving(int step) {
  moveByAxis(step, moveZ, previous_position_z, Qt::ZAxis);
}

void qNormalizeAngle(int &current_angle) {
  int normalized_rotation_angle = 360 * 8 * 2;  // FULL_ROTATION_ANGLE_GL
  while (current_angle < 0) {
    current_angle += normalized_rotation_angle;
  }
  while (current_angle > normalized_rotation_angle) {
    current_angle -= normalized_rotation_angle;
  }
}

void GLWidget::rotateByAxis(int current_angle, int &rotationValue,
                            int &previousValue, int axis) {
  qNormalizeAngle(current_angle);
  if (current_angle != rotationValue) {
    double rotation = -(180.0 - current_angle / 16.0);
    if (axis == Qt::XAxis) {
      rotate_x_axis(&data, rotation, -(180.0 - previousValue / 16.0));
      previousValue = rotationValue = current_angle;
    } else if (axis == Qt::YAxis) {
      rotate_y_axis(&data, rotation, -(180.0 - previousValue / 16.0));
      previousValue = rotationValue = current_angle;
    } else if (axis == Qt::ZAxis) {
      rotate_z_axis(&data, rotation, -(180.0 - previousValue / 16.0));
      previousValue = rotationValue = current_angle;
    }
    update();
  }
}
void GLWidget::setXRotation(int current_angle) {
  rotateByAxis(current_angle, rotateX, previous_angle_x, Qt::XAxis);
  emit xRotationChanged(current_angle);
  // connecting with ui->slider_x_rotation
}
void GLWidget::setYRotation(int current_angle) {
  rotateByAxis(current_angle, rotateY, previous_angle_y, Qt::YAxis);
  emit yRotationChanged(current_angle);
  // connecting with ui->slider_y_rotation
}
void GLWidget::setZRotation(int current_angle) {
  rotateByAxis(current_angle, rotateZ, previous_angle_z, Qt::ZAxis);
}

void GLWidget::mousePressEvent(QMouseEvent *event) { lastPos = event->pos(); }
void GLWidget::mouseMoveEvent(QMouseEvent *event) {
  int dx = event->x() - lastPos.x();
  int dy = event->y() - lastPos.y();
  int movingFactor = 1;

  if (event->buttons() & Qt::LeftButton) {
    setXRotation(rotateX + rotation_factor * dy);
    setYRotation(rotateY + rotation_factor * dx);
  } else if (event->buttons() & Qt::RightButton) {
    setXMoving(moveX + movingFactor * dx);
    setYMoving(moveY + movingFactor * dy);
  }

  lastPos = event->pos();
}

void GLWidget::wheelEvent(QWheelEvent *event) {
  int delta = event->angleDelta().y();
  double scaleFactor = 1.0 + (delta >= 0 ? 0.1 : -0.1);
  int newScale = currentScale * scaleFactor;

  const int minScale = 1;
  const int maxScale = 1000;  // defaultScale * 5

  if (newScale >= minScale && newScale <= maxScale) {
    setScale(newScale);
  }
  event->accept();
}
