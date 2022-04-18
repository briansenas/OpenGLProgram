#include "tablero.h"

_tablero::_tablero(unsigned int steps,float width, float height, float depth){
    this->width = width;
    this->height = height;
    this->depth = depth;
    this->steps = steps;

    Vertices.clear();

    bool reverse=false;
    for (unsigned int x = 0; x < 1; x++) {
        for(unsigned int i=0; i<=steps;i++){
            for(unsigned int j=0;j<=steps;j++){
                Vertices.push_back(_vertex3f(par_x((i*1.0)/steps,reverse),par_y((j*1.0)/steps),0));
            }
        }
        reverse = !reverse;
    }

    int x,y,z;
    int offset = steps+1;
        for (unsigned int i = 0; i < steps; i++) {
            for(unsigned int j=0;j < steps;j++){
                x = (offset*i+1+j) % (Vertices.size());
                y = (offset*i+j) % (Vertices.size());
                z = (offset*(i+1)+j) % (Vertices.size());
                Triangles.push_back(_vertex3ui(x,y,z));
                x = (offset*(i+1)+1+j) % (Vertices.size());
                y = (offset*i+1+j) % (Vertices.size());
                z = (offset*(i+1)+j) % (Vertices.size());
                Triangles.push_back(_vertex3ui(x,y,z));
            }
        }
}

void _tablero::draw_texture(){

    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    glEnable(GL_TEXTURE_2D);
    glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
    glBegin(GL_TRIANGLES);

    float coord = 0;
    float diff = 1.0/(steps*1.0);
    float last = diff;
    for(unsigned int i=0;i<Triangles.size();i+=2){
        glTexCoord2f(last-diff,coord+diff);
        glVertex3fv((const GLfloat *) &Vertices[Triangles[i]._0]);
        glTexCoord2f(last-diff,coord);
        glVertex3fv((const GLfloat *) &Vertices[Triangles[i]._1]);
        glTexCoord2f(last,coord);
        glVertex3fv((const GLfloat *) &Vertices[Triangles[i]._2]);
        coord += diff;
        if(coord==1){
            coord=0;
            last += diff;
        }

    }

    coord = diff;
    last = diff;
    for(unsigned int i=1;i<Triangles.size();i+=2){
        glTexCoord2f(last,coord);
        glVertex3fv((const GLfloat *) &Vertices[Triangles[i]._0]);
        glTexCoord2f(last-diff,coord);
        glVertex3fv((const GLfloat *) &Vertices[Triangles[i]._1]);
        glTexCoord2f(last,coord-diff);
        glVertex3fv((const GLfloat *) &Vertices[Triangles[i]._2]);

        coord += diff;
        if(coord>1){
            coord = diff;
            last += diff;
        }

    }
    glEnd();
    glDisable(GL_TEXTURE_2D);
}

void _tablero::enable_textured_light(unsigned int var){
		this->enable_lights(var);
        glShadeModel(GL_FLAT);
        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE); //Multiply texture colors with light values
        //DECAL only draws the texture, and REPLACE gives priority to the light

        glEnable(GL_TEXTURE_2D);
}

