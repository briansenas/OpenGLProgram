#ifndef EXCAVATOR_H
#define EXCAVATOR_H

#include "wheels_axis.h"
#include "object3d.h"
#include "hydraulic.h"
#include "cube.h"

class _excavator
{
private:
    _hydraulic Hydraulic;
    _cube Chassis;
    _cube Cabine;
    _cube Attach;
    _wheels_axis FrontWheels;
    _wheels_axis BackWheels;
    float alpha = 0;
    float delta = 0;
    float width = 20;
    float depth = 20;
    float height = 10;

public:
	float Tvalue = 0;
	float Tend = 0;
	float MV[6][16];

    _excavator(float Size=1.0, unsigned int N=16, float wheel_size=0.125,
            float width=1,float depth = 1, float height = 0.25 );
    void draw_point();
    void draw_line();
    void draw_fill();
    void draw_fill(int* index);
    void draw_chess();
    void draw_light_flat(unsigned int var=0);
    void draw_light_smooth(unsigned int var=0);
    void draw_something(unsigned int type,unsigned int mat=0,int index=-1);
    void Forward(float step);
    void Spin(float step);
    bool Lift(float step);
    bool Expand(float step);
    bool expand_arm(float step);
    bool Catch(float step);
    bool CatchSize(float step);

	void change_light1(unsigned int val);
	void change_light2(unsigned int val);
	void move_light(int i, int j);

	void setSelected(int i, int j);
	_vertex3f getSelected(int i, int j);
	int getT(){return Tvalue+Tend-1;};
};

#endif
