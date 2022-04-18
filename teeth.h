#ifndef TEETH_H
#define TEETH_H

#include "cube.h"
#include "tooth.h"

class _teeth
{
private:
    _tooth DCHA;
    _tooth IZDA;

public:
	int Tvalue = 0;
	int Tend = 8;
	float MV[2][16];
	float CSize, TSize;

    _teeth(float CSize = 1.0,float TSize=1.0);
    void draw_point();
    void draw_line();
    void draw_fill();
	void draw_fill(int* index);
    void draw_chess();
    void draw_light_flat(unsigned int var=0);
    void draw_light_smooth(unsigned int var=0);
    bool move_lower(float step);
    bool scale_lower(float step);
    void draw_something(unsigned int type,unsigned int mat=0, int index=-1);

	void change_light1(unsigned int val);
	void change_light2(unsigned int val);
	void move_light(int i, int j);

	void setSelected(int i, int j);
	_vertex3f getSelected(int i, int j);
	int getT(){return Tvalue+Tend-1;};
};

#endif
