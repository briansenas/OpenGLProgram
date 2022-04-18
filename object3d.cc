/*! \file
 * Copyright Domingo Martín Perandres
 * email: dmartin@ugr.es
 * web: http://calipso.ugr.es/dmartin
 * 2003-2019
 * GPL 3
 */


#include "object3d.h"
#include <GL/gl.h>


using namespace _colors_ne;


/*****************************************************************************//**
 *
 *
 *
 *****************************************************************************/
_object3D::_object3D(){
	steps = 0;
	stepH = 0;
	stepV = 0;
	ax = 0;
	n1 = 1;
	n2 = 2;
	alpha = 0;
	delta = 0;
    init_mats();
	init_lights();
}

void _object3D::move_light(float step,unsigned int i){
	switch(i){
		case 2:
			this->delta += step;
			break;
		case 1:
			this->alpha += step;
			break;
	}
}

void _object3D::change_light1(int index){
	if(index>=0 and index<Lights.size())
		n1 = index;
}

void _object3D::change_light2(int index){
	if(index>=0 and index<Lights.size())
		n2 = index;
}

void _object3D::init_lights(){
	typeLight White;
	White.Position = _vertex4f(0,0,50,1);
	White.Ambient = _vertex4f(.3,.3,.3,1);
	Lights.push_back(White);
	typeLight Blue;
	Blue.Ambient = _vertex4f(0,0,1,1);
	Blue.Diffuse =  _vertex4f(0,0,1,1);
	Blue.Specular =  _vertex4f(0,0,1,1);
	Lights.push_back(Blue);
	typeLight Red;
	Red.Ambient =  _vertex4f(1,0,0,1);
	Red.Ambient =  _vertex4f(1,0,0,1);
	Red.Diffuse =  _vertex4f(1,0,0,1);
	Red.Specular =  _vertex4f(1,0,0,1);
	Lights.push_back(Red);
	typeLight Green;
	Green.Ambient =  _vertex4f(0,1,0,1);
	Green.Diffuse =  _vertex4f(0,1,0,1);
	Green.Specular =  _vertex4f(0,1,0,1);
	Lights.push_back(Green);
	typeLight TURN_OFF;
	TURN_OFF.Ambient =  _vertex4f(0,0,0,1);
	TURN_OFF.Diffuse =  _vertex4f(0,0,0,1);
	TURN_OFF.Specular =  _vertex4f(0,0,0,1);
	Lights.push_back(TURN_OFF);
}
//http://devernay.free.fr/cours/opengl/materials.html
void _object3D::init_mats(){

    //La suma de los valores debería sumar 0, pero he copiado tal cuál de la
    //página web que pone arriba
    Material WhitePlastic;
    WhitePlastic.Ambient =  _vertex3f(0,0,0);
    WhitePlastic.Diffuse =  _vertex3f(0.55,0.55,0.55);
    WhitePlastic.Specular =  _vertex3f(0.70,0.70,0.70);
    WhitePlastic.Shininess = 0.25 * 128;
    Mats.push_back(WhitePlastic);


    Material Emerald;
    Emerald.Ambient =  _vertex3f(0.0215,0.1745,0.0215);
    Emerald.Diffuse =  _vertex3f(0.07568,0.61424,0.07568);
    Emerald.Specular =  _vertex3f(0.633,0.727811,0.633);
    Emerald.Shininess = 0.6 * 128;
    Mats.push_back(Emerald);

    Material Pearl;
    Pearl.Ambient =  _vertex3f(0.25,0.20725,0.20725);
    Pearl.Diffuse =  _vertex3f(1,0.829,0.829);
    Pearl.Specular =  _vertex3f(0.296648,0.296648,0.296648);
    Pearl.Shininess = 0.088 * 128;
    Mats.push_back(Pearl);

    Material Ruby;
    Ruby.Ambient =  _vertex3f(0.1745,0.01175,0.01175);
    Ruby.Diffuse =  _vertex3f(0.61424,0.04136,0.04136);
    Ruby.Specular =  _vertex3f(0.727811,0.626959,0.626959);
    Ruby.Shininess = 0.6 * 128;
    Mats.push_back(Ruby);

    Material Bronze;
    Bronze.Ambient =  _vertex3f(0.2125,0.1275,0.054);
    Bronze.Diffuse =  _vertex3f(0.714,0.4284,0.18144);
    Bronze.Specular =  _vertex3f(0.393548,0.271906,0.166721);
    Bronze.Shininess = 0.2 * 128;
    Mats.push_back(Bronze);

}

