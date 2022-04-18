#include "excavator.h"

_excavator::_excavator(float Size,unsigned int N, float wheel_size,
    float width,float depth,float height){
    Hydraulic= _hydraulic();
    Chassis= _cube(Size);
    Cabine= _cube(Size);
    Attach= _cube(Size);
    FrontWheels=_wheels_axis(wheel_size,N,wheel_size,width);
    BackWheels=_wheels_axis(wheel_size,N,wheel_size,width);
    this->width = width;
    this->depth = depth;
    this->height = height;
	this->Tend = 1 + 1 + 1 + FrontWheels.Tend + 1 + BackWheels.Tend + 1 + Hydraulic.Tend + 1;
}

void _excavator::setSelected(int i, int j){
	int c = -1;
	if(i>=Tvalue and i<=Tvalue+3)
		c = i;
	else if( i>Tvalue+3 and i<=FrontWheels.getT())
		c = 3;
	else if(i>FrontWheels.getT() and i<=BackWheels.getT())
		c = 4;
	else if(i>BackWheels.getT())
		c = 5;
	switch(c){
		case 0:
			Chassis.setSelected(j);
			break;
		case 1:
			Cabine.setSelected(j);
			break;
		case 2:
			Attach.setSelected(j);
			break;
		case 3:
			FrontWheels.setSelected(i,j);
			break;
		case 4:
			BackWheels.setSelected(i,j);
			break;
		case 5:
			Hydraulic.setSelected(i,j);
			break;
		default:
			Cabine.setSelected(-1);
			Chassis.setSelected(-1);
			Attach.setSelected(-1);
			FrontWheels.setSelected(-1,-1);
			BackWheels.setSelected(-1,-1);
			Hydraulic.setSelected(-1,-1);
			break;
	}
}

_vertex3f _excavator::getSelected(int i, int j){
	_vertex3f res,base;
	int c = -1;
	if(i>=Tvalue and i<=Tvalue+3)
		c = i;
	else if(i>Tvalue+3 and i<=FrontWheels.getT())
		c = 3;
	else if(i>FrontWheels.getT() and i<=BackWheels.getT())
		c = 4;
	else if(i>BackWheels.getT())
		c = 5;
	switch(c){
		case 0:
			base = Chassis.getSelected(j);
			res._0 = MV[0][0]*base._0 + MV[0][4]*base._1 + MV[0][8]*base._2 + MV[0][12];
			res._1 = MV[0][1]*base._0 + MV[0][5]*base._1 + MV[0][9]*base._2 + MV[0][13];
			res._2 = MV[0][2]*base._0 + MV[0][6]*base._1 + MV[0][10]*base._2 +MV[0][14];
			break;
		case 1:
			base =  Cabine.getSelected(j);
			res._0 = MV[1][0]*base._0 + MV[1][4]*base._1 + MV[1][8]*base._2 + MV[1][12];
			res._1 = MV[1][1]*base._0 + MV[1][5]*base._1 + MV[1][9]*base._2 + MV[1][13];
			res._2 = MV[1][2]*base._0 + MV[1][6]*base._1 + MV[1][10]*base._2 +MV[1][14];
			break;
		case 2:
			base =  Attach.getSelected(j);
			res._0 = MV[2][0]*base._0 + MV[2][4]*base._1 + MV[2][8]*base._2 + MV[2][12];
			res._1 = MV[2][1]*base._0 + MV[2][5]*base._1 + MV[2][9]*base._2 + MV[2][13];
			res._2 = MV[2][2]*base._0 + MV[2][6]*base._1 + MV[2][10]*base._2 +MV[2][14];
			break;
		case 3:
			base =  FrontWheels.getSelected(i,j);
			res._0 = MV[3][0]*base._0 + MV[3][4]*base._1 + MV[3][8]*base._2 + MV[3][12];
			res._1 = MV[3][1]*base._0 + MV[3][5]*base._1 + MV[3][9]*base._2 + MV[3][13];
			res._2 = MV[3][2]*base._0 + MV[3][6]*base._1 + MV[3][10]*base._2 +MV[3][14];
			break;
		case 4:
			base =  BackWheels.getSelected(i,j);
			res._0 = MV[4][0]*base._0 + MV[4][4]*base._1 + MV[4][8]*base._2 + MV[4][12];
			res._1 = MV[4][1]*base._0 + MV[4][5]*base._1 + MV[4][9]*base._2 + MV[4][13];
			res._2 = MV[4][2]*base._0 + MV[4][6]*base._1 + MV[4][10]*base._2 +MV[4][14];
			break;
		case 5:
			base =  Hydraulic.getSelected(i,j);
			res._0 = MV[5][0]*base._0 + MV[5][4]*base._1 + MV[5][8]*base._2 + MV[5][12];
			res._1 = MV[5][1]*base._0 + MV[5][5]*base._1 + MV[5][9]*base._2 + MV[5][13];
			res._2 = MV[5][2]*base._0 + MV[5][6]*base._1 + MV[5][10]*base._2 +MV[5][14];
			break;
		default:break;
	}
	return res;
}

