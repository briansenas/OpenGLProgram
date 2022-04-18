/*! \file
 * Copyright Domingo Martín Perandres
 * email: dmartin@ugr.es
 * web: http://calipso.ugr.es/dmartin
 * 2003-2019 GPL 3
 */


#include "glwidget.h"
#include "window.h"
#include <math.h>
#define PLYDIST	15

using namespace std;
using namespace _gl_widget_ne;
using namespace _colors_ne;

_vertex3f test;



/*****************************************************************************//**
 *
 *
 *
 *****************************************************************************/

_gl_widget::_gl_widget(_window *Window1):Window(Window1)
{
  setMinimumSize(300, 300);
  setFocusPolicy(Qt::StrongFocus);
}

void _gl_widget::mousePressEvent(QMouseEvent *Event){
	if(Event->buttons() & Qt::LeftButton){
		change_position = true;
		initial_mouse_x = Event->x();
		initial_mouse_y = Event->y();
	}
	else if(Event->buttons() & Qt::RightButton){
		Selection_position_x = Event->x();
		Selection_position_y = height() - Event->y();
	}
	else if(Event->buttons() & Qt::MiddleButton){
		Error_msg("[ERROR] I have not implemented the middle-button click yet");
	}
}
void _gl_widget::mouseReleaseEvent(QMouseEvent *Event){
	change_position = false;
	if(Event->button() & Qt::RightButton){
		this->pick();
	}
	update();
}

void inBounds(float *val, float bound){
	if(*val>=bound)
		*val -= bound;
	if(*val<=-bound)
		*val += bound;
}


void _gl_widget::move_screen(){
	if(change_position){
		float x = QWidget::mapFromGlobal(QCursor::pos()).x();
		float y = QWidget::mapFromGlobal(QCursor::pos()).y();
		float x_val = initial_mouse_x - x;
		float y_val = initial_mouse_y - y;
		initial_mouse_x=x;
		initial_mouse_y=y;
		if(Observer_angle_x+120>0 || abs(Observer_angle_x)>270)
			Observer_angle_y -= x_val/20;
		else
			Observer_angle_y += x_val/20;

		Observer_angle_x -= y_val/20;
		inBounds(&Observer_angle_x,360);
		inBounds(&Observer_angle_y,360);
		update();
	}
}

void _gl_widget::wheelEvent(QWheelEvent *Event){
	if(Event->angleDelta().y() > 0){
		Observer_distance*=1.2;
		x_clipSize *= x_zoomRate;
		y_clipSize *= y_zoomRate;
	}
	else{
		Observer_distance/=1.2;
		if(x_clipSize>0)
			x_clipSize /= x_zoomRate;
		if(y_clipSize>0)
			y_clipSize /= y_zoomRate;
	}

	update();
}

//Remember that the vectors are normalized (alledgedly)
void _gl_widget::camera_parameters(_vertex3f VRP, _vertex3f VPN, _vertex3f VUP){

	//VPN = VRP - LookAt //Opposite direction we looking at, also called n vector
	_vertex3f cameraRight = _operate::normalize( _operate::cross_product(VUP,VPN)); // u vector
	//The u vector is the X axis of the view C.S.
	_vertex3f cameraUp = _operate::normalize(_operate::cross_product(VPN,cameraRight)); //v vector
	//The v vector is the Y axis of the view C.S.

	// radianes
	this->alpha=  atanf(VPN._1/VPN._2);
	_vertex3f VPN2 = _vertex3f(VPN._0,VPN._1*cos(alpha)-VPN._2*sin(alpha),-VPN._1 * sin(alpha)+VPN._2*cos(alpha));
	_vertex3f cameraRight1 = _vertex3f(cameraRight._0,cameraRight._1*cos(alpha)-cameraRight._2*sin(alpha),-cameraRight._1 * sin(alpha)+cameraRight._2*cos(alpha));

	this->beta =  atanf(VPN2._0/VPN2._2);
	_vertex3f cameraRight2 = _vertex3f(cameraRight1._0*cos(beta)-cameraRight1._2*sin(beta),cameraRight1._1,-cameraRight1._0* sin(beta)+cameraRight1._2*cos(beta));

	this->gamma =  atanf(cameraRight2._1/cameraRight2._0);
	this->Translate = VRP;
}

