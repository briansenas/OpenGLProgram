#include "operate.h"
/*****************************************************************************//**
 *
 *
 *
 *****************************************************************************/

_vertex3f _operate::normalize(_vertex3f v){
    float length = sqrt(v._0*v._0 + v._1*v._1 + v._2*v._2);
    return _vertex3f(v._0/length,v._1/length,v._2/length);
}

/*****************************************************************************//**
 *
 *
 * *****************************************************************************/
_vertex3f _operate::cross_product(_vertex3f a, _vertex3f b){
    return _vertex3f(a._1*b._2-a._2*b._1,-(a._0*b._2-a._2*b._0),
            a._0*b._1-a._1*b._0);
}

/*
 * Lo que hacemos en esta función es específicamente generar las tapas de los
 * objetos en caso de que sea necesario.
 */
void _operate::closure(vector<_vertex3f>& temp, vector<_vertex3ui>& T1, vector<_vertex3f> axis,unsigned int N,
                                     unsigned int t_sz,bool slope, bool upper, bool lower){
    if(axis.size()>=2){
        bool axslope = ((axis[0]._0<axis[1]._0)or
                (axis[0]._1<axis[1]._1)or
                (axis[0]._2<axis[1]._2));
        if((slope and axslope)or
           (!slope and !axslope)){
            temp.push_back(axis[0]);
            temp.push_back(axis[1]);
        }else if((slope and !axslope)or
                 (!slope and axslope)){
            temp.push_back(axis[1]);
            temp.push_back(axis[0]);
        }
        for(unsigned int j=0;j<2;j++){
            if(j==0)  {
               for(unsigned int i=0;i<N-1;i++)
                   T1.push_back(_vertex3ui(temp.size()-2,(i+1)*t_sz,i*t_sz));
               T1.push_back(_vertex3ui(temp.size()-2,0,(N-1)*t_sz));
            }else{
                for(unsigned int i=0;i<N-1;i++)
                    T1.push_back(_vertex3ui(temp.size()-1,temp.size()-3-(i+1)*t_sz,temp.size()-3-i*t_sz));
                T1.push_back(_vertex3ui(temp.size()-1,temp.size()-3,temp.size()-3-(N-1)*t_sz));
            }
        }
    }
    else if(axis.size()==1 and lower){
        temp.push_back(axis[0]);
        for(unsigned int i=0;i<N-1;i++)
            T1.push_back(_vertex3ui(temp.size()-1,(i+1)*t_sz,i*t_sz));
        T1.push_back(_vertex3ui(temp.size()-1,0,(N-1)*t_sz));
    }
    else if(axis.size()==1 and upper){
        temp.push_back(axis[0]);
        for(unsigned int i=0;i<N-1;i++)
            T1.push_back(_vertex3ui(temp.size()-1,temp.size()-2-(i+1)*t_sz,temp.size()-2-i*t_sz));
        T1.push_back(_vertex3ui(temp.size()-1,temp.size()-2,temp.size()-2-(N-1)*t_sz));
    }
    else
        cout << "[WARNING] Empty axis " << endl;

}

/*
 * Todas las funciones son similares, básicamente lo que hacemos es verificar
 * si el objeto tiene tapa; En caso de tenerla tenemos que retirar y luego volver
 * a insertar-la una vez rotado el objeto. Para así evitar la repetición de puntos
 * y la creación de triángulos degenerados.
 * La única cosa es que los booleanos upper-lower determinan si forzamos una tapa
 * en caso de que falte o incluso quitarlas si el usuario lo desea con un valor falso.
 * En caso de que no hay tapa, pondremos aquella con mayor valor para la superior
 * y con menor valor para la inferior (No podemos elegir un sitio).
 */
