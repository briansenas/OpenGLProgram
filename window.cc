/*! \file
 * Copyright Domingo Martín Perandres
 * email: dmartin@ugr.es
 * web: http://calipso.ugr.es/dmartin
 * 2003-2019
 * GPL 3
 */


#include <QApplication>
#include <QAction>
#include <QMenuBar>
#include <QFrame>
#include <QDesktopWidget>

#include "window.h"
#include "glwidget.h"


/*****************************************************************************//**
 *
 *
 *
 *****************************************************************************/
QString ENUMTEX[4] = {"Primer","Segundo","Tercer","Cuarto"};

_window::_window()
{

  QSizePolicy Q(QSizePolicy::Expanding,QSizePolicy::Expanding);

  QWidget *Central_widget = new QWidget(this);

  QFrame *Framed_widget= new QFrame(Central_widget);
  Framed_widget->setSizePolicy(Q);
  Framed_widget->setFrameStyle(QFrame::Panel);
  Framed_widget->setLineWidth(3);


  GL_widget = new _gl_widget(this);
  GL_widget->setSizePolicy(Q);

  QHBoxLayout *Horizontal_frame = new QHBoxLayout();
  Horizontal_frame->setContentsMargins(1,1,1,1);

  Horizontal_frame->addWidget(GL_widget);
  Framed_widget->setLayout(Horizontal_frame);

  QHBoxLayout *Horizontal_main = new QHBoxLayout(Central_widget);

  Horizontal_main->addWidget(Framed_widget);

  //*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  QWidget *W1 = new QWidget;
  QWidget *W2 = new QWidget;
  QWidget *W3 = new QWidget;
  QWidget *W4 = new QWidget;
  QHBoxLayout *LHPV1 = new QHBoxLayout;
  QVBoxLayout *LVP1 = new QVBoxLayout;
  QVBoxLayout *LVP2 = new QVBoxLayout;
  QVBoxLayout *LVP3 = new QVBoxLayout;
  QLabel *L0 = new QLabel("Points");
  CB0 = new QCheckBox;
  QLabel *L1 = new QLabel("Lines");
  CB1 = new QCheckBox;
  QLabel *L2 = new QLabel("Fill");
  CB2 = new QCheckBox;
  QLabel *L3 = new QLabel("Chess");
  CB3 = new QCheckBox;
  QLabel *L4 = new QLabel("Flat light");
  CB4 = new QCheckBox;
  QLabel *L5 = new QLabel("Gourand light");
  CB5 = new QCheckBox;
  QLabel *L6 = new QLabel("Texture Map");
  CB6 = new QCheckBox;
  QLabel *L7 = new QLabel("Flat Texture Map");
  CB7 = new QCheckBox;
  QLabel *LL1 = new QLabel("Smooth Texture Map");
  CB8 = new QCheckBox;
  QLabel *L8 = new QLabel("Choose Figure:");
  CBB1 = new QComboBox;
  CBB1->addItem("Tetraedro"); CBB1->addItem("Cubo");
  CBB1->addItem("Cono"); CBB1->addItem("Cilindro");
  CBB1->addItem("Esfera");
  CBB1->addItem("Rueda"); CBB1->addItem("Axis Rueda");
  CBB1->addItem("Diente");
  CBB1->addItem("Dientes Escavadora");
  CBB1->addItem("AnteBrazo Escavadora");
  CBB1->addItem("Hidraulica");
  CBB1->addItem("Excavator");
  CBB1->addItem("Tablero");
  CBB1->addItem("ObjetoPly");
  CBB1->addItem("Objeto Ply Rotado");
  CBB1->addItem("Multiple Objectos");

  QLabel *L9 = new QLabel("Choose Material:");
  CBB2 = new QComboBox;
  CBB2->addItem("WhitePlastic");
  CBB2->addItem("Emerald");
  CBB2->addItem("Pearl");
  CBB2->addItem("Ruby");
  CBB2->addItem("Bronze");

  LVP3->addWidget(L0); LVP3->addWidget(CB0);
  LVP3->addWidget(L1); LVP3->addWidget(CB1);
  LVP3->addWidget(L2); LVP3->addWidget(CB2);
  LVP3->addWidget(L3); LVP3->addWidget(CB3);

  LVP2->addWidget(L4); LVP2->addWidget(CB4);
  LVP2->addWidget(L5); LVP2->addWidget(CB5);
  LVP2->addWidget(L6); LVP2->addWidget(CB6);
  LVP2->addWidget(L7); LVP2->addWidget(CB7);
  LVP2->addWidget(LL1); LVP2->addWidget(CB8);

  LVP1->addWidget(L8); LVP1->addWidget(CBB1);
  LVP1->addWidget(L9); LVP1->addWidget(CBB2);

  QHBoxLayout *LHPV2 = new QHBoxLayout;
  QPushButton *PBL1 = new QPushButton("Cambiar Luz 1");
  QPushButton *PBCP	= new QPushButton("Cambiar Proyeccion");
  QPushButton *PBL2 = new QPushButton("Cambiar Luz 2");
  connect(PBL1,SIGNAL(clicked()), SLOT(change_light1()));
  connect(PBL2,SIGNAL(clicked()), SLOT(change_light2()));
  connect(PBCP,SIGNAL(clicked()), SLOT(change_proyection()));
  LHPV2->addWidget(PBL1);  LHPV2->addWidget(PBCP); LHPV2->addWidget(PBL2);
  QWidget *WHBL = new QWidget;
  WHBL->setLayout(LHPV2);
  LVP1->addWidget(WHBL);

  unsigned int DELAY = 1; //PushButton auto repeat delay in miliseconds.

  QHBoxLayout *LHPV3 = new QHBoxLayout;
  QPushButton *PBML1 = new QPushButton("Rotar+ Luz 1");
  PBML1->setAutoRepeat(true);
  PBML1->setAutoRepeatDelay(DELAY);
  QPushButton *PBML2 = new QPushButton("Rotar- Luz 1");
  PBML2->setAutoRepeat(true);
  PBML2->setAutoRepeatDelay(DELAY);
  QPushButton *PBML3 = new QPushButton("Rotar+ Luz 2");
  PBML3->setAutoRepeat(true);
  PBML3->setAutoRepeatDelay(DELAY);
  QPushButton *PBML4 = new QPushButton("Rotar- Luz 2");
  PBML4->setAutoRepeat(true);
  PBML4->setAutoRepeatDelay(DELAY);
  connect(PBML1,SIGNAL(clicked()),SLOT(move_pos_light1()));
  connect(PBML2,SIGNAL(clicked()),SLOT(move_neg_light1()));
  connect(PBML3,SIGNAL(clicked()),SLOT(move_pos_light2()));
  connect(PBML4,SIGNAL(clicked()),SLOT(move_neg_light2()));
  LHPV3->addWidget(PBML1); LHPV3->addWidget(PBML2);
  LHPV3->addWidget(PBML3); LHPV3->addWidget(PBML4);
  QWidget *WHBML = new QWidget;
  WHBML->setLayout(LHPV3);
  LVP1->addWidget(WHBML);

  QHBoxLayout *LHPV4 = new QHBoxLayout;
  QPushButton *PBMLS1 = new QPushButton("Paso+ Luz 1");
  PBMLS1->setAutoRepeat(true);
  PBMLS1->setAutoRepeatDelay(DELAY);
  QPushButton *PBMLS2 = new QPushButton("Paso- Luz 1");
  PBMLS2->setAutoRepeat(true);
  PBMLS2->setAutoRepeatDelay(DELAY);
  QPushButton *PBMLS3 = new QPushButton("Paso+ Luz 2");
  PBMLS3->setAutoRepeat(true);
  PBMLS3->setAutoRepeatDelay(DELAY);
  QPushButton *PBMLS4 = new QPushButton("Paso- Luz 2");
  PBMLS4->setAutoRepeat(true);
  PBMLS4->setAutoRepeatDelay(DELAY);
  connect(PBMLS1,SIGNAL(clicked()),SLOT(inc_step_light1()));
  connect(PBMLS2,SIGNAL(clicked()),SLOT(dec_step_light1()));
  connect(PBMLS3,SIGNAL(clicked()),SLOT(inc_step_light2()));
  connect(PBMLS4,SIGNAL(clicked()),SLOT(dec_step_light2()));
  LHPV4->addWidget(PBMLS1); LHPV4->addWidget(PBMLS2);
  LHPV4->addWidget(PBMLS3); LHPV4->addWidget(PBMLS4);
  QWidget *WHBMLS = new QWidget;
  WHBMLS->setLayout(LHPV4);
  LVP1->addWidget(WHBMLS);

  W2->setLayout(LVP2); W3->setLayout(LVP3);
  LHPV1->addWidget(W2); LHPV1->addWidget(W3);
  W4->setLayout(LHPV1); LVP1->addWidget(W4);
  QPushButton *PB1 = new QPushButton("Animación");
  connect(PB1,SIGNAL(clicked()), SLOT(set_animate()));
  LVP1->addWidget(PB1);

  for(unsigned int i=0;i<3;i++){
      QLabel *L5 = new QLabel(ENUMTEX[i] + " Grados de Libertad:");
      LVP1->addWidget(L5);
      QWidget *WHB1 = new QWidget;
      QHBoxLayout *LHP1 = new QHBoxLayout;
      QPushButton *PBI1 = new QPushButton("Rotar+");
	  PBI1->setAutoRepeat(true);
	  PBI1->setAutoRepeatDelay(DELAY);
      QPushButton *PBD1 = new QPushButton("Rotar-");
	  PBD1->setAutoRepeat(true);
	  PBD1->setAutoRepeatDelay(DELAY);
      QPushButton *PBI12 = new QPushButton("Escalado+");
	  PBI12->setAutoRepeat(true);
	  PBI12->setAutoRepeatDelay(DELAY);
      QPushButton *PBD12 = new QPushButton("Escalado-");
	  PBD12->setAutoRepeat(true);
	  PBD12->setAutoRepeatDelay(DELAY);
      QWidget *WHBR1 = new QWidget;
      QHBoxLayout *LHPR1 = new QHBoxLayout;
      QPushButton *PBRI1 = new QPushButton("Step+");
	  PBRI1->setAutoRepeat(true);
	  PBRI1->setAutoRepeatDelay(DELAY);
      QPushButton *PBRD1 = new QPushButton("Step-");
	  PBRD1->setAutoRepeat(true);
	  PBRD1->setAutoRepeatDelay(DELAY);
      LHP1->addWidget(PBI1); LHP1->addWidget(PBD1);
      LHP1->addWidget(PBI12); LHP1->addWidget(PBD12);
      LHPR1->addWidget(PBRI1); LHPR1->addWidget(PBRD1);
      WHB1->setLayout(LHP1); WHBR1->setLayout(LHPR1);
      if(i==0){
          connect(PBI1,SIGNAL(clicked()), SLOT(set_Fpos()));
          connect(PBD1,SIGNAL(clicked()), SLOT(set_Fneg()));
          connect(PBI12,SIGNAL(clicked()), SLOT(set_F2pos()));
          connect(PBD12,SIGNAL(clicked()), SLOT(set_F2neg()));
          connect(PBRI1,SIGNAL(clicked()), SLOT(set_firate()));
          connect(PBRD1,SIGNAL(clicked()), SLOT(set_fdrate()));
      }
      else if(i==1){
          connect(PBI1,SIGNAL(clicked()), SLOT(set_Spos()));
          connect(PBD1,SIGNAL(clicked()), SLOT(set_Sneg()));
          connect(PBI12,SIGNAL(clicked()), SLOT(set_S2pos()));
          connect(PBD12,SIGNAL(clicked()), SLOT(set_S2neg()));
          connect(PBRI1,SIGNAL(clicked()), SLOT(set_sirate()));
          connect(PBRD1,SIGNAL(clicked()), SLOT(set_sdrate()));
      }else{
          connect(PBI1,SIGNAL(clicked()), SLOT(set_Tpos()));
          connect(PBD1,SIGNAL(clicked()), SLOT(set_Tneg()));
          connect(PBI12,SIGNAL(clicked()), SLOT(set_T2pos()));
          connect(PBD12,SIGNAL(clicked()), SLOT(set_T2neg()));
          connect(PBRI1,SIGNAL(clicked()), SLOT(set_tirate()));
          connect(PBRD1,SIGNAL(clicked()), SLOT(set_tdrate()));
      }
      LVP1->addWidget(WHB1); LVP1->addWidget(WHBR1);
  }
  QPushButton *FPS = new QPushButton("FPS++");
  QPushButton *FPS2 = new QPushButton("FPS--");
  QHBoxLayout *LHP1 = new QHBoxLayout;
  QWidget *WHB1 = new QWidget;
  LHP1->addWidget(FPS); LHP1->addWidget(FPS2);
  WHB1->setLayout(LHP1);
  QLabel *LFP = new QLabel("Modificar valor QTimer:");
  LVP1->addWidget(LFP); LVP1->addWidget(WHB1);
  LVP1->addStretch();
  W1->setLayout(LVP1);
  W1->setMinimumWidth(200);

  connect(CB0,SIGNAL(stateChanged(int)),this,SLOT(set_point_slot(int)));
  connect(CB1,SIGNAL(stateChanged(int)),this,SLOT(set_line_slot(int)));
  connect(CB2,SIGNAL(stateChanged(int)),this,SLOT(set_fill_slot(int)));
  connect(CB3,SIGNAL(stateChanged(int)),this,SLOT(set_chess_slot(int)));
  connect(CB4,SIGNAL(stateChanged(int)),this,SLOT(set_flat_slot(int)));
  connect(CB5,SIGNAL(stateChanged(int)),this,SLOT(set_smooth_slot(int)));
  connect(CB6,SIGNAL(stateChanged(int)),this,SLOT(set_texture_slot(int)));
  connect(CB7,SIGNAL(stateChanged(int)),this,SLOT(set_flat_texture(int)));
  connect(CB8,SIGNAL(stateChanged(int)),this,SLOT(set_smooth_texture(int)));
  connect(CBB1,SIGNAL(activated(int)),this,SLOT(set_object_slot(int)));
  connect(CBB2,SIGNAL(activated(int)),this,SLOT(set_material_slot(int)));
  connect(FPS,SIGNAL(clicked()),this,SLOT(inc_fps()));
  connect(FPS2,SIGNAL(clicked()),this,SLOT(dec_fps()));

  //*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  //Adding my new widget
  Horizontal_main->addWidget(W1);

  Central_widget->setLayout(Horizontal_main);
  setCentralWidget(Central_widget);

  // actions for file menu
  QAction *Exit = new QAction(QIcon("./icons/exit.png"), tr("&Exit..."), this);
  Exit->setShortcut(tr("Ctrl+Q"));
  Exit->setToolTip(tr("Exit the application"));
  connect(Exit, SIGNAL(triggered()), this, SLOT(close()));
  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  QAction *LoadPly = new QAction(QIcon("./icons/load.png"),tr("&LoadPly..."),this);
  LoadPly->setShortcut(tr("Key_6"));
  LoadPly->setToolTip(tr("Load the ply model"));
  connect(LoadPly, SIGNAL(triggered()), this, SLOT(set_load_ply()));
  QAction *LoadSweep = new QAction(QIcon("./icons/load.png"),tr("&LoadSweep..."),this);
  LoadPly->setShortcut(tr("Key_7"));
  LoadPly->setToolTip(tr("Load the sweep model"));
  connect(LoadSweep, SIGNAL(triggered()), this, SLOT(set_load_sweep()));
  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  // menus
  QMenu *File_menu=menuBar()->addMenu(tr("&File"));
  File_menu->addAction(Exit);
  File_menu->setAttribute(Qt::WA_AlwaysShowToolTips);
  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  QMenu *Ply_menu=menuBar()->addMenu(tr("&Ply"));
  Ply_menu->addAction(LoadPly);
  Ply_menu->addAction(LoadSweep);
  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  setWindowTitle(tr("Práctica Final - Brian Sena Simons"));

  resize(800,800);
}