void _excavator::draw_something(unsigned int type,unsigned int mat, int index){
	if(index>=0)
		this->Tvalue = index;
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glTranslatef(0.6*width/2.0,0,0);
    //##################### CHASSIS  ###############################
    glPushMatrix();
        glPushMatrix();
            glTranslatef(-0.01*width,height/2.0,0);
            glScalef(0.95*width,height,depth);
            switch(type){
                case 1:
                    Chassis.draw_point();
                    break;
                case 2:
                    Chassis.draw_line();
                    break;
                case 3:
                    Chassis.draw_fill();
                    break;
                case 4:
                    Chassis.draw_chess();
                    break;
                case 5:
                    Chassis.draw_light_flat(mat);
                    break;
                case 6:
                    Chassis.draw_light_smooth(mat);
                    break;
                case 7:
                    Chassis.draw_fill(&index);
                    break;

            }
			glPushMatrix();
			glLoadIdentity();
			glTranslatef(0.6*width/2.0,0,0);
            glTranslatef(-0.01*width,height/2.0,0);
            glScalef(0.95*width,height,depth);
			glGetFloatv(GL_MODELVIEW_MATRIX,MV[0]);
			glPopMatrix();
        glPopMatrix();

        glPushMatrix();
            glTranslatef(0.2*width,height*1.5,0);
            glScalef(0.5*width,1*height,0.4*depth);
            switch(type){
                case 1:
                    Cabine.draw_point();
                    break;
                case 2:
                    Cabine.draw_line();
                    break;
                case 3:
                    Cabine.draw_fill();
                    break;
                case 4:
                    Cabine.draw_chess();
                    break;
                case 5:
                    Cabine.draw_light_flat(mat);
                    break;
                case 6:
                    Cabine.draw_light_smooth(mat);
                    break;
                case 7:
                    Cabine.draw_fill(&index);
                    break;

            }
			glPushMatrix();
			glLoadIdentity();
			glTranslatef(0.6*width/2.0,0,0);
            glTranslatef(0.2*width,height*1.5,0);
            glScalef(0.5*width,1*height,0.4*depth);
			glGetFloatv(GL_MODELVIEW_MATRIX,MV[1]);
			glPopMatrix();
        glPopMatrix();
    glPopMatrix();
    //##################### CHASSIS  ###############################
    glPushMatrix();
        glPushMatrix();
            glTranslatef(-0.3*width, height*3/2.0+height,0);
            glScalef(0.2*width,height*3,0.2*depth);
            switch(type){
                case 1:
                    Attach.draw_point();
                    break;
                case 2:
                    Attach.draw_line();
                    break;
                case 3:
                    Attach.draw_fill();
                    break;
                case 4:
                    Attach.draw_chess();
                    break;
                case 5:
                    Attach.draw_light_flat(mat);
                    break;
                case 6:
                    Attach.draw_light_smooth(mat);
                    break;
                case 7:
                    Attach.draw_fill(&index);
                    break;

            }
			glPushMatrix();
			glLoadIdentity();
			glTranslatef(0.6*width/2.0,0,0);
            glTranslatef(-0.3*width, height*3/2.0+height,0);
            glScalef(0.2*width,height*3,0.2*depth);
			glGetFloatv(GL_MODELVIEW_MATRIX,MV[2]);
			glPopMatrix();
        glPopMatrix();
    glPopMatrix();
 //##################### WHEELSAXIS ###############################
        glPushMatrix();
            glPushMatrix();
            glTranslatef(0,0,0.3*(depth));
            switch(type){
                case 1:
                    FrontWheels.draw_point();
                    break;
                case 2:
                    FrontWheels.draw_line();
                    break;
                case 3:
                    FrontWheels.draw_fill();
                    break;
                case 4:
                    FrontWheels.draw_chess();
                    break;
                case 5:
                    FrontWheels.draw_light_flat(mat);
                    break;
                case 6:
                    FrontWheels.draw_light_smooth(mat);
                    break;
                case 7:
                    FrontWheels.draw_fill(&index);
                    break;

            }
			glPushMatrix();
			glLoadIdentity();
			glTranslatef(0.6*width/2.0,0,0);
            glTranslatef(0,0,0.3*(depth));
			glGetFloatv(GL_MODELVIEW_MATRIX,MV[3]);
			glPopMatrix();

            glPopMatrix();


            glPushMatrix();
            glTranslatef(0,0,-(depth*0.3));
            switch(type){
                case 1:
                    BackWheels.draw_point();
                    break;
                case 2:
                    BackWheels.draw_line();
                    break;
                case 3:
                    BackWheels.draw_fill();
                    break;
                case 4:
                    BackWheels.draw_chess();
                    break;
                case 5:
                    BackWheels.draw_light_flat(mat);
                    break;
                case 6:
                    BackWheels.draw_light_smooth(mat);
                    break;
                case 7:
                    BackWheels.draw_fill(&index);
                    break;
            }
			glPushMatrix();
			glLoadIdentity();
			glTranslatef(0.6*width/2.0,0,0);
            glTranslatef(0,0,-(depth*0.3));
			glGetFloatv(GL_MODELVIEW_MATRIX,MV[4]);
			glPopMatrix();

            glPopMatrix();

        glPopMatrix();
        //##################### WHEELSAXIS ###############################
    glPopMatrix();

    glPopMatrix();
        glPushMatrix();
        glRotatef(alpha,0,1,0);
        glTranslatef(0,4*height,0);
        glScalef(0.2*width,0.5*height,0.2*depth);
        switch(type){
                case 1:
                    Hydraulic.draw_point();
                    break;
                case 2:
                    Hydraulic.draw_line();
                    break;
                case 3:
                    Hydraulic.draw_fill();
                    break;
                case 4:
                    Hydraulic.draw_chess();
                    break;
                case 5:
                   Hydraulic.draw_light_flat(mat);
                    break;
                case 6:
                   Hydraulic.draw_light_smooth(mat);
                    break;
                case 7:
                   Hydraulic.draw_fill(&index);
                    break;

            }
			glPushMatrix();
			glLoadIdentity();
			glRotatef(alpha,0,1,0);
			glTranslatef(0,4*height,0);
			glScalef(0.2*width,0.5*height,0.2*depth);
			glGetFloatv(GL_MODELVIEW_MATRIX,MV[5]);
			glPopMatrix();
        glPopMatrix();
    //##################### HYDRAULIC  ###############################
   }