void _object3D::draw_line()
{
  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  glBegin(GL_TRIANGLES);
  for(unsigned int i = 0; i < Triangles.size(); i++){
	  glColor3fv((const GLfloat*) &BLACK);
      glVertex3fv((const GLfloat *) &Vertices[Triangles[i]._0]);
      glVertex3fv((const GLfloat *) &Vertices[Triangles[i]._1]);
      glVertex3fv((const GLfloat *) &Vertices[Triangles[i]._2]);
  }
  glEnd();
}


/*****************************************************************************//**
 *
 *
 *
 *****************************************************************************/

void _object3D::draw_fill()
{
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  //glShadeModel(GL_FLAT);
  glBegin(GL_TRIANGLES);
  for(unsigned int i = 0; i < Triangles.size(); i++){
	  if(i!=selected)
		glColor3fv((const GLfloat*) &COLORS[0]);
	  else
		glColor3fv((const GLfloat*) &COLORS[5]);
      glVertex3fv((const GLfloat*) &Vertices[Triangles[i]._0]);
      glVertex3fv((const GLfloat*) &Vertices[Triangles[i]._1]);
      glVertex3fv((const GLfloat*) &Vertices[Triangles[i]._2]);
  }
  glEnd();
}

void _object3D::draw_fill(int* pos)
{
  int index = *pos;
  *pos += 1;
  glPolygonMode(GL_FRONT, GL_FILL);
  glBegin(GL_TRIANGLES);
  _vertex3f color;
  bool every_time = (index < 0);
  unsigned int var = (unsigned int) index;
  for(unsigned int i=0;i<Triangles.size();i++){
	  if(every_time) {
		  index = i ;
		color = _vertex3f((index & 0x00FF0000)>>16,(i&0x0000FF00)>>8,i&0x000000FF);
	  }else
		color = _vertex3f(var,(i&0x0000FF00)>>8,i&0x000000FF);
	  color /= 255.0;
	  glColor3fv((const GLfloat*) &color);
	  glVertex3fv((const GLfloat*) &Vertices[Triangles[i]._0]);
	  glVertex3fv((const GLfloat*) &Vertices[Triangles[i]._1]);
	  glVertex3fv((const GLfloat*) &Vertices[Triangles[i]._2]);
  }
  glEnd();
}

_vertex3f _object3D::getSelected(int index){
	_vertex3f res;
	if(index>=0 and index<Triangles.size()){
		res = Vertices[Triangles[index]._0];
		res += Vertices[Triangles[index]._1];
		res += Vertices[Triangles[index]._2];
		res /= 3.0;
	}
	return res;
}
/*****************************************************************************//**
 *
 *
 *
 *****************************************************************************/

void _object3D::draw_chess()
{
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    //glShadeModel(GL_FLAT);
    glBegin(GL_TRIANGLES);
    int cont = 0;
    for(unsigned int i = 0; i < Triangles.size(); i++){
		if(i!=selected)
			glColor3fv((const GLfloat*) &COLORS[6+cont]);
		else
			glColor3fv((const GLfloat*) &COLORS[5]);
        glVertex3fv((const GLfloat*) &Vertices[Triangles[i]._0]);
        glVertex3fv((const GLfloat*) &Vertices[Triangles[i]._1]);
        glVertex3fv((const GLfloat*) &Vertices[Triangles[i]._2]);
        cont = (cont+1)%2;
    }
    glEnd();
}