float _window::getGLHeight(){
	return GL_widget->height();
}

float _window::getGLWidth(){
	return GL_widget->width();
}
/*****************************************************************************//**
 * Interfaz de usuario
 *
 *
 *
 *****************************************************************************/

void _window::change_proyection(){
	GL_widget->swap_camera();
}

void _window::change_light1(){
	GL_widget->change_light1();
}

void _window::move_pos_light1(){
	GL_widget->move_light(1,1);
}
void _window::move_neg_light1(){
	GL_widget->move_light(-1,1);
}

void _window::inc_step_light1(){
	GL_widget->set_lightStep(1);
}

void _window::dec_step_light1(){
	GL_widget->set_lightStep(-1);
}

void _window::change_light2(){
	GL_widget->change_light2();
}

void _window::move_pos_light2(){
	GL_widget->move_light(1,2);
}

void _window::move_neg_light2(){
	GL_widget->move_light(-1,2);
}

void _window::inc_step_light2(){
	GL_widget->set_lightStep(2);
}

void _window::dec_step_light2(){
	GL_widget->set_lightStep(-2);
}

void _window::update_boxes(){
	this->set_fill_mode(GL_widget->get_fill());
	this->set_chess_mode(GL_widget->get_chess());
	this->set_light_flat(GL_widget->get_flat());
	this->set_light_smooth(GL_widget->get_smooth());
	this->set_unlit_texture(GL_widget->get_texture());
	this->set_texture_flat(GL_widget->get_texture_flat());
	this->set_texture_smooth(GL_widget->get_texture_smooth());
}

