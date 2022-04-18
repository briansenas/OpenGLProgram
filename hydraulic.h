#ifndef HYDRAULIC_H
#define HYDRAULIC_H

#include "object3d.h"
#include "teeth.h"
#include "forearm.h"
#include "cube.h"

class _hydraulic
{
private:
	_forearm myarm;
    _cube center;
public:
	float Tvalue = 0;
	float Tend = 0;
	float MV[2][16];

	float Size, LSize, USize, TSize;

    float alpha = 0; //Lifting
    float delta = 0;
    float adjust_up = 0;
    _hydraulic(float Size=1.0,float LSize=10.0,float USize=5.0,float TSize=1.0);
    void draw_point();
    void draw_line();
    void draw_fill();
    void draw_fill(int* index);
    void draw_chess();
    void draw_light_flat(unsigned int var=0);
    void draw_light_smooth(unsigned int var=0);
    void draw_something(unsigned int type,unsigned int mat=0, int index=-1);

    bool move_claw(float step);
    bool expand_claw(float step);
	bool expand_arm(float step);
    bool lift_claw(float step);
    bool expand(float step);

	void change_light1(unsigned int val);
	void change_light2(unsigned int val);
	void move_light(int i, int j);

	void setSelected(int i, int j);
	_vertex3f getSelected(int i, int j);
	int getT(){return Tend-Tvalue-1;};
};
#endif //HYDRAULIC_H
