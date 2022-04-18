#include "hydraulic.h"

_hydraulic::_hydraulic(float Size,float LSize, float USize, float TSize){
	myarm =  _forearm(Size,USize,TSize);
    center =  _cube(Size);
	this->Size = Size;
	this->LSize = LSize;
	this->USize = USize;
	this->TSize = TSize;
	this->Tend = myarm.Tend + 1 + 1;
}

void _hydraulic::setSelected(int i, int j){
	int c = -1;
	if(i>=Tvalue and i<=myarm.getT())
		c = 0;
	else if(i>myarm.getT())
		c = 1;
	switch(c){
		case 0:
			myarm.setSelected(i,j);
			break;
		case 1:
			center.setSelected(j);
			break;
		default:
			myarm.setSelected(-1,-1);
			center.setSelected(-1);
			break;
	}
}

_vertex3f _hydraulic::getSelected(int i, int j){
	_vertex3f res,base;
	int c = -1;
	if( i>=0 and i<=myarm.getT())
		c = 0;
	else if(i>myarm.getT())
		c = 1;
	switch(c){
		case 0:
			base = myarm.getSelected(i,j);
			res._0 = MV[0][0]*base._0 + MV[0][4]*base._1 + MV[0][8]*base._2 + MV[0][12];
			res._1 = MV[0][1]*base._0 + MV[0][5]*base._1 + MV[0][9]*base._2 + MV[0][13];
			res._2 = MV[0][2]*base._0 + MV[0][6]*base._1 + MV[0][10]*base._2 +MV[0][14];
			break;
		case 1:
			base = center.getSelected(j);
			res._0 = MV[1][0]*base._0 + MV[1][4]*base._1 + MV[1][8]*base._2 + MV[1][12];
			res._1 = MV[1][1]*base._0 + MV[1][5]*base._1 + MV[1][9]*base._2 + MV[1][13];
			res._2 = MV[1][2]*base._0 + MV[1][6]*base._1 + MV[1][10]*base._2 +MV[1][14];
			break;
		default:break;
	}
	return res;
}

void _hydraulic::draw_something(unsigned int type,unsigned int mat, int index){
	if(index>=0)
		this->Tvalue = index;
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
        glTranslatef((-LSize-Size/2.0-delta*Size)*cos(alpha*M_PI/180.0),(LSize+delta*Size)*sin(alpha*M_PI/180.0),0);
		glTranslatef(0,(-Size*USize+Size)/2.0,0);
		switch(type){
                case 1:
                    myarm.draw_point();
                    break;
                case 2:
                    myarm.draw_line(); break;
                case 3:
                    myarm.draw_fill();
                    break;
                case 4:
                    myarm.draw_chess();
                    break;
                case 5:
                    myarm.draw_light_flat(mat);
                    break;
                case 6:
                    myarm.draw_light_smooth(mat);
                    break;
                case 7:
                    myarm.draw_fill(&index);
                    break;
                }
		glLoadIdentity();
        glTranslatef((-LSize-0.5-delta*Size)*cos(alpha*M_PI/180.0),(LSize+delta*Size)*sin(alpha*M_PI/180.0),0);
		glTranslatef(0,(-Size*USize+Size)/2.0,0);
		glGetFloatv(GL_MODELVIEW_MATRIX,MV[0]);
    glPopMatrix();

    glPushMatrix();
        glPushMatrix();
			glRotatef(-alpha,0,0,1);
            glTranslatef(-LSize/2.0-(delta*Size)/2.0,0,0);
            glRotatef(90,0,0,1);
            glScalef(1,LSize/Size+delta,1);
            switch(type){
                case 1:
                    center.draw_point();
                    break;
                case 2:
                    center.draw_line();
                    break;
                case 3:
                    center.draw_fill();
                    break;
                case 4:
                    center.draw_chess();
                    break;
                case 5:
                    center.draw_light_flat(mat);
                    break;
                case 6:
                    center.draw_light_smooth(mat);
                    break;
                case 7:
                    center.draw_fill(&index);
                    break;

                }
			glLoadIdentity();
			glRotatef(-alpha,0,0,1);
            glTranslatef(-LSize/2.0-(delta*Size)/2.0,0,0);
            glRotatef(90,0,0,1);
            glScalef(1,LSize/Size+delta,1);
			glGetFloatv(GL_MODELVIEW_MATRIX,MV[1]);
        glPopMatrix();
    glPopMatrix();
}

bool _hydraulic::move_claw(float step){
    return this->myarm.move_claw(step);
}
bool _hydraulic::expand_claw(float step){
    return this->myarm.expand_claw(step);
}

bool _hydraulic::expand_arm(float step){
	return this->myarm.expand(step);
}

bool _hydraulic::lift_claw(float step){
    if(alpha+step>=0 and alpha+step<=30){
        alpha += step;
        return true;
    }
    else{
        return false;
    }
}

bool _hydraulic::expand(float step){
    if(delta+step/10>0 and delta+step/10<=5){
        delta+=step/10;
        return true;
    }
    else{
        //cout << "MAX/MIN EXPANSION" << endl;
        return false;
    }
}


void _hydraulic::draw_point(){
    this->draw_something(1);
}
void _hydraulic::draw_line(){
    this->draw_something(2);
}
void _hydraulic::draw_fill(){
    this->draw_something(3);
}
void _hydraulic::draw_fill(int* index){
    this->draw_something(7,-1,*index);
	*index += this->Tend;
}
void _hydraulic::draw_chess(){
    this->draw_something(4);
}
void _hydraulic::draw_light_flat(unsigned int mat){
    this->draw_something(5,mat);
}
void _hydraulic::draw_light_smooth(unsigned int mat){
    this->draw_something(6,mat);
}
void _hydraulic::change_light1(unsigned int val){
	myarm.change_light1(val);
	center.change_light1(val);
}

void _hydraulic::change_light2(unsigned int val){
	myarm.change_light2(val);
	center.change_light2(val);
}
void _hydraulic::move_light(int i, int j){
	myarm.move_light(i,j);
	center.move_light(i,j);
}