void _window::set_point_slot(int State){
    GL_widget->set_point(State);
    GL_widget->update();
}
void _window::set_line_slot(int State){
    GL_widget->set_line(State);
    GL_widget->update();
}
void _window::set_fill_slot(int State){
    GL_widget->set_fill(State);
    GL_widget->update();
}

void _window::set_chess_slot(int State){
    GL_widget->set_chess(State);
    GL_widget->update();
}

void _window::set_flat_slot(int State){
    GL_widget->set_flat(State);
    GL_widget->update();
}

void _window::set_smooth_slot(int State){
    GL_widget->set_smooth(State);
    GL_widget->update();
}

void _window::set_texture_slot(int State){
    GL_widget->set_texture(State);
    GL_widget->update();
}

void _window::set_flat_texture(int State){
    GL_widget->set_texture_flat(State);
    GL_widget->update();
}

void _window::set_smooth_texture(int State){
    GL_widget->set_texture_smooth(State);
    GL_widget->update();
}

void _window::set_object_slot(int object){
    GL_widget->setObject(object);
    GL_widget->update();
}

void _window::set_material_slot(int material){
    GL_widget->setMaterial(material);
    GL_widget->update();
}

void _window::set_load_sweep(){
    QString fileName = QFileDialog::getOpenFileName(this,"Open A File","./ply_sweep",tr("*.ply"));
	if(!fileName.isEmpty()){
		bool upper = false, lower = false;
		QMessageBox::StandardButton reply;
		reply = QMessageBox::question(this,"Tapa Superior","¿Quieres Tapa Superior?",
				QMessageBox::Yes | QMessageBox::No);
		if(reply==QMessageBox::Yes)
			upper = true;
		QMessageBox::StandardButton reply2;
		reply2 = QMessageBox::question(this,"Tapa Inferior","¿Quieres Tapa Inferior?",
				QMessageBox::Yes | QMessageBox::No);
		if(reply2==QMessageBox::Yes)
			lower = true;
		bool ok;
		QString text = QInputDialog::getText(this,tr("Rotaciones sobre eje - Y"),
				tr("Número de Rotaciones"),QLineEdit::Normal,
				"16",&ok);
		unsigned int N;
		if(ok && !text.isEmpty())
			N = text.toInt();
		else
			N = 16;
		GL_widget->update_sweep(fileName.toStdString(), N,upper,lower);
		GL_widget->setObject(_gl_widget_ne::OBJECT_PLYSWEEP);
		GL_widget->update();
	}
}