void _operate::_rotateY(vector<_vertex3f>& P1,vector<_vertex3ui>& T1, unsigned int N, unsigned int angle, bool upper,bool lower){
    float rot = angle/((float)N)* M_PI/180;
    N = (angle < 360) ? N+1 : N;
    float value;
    vector<_vertex3f> temp;
    vector<_vertex3f> axis;;
    bool slope = false;
    bool diff = false;
    /******************************** Forzar/Buscar Tapa ***************************************/
    for(unsigned int i=0;i<P1.size();i++){
        if((abs(P1[i]._0)<1e-7) and (abs(P1[i]._2))<1e-7){
            axis.push_back(_vertex3f(P1[i]._0,P1[i]._1,P1[i]._2));
            continue;
        }
        if(i<P1.size()-1 and !slope){
            if(P1[i]._1 < P1[i+1]._1){
                slope = true;
                diff = true;
            }
            else if(P1[i]._1 > P1[i+1]._1){
                slope = false;
                diff = true;
            }
        }
        temp.push_back(_vertex3f(P1[i]._0,P1[i]._1,P1[i]._2));
    }
    /*************************************** Empezar a Rotar *****************************************/
	if(!temp.empty()){
		unsigned int t_sz = temp.size();
		for(unsigned int i=1;i<N; i++){
			value = rot * i;
			for(unsigned int j=0;j<t_sz;j++){
				temp.push_back(_vertex3f(temp[j]._0*cos(value)-temp[j]._2*sin(value),
							temp[j]._1,-temp[j]._0* sin(value)+temp[j]._2*cos(value)));
			}
		}

		T1 = _triangles(temp,N);

		//Determinar cuando forzar una tapa, o permitir la que viene definida
		if(axis.size()<2 and diff){
			if(!axis.empty()){
				if(lower){
					if(axis.back()._1 > temp[0]._1)
						axis.push_back(_vertex3f(0,temp[0]._1,0));
				}
				if(upper){
					if(axis.back()._1 < temp[temp.size()-1]._1)
						axis.push_back(_vertex3f(0,temp[temp.size()-1]._1,0));
				}
			}
			else{
				if(lower)
					axis.push_back(_vertex3f(0,temp[0]._1,0));
				if(upper)
					axis.push_back(_vertex3f(0,temp[temp.size()-1]._1,0));
			}
			if(!slope and axis.size()==2)
				std::swap(axis[0],axis[1]);
		}
		if(axis.size()==2 and (!lower or !upper)){
			if((!lower and axis[0]._1<axis[1]._1) or
			   (!upper and axis[1]._1<axis[0]._1)){
				axis[0] = axis[1];
				axis.pop_back();
			}else if(
			   (!lower and axis[1]._1<axis[0]._1) or
			   (!upper and axis[0]._1<axis[1]._1)){
				axis[1] = axis[0];
				axis.pop_back();
			}

		}

		if(diff)
			_operate::closure(temp,T1,axis,N,t_sz,slope,upper,lower);
		P1 = temp;
	}
}

void _operate::_rotateX(vector<_vertex3f>& P1,vector<_vertex3ui>& T1, unsigned int N, unsigned int angle, bool upper, bool lower){
    float rot = angle/((float)N)* M_PI/180;
    N = (angle < 360) ? N+1 : N;
    float value;
    vector<_vertex3f> temp;
    vector<_vertex3f> axis;
    bool slope = false;
    bool diff = false;
    /******************************** Forzar/Buscar Tapa ***************************************/
    for(unsigned int i=0;i<P1.size();i++){
        if((abs(P1[i]._1)-0<1e-7) and (abs(P1[i]._2)-0)<1e-7){
            axis.push_back(_vertex3f(P1[i]._0,P1[i]._1,P1[i]._2));
            continue;
        }
        if(i<P1.size()-1 and !slope){
            if(P1[i]._0 < P1[i+1]._0){
                slope = true;
                diff = true;
            }
            else if(P1[i]._0 > P1[i+1]._0){
                slope = false;
                diff = true;
            }
        }
        temp.push_back(_vertex3f(P1[i]._0,P1[i]._1,P1[i]._2));
    }
    /*************************************** Empezar a Rotar *****************************************/
	if(!temp.empty()){
		unsigned int t_sz = temp.size();
		for(unsigned int i=1;i<N; i++){
			value = rot * i;
			for(unsigned int j=0;j<t_sz;j++){
				temp.push_back(_vertex3f(temp[j]._0,temp[j]._1*cos(value)-temp[j]._2*sin(value)
							,-temp[j]._1 * sin(value)+temp[j]._2*cos(value)));
			}
		}

		T1 = _triangles(temp,N);

		//Determinar cuando forzar una tapa, o permitir la que viene definida
		if(axis.size()<2 and diff){
			if(!axis.empty()){
				if(lower){
					if(axis.back()._0 > temp[0]._0)
						axis.push_back(_vertex3f(temp[0]._0,0,0));
				}
				if(upper){
					if(axis.back()._0 < temp[temp.size()-1]._0)
						axis.push_back(_vertex3f(temp[temp.size()-1]._0,0,0));
				}
			}
			else{
				if(lower)
					axis.push_back(_vertex3f(temp[0]._0,0,0));
				if(upper)
					axis.push_back(_vertex3f(temp[temp.size()-1]._0,0,0));
			}
			if(!slope and axis.size()==2)
				std::swap(axis[0],axis[1]);
		}
		if(axis.size()==2 and (!lower or !upper)){
			if((!lower and axis[0]._0<axis[1]._0) or
			   (!upper and axis[1]._0<axis[0]._0)){
				axis[0] = axis[1];
				axis.pop_back();
			}else if(
			   (!lower and axis[1]._0<axis[0]._0) or
			   (!upper and axis[0]._0<axis[1]._0)){
				axis[1] = axis[0];
				axis.pop_back();
			}

		}

		if(diff)
			_operate::closure(temp,T1,axis,N,t_sz,slope,upper,lower);
		P1 = temp;
	}
}