void _gl_widget::pick()
{
  makeCurrent();

  // Frame Buffer Object to do the off-screen rendering
  GLuint FBO;
  glGenFramebuffers(1,&FBO);
  glBindFramebuffer(GL_FRAMEBUFFER,FBO);

  // Texture for drawing
  GLuint Color_texture;
  glGenTextures(1,&Color_texture);
  glBindTexture(GL_TEXTURE_2D,Color_texture);
  // RGBA8
  glTexStorage2D(GL_TEXTURE_2D,1,GL_RGBA8, width(),height());
  // this implies that there is not mip mapping
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);

  // Texure for computing the depth
  GLuint Depth_texture;
  glGenTextures(1,&Depth_texture);
  glBindTexture(GL_TEXTURE_2D,Depth_texture);
  // Float
  glTexStorage2D(GL_TEXTURE_2D,1,GL_DEPTH_COMPONENT24, width(),height());

  // Attatchment of the textures to the FBO
  glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,Color_texture,0);
  glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,Depth_texture,0);

  // OpenGL will draw to these buffers (only one in this case)
  static const GLenum Draw_buffers[]={GL_COLOR_ATTACHMENT0};
  glDrawBuffers(1,Draw_buffers);

  //Dibujamos la escena.
  glClearColor(1,1,1,1);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

  bool MultipleObject = false;
  bool MultipleIndex = false;
  vector<_vertex3f> Vertices;
  vector<_vertex3ui> Triangles;
  int i=-1,j=0;
  switch (Object){
	  case OBJECT_TETRAHEDRON:
		  Tetrahedron.draw_fill(&i);
		  break;
	  case OBJECT_CUBE:
		  Cube.draw_fill(&i);
		  break;
	  case OBJECT_PLY:
		  Ply.draw_fill(&i);
		  break;
	  case OBJECT_CONE:
		  Cone.draw_fill(&i);
		  break;
	  case OBJECT_CYLINDER:
		  Cylinder.draw_fill(&i);
		  break;
	  case OBJECT_SPHERE:
		  Sphere.draw_fill(&i);
		  break;
	  case OBJECT_WHEEL:
		  Wheel.draw_fill(&i);
		  break;
	  case OBJECT_WHEELS_AXIS:
		  WheelAxis.draw_fill(&j);
		  MultipleIndex = true;
		  break;
	  case OBJECT_TOOTH:
		  Tooth.draw_fill(&j);
		  MultipleIndex = true;
		  break;
	  case OBJECT_TEETH:
		  Teeth.draw_fill(&j);
		  MultipleIndex = true;
		  break;
	  case OBJECT_FOREARM:
		  Forearm.draw_fill(&j);
		  MultipleIndex = true;
		  break;
	  case OBJECT_HYDRAULIC:
		  Hydraulic.draw_fill(&j);
		  MultipleIndex = true;
		  break;
	  case OBJECT_EXCAVATOR:
		  Excavator.draw_fill(&j);
		  MultipleIndex = true;
		  break;
	  case OBJECT_PLYSWEEP:
		  Plysweep.draw_fill(&j);
		  break;
	  case OBJECT_TABLERO:
		  Tablero.draw_fill(&j);
		  break;
	  case OBJECT_MULTIPLE:
		  MultipleObject = true;
		  MultipleIndex = true;
		  break;
	  default:break;
  }
  i = -1, j = -1;
  if(MultipleObject){
	  glPolygonMode(GL_FRONT, GL_FILL);
	  _vertex3f color;
	  unsigned int cont = 0;
	  glMatrixMode(GL_MODELVIEW);
	  glPushMatrix();
	  for(unsigned int i=0;i<PlySelect.size()/row_size;i++){
		  for(unsigned int x=0;x<row_size;x++){
			  glPushMatrix();
			  glTranslatef(-offset+diff*x,0,+offset-diff*i);
			  glBegin(GL_TRIANGLES);
			  Vertices = PlySelect[cont].Vertices;
			  Triangles = PlySelect[cont].Triangles;
			  for(unsigned int j=0;j<Triangles.size();j++){
				  color._0 = cont;
				  color._1 = (j&0x0000FF00)>>8;
				  color._2 = (j&0x000000FF);
				  color /= 255.0;
				  //cout << color._0 << " " << color._1 << " " << color._2 << endl;
				  glColor3fv((const GLfloat*) &color);
				  glVertex3fv((const GLfloat*) &Vertices[Triangles[j]._0]);
				  glVertex3fv((const GLfloat*) &Vertices[Triangles[j]._1]);
				  glVertex3fv((const GLfloat*) &Vertices[Triangles[j]._2]);
			  }
			  glEnd();
			  glPopMatrix();
		   cont++;
		  }
	  }
	  glPopMatrix();
  }

  // get the pixel
  //cout << Selection_position_x << " - " << Selection_position_y << endl;
  int Color;
  glReadBuffer(GL_FRONT);
  glPixelStorei(GL_PACK_ALIGNMENT,1);
  glReadPixels(Selection_position_x,Selection_position_y,1,1,GL_RGBA,GL_UNSIGNED_BYTE,&Color);

  if(!MultipleObject and !MultipleIndex){
	  // convertir de RGB a identificador
	  uint B = uint((Color & 0x00FF0000) >> 16);
	  uint G = uint((Color & 0x0000FF00) >> 8);
	  uint R = uint((Color & 0x000000FF));
	  i = (R<<16)+(G<<8)+B;
	  //cout << "Color: " << Color << "-"  << R << "-" << B << "-" << G << " Produces: " << i << endl;
  }else{
	  //cout << Color << endl;
	  // convertir de RGB a identificador
	  uint B = uint((Color & 0x00FF0000) >> 16);
	  uint G = uint((Color & 0x0000FF00) >> 8);
	  uint R = uint((Color & 0x000000FF));
	  i = (0<<16)+(0<<8)+R;
	  j = (0<<16)+(G<<8)+B;
	  //cout << "Color: " << Color << "-" << R << "-" << B
		  //<< "-" << G << " Produces i: " << i << " with j:" << j << endl;
  }

  if(i>=16777215 or i<0 or (i==255 and MultipleIndex)) {
	  selected = false;
	  i = -1;
  }else{
	  selected = true;
  }

  selected_index_i = i;
  selected_index_j = j;
  unSelect();
  if(selected)
	updateSelected();

  glDeleteTextures(1,&Color_texture);
  glDeleteTextures(1,&Depth_texture);
  glDeleteFramebuffers(1,&FBO);
  // the normal framebuffer takes the control of drawing
  glBindFramebuffer(GL_DRAW_FRAMEBUFFER,defaultFramebufferObject());
}

void _gl_widget::updateSelected(){
	if(selected){
		int i = selected_index_i;
		int j = selected_index_j;
		// actualizar el identificador de la parte seleccionada en el objeto
		switch(Object){
			case OBJECT_TETRAHEDRON:
				Tetrahedron.setSelected(i);
				test = Tetrahedron.getSelected(i);
				break;
			case OBJECT_CUBE:
				Cube.setSelected(i);
				test = Cube.getSelected(i);
				break;
			case OBJECT_PLY:
				Ply.setSelected(i);
				test = Ply.getSelected(i);
				break;
			case OBJECT_CONE:
				Cone.setSelected(i);
				test = Cone.getSelected(i);
				break;
			case OBJECT_CYLINDER:
				Cylinder.setSelected(i);
				test = Cylinder.getSelected(i);
				break;
			case OBJECT_SPHERE:
				Sphere.setSelected(i);
				test = Sphere.getSelected(i);
				break;
			case OBJECT_PLYSWEEP:
				Plysweep.setSelected(i);
				test = Plysweep.getSelected(i);
				break;
			case OBJECT_TABLERO:
				Tablero.setSelected(i);
				test = Tablero.getSelected(i);
				break;
			case OBJECT_WHEEL:
				Wheel.setSelected(i);
				test = Wheel.getSelected(i);
				break;
			case OBJECT_WHEELS_AXIS:
				WheelAxis.setSelected(i,j);
				test = WheelAxis.getSelected(i,j);
				break;
			case OBJECT_TOOTH:
				Tooth.setSelected(i,j);
				test = Tooth.getSelected(i,j);
				break;
			case OBJECT_TEETH:
				Teeth.setSelected(i,j);
				test = Teeth.getSelected(i,j);
				break;
			case OBJECT_FOREARM:
				Forearm.setSelected(i,j);
				test = Forearm.getSelected(i,j);
				break;
			case OBJECT_HYDRAULIC:
				Hydraulic.setSelected(i,j);
				test = Hydraulic.getSelected(i,j);
				break;
			case OBJECT_EXCAVATOR:
				Excavator.setSelected(i,j);
				test = Excavator.getSelected(i,j);
				break;
			case OBJECT_MULTIPLE:
				{
					if(i>=0 and i<PlySelect.size()){
						if(j>=0 and j<PlySelect[i].Triangles.size()){
							PlySelect[i].setSelected(j);
							test  = PlySelect[i].Vertices[PlySelect[i].Triangles[j]._0];
							test += PlySelect[i].Vertices[PlySelect[i].Triangles[j]._1];
							test += PlySelect[i].Vertices[PlySelect[i].Triangles[j]._2];
							test /= 3.0;
							test._0 += (-offset + diff * (i % (PlySelect.size()/row_size)));
							test._2 += (offset - diff * (i/row_size));
						}else
							test =  _vertex3f(0,0,0);
					}else{
						test =  _vertex3f(0,0,0);
					}
					int cont = 0;
					while(cont < PlySelect.size()){
						if(cont!=i)
							PlySelect[cont].setSelected(-1);
						cont++;
					}
					break;
				}
			default:break;
		}
	}
}

void _gl_widget::unSelect(){
	Tetrahedron.setSelected(-1);
	Cube.setSelected(-1);
	Ply.setSelected(-1);
	Cone.setSelected(-1);
	Cylinder.setSelected(-1);
	Sphere.setSelected(-1);
	Plysweep.setSelected(-1);
	Tablero.setSelected(-1);
	Wheel.setSelected(-1);
	WheelAxis.setSelected(-1,-1);
	Tooth.setSelected(-1,-1);
	Teeth.setSelected(-1,-1);
	Forearm.setSelected(-1,-1);
	Hydraulic.setSelected(-1,-1);
	Excavator.setSelected(-1,-1);
}

/*****************************************************************************//**
 * Evento tecla pulsada
 *
 *
 *
 *****************************************************************************/

