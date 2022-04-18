/*! \file
 * Copyright Domingo Martín Perandres
 * email: dmartin@ugr.es
 * web: http://calipso.ugr.es/dmartin
 * 2003-2019
 * GPL 3
 */


#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStandardPaths>
#include <QFileDialog>
#include <QDialog>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QGuiApplication>
#include <QLabel>
#include <QCheckBox>
#include <QComboBox>
#include <QPushButton>
#include <QPoint>
#include <QInputDialog>


class _gl_widget;


/*****************************************************************************//**
 *
 *
 *
 *****************************************************************************/

class _window : public QMainWindow
{
    Q_OBJECT

public:
    _window();

    void set_point_mode(bool State);
    void set_line_mode(bool State);
    void set_fill_mode(bool State);
    void set_chess_mode(bool State);
    void set_light_flat(bool State);
    void set_light_smooth(bool State);
    void set_unlit_texture(bool State);
    void set_texture_flat(bool State);
	void set_texture_smooth(bool State);
    void update_CBB1(int index);
    void update_CBB2(int index);

	void update_boxes();

	float getGLWidth();
	float getGLHeight();

public slots:
	void change_light1();
	void change_light2();
    void set_point_slot(int State);
    void set_line_slot(int State);
    void set_fill_slot(int State);
    void set_chess_slot(int State);
    void set_flat_slot(int State);
    void set_smooth_slot(int State);
    void set_texture_slot(int State);
    void set_flat_texture(int State);
    void set_smooth_texture(int State);
    void set_object_slot(int object);
    void set_material_slot(int material);
    void set_load_sweep();
    void set_load_ply();
    void set_animate();
    void set_firate(); void set_fdrate();
    void set_Fpos(); void set_Fneg();
    void set_F2pos(); void set_F2neg();
    void set_sirate(); void set_sdrate();
    void set_Spos(); void set_Sneg();
    void set_S2pos(); void set_S2neg();
    void set_tirate(); void set_tdrate();
    void set_Tpos(); void set_Tneg();
    void set_T2pos(); void set_T2neg();
    void inc_fps(); void dec_fps();

	void change_proyection();

	void move_pos_light1(); void move_neg_light1();
	void move_pos_light2(); void move_neg_light2();
	void inc_step_light1(); void dec_step_light1();
	void inc_step_light2(); void dec_step_light2();

private:
  _gl_widget *GL_widget;

  QCheckBox *CB0=nullptr;
  QCheckBox *CB1=nullptr;
  QCheckBox *CB2=nullptr;
  QCheckBox *CB3=nullptr;
  QCheckBox *CB4=nullptr;
  QCheckBox *CB5=nullptr;
  QCheckBox *CB6=nullptr;
  QCheckBox *CB7=nullptr;
  QCheckBox *CB8=nullptr;
  QComboBox *CBB1=nullptr;
  QComboBox *CBB2=nullptr;
};

#endif
