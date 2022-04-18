/*! \file
 * Copyright Domingo Martín Perandres
 * email: dmartin@ugr.es
 * web: http://calipso.ugr.es/dmartin
 * 2003-2019
 * GPL 3
 */


#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <GL/glew.h>
//#include <GL/gl.h>
//#include <GL/glu.h>
#include <QOpenGLWidget>
#include <QImageReader>
#include <QKeyEvent>
#include <iostream>
#include <QTimer>
#include "object3d.h"
#include "vertex.h"
#include "colors.h"
#include "axis.h"
#include "tetrahedron.h"
#include "cube.h"
#include "plymodel.h"
#include "cylinder.h"
#include "sphere.h"
#include "cone.h"
#include "plysweep.h"
#include "wheel.h"
#include "wheels_axis.h"
#include "tooth.h"
#include "teeth.h"
#include "forearm.h"
#include "hydraulic.h"
#include "excavator.h"
#include "tablero.h"

#include <algorithm>
#include <random>

namespace _gl_widget_ne {

  const float X_MIN=-.1;
  const float X_MAX=.1;
  const float Y_MIN=-.1;
  const float Y_MAX=.1;
  const float FRONT_PLANE_PERSPECTIVE=(X_MAX-X_MIN)/2;
  const float BACK_PLANE_PERSPECTIVE=1000;
  const float DEFAULT_DISTANCE=2;
  const float ANGLE_STEP=1;

  typedef enum {MODE_DRAW_POINT,MODE_DRAW_LINE,MODE_DRAW_FILL,MODE_DRAW_CHESS} _mode_draw;
  typedef enum {OBJECT_TETRAHEDRON,OBJECT_CUBE,OBJECT_CONE, OBJECT_CYLINDER,
      OBJECT_SPHERE,OBJECT_WHEEL,OBJECT_WHEELS_AXIS,OBJECT_TOOTH,OBJECT_TEETH,
	  OBJECT_FOREARM, OBJECT_HYDRAULIC, OBJECT_EXCAVATOR,OBJECT_TABLERO,
	  OBJECT_PLY,OBJECT_PLYSWEEP,OBJECT_MULTIPLE} _object;
}

class _window;


/*****************************************************************************//**
 *
 *
 *
 *****************************************************************************/

class _gl_widget : public QOpenGLWidget
{
Q_OBJECT
public slots:
    void set_point(bool State);
    void set_line(bool State);
    void set_fill(bool State);
    void set_chess(bool State);
    void set_flat(bool State);
    void set_smooth(bool State);
    void set_texture(bool State);
    void set_texture_flat(bool State);
    void set_texture_smooth(bool State);
    void set_animate();
    void set_step(int vx);

	void change_mat();
	void change_light1();
	void change_light2();
	void move_light(float step,unsigned int i);
	void set_lightStep(int vx);

	void swap_camera();

    void setObject(int index);
    void setMaterial(int index);
    void update_ply(string name);
    void update_sweep(string name,unsigned int N, bool upper, bool lower);
    void idle_event();
    void set_first_animation(int vx);
    void set_second_animation(int vx);
    void set_third_animation(int vx);
    void set_fps(int vx);

	void move_screen();
    void updateSelected();


public:
  _gl_widget(_window *Window1);

  void clear_window();
  void change_projection();
  void change_observer();

  void draw_axis();
  void draw_objects();

  void draw_multiple(unsigned int type);
  void Error_msg(QString text);
  void update_boxes();

  bool get_fill();
  bool get_chess();
  bool get_flat();
  bool get_smooth();
  bool get_texture();
  bool get_texture_flat();
  bool get_texture_smooth();

  void camera_parameters(_vertex3f VRP, _vertex3f VPN, _vertex3f VUP);

protected:
  void resizeGL(int Width1, int Height1) Q_DECL_OVERRIDE;
  void paintGL() Q_DECL_OVERRIDE;
  void initializeGL() Q_DECL_OVERRIDE;
  void keyPressEvent(QKeyEvent *Keyevent) Q_DECL_OVERRIDE;
  void mousePressEvent(QMouseEvent *Event) Q_DECL_OVERRIDE;
  void mouseReleaseEvent(QMouseEvent *Event) Q_DECL_OVERRIDE;
  void wheelEvent(QWheelEvent *Event) Q_DECL_OVERRIDE;
  void pick();

  void set_all_false();
  void unSelect();


private:
  float alpha, beta, gamma;
  bool cameraVectors;
  _vertex3f Translate;
  _window *Window;

  _axis Axis;
  _tetrahedron Tetrahedron;
  _cube Cube;
  _plymodel Ply;
  _cone Cone;
  _cylinder Cylinder;
  _sphere Sphere;
  _plysweep Plysweep;
  _wheel Wheel;
  _wheels_axis WheelAxis = _wheels_axis(0.25,32,0.25,1.5);
  _tooth Tooth = _tooth(0.5);
  _teeth Teeth = _teeth(0.5,0.5);
  _forearm Forearm = _forearm(0.5,3,0.5);
  _hydraulic Hydraulic = _hydraulic(0.5,4,4,0.5);
  _excavator Excavator;
  _tablero Tablero;
  vector<_plymodel> PlySelect;
  unsigned int row_size;

  QTimer *timer = new QTimer(this);
  QTimer *mover = new QTimer(this);
  QTimer *selecter = new QTimer(this);


  _gl_widget_ne::_object Object;

  unsigned int n1;
  unsigned int n2;
  unsigned int Max_lights;
  unsigned int mat;
  unsigned int Max_mats;

  vector<float> LSTEP{5.0,5.0};
  float C2_step;
  float Animation_step;
  float Light_step;

  float initial_mouse_x;
  float initial_mouse_y;
  bool change_position;

  int Selection_position_x;
  float Selection_position_y;
  bool selected;
  int selected_index_i, selected_index_j;

  float offset;
  float diff;

  bool change_camera;
  float x_clipSize;
  float x_zoomRate;
  float y_clipSize;
  float y_zoomRate;

  bool Draw_point;
  bool Draw_line;
  bool Draw_fill;
  bool Draw_chess;
  bool Draw_light_flat;
  bool Draw_light_smooth;
  bool Draw_unlit_texture;
  bool Draw_flat_texture;
  bool Draw_smooth_texture;
  bool Animate;

  float Observer_angle_x;
  float Observer_angle_y;
  float Observer_distance;
  vector<float> STEP{1.0,1.0,1.0};
  vector<float> ANIM{1.0,1.0,1.0};
  float FPS = 50;
  };

#endif