void _gl_widget::keyPressEvent(QKeyEvent *Keyevent)
{
  switch(Keyevent->key()){
  case Qt::Key_1:
      Object=OBJECT_TETRAHEDRON;
      Window->update_CBB1(0);
	  selected = false;
	unSelect();
      break;
  case Qt::Key_2:
      Object=OBJECT_CUBE;
      Window->update_CBB1(1);
	  selected = false;
	unSelect();
      break;
  case Qt::Key_3:
      Object=OBJECT_CONE;
      Window->update_CBB1(2);
	  selected = false;
	unSelect();
      break;
  case Qt::Key_4:
      Object=OBJECT_CYLINDER; Window->update_CBB1(3);
	  selected = false;
	unSelect();
      break;
  case Qt::Key_5:
      Object=OBJECT_SPHERE;
      Window->update_CBB1(4);
	  selected = false;
	unSelect();
      break;
  case Qt::Key_6:
      if(Keyevent->modifiers() && Qt::ShiftModifier){
		  Object=OBJECT_MULTIPLE;
		  Window->update_CBB1(15);
	  }
	  else{
		  Object=OBJECT_PLY;
		  Window->update_CBB1(13);
	  }
	  selected = false;
	unSelect();
	  break;
  case Qt::Key_7:
	  Object=OBJECT_PLYSWEEP;
	  Window->update_CBB1(14);
	  selected = false;
	unSelect();
	  break;
  case Qt::Key_8:
        Object=OBJECT_TABLERO;
        Window->update_CBB1(12);
	  selected = false;
	unSelect();
        break;
  case Qt::Key_9:
        Object=OBJECT_WHEELS_AXIS;
        Window->update_CBB1(6);
	  selected = false;
	unSelect();
        break;
  case Qt::Key_0:
        if(Keyevent->modifiers() && Qt::ShiftModifier){
			Object=OBJECT_TOOTH;
			Window->update_CBB1(7);
		}
		else{
			Object=OBJECT_TEETH;
			Window->update_CBB1(8);
		}
	  selected = false;
	unSelect();
        break;
  case Qt::Key_G:
		if(Keyevent->modifiers() && Qt::ShiftModifier){
			Object=OBJECT_FOREARM;
			Window->update_CBB1(9);
		}
		else{
			Object=OBJECT_HYDRAULIC;
			Window->update_CBB1(10);
		}
	  selected = false;
	unSelect();
        break;
  case Qt::Key_H:
        Object=OBJECT_EXCAVATOR;
        Window->update_CBB1(11);
	  selected = false;
	unSelect();
        break;
  case Qt::Key_B:
        Object=OBJECT_WHEEL;
        Window->update_CBB1(5);
	  selected = false;
	unSelect();
        break;

  case Qt::Key_P:
      Draw_point=!Draw_point;
      Window->set_point_mode(Draw_point);
      break;
  case Qt::Key_L:
      Draw_line=!Draw_line;
      Window->set_line_mode(Draw_line);
      break;
  case Qt::Key_F:
	  this->set_fill(!Draw_fill);
    break;

  case Qt::Key_C:
	  this->set_chess(!Draw_chess);
    break;
  case Qt::Key_V:
	change_camera = !change_camera;
	update();
    break;
  case Qt::Key_F3:
	  this->set_flat(!Draw_light_flat);
      break;
  case Qt::Key_F4:
	  this->set_smooth(!Draw_light_smooth);
      break;

  case Qt::Key_F5:
	  this->set_texture(!Draw_unlit_texture);
      break;

  case Qt::Key_F6:
	  this->set_texture_flat(!Draw_flat_texture);
      break;
  case Qt::Key_F7:
	  this->set_texture_smooth(!Draw_smooth_texture);
      break;

  case Qt::Key_A:
		if(Keyevent->modifiers() & Qt::ShiftModifier){
			this->set_step(1);
			this->set_step(2);
			this->set_step(3);
		}
		else if(Keyevent->modifiers() & Qt::ControlModifier){
			this->set_step(-1);
			this->set_step(-2);
			this->set_step(-3);
		}
		else
			this->set_animate();
        break;
  case Qt::Key_Q:
        if(Keyevent->modifiers() & Qt::ShiftModifier)
            this->set_first_animation(2);
        else
            this->set_first_animation(1);
       break;
  case Qt::Key_W:
        if(Keyevent->modifiers() & Qt::ShiftModifier)
            this->set_first_animation(-2);
        else
            this->set_first_animation(-1);
       break;
  case Qt::Key_S:
        if(Keyevent->modifiers() & Qt::ShiftModifier)
            this->set_second_animation(2);
        else
            this->set_second_animation(1);
       break;
  case Qt::Key_D:
        if(Keyevent->modifiers() & Qt::ShiftModifier)
            this->set_second_animation(-2);
        else
            this->set_second_animation(-1);
       break;
  case Qt::Key_Z:
        if(Keyevent->modifiers() & Qt::ShiftModifier)
            this->set_third_animation(2);
        else
            this->set_third_animation(1);
       break;
  case Qt::Key_X:
        if(Keyevent->modifiers() & Qt::ShiftModifier)
            this->set_third_animation(-2);
        else
            this->set_third_animation(-1);
       break;
  case Qt::Key_M:
	   this->change_mat();
       break;
  case Qt::Key_J:
	   if(Keyevent->modifiers() & Qt::ControlModifier)
		   this->change_light1();
	   else if(Keyevent->modifiers() & Qt::ShiftModifier)
		   this->move_light(-1,1);
	   else
		   this->move_light(1,1);
	   break;
  case Qt::Key_K:
	   if(Keyevent->modifiers() & Qt::ControlModifier)
		   this->change_light2();
	   else if(Keyevent->modifiers() & Qt::ShiftModifier)
		   this->move_light(-1,2);
	   else
		   this->move_light(1,2);
	   break;
  case Qt::Key_E:
	   if(Keyevent->modifiers() & Qt::ShiftModifier)
		   this->set_lightStep(1);
	   else
		   this->set_step(1);
       break;
  case Qt::Key_R:
	   if(Keyevent->modifiers() & Qt::ShiftModifier)
		   this->set_lightStep(-1);
	   else
		   this->set_step(-1);
       break;
  case Qt::Key_T:
	   if(Keyevent->modifiers() & Qt::ShiftModifier)
		   this->set_lightStep(2);
	   else
		   this->set_step(2);
       break;
  case Qt::Key_Y:
	   if(Keyevent->modifiers() & Qt::ShiftModifier)
		   this->set_lightStep(-2);
	   else
		   this->set_step(-2);
       break;
  case Qt::Key_U:
		this->set_step(3);
       break;
  case Qt::Key_I:
		this->set_step(-3);
       break;

  case Qt::Key_Left:Observer_angle_y-=ANGLE_STEP;break;
  case Qt::Key_Right:Observer_angle_y+=ANGLE_STEP;break;
  case Qt::Key_Up:Observer_angle_x-=ANGLE_STEP;break;
  case Qt::Key_Down:Observer_angle_x+=ANGLE_STEP;break;
  case Qt::Key_PageUp:
		Observer_distance*=1.2;
		x_clipSize *= x_zoomRate;
		y_clipSize *= y_zoomRate;
		break;
  case Qt::Key_PageDown:
		Observer_distance/=1.2;
		if(x_clipSize>0)
			x_clipSize/=x_zoomRate;
		if(y_clipSize>0)
			y_clipSize/=y_zoomRate;
		break;
  }

  update();
}