void _object3D::enable_lights(unsigned int var){
	glLightfv(GL_LIGHT0,GL_AMBIENT,(GLfloat*) &Lights[0].Ambient);
	glLightfv(GL_LIGHT0,GL_DIFFUSE,(GLfloat*) &Lights[0].Diffuse);
	glLightfv(GL_LIGHT0,GL_SPECULAR,(GLfloat*) &Lights[0].Specular);

	glPushMatrix();
	glRotatef(alpha,0,1,0);
	glLightfv(GL_LIGHT1,GL_POSITION,(GLfloat*) &Lights[0].Position);
	glPopMatrix();
    glLightfv(GL_LIGHT1,GL_DIFFUSE,(GLfloat*) &Lights[n1].Diffuse);
    glLightfv(GL_LIGHT1,GL_SPECULAR,(GLfloat*) &Lights[n1].Specular);

	glPushMatrix();
	glRotatef(delta,0,1,0);
	glLightfv(GL_LIGHT2,GL_POSITION,(GLfloat*) &*(new _vertex4f(0,0,-5,0)));
	glPopMatrix();
    //glLightfv(GL_LIGHT2,GL_AMBIENT,(GLfloat*) &Lights[1].Ambient);
    glLightfv(GL_LIGHT2,GL_DIFFUSE,(GLfloat*) &Lights[n2].Diffuse);
    glLightfv(GL_LIGHT2,GL_SPECULAR,(GLfloat*) &Lights[n2].Specular);
    glLightfv(GL_LIGHT2,GL_SPOT_DIRECTION,(GLfloat*) &Lights[n2].Spot_Direction);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    glMaterialfv(GL_FRONT,GL_AMBIENT,(GLfloat*) &Mats[var].Ambient);
    glMaterialfv(GL_FRONT,GL_SPECULAR,(GLfloat*) &Mats[var].Specular);

    glMaterialf(GL_FRONT,GL_SHININESS,Mats[var].Shininess);
    glMaterialfv(GL_FRONT,GL_DIFFUSE,(GLfloat*) &Mats[var].Diffuse);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	glEnable(GL_LIGHT2);
}

void _object3D::draw_light_flat(unsigned int var){
    if(TNormals.empty())
        this->compute_normal_triangles();

    if(TNormals.empty())
        cout << "[ERROR]Empty Normal Vector...." << endl;
    else{

	this->enable_lights(var);
	glEnable(GL_NORMALIZE);

    glShadeModel(GL_FLAT);
    glBegin(GL_TRIANGLES);
    for(unsigned int i=0;i<Triangles.size();i++){
        glNormal3fv((const GLfloat *) &TNormals[i]); //Siempre realiza una interpolación aunque sea una sola norma
        glVertex3fv((const GLfloat *) &Vertices[Triangles[i]._0]);
        glVertex3fv((const GLfloat *) &Vertices[Triangles[i]._1]);
        glVertex3fv((const GLfloat *) &Vertices[Triangles[i]._2]);
    }
    glEnd();

	glDisable(GL_NORMALIZE);
	this->disable_lights();

    }
}

void _object3D::disable_lights(){
	glDisable(GL_LIGHTING);
    glDisable(GL_LIGHT0);
	glDisable(GL_LIGHT1);
	glDisable(GL_LIGHT2);
}

