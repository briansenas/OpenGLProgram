#ifndef FOREARM_H
#define FOREARM_H

#include "object3d.h"
#include "cube.h"
#include "teeth.h"

class _forearm : public _object3D
{
private:
    _teeth right;
    _teeth left;
    _cube upper;
public:
	int Tvalue = 0;
	int Tend = 9;
	float MV[3][16];
	float OSize, CSize, TSize;

    float delta = 0;
    float adjust_up = 0;
    _forearm(float CSize=1.0,float OSize=5.0,float TSize=1.0);
    void draw_point();
    void draw_line();
    void draw_fill();
	void draw_fill(int* index);
    void draw_chess();
    void draw_light_flat(unsigned int var=0);
    void draw_light_smooth(unsigned int var=0);
    void draw_something(unsigned int type,unsigned int mat=0,int index=-1);

    bool move_claw(float step);
    bool expand_claw(float step);
    bool expand(float step);

	void change_light1(unsigned int val);
	void change_light2(unsigned int val);
	void move_light(int i, int j);

	void setSelected(int i, int j);
	_vertex3f getSelected(int i, int j);
	int getT(){return Tvalue+Tend-1;};
};

#endif // CYLINDER_H