void _gl_widget::swap_camera(){
	change_camera = !change_camera;
	update();
}

void _gl_widget::change_light1(){
	n1 = (n1+1) % Max_lights;
 switch (Object){
    case OBJECT_TETRAHEDRON:Tetrahedron.change_light1(n1);break;
    case OBJECT_CUBE:Cube.change_light1(n1);break;
    case OBJECT_PLY:Ply.change_light1(n1);break;
    case OBJECT_CONE:Cone.change_light1(n1);break;
    case OBJECT_CYLINDER:Cylinder.change_light1(n1);break;
    case OBJECT_SPHERE:Sphere.change_light1(n1);break;
    case OBJECT_PLYSWEEP:Plysweep.change_light1(n1);break;
    case OBJECT_WHEEL:Wheel.change_light1(n1);break;
    case OBJECT_WHEELS_AXIS:WheelAxis.change_light1(n1);break;
    case OBJECT_TOOTH:Tooth.change_light1(n1);break;
    case OBJECT_TEETH:Teeth.change_light1(n1);break;
    case OBJECT_FOREARM:Forearm.change_light1(n1);break;
    case OBJECT_HYDRAULIC:Hydraulic.change_light1(n1);break;
    case OBJECT_EXCAVATOR:Excavator.change_light1(n1);break;
    case OBJECT_TABLERO:Tablero.change_light1(n1);break;
    default:break;
 }
 update();
}
void _gl_widget::move_light(float step,unsigned int i){
	if(i==0)
		i=1;
	step *= LSTEP[i-1];
	switch (Object){
		case OBJECT_TETRAHEDRON:Tetrahedron.move_light(step,i);break;
		case OBJECT_CUBE:Cube.move_light(step,i);break;
		case OBJECT_PLY:Ply.move_light(step,i);break;
		case OBJECT_CONE:Cone.move_light(step,i);break;
		case OBJECT_CYLINDER:Cylinder.move_light(step,i);break;
		case OBJECT_SPHERE:Sphere.move_light(step,i);break;
		case OBJECT_PLYSWEEP:Plysweep.move_light(step,i);break;
		case OBJECT_WHEEL:Wheel.move_light(step,i);break;
		case OBJECT_WHEELS_AXIS:WheelAxis.move_light(step,i);break;
		case OBJECT_TOOTH:Tooth.move_light(step,i);break;
		case OBJECT_TEETH:Teeth.move_light(step,i);break;
		case OBJECT_FOREARM:Forearm.move_light(step,i);break;
		case OBJECT_HYDRAULIC:Hydraulic.move_light(step,i);break;
		case OBJECT_EXCAVATOR:Excavator.move_light(step,i);break;
		case OBJECT_TABLERO:Tablero.move_light(step,i);break;
		default:break;
	}
	update();
}

void _gl_widget::change_light2(){
	n2 = (n2+1)% Max_lights;
 switch (Object){
    case OBJECT_TETRAHEDRON:Tetrahedron.change_light2(n2);break;
    case OBJECT_CUBE:Cube.change_light2(n2);break;
    case OBJECT_PLY:Ply.change_light2(n2);break;
    case OBJECT_CONE:Cone.change_light2(n2);break;
    case OBJECT_CYLINDER:Cylinder.change_light2(n2);break;
    case OBJECT_SPHERE:Sphere.change_light2(n2);break;
    case OBJECT_PLYSWEEP:Plysweep.change_light2(n2);break;
    case OBJECT_WHEEL:Wheel.change_light2(n1);break;
    case OBJECT_WHEELS_AXIS:WheelAxis.change_light2(n1);break;
    case OBJECT_TOOTH:Tooth.change_light2(n1);break;
    case OBJECT_TEETH:Teeth.change_light2(n1);break;
    case OBJECT_FOREARM:Forearm.change_light2(n1);break;
    case OBJECT_HYDRAULIC:Hydraulic.change_light2(n1);break;
    case OBJECT_EXCAVATOR:Excavator.change_light2(n1);break;
    case OBJECT_TABLERO:Tablero.change_light2(n2);break;
    default:break;
 }
 update();
}

void _gl_widget::update_boxes(){
	Window->set_fill_mode(Draw_fill);
	Window->set_chess_mode(Draw_chess);
	Window->set_light_flat(Draw_light_flat);
	Window->set_light_smooth(Draw_light_smooth);
	Window->set_unlit_texture(Draw_unlit_texture);
	Window->set_texture_flat(Draw_flat_texture);
	Window->set_texture_smooth(Draw_smooth_texture);
}

void _gl_widget::set_point(bool state){
    Draw_point = state;
}
void _gl_widget::set_line(bool state){
    Draw_line = state;
}
void _gl_widget::set_fill(bool state){
	this->set_all_false();
	Draw_fill = state;
	this->update_boxes();
}
void _gl_widget::set_chess(bool state){
	this->set_all_false();
	Draw_chess = state;
	this->update_boxes();
}
void _gl_widget::set_flat(bool state){
	this->set_all_false();
	Draw_light_flat = state;
	this->update_boxes();
}
void _gl_widget::set_smooth(bool state){
	this->set_all_false();
	Draw_light_smooth = state;
	this->update_boxes();
}
void _gl_widget::set_texture(bool state){
	this->set_all_false();
	Draw_unlit_texture = state;
	this->update_boxes();
}
void _gl_widget::set_texture_flat(bool state){
	this->set_all_false();
	Draw_flat_texture = state;
	this->update_boxes();
}
void _gl_widget::set_texture_smooth(bool state){
	this->set_all_false();
	Draw_smooth_texture = state;
	this->update_boxes();
}

void _gl_widget::set_all_false(){
	Draw_fill=false;
	Draw_chess=false;
	Draw_light_flat=false;
	Draw_light_smooth=false;
	Draw_unlit_texture=false;
	Draw_flat_texture=false;
    Draw_smooth_texture=false;
}

bool _gl_widget::get_fill(){
	return Draw_fill;
}

bool _gl_widget::get_chess(){
	return Draw_chess;
}

bool _gl_widget::get_flat(){
	return Draw_light_flat;
}

bool _gl_widget::get_smooth(){
	return Draw_light_smooth;
}

bool _gl_widget::get_texture(){
	return Draw_unlit_texture;
}

bool _gl_widget::get_texture_flat(){
	return Draw_flat_texture;
}

bool _gl_widget::get_texture_smooth(){
	return Draw_smooth_texture;
}

void _gl_widget::set_animate(){
    if(!(Object==OBJECT_WHEELS_AXIS or
            Object==OBJECT_EXCAVATOR or
			Object==OBJECT_TOOTH or
            Object==OBJECT_TEETH or
			Object==OBJECT_FOREARM or
            Object==OBJECT_HYDRAULIC)){
		Error_msg("[ERROR] No hay animación definida para esa entidad");
		Animate = false;
		this->timer->stop();
    }else{
        Animate = !Animate;
        if(!timer->isActive())
			this->timer->start(FPS);
		else
            this->timer->stop();
    }
}
void _gl_widget::set_step(int vx){
    if(vx>=0)
        STEP[abs(vx)-1]+=Animation_step;
    else{
        if(abs(STEP[abs(vx)-1]-0>1e-9))
            STEP[abs(vx)-1]-=Animation_step;
		else
			STEP[abs(vx)-1] = 0;
    }
}

