#ifndef WHEEL_H
#define WHEEL_H

#include "object3d.h"
#include "cylinder.h"
#include "operate.h"

class _wheel
{
private:
    _cylinder Cylinder;
public:
	int Tvalue = 0;
	int Tend = 1;
	float MV[16] = {0};

    _wheel(float Size=1.0,unsigned int N=16);
    void draw_point();
    void draw_line();
    void draw_fill();
	void draw_fill(int* index);
    void draw_chess();
    void draw_light_flat(unsigned int var=0);
    void draw_light_smooth(unsigned int var=0);
    void draw_something(unsigned int type, unsigned int mat=0,int index=-1);

	void setSelected(int index);
	_vertex3f getSelected(int index);
	int getT(){return Tvalue+Tend-1;};

	void change_light1(unsigned int val);
	void change_light2(unsigned int val);
	void move_light(int i, int j);
};

#endif // WHEEL_H
