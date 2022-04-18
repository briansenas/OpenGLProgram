/*! \file
 * Copyright Domingo Martín Perandres
 * email: dmartin@ugr.es
 * web: http://calipso.ugr.es/dmartin
 * 2003-2019
 * GPL 3
 */


#ifndef OBJECT3D_H
#define OBJECT3D_H

#include "basic_object3d.h"
#include "operate.h"

/*****************************************************************************//**
 *
 *
 *
 *****************************************************************************/
struct typeLight{
	_vertex4f Position;
	_vertex4f Ambient;
	_vertex4f Diffuse;
	_vertex4f Specular;
	_vertex4f Spot_Direction;
	GLfloat Spot_Exponent;
	GLfloat Spot_Cutoff;
	//_vertex3f Attenuation;

    typeLight(){ //Default values for LIGHT0 and so on
        Position =  _vertex4f(0,0,-50,1);
		Ambient =  _vertex4f(0.3,0.3,0.3,1);
		Diffuse =  _vertex4f(0.4,0.4,0.4,1);
		Specular =  _vertex4f(0.4,0.4,0.4,1);
		Spot_Direction =  _vertex4f(0,0,-1,1);
		Spot_Exponent = 0; //Uniform light distribution;
		Spot_Cutoff = 180; //Uniform light Distribution;
    }
};

struct Material{
    _vertex3f Ambient;
    _vertex3f Diffuse;
    _vertex3f Specular;
    float Shininess;

};

class _object3D : public _basic_object3D
{
private:
    void init_mats();
	void init_lights();


protected:
	bool upper, lower;
	unsigned int ax;
	bool repeated=false;
	void repeat_vertex();

	void enable_lights(unsigned int var);
	void disable_lights();

	void enable_texture_light(unsigned int var);

	unsigned int n1;
	unsigned int n2;

	unsigned int stepV;
	unsigned int stepH;
	unsigned int steps;

	float alpha;
	float delta;

	unsigned int selected=-1;

public:
  vector<_vertex3ui> Triangles;
  vector<_vertex3f> TNormals;
  vector<_vertex3f> PNormals;
  vector<Material> Mats;
  vector<typeLight> Lights;

  _object3D();

  void draw_line();
  void draw_fill();
  void draw_fill(int* index);
  _vertex3f getSelected(int index);
  void draw_chess() ;
  void draw_light_flat(unsigned int var=0);
  void draw_light_smooth(unsigned int var=0);

  void setSelected(int i){selected=i;}

  void change_light1(int index);
  void change_light2(int index);
  void move_light(float step,unsigned int i);

  void compute_normal_triangles();
  void compute_normal_vertex();
  _vertex3f normal(_vertex3f a);

};

#endif // OBJECT3D_H
