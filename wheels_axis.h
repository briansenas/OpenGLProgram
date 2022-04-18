#ifndef WHEELS_AXIS_H
#define WHEELS_AXIS_H

#include "wheel.h"
#include "cylinder.h"
#include "object3d.h"

class _wheels_axis
{
private:
        _wheel Wheel;
		_wheel Wheel2;
        _cylinder Cylinder;
        float wheel_size = 1;
        float width = 1;
public:
		float MV[3][16];
		float ASize;
		float WSize;

		int Tvalue = 0;
		int Tend = 0;

        float alpha = 0;
        float delta = 0;
        float theta = 0;
        _wheels_axis(float Size=1.0,unsigned int N=16,float wheel_size=1.0,float width=6.5);
        void clear();
        void draw_point();
        void draw_line();
        void draw_fill();
        void draw_fill(int* index);
        void draw_chess();
        void draw_light_flat(unsigned int mat=0);
        void draw_light_smooth(unsigned int mat=0);
        void draw_something(unsigned int type,unsigned int mat=0,int index=-1);
        void rotate(float step);

		void setSelected(int i, int j);
		_vertex3f getSelected(int i, int j);
		int getT(){return Tvalue+Tend-1;};

		void change_light1(unsigned int val);
		void change_light2(unsigned int val);
		void move_light(int i, int j);
};

#endif // WHEELS_AXIS_H