void _operate::_rotateZ(vector<_vertex3f>& P1,vector<_vertex3ui>& T1, unsigned int N, unsigned int angle, bool upper, bool lower){
    float rot = angle/((float)N)* M_PI/180.0;
    N = (angle < 360) ? N+1 : N;
    float value;
    vector<_vertex3f> temp;
    vector<_vertex3f> axis;
    bool slope = false;
    bool diff = false;
    /******************************** Forzar/Buscar Tapa ***************************************/
    for(unsigned int i=0;i<P1.size();i++){
        if((abs(P1[i]._0)<1e-7) and (abs(P1[i]._1)-0)<1e-7){
            axis.push_back(_vertex3f(P1[i]._0,P1[i]._1,P1[i]._2));
            continue;
        }
        if(i<P1.size()-1 and !slope){
            if(P1[i]._2 < P1[i+1]._2){
                slope = true;
                diff = true;
            }
            else if(P1[i]._2 > P1[i+1]._2){
                slope = false;
                diff = true;
            }
        }
        temp.push_back(_vertex3f(P1[i]._0,P1[i]._1,P1[i]._2));
    }
    /*************************************** Empezar a Rotar *****************************************/
	if(!temp.empty()){
		unsigned int t_sz = temp.size();
		for(unsigned int i=1;i<N; i++){
			value = rot * i;
			for(unsigned int j=0;j<t_sz;j++){
				temp.push_back(_vertex3f(temp[j]._0*cos(value)-temp[j]._1*sin(value),
							-temp[j]._0*sin(value)+temp[j]._1*cos(value),temp[j]._2));
			}
		}
		T1 = _triangles(temp,N);

		//Determinar cuando forzar una tapa, o permitir la que viene definida
		if(axis.size()<2 and diff){
			if(!axis.empty()){
				if(lower){
					if(axis.back()._2 > temp[0]._2)
						axis.push_back(_vertex3f(0,0,temp[0]._2));
				}
				if(upper){
					if(axis.back()._2 < temp[temp.size()-1]._2)
						axis.push_back(_vertex3f(0,0,temp[temp.size()-1]._2));
				}
			}
			else{
				if(lower)
					axis.push_back(_vertex3f(0,0,temp[0]._2));
				if(upper)
					axis.push_back(_vertex3f(0,0,temp[temp.size()-1]._2));
			}
			if(!slope and axis.size()==2)
				std::swap(axis[0],axis[1]);
		}
		if(axis.size()==2 and (!lower or !upper)){
			if((!lower and axis[0]._2<axis[1]._2) or
			   (!upper and axis[1]._2<axis[0]._2)){
				axis[0] = axis[1];
				axis.pop_back();
			}else if(
			   (!lower and axis[1]._2<axis[0]._2) or
			   (!upper and axis[0]._2<axis[1]._2)){
				axis[1] = axis[0];
				axis.pop_back();
			}

		}

		if(diff)
			_operate::closure(temp,T1,axis,N,t_sz,slope,upper,lower);
		P1 = temp;
	}
}

// I am not sure what my intention was with this unused function
inline bool oppositeSigns(float x, float y){
    if(!((abs(x)-0<1e-9) and (abs(y)-0<1e-9)))
        return ((x*y)<0.0f);
    else return false;
}

/*
 * Esta es la función generatriz de triangulos que utiliza una sucesión
 * geo-aritmética para unir los puntos de cada triangulo; Hay que tener
 * en cuenta el número de puntos iniciales y el número de veces que se rota
 * el objeto.
 */

vector<_vertex3ui>_operate::_triangles(vector<_vertex3f> P1, unsigned int N){
    vector<_vertex3ui> temp;
    unsigned int N_cols = (P1.size())/N;
    unsigned int x,y,z;
    for(unsigned int i=0;i<N-1;i++){
        for(unsigned int j=0;j<N_cols-1;j++){
            x =     N_cols*i+1+j;
            y =     N_cols*i+0+j;
            z =     N_cols*(i+1)+0+j;
            temp.push_back(_vertex3ui(x,y,z));
            x =     N_cols*(i+1)+1+j;
            y =     N_cols*i+1+j;
            z =     N_cols*(i+1)+0+j;
            temp.push_back(_vertex3ui(x,y,z));
        }
    }
    for(unsigned int i=0;i<N_cols-1;i++){
        y = (P1.size()-N_cols+i );
        x = (P1.size()-N_cols+1+i);
        z = (0+i);
        temp.push_back(_vertex3ui(x,y,z));
        x = (1+i);
        y = (P1.size()-N_cols+1+i);
        z = (0+i);
        temp.push_back(_vertex3ui(x,y,z));
    }
    return temp;
}