void _gl_widget::set_lightStep(int vx){
    if(vx>=0)
        LSTEP[abs(vx)-1]+=Light_step;
    else{
        if(abs(LSTEP[abs(vx)-1]-0>1e-9))
            LSTEP[abs(vx)-1]-=1.0/10;
		else
			LSTEP[abs(vx)-1] = 0;
    }
}

void _gl_widget::set_fps(int vx){
    if(vx>0){
       FPS += 1;
        cout << "DELAY increased to " << FPS << endl;
    }
    else{
        if(FPS>2)
            FPS-=1;

        cout << "DELAY decreased to " << FPS << endl;
    }
}

void _gl_widget::change_mat(){
	mat = (mat+1)%Max_mats;
	Window->update_CBB2(mat);
	Window->update();
}

void _gl_widget::set_first_animation(int vx){
    if(STEP[0]==0)
        STEP[0]++;
    switch(vx){
        case 2:
            switch(Object){
				case OBJECT_TOOTH:
                    Tooth.scale_lower(STEP[0]);
                    break;
                case OBJECT_TEETH:
                    Teeth.scale_lower(STEP[0]);
                    break;
                case OBJECT_FOREARM:
                    Forearm.expand_claw(STEP[0]);
                    break;
                case OBJECT_HYDRAULIC:
                    Hydraulic.expand_claw(STEP[0]);
                    break;
                case OBJECT_EXCAVATOR:
                    Excavator.CatchSize(STEP[0]);
                    break;
                default:
					Error_msg("[ERROR] No existe esa animación para esa entidad");
					break;
			}
			break;
        case 1:
            switch(Object){
                case OBJECT_WHEELS_AXIS:
                    WheelAxis.alpha+=STEP[0];
                    break;
                case OBJECT_TOOTH:
                    Tooth.move_lower(STEP[0]);
                    break;
                case OBJECT_FOREARM:
                    Forearm.move_claw(STEP[0]);
                    break;
                case OBJECT_TEETH:
                    Teeth.move_lower(STEP[0]);
                    break;
                case OBJECT_HYDRAULIC:
                    Hydraulic.move_claw(STEP[0]);
                    break;
                case OBJECT_EXCAVATOR:
                    Excavator.Catch(STEP[0]);
                    break;
                default:
					Error_msg("[ERROR] No existe esa animación para esa entidad");
					break;
			}
			break;
        case -1:
            switch(Object){
                case OBJECT_WHEELS_AXIS:
                    WheelAxis.alpha-=STEP[0];
                    break;
				case OBJECT_TOOTH:
                    Tooth.move_lower(-STEP[0]);
                    break;
                case OBJECT_TEETH:
                    Teeth.move_lower(-STEP[0]);
                    break;
                case OBJECT_FOREARM:
                    Forearm.move_claw(-STEP[0]);
                    break;
                case OBJECT_HYDRAULIC:
                    Hydraulic.move_claw(-STEP[0]);
                    break;
                case OBJECT_EXCAVATOR:
                    Excavator.Catch(-STEP[0]);
                    break;
                default:
					Error_msg("[ERROR] No existe esa animación para esa entidad");
					break;
			}
			break;
        case -2:
            switch(Object){
                case OBJECT_TOOTH:
                    Tooth.scale_lower(-STEP[0]);
                    break;
                case OBJECT_TEETH:
                    Teeth.scale_lower(-STEP[0]);
                    break;
                case OBJECT_FOREARM:
                    Forearm.expand_claw(-STEP[0]);
                    break;
                case OBJECT_HYDRAULIC:
                    Hydraulic.expand_claw(-STEP[0]);
                    break;
                case OBJECT_EXCAVATOR:
                    Excavator.CatchSize(-STEP[0]);
                    break;
                default:
					Error_msg("[ERROR] No existe esa animación para esa entidad");
					break;
			}
			break;
	}
	update();
}

void _gl_widget::set_second_animation(int vx){
    if(STEP[1]==0)
        STEP[1]++;

    switch(vx){
        case 2:
            switch(Object){
                case OBJECT_HYDRAULIC:
                    Hydraulic.expand(STEP[1]);
                    break;
                case OBJECT_EXCAVATOR:
                    Excavator.Expand(STEP[1]);
                    break;
                default:
					QMessageBox MsgBox(this);
					Error_msg("[ERROR] No existe esa animación para esa entidad");
					break;
			}
			break;
        case 1:
            switch(Object){
                case OBJECT_WHEELS_AXIS:
                    break;
				case OBJECT_FOREARM:
					Forearm.expand(STEP[1]);
					break;
                case OBJECT_HYDRAULIC:
                    Hydraulic.lift_claw(STEP[1]);
                    break;
                case OBJECT_EXCAVATOR:
                    Excavator.Lift(STEP[1]);
                    break;
                default:
					QMessageBox MsgBox(this);
					Error_msg("[ERROR] No existe esa animación para esa entidad");
					break;
			}
			break;
        case -1:
            switch(Object){
                case OBJECT_WHEELS_AXIS:
                    WheelAxis.delta-=STEP[1];
                    break;
				case OBJECT_FOREARM:
					Forearm.expand(-STEP[1]);
					break;
                case OBJECT_HYDRAULIC:
                    Hydraulic.lift_claw(-STEP[1]);
                    break;
                case OBJECT_EXCAVATOR:
                    Excavator.Lift(-STEP[1]);
                    break;
                default:
					QMessageBox MsgBox(this);
					Error_msg("[ERROR] No existe esa animación para esa entidad");
					break;
			}
			break;
        case -2:
            switch(Object){
                case OBJECT_HYDRAULIC:
                    Hydraulic.expand(-STEP[1]);
                    break;
                case OBJECT_EXCAVATOR:
                    Excavator.Expand(-STEP[1]);
                    break;
                default:
					QMessageBox MsgBox(this);
					Error_msg("[ERROR] No existe esa animación para esa entidad");
					break;
			}
			break;
    }
	update();
}

void _gl_widget::set_third_animation(int vx){
    if(STEP[2]==0)
        STEP[2]++;
    switch(vx){
        case 2:
            switch(Object){
                case OBJECT_EXCAVATOR:
                    Excavator.Spin(STEP[2]);
                    break;
                default:
					QMessageBox MsgBox(this);
					Error_msg("[ERROR] No existe esa animación para esa entidad");
					break;
			}
			break;
        case 1:
            switch(Object){
                case OBJECT_WHEELS_AXIS:
                    WheelAxis.theta+=STEP[2];
                    break;
                case OBJECT_EXCAVATOR:
                    Excavator.expand_arm(STEP[2]);
                    break;
                default:
					QMessageBox MsgBox(this);
					Error_msg("[ERROR] No existe esa animación para esa entidad");
					break;
			}
			break;
        case -1:
            switch(Object){
                case OBJECT_WHEELS_AXIS:
                    WheelAxis.theta-=STEP[2];
                    break;
                case OBJECT_EXCAVATOR:
                    Excavator.expand_arm(-STEP[2]);
                    break;
                default:
					QMessageBox MsgBox(this);
					Error_msg("[ERROR] No existe esa animación para esa entidad");
					break;
			}
			break;
        case -2:
            switch(Object){
                case OBJECT_EXCAVATOR:
                    Excavator.Spin(-STEP[2]);
                    break;
                default:
					QMessageBox MsgBox(this);
					Error_msg("[ERROR] No existe esa animación para esa entidad");
					break;
			}
			break;
    }
	update();
}