void _excavator::Forward(float step){
    FrontWheels.rotate(step);
    BackWheels.rotate(step);
}

void _excavator::Spin(float step){
    alpha = (alpha + step);
}

bool _excavator::Lift(float step){
    bool moved = Hydraulic.lift_claw(step);
    if(moved)
        delta += step;
    return moved;
}

bool _excavator::expand_arm(float step){
    return Hydraulic.expand_arm(step);
}

bool _excavator::Expand(float step){
    return Hydraulic.expand(step);
}

bool _excavator::Catch(float step){
    return Hydraulic.move_claw(step);
}

bool _excavator::CatchSize(float step){
    return Hydraulic.expand_claw(step);
}

void _excavator::draw_point(){
    this->draw_something(1);
}
void _excavator::draw_line(){
    this->draw_something(2);
}
void _excavator::draw_fill(){
    this->draw_something(3);
}
void _excavator::draw_fill(int* index){
    this->draw_something(7,-1,*index);
	*index += this->Tend;
}
void _excavator::draw_chess(){
    this->draw_something(4);
}
void _excavator::draw_light_flat(unsigned int mat){
    this->draw_something(5,mat);
}
void _excavator::draw_light_smooth(unsigned int mat){
    this->draw_something(6,mat);
}
void _excavator::change_light1(unsigned int val){
    Hydraulic.change_light1(val);
    Chassis.change_light1(val);
    Cabine.change_light1(val);
    Attach.change_light1(val);
    FrontWheels.change_light1(val);
    BackWheels.change_light1(val);
}

void _excavator::change_light2(unsigned int val){
    Hydraulic.change_light2(val);
    Chassis.change_light2(val);
    Cabine.change_light2(val);
    Attach.change_light2(val);
    FrontWheels.change_light2(val);
    BackWheels.change_light2(val);
}
void _excavator::move_light(int i, int j){
    Hydraulic.move_light(i,j);
    Chassis.move_light(i,j);
    Cabine.move_light(i,j);
    Attach.move_light(i,j);
    FrontWheels.move_light(i,j);
    BackWheels.move_light(i,j);
}

