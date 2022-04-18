#include "wheel.h"

_wheel::_wheel(float Size, unsigned int N){
    this->Cylinder =  _cylinder(Size,N);
	this->Tend = 1;
}

void _wheel::setSelected(int index){
	Cylinder.setSelected(index);
}

_vertex3f _wheel::getSelected(int index){
	_vertex3f base = Cylinder.getSelected(index);
	_vertex3f res;
	res._0 = MV[0]*base._0 + MV[4]*base._1 + MV[8]*base._2 + MV[12];
	res._1 = MV[1]*base._0 + MV[5]*base._1 + MV[9]*base._2 + MV[13];
	res._2 = MV[2]*base._0 + MV[6]*base._1 + MV[10]*base._2 + MV[14];
	return res;
}

void _wheel::draw_something(unsigned int type,unsigned int mat, int index){
	if(index>0)
		this->Tvalue = index;
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glRotatef(90,0,0,1);
    glScalef(2,0.4,2);
    switch(type){
        case 1:
            Cylinder.draw_point();
            break;
        case 2:
            Cylinder.draw_line();
            break;
        case 3:
            Cylinder.draw_fill();
            break;
        case 4:
            Cylinder.draw_chess();
            break;
        case 5:
            Cylinder.draw_light_flat(mat);
            break;
        case 6:
            Cylinder.draw_light_smooth(mat);
            break;
        case 7:
            Cylinder.draw_fill(&index);
            break;

	}
	glLoadIdentity();
    glRotatef(90,0,0,1);
    glScalef(2,0.4,2);
	glGetFloatv(GL_MODELVIEW_MATRIX,MV);
    glPopMatrix();
}

void _wheel::draw_point(){
    this->draw_something(1);
}

void _wheel::draw_line(){
    this->draw_something(2);
}

void _wheel::draw_fill(){
    this->draw_something(3);
}

void _wheel::draw_fill(int* index){
    this->draw_something(7,-1,*index);
	*index += this->Tend;
}

void _wheel::draw_chess(){
    this->draw_something(4);
}

void _wheel::draw_light_flat(unsigned int mat){
    this->draw_something(5,mat);
}
void _wheel::draw_light_smooth(unsigned int mat){
    this->draw_something(6,mat);
}
void _wheel::change_light1(unsigned int val){
    this->Cylinder.change_light1(val);
}
void _wheel::change_light2(unsigned int val){
    this->Cylinder.change_light2(val);
}
void _wheel::move_light(int i, int j){
	this->Cylinder.move_light(i,j);
}