void _object3D::draw_light_smooth(unsigned int var){
    if(PNormals.empty())
        this->compute_normal_vertex();

    if(PNormals.empty())
        cout << "[ERROR]Empty Normal Vector...." << endl;
    else{
		this->enable_lights(var);
		glEnable(GL_NORMALIZE);

        glShadeModel(GL_SMOOTH);
        glBegin(GL_TRIANGLES);
        for(unsigned int i=0;i<Triangles.size();i++){
            glNormal3fv((const GLfloat *) &PNormals[Triangles[i]._0]);
            glVertex3fv((const GLfloat *) &Vertices[Triangles[i]._0]);
            glNormal3fv((const GLfloat *) &PNormals[Triangles[i]._1]);
            glVertex3fv((const GLfloat *) &Vertices[Triangles[i]._1]);
            glNormal3fv((const GLfloat *) &PNormals[Triangles[i]._2]);
            glVertex3fv((const GLfloat *) &Vertices[Triangles[i]._2]);
        }
        glEnd();

		glDisable(GL_NORMALIZE);

		this->disable_lights();
    }
}
/*****************************************************************************//**
 *
 *
 *
 *****************************************************************************/

void _object3D::repeat_vertex(){
	if(!repeated){
		this->ax = 0;
		_vertex3f upperV,lowerV;
		if(upper){
			upperV = Vertices.back();
			Vertices.pop_back();
			ax++;
		}
		if(lower){
			lowerV = Vertices.back();
			Vertices.pop_back();
			ax++;
		}

		for(unsigned int i=0;i<this->stepV;i++){ //Repetimos puntos iniciales
			Vertices.push_back(Vertices[i]);
		}

		unsigned int new_pos = Vertices.size();
		if(lower){
			for(unsigned int i=0;i<this->stepH;i++){ //Repetimos tapa inferior
				Vertices.push_back(lowerV);
				Triangles[Triangles.size()-(ax*stepH)+i]._0 = new_pos;
			}
			new_pos+=stepH;
		}
		if(upper){
			for(unsigned int i=0;i<this->stepH;i++){ //Repetimos tapa superior
				Vertices.push_back(upperV);
				Triangles[Triangles.size()-stepH+i]._0 = new_pos;
			}
		}

		repeated = true;
	}
}
_vertex3f _object3D::normal(_vertex3f a){
    _vertex3f b,c;
    b = Vertices[a._1]-Vertices[a._0];
    c = Vertices[a._2]-Vertices[a._0];
    return _operate::_operate::normalize(_operate::_operate::cross_product(b,c));
}
/*****************************************************************************//**
 *
 *
 *
 *****************************************************************************/
void _object3D::compute_normal_triangles(){
    TNormals.clear();
    _vertex3f a, b;
    for(unsigned int i=0;i<Triangles.size();i++){
        a = Vertices[Triangles[i]._1]-Vertices[Triangles[i]._0];
        b = Vertices[Triangles[i]._2]-Vertices[Triangles[i]._0];
        TNormals.push_back(_operate::normalize(_operate::cross_product(a,b)));
    }
}
/*****************************************************************************//**
 *
 *
 *
 *****************************************************************************/
void _object3D::compute_normal_vertex(){
    PNormals.clear();
    PNormals.resize(Vertices.size());
    if(TNormals.empty()){
		_vertex3f a,b,c;
		for (unsigned int i = 0; i < Triangles.size(); i++) {
			a = Vertices[Triangles[i]._1]-Vertices[Triangles[i]._0];
			b = Vertices[Triangles[i]._2]-Vertices[Triangles[i]._0];
			c = _operate::normalize(_operate::cross_product(a,b));
			PNormals[Triangles[i]._0] += c;
			PNormals[Triangles[i]._1] += c;
			PNormals[Triangles[i]._2] += c;
		}
		for (unsigned int i = 0; i < PNormals.size(); i++) {
			PNormals[i] = _operate::normalize(PNormals[i]);
		}
    }else{
        for(unsigned int i=0; i<TNormals.size();i++){
            PNormals[Triangles[i]._0] += TNormals[i];
            PNormals[Triangles[i]._1] += TNormals[i];
            PNormals[Triangles[i]._2] += TNormals[i];
        }
        for (unsigned int i = 0; i < PNormals.size(); i++) {
            PNormals[i] = _operate::normalize(PNormals[i]);
        }
    }
}