void _gl_widget::setObject(int index){
    Object = _gl_widget_ne::_object(index);
	selected = false;
	unSelect();
}

void _gl_widget::setMaterial(int index){
    mat = index;
}

void _gl_widget::update_ply(string name){
    Ply =  _plymodel(name);
	selected = false;
	unSelect();
}

void _gl_widget::update_sweep(string name, unsigned int N,bool upper, bool lower){
    Plysweep =  _plysweep(name,N,upper,lower);
	selected = false;
	unSelect();
}

void _gl_widget::idle_event(){
    if(ANIM[0]>=0) ANIM[0] = STEP[0];
    else ANIM[0] = -STEP[0];
    if(ANIM[1]>=0) ANIM[1] = STEP[1];
    else ANIM[1] = -STEP[1];
    if(ANIM[2]>=0) ANIM[2] = STEP[2];
    else ANIM[2] = -STEP[2];
    bool moved = true;
    switch(Object){
        case OBJECT_WHEELS_AXIS:
            WheelAxis.alpha += STEP[0];
            WheelAxis.delta += STEP[1];
            WheelAxis.theta += STEP[2];
            break;
        case OBJECT_TOOTH:
            moved = Tooth.move_lower(ANIM[0]);
            if(!moved)
                ANIM[0] = -ANIM[0];
            moved = Tooth.scale_lower(ANIM[1]);
            if(!moved)
                ANIM[1] = -ANIM[1];
            break;
        case OBJECT_TEETH:
            moved = Teeth.move_lower(ANIM[0]);
            if(!moved)
                ANIM[0] = -ANIM[0];
            moved = Teeth.scale_lower(ANIM[1]);
            if(!moved)
                ANIM[1] = -ANIM[1];
            break;
        case OBJECT_FOREARM:
            Forearm.expand_claw(ANIM[0]);
            moved = Forearm.move_claw(ANIM[0]);
            if(!moved) ANIM[0] = -ANIM[0];
            moved = Forearm.expand(ANIM[1]);
            if(!moved) ANIM[1] = -ANIM[1];
            break;
        case OBJECT_HYDRAULIC:
            Hydraulic.expand_claw(ANIM[0]);
            moved = Hydraulic.move_claw(ANIM[0]);
            if(!moved) ANIM[0] = -ANIM[0];
            Hydraulic.lift_claw(ANIM[1]);
            moved = Hydraulic.expand(ANIM[1]);
            if(!moved) ANIM[1] = -ANIM[1];
            break;
        case OBJECT_EXCAVATOR:
            Excavator.CatchSize(ANIM[0]);
            moved = Excavator.Catch(ANIM[0]);
            if(!moved) ANIM[0] = -ANIM[0];
            Excavator.Expand(ANIM[1]);
            moved = Excavator.Lift(ANIM[1]);
            Excavator.expand_arm(ANIM[1]);
            if(!moved) ANIM[1] = -ANIM[1];
            Excavator.Spin(ANIM[2]);
			Excavator.Forward(ANIM[2]);
            break;
        default:
            this->set_animate();

    }

    update();
}

/*****************************************************************************//**
 * Limpiar ventana
 *
 *
 *
 *****************************************************************************/

void _gl_widget::clear_window()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
}



/*****************************************************************************//**
 * Funcion para definir la transformación de proyeccion
 *
 *
 *
 *****************************************************************************/

void _gl_widget::change_projection()
{
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  // formato(x_minimo,x_maximo, y_minimo, y_maximo,Front_plane, plano_traser)
  // Front_plane>0  Back_plane>PlanoDelantero)
  if(change_camera) //Probably here.
	  glOrtho(-x_clipSize,x_clipSize,-y_clipSize,y_clipSize,FRONT_PLANE_PERSPECTIVE,BACK_PLANE_PERSPECTIVE);
  else
	  glFrustum(X_MIN,X_MAX,Y_MIN,Y_MAX,FRONT_PLANE_PERSPECTIVE,BACK_PLANE_PERSPECTIVE);

}



/*****************************************************************************//**
 * Funcion para definir la transformación de vista (posicionar la camara)
 *
   _vertex3f pos(0,0,-Observer_distance);
  _vertex3f up (0,1,0);
  gluLookAt(
		  pos._0,pos._1,pos._2,
		  test._0,test._1,test._2,
		  up._0, up._1, up._2
		  );*
 *
 *****************************************************************************/

void _gl_widget::change_observer()
{
  // posicion del observador
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
   if(Observer_distance<=0.01)
	   Observer_distance = 0.01;
  if(!cameraVectors){
	  if(selected){
		  float x = test._0;
		  float y = test._1;
		  float z = test._2;
		  glTranslatef(0,0,-Observer_distance);
		  glRotatef(Observer_angle_x,1,0,0);
		  glRotatef(Observer_angle_y,0,1,0);
		  glTranslatef(-x,-y,-z);
	  }
	  else{
		  glTranslatef(0,0,-Observer_distance);
		  glRotatef(Observer_angle_x,1,0,0);
		  glRotatef(Observer_angle_y,0,1,0);
	  }
  }else{
	  alpha = 0, beta = 0, gamma = 0;
	  _vertex3f VRP =  _vertex3f(0,0,1);
	  _vertex3f VPN =  _vertex3f(1,0,1);
	  _vertex3f VUP =  _vertex3f(0,1,0);
	  camera_parameters(VRP,VPN,VUP);

	glTranslatef(-Translate._0,-Translate._1,-Translate._2);
	glTranslatef(0,0,-Observer_distance);
	glRotatef(alpha*180/M_PI+Observer_angle_x,1,0,0);
	glRotatef(beta*180/M_PI+Observer_angle_y,0,1,0);
	glRotatef(gamma*180/M_PI,0,0,1);
  }
}


/*****************************************************************************//**
 * Funcion que dibuja los objetos
 *
 *
 *
 *****************************************************************************/
void _gl_widget::Error_msg(QString text){
			  QMessageBox MsgBox(this);
			  MsgBox.setText(text);
			  MsgBox.exec();
}

void _gl_widget::draw_multiple(unsigned int type){
	unsigned int cont = 0;
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	for(unsigned int i=0;i<PlySelect.size()/row_size;i++){
		for(unsigned int j=0;j<row_size;j++){
			glPushMatrix();
			glTranslatef(-offset+diff*j,0,+offset-diff*i);
			switch(type){
				case 1: PlySelect[cont].draw_point(); break;
				case 2: PlySelect[cont].draw_line(); break;
				case 3: PlySelect[cont].draw_fill(); break;
				case 4: PlySelect[cont].draw_chess(); break;
				case 5: PlySelect[cont].draw_light_flat(); break;
				case 6: PlySelect[cont].draw_light_smooth(); break;
			}
			cont++;
			glPopMatrix();
		}
	}
	glPopMatrix();
}