void _window::set_load_ply(){
    QString fileName = QFileDialog::getOpenFileName(this,"Open A File","./ply_models",tr("*.ply"));
	if(!fileName.isEmpty()){
    GL_widget->update_ply(fileName.toStdString());
    GL_widget->setObject(_gl_widget_ne::OBJECT_PLY);
    GL_widget->update();
	}
}

void _window::set_point_mode(bool State){
    CB0->blockSignals(true);

    if(State)
        CB0->setCheckState(Qt::Checked);
    else
        CB0->setCheckState(Qt::Unchecked);

   CB0->blockSignals(false);

}
void _window::set_line_mode(bool State){
    CB1->blockSignals(true);

    if(State)
        CB1->setCheckState(Qt::Checked);
    else
        CB1->setCheckState(Qt::Unchecked);

   CB1->blockSignals(false);
}
void _window::set_fill_mode(bool State){
    CB2->blockSignals(true);

    if(State)
        CB2->setCheckState(Qt::Checked);
    else
        CB2->setCheckState(Qt::Unchecked);

   CB2->blockSignals(false);
}
void _window::set_chess_mode(bool State){
    CB3->blockSignals(true);

    if(State)
        CB3->setCheckState(Qt::Checked);
    else
        CB3->setCheckState(Qt::Unchecked);

   CB3->blockSignals(false);
}
void _window::set_light_flat(bool State){
    CB4->blockSignals(true);

    if(State)
        CB4->setCheckState(Qt::Checked);
    else
        CB4->setCheckState(Qt::Unchecked);

   CB4->blockSignals(false);
}
void _window::set_light_smooth(bool State){
    CB5->blockSignals(true);

    if(State)
        CB5->setCheckState(Qt::Checked);
    else
        CB5->setCheckState(Qt::Unchecked);

   CB5->blockSignals(false);
}
void _window::set_unlit_texture(bool State){
    CB6->blockSignals(true);

    if(State)
        CB6->setCheckState(Qt::Checked);
    else
        CB6->setCheckState(Qt::Unchecked);

   CB6->blockSignals(false);
}
void _window::set_texture_flat(bool State){
    CB7->blockSignals(true);

    if(State)
        CB7->setCheckState(Qt::Checked);
    else
        CB7->setCheckState(Qt::Unchecked);

   CB7->blockSignals(false);
}
void _window::set_texture_smooth(bool State){
    CB8->blockSignals(true);

    if(State)
        CB8->setCheckState(Qt::Checked);
    else
        CB8->setCheckState(Qt::Unchecked);

   CB8->blockSignals(false);
}


