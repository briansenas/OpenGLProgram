#include "tooth.h"

_tooth::_tooth(float Size){
    Sup = _cube(Size);
    Inf = _cube(Size);
	this->Size = Size;
	this->Tend = 2;
}

void _tooth::setSelected(int i, int j){
	int c = -1;
	if(i>=Tvalue and i<this->Tvalue+1)
		c = 0;
	else if(i>=this->Tvalue+1)
		c = 1;
	switch(c){
		case 0:
			Sup.setSelected(j);
			break;
		case 1:
			Inf.setSelected(j);
			break;
		default:
			Sup.setSelected(-1);
			Inf.setSelected(-1);
			break;
	}
}

_vertex3f _tooth::getSelected(int i, int j){
	_vertex3f res,base;
	int c  = -1;
	if(i>=Tvalue and i<this->Tvalue+1)
		c = 0;
	else if(i>=this->Tvalue+1)
		c = 1;
	switch(c){
		case 0:
			base = Sup.getSelected(j);
			res._0 = MV[0][0]*base._0 + MV[0][4]*base._1 + MV[0][8]*base._2 + MV[0][12];
			res._1 = MV[0][1]*base._0 + MV[0][5]*base._1 + MV[0][9]*base._2 + MV[0][13];
			res._2 = MV[0][2]*base._0 + MV[0][6]*base._1 + MV[0][10]*base._2 +MV[0][14];
			break;
		case 1:
			base = Inf.getSelected(j);
			res._0 = MV[1][0]*base._0 + MV[1][4]*base._1 + MV[1][8]*base._2 + MV[1][12];
			res._1 = MV[1][1]*base._0 + MV[1][5]*base._1 + MV[1][9]*base._2 + MV[1][13];
			res._2 = MV[1][2]*base._0 + MV[1][6]*base._1 + MV[1][10]*base._2 +MV[1][14];
			break;
	}
	return res;
}

void _tooth::draw_something(unsigned int type, unsigned int mat,int index){
	if(index>=0)
		this->Tvalue = index;
	glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glRotatef(alpha,0,0,1); //Open and close this shit
    glPushMatrix();
			glTranslatef(-(delta-1)*Size/2.5,-(delta-1)*Size/2.5,0);
			glRotatef(-45,0,0,1);
			glScalef(1,delta,1);
        switch(type){
            case 1:
                Sup.draw_point();
                break;
            case 2:
                Sup.draw_line();
                break;
            case 3:
                Sup.draw_fill();
                break;
            case 4:
                Sup.draw_chess();
                break;
            case 5:
                Sup.draw_light_flat(mat);
                break;
            case 6:
                Sup.draw_light_smooth(mat);
                break;
            case 7:
                Sup.draw_fill(&index);
                break;

        }
		glLoadIdentity();
		glRotatef(alpha,0,0,1);
		glTranslatef(-(delta-1)*Size/2.5,-(delta-1)*Size/2.5,0);
		glRotatef(-45,0,0,1);
		glScalef(1,delta,1);
		glGetFloatv(GL_MODELVIEW_MATRIX,MV[0]);
        glPopMatrix();

        glPushMatrix();
        glTranslatef(-(delta-1)*Size*cos(45*M_PI/180),-(delta-1)*Size*1.25,0);
		glTranslatef(0,-Size,0);
        glScalef(1,delta,1);
        switch(type){
            case 1:
                Inf.draw_point();
                break;
            case 2:
                Inf.draw_line();
                break;
            case 3:
                Inf.draw_fill();
                break;
            case 4:
                Inf.draw_chess();
                break;
            case 5:
                Inf.draw_light_flat(mat);
                break;
            case 6:
                Inf.draw_light_smooth(mat);
                break;
            case 7:
                Inf.draw_fill(&index);
                break;

        }
		glLoadIdentity();
		glRotatef(alpha,0,0,1);
		glTranslatef(-(delta-1)*Size*cos(45*M_PI/180),-(delta-1)*Size*1.25,0);
		glTranslatef(0,-Size,0);
		glScalef(1,delta,1);
		glGetFloatv(GL_MODELVIEW_MATRIX,MV[1]);
		glPopMatrix();

    glPopMatrix();
}

bool _tooth::move_lower(float step){
    if(alpha+step>=-50 and alpha+step<=12){
        alpha += step;
        return true;
    }
    return false;
}

bool _tooth::scale_lower(float step){
    if(delta+step/10.0>1 and delta+step/10.0<=3){
        delta += step/10.0;
        return true;
    }
    return false;
}

void _tooth::draw_point(){
    this->draw_something(1);
}

void _tooth::draw_line(){
    this->draw_something(2);
}

void _tooth::draw_fill(){
    this->draw_something(3);
}

void _tooth::draw_fill(int* index){
	this->draw_something(7,-1,*index);
	*index += this->Tend;
}

void _tooth::draw_chess(){
    this->draw_something(4);
}
void _tooth::draw_light_flat(unsigned int mat){
    this->draw_something(5,mat);
}
void _tooth::draw_light_smooth(unsigned int mat){
    this->draw_something(6,mat);
}
void _tooth::change_light1(unsigned int val){
    Sup.change_light1(val) ;
    Inf.change_light1(val) ;
}
void _tooth::change_light2(unsigned int val){
    Sup.change_light2(val) ;
    Inf.change_light2(val) ;
}
void _tooth::move_light(int i, int j){
    Sup.move_light(i,j) ;
    Inf.move_light(i,j) ;
}