void _gl_widget::draw_objects()
{

  Axis.draw_line();

  if (Draw_point){
    glPointSize(5);
    glColor3fv((GLfloat *) &BLACK);
    switch (Object){
    case OBJECT_TETRAHEDRON:Tetrahedron.draw_point();break;
    case OBJECT_CUBE:Cube.draw_point();break;
    case OBJECT_PLY:Ply.draw_point();break;
    case OBJECT_CONE:Cone.draw_point();break;
    case OBJECT_CYLINDER:Cylinder.draw_point();break;
    case OBJECT_SPHERE:Sphere.draw_point();break;
    case OBJECT_PLYSWEEP:Plysweep.draw_point();break;
    case OBJECT_WHEEL:Wheel.draw_point();break;
    case OBJECT_WHEELS_AXIS:WheelAxis.draw_point();break;
    case OBJECT_TOOTH:Tooth.draw_point();break;
    case OBJECT_TEETH:Teeth.draw_point();break;
    case OBJECT_FOREARM:Forearm.draw_point();break;
    case OBJECT_HYDRAULIC:Hydraulic.draw_point();break;
    case OBJECT_EXCAVATOR:Excavator.draw_point();break;
    case OBJECT_TABLERO:Tablero.draw_point();break;
	case OBJECT_MULTIPLE: draw_multiple(1); break;
          default:
			  Draw_point = false;
			  update_boxes();
			  Error_msg(
					  "[ERROR] No hemos terminado de definir esa funcionalidad!"
					  );
			  break;
    }
  }

  if (Draw_line){
    glLineWidth(3);
    glColor3fv((GLfloat *) &MAGENTA);
    switch (Object){
    case OBJECT_TETRAHEDRON:Tetrahedron.draw_line();break;
    case OBJECT_CUBE:Cube.draw_line();break;
    case OBJECT_PLY:Ply.draw_line();break;
    case OBJECT_CONE:Cone.draw_line();break;
    case OBJECT_CYLINDER:Cylinder.draw_line();break;
    case OBJECT_SPHERE:Sphere.draw_line();break;
    case OBJECT_PLYSWEEP:Plysweep.draw_line();break;
    case OBJECT_WHEEL:Wheel.draw_line();break;
    case OBJECT_WHEELS_AXIS:WheelAxis.draw_line();break;
    case OBJECT_TOOTH:Tooth.draw_line();break;
    case OBJECT_TEETH:Teeth.draw_line();break;
    case OBJECT_FOREARM:Forearm.draw_line();break;
    case OBJECT_HYDRAULIC:Hydraulic.draw_line();break;
    case OBJECT_EXCAVATOR:Excavator.draw_line();break;
    case OBJECT_TABLERO:Tablero.draw_line();break;
	case OBJECT_MULTIPLE: draw_multiple(2); break;
          default:
			  Draw_line = false;
			  update_boxes();
			  Error_msg(
					  "[ERROR] No hemos terminado de definir esa funcionalidad!"
					  );
			  break;
    }
  }

  if (Draw_fill){
    glColor3fv((GLfloat *) &BLUE);
    switch (Object){
    case OBJECT_TETRAHEDRON:Tetrahedron.draw_fill();break;
    case OBJECT_CUBE:Cube.draw_fill();break;
    case OBJECT_PLY:Ply.draw_fill();break;
    case OBJECT_CONE:Cone.draw_fill();break;
    case OBJECT_CYLINDER:Cylinder.draw_fill();break;
    case OBJECT_SPHERE:Sphere.draw_fill();break;
    case OBJECT_PLYSWEEP:Plysweep.draw_fill();break;
    case OBJECT_WHEEL:Wheel.draw_fill();break;
    case OBJECT_WHEELS_AXIS:WheelAxis.draw_fill();break;
    case OBJECT_TOOTH:Tooth.draw_fill();break;
    case OBJECT_TEETH:Teeth.draw_fill();break;
    case OBJECT_FOREARM:Forearm.draw_fill();break;
    case OBJECT_HYDRAULIC:Hydraulic.draw_fill();break;
    case OBJECT_EXCAVATOR:Excavator.draw_fill();break;
    case OBJECT_TABLERO:Tablero.draw_fill();break;
	case OBJECT_MULTIPLE: draw_multiple(3); break;
          default:
			  Draw_fill = false;
			  update_boxes();
			  Error_msg(
					  "[ERROR] No hemos terminado de definir esa funcionalidad!"
					  );
			  break;
    }
  }

  if (Draw_chess){
    switch (Object){
    case OBJECT_TETRAHEDRON:Tetrahedron.draw_chess();break;
    case OBJECT_CUBE:Cube.draw_chess();break;
    case OBJECT_PLY:Ply.draw_chess();break;
    case OBJECT_CONE:Cone.draw_chess();break;
    case OBJECT_CYLINDER:Cylinder.draw_chess();break;
    case OBJECT_SPHERE:Sphere.draw_chess();break;
    case OBJECT_PLYSWEEP:Plysweep.draw_chess();break;
    case OBJECT_WHEEL:Wheel.draw_chess();break;
    case OBJECT_WHEELS_AXIS:WheelAxis.draw_chess();break;
    case OBJECT_TOOTH:Tooth.draw_chess();break;
    case OBJECT_TEETH:Teeth.draw_chess();break;
    case OBJECT_FOREARM:Forearm.draw_chess();break;
    case OBJECT_HYDRAULIC:Hydraulic.draw_chess();break;
    case OBJECT_EXCAVATOR:Excavator.draw_chess();break;
    case OBJECT_TABLERO:Tablero.draw_chess();break;
	case OBJECT_MULTIPLE: draw_multiple(4); break;
          default:
			  Draw_chess = false;
			  update_boxes();
			  Error_msg(
					  "[ERROR] No hemos terminado de definir esa funcionalidad!"
					  );
			  break;
    }
  }
  if (Draw_light_flat) {
    switch(Object){
    case OBJECT_TETRAHEDRON:Tetrahedron.draw_light_flat(mat);break;
    case OBJECT_CUBE:Cube.draw_light_flat(mat);break;
    case OBJECT_PLY:Ply.draw_light_flat(mat);break;
    case OBJECT_CONE:Cone.draw_light_flat(mat);break;
    case OBJECT_CYLINDER:Cylinder.draw_light_flat(mat);break;
    case OBJECT_SPHERE:Sphere.draw_light_flat(mat);break;
    case OBJECT_PLYSWEEP:Plysweep.draw_light_flat(mat);break;
    case OBJECT_WHEEL:Wheel.draw_light_flat(mat);break;
    case OBJECT_WHEELS_AXIS:WheelAxis.draw_light_flat(mat);break;
    case OBJECT_TOOTH:Tooth.draw_light_flat(mat);break;
    case OBJECT_TEETH:Teeth.draw_light_flat(mat);break;
    case OBJECT_FOREARM:Forearm.draw_light_flat(mat);break;
    case OBJECT_HYDRAULIC:Hydraulic.draw_light_flat(mat);break;
    case OBJECT_EXCAVATOR:Excavator.draw_light_flat(mat);break;
    case OBJECT_TABLERO:Tablero.draw_light_flat(mat);break;
	case OBJECT_MULTIPLE: draw_multiple(5); break;
          default:
			  Draw_light_flat = false;
			  update_boxes();
			  Error_msg(
					  "[ERROR] No hemos terminado de definir esa funcionalidad!"
					  );
			  break;
    }
  }
  if (Draw_light_smooth) {
    switch(Object){
    case OBJECT_TETRAHEDRON:Tetrahedron.draw_light_smooth(mat);break;
    case OBJECT_CUBE:Cube.draw_light_smooth(mat);break;
    case OBJECT_PLY:Ply.draw_light_smooth(mat);break;
    case OBJECT_CONE:Cone.draw_light_smooth(mat);break;
    case OBJECT_CYLINDER:Cylinder.draw_light_smooth(mat);break;
    case OBJECT_SPHERE:Sphere.draw_light_smooth(mat);break;
    case OBJECT_PLYSWEEP:Plysweep.draw_light_smooth(mat);break;
    case OBJECT_WHEEL:Wheel.draw_light_smooth(mat);break;
    case OBJECT_WHEELS_AXIS:WheelAxis.draw_light_smooth(mat);break;
    case OBJECT_TOOTH:Tooth.draw_light_smooth(mat);break;
    case OBJECT_TEETH:Teeth.draw_light_smooth(mat);break;
    case OBJECT_FOREARM:Forearm.draw_light_smooth(mat);break;
    case OBJECT_HYDRAULIC:Hydraulic.draw_light_smooth(mat);break;
    case OBJECT_EXCAVATOR:Excavator.draw_light_smooth(mat);break;
    case OBJECT_TABLERO:Tablero.draw_light_smooth(mat);break;
	case OBJECT_MULTIPLE: draw_multiple(6); break;
          default:
			  Draw_light_smooth = false;
			  update_boxes();
			  Error_msg(
					  "[ERROR] No hemos terminado de definir esa funcionalidad!"
					  );
			  break;
    }
  }
  if(Draw_unlit_texture){
      switch(Object){
          case OBJECT_CYLINDER:Cylinder.draw_texture();break;
          case OBJECT_SPHERE:Sphere.draw_texture();break;
          case OBJECT_TABLERO:Tablero.draw_texture();break;
          default:
			  Draw_unlit_texture = false;
			  update_boxes();
			  Error_msg(
					  "[ERROR] No hemos terminado de definir esa funcionalidad!"
					  );
			  break;
      }
  }
  if(Draw_flat_texture){
        switch(Object){
          case OBJECT_CYLINDER:Cylinder.draw_texture_flat(mat);break;
          case OBJECT_SPHERE:Sphere.draw_texture_flat(mat);break;
          case OBJECT_TABLERO:Tablero.draw_texture_flat(mat);break;
          default:
			  Draw_flat_texture = false;
			  update_boxes();
			  Error_msg(
					  "[ERROR] No hemos terminado de definir esa funcionalidad!"
					  );
			  break;
      }
  }
  if(Draw_smooth_texture){
        switch(Object){
          case OBJECT_CYLINDER:Cylinder.draw_texture_smooth(mat);break;
          case OBJECT_SPHERE:Sphere.draw_texture_smooth(mat);break;
          case OBJECT_TABLERO:Tablero.draw_texture_smooth(mat);break;
          default:
			  Draw_smooth_texture = false;
			  update_boxes();
			  Error_msg(
					  "[ERROR] No hemos terminado de definir esa funcionalidad!"
					  );
			  break;
      }
  }

}