void _window::update_CBB1(int index){
    CBB1->setCurrentIndex(index);
}

void _window::update_CBB2(int index){
	CBB2->setCurrentIndex(index);
}

void _window::set_Fpos(){
    GL_widget->set_first_animation(1);
}
void _window::set_Fneg(){
    GL_widget->set_first_animation(-1);
}
void _window::set_F2pos(){
    GL_widget->set_first_animation(2);
}
void _window::set_F2neg(){
    GL_widget->set_first_animation(-2);
}

void _window::set_firate(){
    GL_widget->set_step(1);
}

void _window::set_fdrate(){
    GL_widget->set_step(-1);
}

void _window::set_Spos(){
    GL_widget->set_second_animation(1);
}
void _window::set_Sneg(){
    GL_widget->set_second_animation(-1);
}
void _window::set_S2pos(){
    GL_widget->set_second_animation(2);
}
void _window::set_S2neg(){
    GL_widget->set_second_animation(-2);
}
void _window::set_sirate(){
    GL_widget->set_step(2);
}
void _window::set_sdrate(){
    GL_widget->set_step(-2);
}

void _window::set_Tpos(){
    GL_widget->set_third_animation(1);
}
void _window::set_Tneg(){
    GL_widget->set_third_animation(-1);
}
void _window::set_T2pos(){
    GL_widget->set_third_animation(2);
}
void _window::set_T2neg(){
    GL_widget->set_third_animation(-2);
}
void _window::set_tirate(){
    GL_widget->set_step(3);
}

void _window::set_tdrate(){
    GL_widget->set_step(-3);
}

void _window::set_animate(){
    GL_widget->set_animate();
}

void _window::inc_fps(){
    GL_widget->set_fps(-1);
}

void _window::dec_fps(){
    GL_widget->set_fps(1);
}