void _tablero::draw_texture_flat(unsigned int var){
    if(TNormals.empty())
        this->compute_normal_triangles();

    if(TNormals.empty())
        cout << "[ERROR]Empty Normal Vector...." << endl;
    else{

	   this->enable_textured_light(var);
       glBegin(GL_TRIANGLES);

        float coord = 0;
        float diff = 1.0/(steps*1.0);
        float last = diff;
        for(unsigned int i=0;i<Triangles.size();i+=2){
            glNormal3fv((const GLfloat *) &TNormals[i]); //Siempre realiza una interpolación aunque sea una sola norma
            glTexCoord2f(last-diff,coord+diff);
            glVertex3fv((const GLfloat *) &Vertices[Triangles[i]._0]);
            glTexCoord2f(last-diff,coord);
            glVertex3fv((const GLfloat *) &Vertices[Triangles[i]._1]);
            glTexCoord2f(last,coord);
            glVertex3fv((const GLfloat *) &Vertices[Triangles[i]._2]);
            coord += diff;
            if(coord==1){
                coord=0;
                last += diff;
            }

        }

        coord = diff;
        last = diff;
        for(unsigned int i=1;i<Triangles.size();i+=2){
            glNormal3fv((const GLfloat *) &TNormals[i]); //Siempre realiza una interpolación aunque sea una sola norma
            glTexCoord2f(last,coord);
            glVertex3fv((const GLfloat *) &Vertices[Triangles[i]._0]);
            glTexCoord2f(last-diff,coord);
            glVertex3fv((const GLfloat *) &Vertices[Triangles[i]._1]);
            glTexCoord2f(last,coord-diff);
            glVertex3fv((const GLfloat *) &Vertices[Triangles[i]._2]);

            coord += diff;
            if(coord>1){
                coord = diff;
                last += diff;
            }

        }
        glEnd();
		this->disable_textured_light();
    }

}
void _tablero::disable_textured_light(){
        glDisable(GL_TEXTURE_2D);
		this->disable_lights();
}

void _tablero::draw_texture_smooth(unsigned int mat){
    if(PNormals.empty())
        this->compute_normal_vertex();

    if(PNormals.empty())
        cout << "[ERROR]Empty Normal Vector...." << endl;
    else{

	   this->enable_textured_light(mat);
       glBegin(GL_TRIANGLES);

        float coord = 0;
        float diff = 1.0/(steps*1.0);
        float last = diff;
        for(unsigned int i=0;i<Triangles.size();i+=2){
            glNormal3fv((const GLfloat *) &PNormals[Triangles[i]._0]);
            glTexCoord2f(last-diff,coord+diff);
            glVertex3fv((const GLfloat *) &Vertices[Triangles[i]._0]);
            glNormal3fv((const GLfloat *) &PNormals[Triangles[i]._0]);
            glTexCoord2f(last-diff,coord);
            glVertex3fv((const GLfloat *) &Vertices[Triangles[i]._1]);
            glNormal3fv((const GLfloat *) &PNormals[Triangles[i]._0]);
            glTexCoord2f(last,coord);
            glVertex3fv((const GLfloat *) &Vertices[Triangles[i]._2]);
            coord += diff;
            if(coord==1){
                coord=0;
                last += diff;
            }

        }

        coord = diff;
        last = diff;
        for(unsigned int i=1;i<Triangles.size();i+=2){
            glNormal3fv((const GLfloat *) &PNormals[Triangles[i]._0]);
            glTexCoord2f(last,coord);
            glVertex3fv((const GLfloat *) &Vertices[Triangles[i]._0]);
            glNormal3fv((const GLfloat *) &PNormals[Triangles[i]._0]);
            glTexCoord2f(last-diff,coord);
            glVertex3fv((const GLfloat *) &Vertices[Triangles[i]._1]);
            glNormal3fv((const GLfloat *) &PNormals[Triangles[i]._0]);
            glTexCoord2f(last,coord-diff);
            glVertex3fv((const GLfloat *) &Vertices[Triangles[i]._2]);

            coord += diff;
            if(coord>1){
                coord = diff;
                last += diff;
            }

        }
        glEnd();
		this->disable_textured_light();
    }
}

float _tablero::par_x(float t, bool reverse){
    if(reverse)
        return (1.0-t)*(this->width/2.0)+(t)*(-this->width/2.0);
    return (1.0-t)*(-this->width/2.0)+(t)*(this->width/2.0);
}

float _tablero::par_y(float t, bool reverse){
    if(reverse)
        return (1.0-t)*(this->height/2.0)+(t)*(-this->height/2.0);
    return (1.0-t)*(-this->height/2.0)+(t)*(this->height/2.0);
}

float _tablero::par_z(float t, bool reverse){
    if(reverse)
        return (1.0-t)*(this->depth/2.0)+(t)*(-this->depth/2.0);
    return (1.0-t)*(-this->depth/2.0)+(t)*(this->depth/2.0);
}