/*****************************************************************************//**
 * Evento de dibujado
 *
 *
 *
 *****************************************************************************/

void _gl_widget::paintGL()
{
  clear_window();
  change_projection();
  change_observer();
  draw_objects();
}


/*****************************************************************************//**
 * Evento de cambio de tamaño de la ventana
 *
 *
 *
 *****************************************************************************/

void _gl_widget::resizeGL(int Width1, int Height1)
{
  glViewport(0,0,Width1,Height1);
}


/*****************************************************************************//**
 * Inicialización de OpenGL
 *
 *
 *
 *****************************************************************************/

void _gl_widget::initializeGL()
{
  glewExperimental = GL_TRUE;
  int err = glewInit();

  if(GLEW_OK != err){
	  // Problem: glewInit failed, something is seriously wrong.
	  QMessageBox MsgBox(this);
	  MsgBox.setText(
			  "[ERROR]: There is not OpenGL drivers\n\nPlease, \
			  look for the information of your GPU (AMD, INTEL or NVIDIA) and \
			  install the drivers"
			  );
	  MsgBox.exec();
	  Window->close();
  }

  const GLubyte* strm;

  strm = glGetString(GL_VENDOR);
  std::cerr << "Vendor: " << strm << "\n";
  strm = glGetString(GL_RENDERER);
  std::cerr << "Renderer: " << strm << "\n";
  strm = glGetString(GL_VERSION);
  std::cerr << "OpenGL Version: " << strm << "\n";

  if (strm[0] == '1'){
    std::cerr << "Only OpenGL 1.X supported!\n";
    exit(-1);
  }

  strm = glGetString(GL_SHADING_LANGUAGE_VERSION);
  std::cerr << "GLSL Version: " << strm << "\n";

  int Max_texture_size=0;
  glGetIntegerv(GL_MAX_TEXTURE_SIZE, &Max_texture_size);
  std::cerr << "Max texture size: " << Max_texture_size << "\n";

  glClearColor(1.0,1.0,1.0,1.0);
  glEnable(GL_DEPTH_TEST);

	Object = _gl_widget_ne::OBJECT_TETRAHEDRON;

  Observer_angle_x=0;
  Observer_angle_y=0;
  Observer_distance=DEFAULT_DISTANCE;

  selected_index_i = -1;
  selected_index_j = -1;
  selected = false;

  connect(this->timer,SIGNAL(timeout()),
                        this,SLOT(idle_event()));
  mover->start(90);
  connect(this->mover,SIGNAL(timeout()),
                        this,SLOT(move_screen()));

  selecter->start(60);
  connect(this->mover,SIGNAL(timeout()),
                        this,SLOT(updateSelected()));

  initial_mouse_x = 0;
  initial_mouse_y = 0;
  change_position = false;

  change_camera = false;
  x_clipSize = 2;
  x_zoomRate = 1.2;
  y_clipSize = 2;
  y_zoomRate = 1.2;

  Animation_step = 1.0/10.0;
  Light_step = 1;

  mat = 0;
  n1 = 1;
  n2 = 2;
  Max_mats = 5;
  Max_lights = 5;

  Draw_point=false;
  Draw_line=false;
  Draw_fill=false;
  Draw_chess=false;
  Draw_light_flat=false;
  Draw_light_smooth=false;
  Draw_unlit_texture=false;
  Draw_flat_texture=false;
  Draw_smooth_texture=false;

  // Code for reading an image
  QString File_name("./texturas/debug.jpg");
  QImage Image;
  QImageReader Reader(File_name);
  Reader.setAutoTransform(true);
  Image = Reader.read();
  if (Image.isNull()) {
      QMessageBox::information(this, QGuiApplication::applicationDisplayName(),
              tr("Cannot load %1.").arg(QDir::toNativeSeparators(File_name)));
      exit(-1);
  }
  Image=Image.mirrored();
  Image=Image.convertToFormat(QImage::Format_RGB888);

  // Code to control the application of the texture
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  //glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);

  // Code to pass the image to OpenGL to form a texture 2D
  glTexImage2D(GL_TEXTURE_2D,0,3,Image.width(),Image.height(),0,GL_RGB,GL_UNSIGNED_BYTE,Image.bits());

  row_size = 4;
  for(unsigned int i=0;i<row_size;i++){
	  PlySelect.push_back( _plymodel("./ply_models/ant.ply"));
	  PlySelect.push_back( _plymodel("./ply_models/beethoven.ply"));
	  PlySelect.push_back( _plymodel("./ply_models/big_porsche.ply"));
	  PlySelect.push_back( _plymodel("./ply_models/sandal.ply"));
  }
  auto rng = std::default_random_engine{};
  std::shuffle(std::begin(PlySelect),std::end(PlySelect),rng);

  offset = (row_size-1)*PLYDIST;
  diff = PLYDIST*2;


  cameraVectors = false;

}
