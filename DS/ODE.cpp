#include "stdafx.h"

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Collections::Generic;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;

public value class PointD {
  public:
	Double X; Double Y; //System::Double
};

public ref class RightPart abstract {
	  public :
		virtual double f(double t, array<Double>^ x) = 0;
};

//правая часть 1 уравнения
public ref class RP1 : public RightPart {
	public :
		virtual double f(double t, array<Double>^ x) override {
			return x[1];
		}
};

//правая часть 2 уравнения
public ref class RP2 : public RightPart {
   private :
	  double a;
	  double b;
   public :
	  RP2(double a1, double b1) {
		a = a1;
		b = b1;
	  }

	  virtual double f(double t, array<Double>^ x) override {
			return -sin(x[0]);
	  }
};

public ref class ODE abstract {
   protected:
	   int n;
	   double koef;
	   int iterCount;
	   double step;
	   array<Double>^ x0;
	   List<array<array<Double>^>^>^ traces;
	   array<RightPart^>^ rps;
   public:
	   virtual Bitmap^ updateBitmap() = 0;

	   //метод Рунге-Кутте 4 порядка
	   array<array<Double>^>^ rk4(){
		   double t, tHalf;
		   koef = 1.0/6;
		
		   array<array<Double>^>^ x = gcnew array<array<Double>^>(iterCount);

		   for(int i = 0; i < iterCount; i++)
			   x[i] = gcnew array<Double>(n);

		for(int i = 0; i < n; i++)
			x[0][i] = x0[i];

		array<array<Double>^>^ k = gcnew array<array<Double>^>(n);
		for(int i = 0; i < n; i++)
			k[i] = gcnew array<Double>(4);//коэффициенты

		array<Double>^ args =  gcnew array<Double>(n);
		
		for(int i = 0; i < iterCount-1; i++) {
            t = step*i;
			tHalf = t + step/2;
            for(int j = 0; j < n; j++) {
                k[j][0] = step * rps[j]->f(t, x[i]);
                args[j] = x[i][j] + k[j][0]/2;
            }
            
            for(int j = 0; j < n; j++) {
                k[j][1] = step * rps[j]->f(tHalf, args);
            }
            
            for(int j = 0; j < n; j++) {
                 args[j] = x[i][j] + k[j][1]/2;
            }
             
            for(int j = 0; j < n; j++) {
                k[j][2] = step * rps[j]->f(tHalf, args);
            }
            
            for(int j = 0; j < n; j++) {
                 args[j] = x[i][j] + k[j][2];
            }
            
            for(int j = 0; j < n; j++) {
                k[j][3] = step * rps[j]->f(t, args);
            }
            
            for(int j = 0; j < n; j++) {
                x[i+1][j] = x[i][j] + koef *(k[j][0] + 2*k[j][1] + 2*k[j][2] + k[j][3]);
           }
        }
		traces->Add(x);
		
		return x;
	}
	
	virtual void setX0(... array<Double>^ arr) {
		int len = arr->GetLength(0); //длина переданного массива (для моего 2)
		for(int i = 0; i < len; i++)
			x0[i] = arr[i];
	}
};

public ref class CoordChanger {
	private:
    //края изображения
    double Xmin; 
	double Xmax; 
	double Ymin; 
	double Ymax;
    //центр изображения
	double Xcentre; 
	double Ycentre;
	//расстояние от центра (масштаб)
	double dist;
	//ширина и высота экранной системы
	double wS;
	double hS;
  public:
	  CoordChanger(double w, double h) {
		Xmin = -2; Xmax = 2; 
		Ymin = -2; Ymax = 2;
		Xcentre = Ycentre = 0.0;
		dist = 2.0;

		wS = w;
		hS = h;
	 }

	System::Void ToScreen(PointD W, Point &S) { 
	  S.X = wS*(W.X  - Xmin)/(Xmax-Xmin);
      S.Y = hS*(Ymax - W.Y)/(Ymax-Ymin);
    }

    System::Void ToWorld(Point S, PointD &W) {
	  W.X = S.X*(Xmax-Xmin)/wS + Xmin;
	  W.Y = S.Y*(Ymin-Ymax)/hS + Ymax;
    }
    
	bool isInner(PointD &W) {
		return (W.X >= Xmin) && (W.X < Xmax) &&
			((W.Y >= Ymin) && (W.Y < Ymax));
	}

	System::Void setArea() {
			 Xmin = Xcentre-dist;
			 Xmax = Xcentre+dist;
			 Ymin = Ycentre-dist;
			 Ymax = Ycentre+dist;
	}

	void setXcentre(double xc){
		Xcentre = xc;
		setArea();
	}

	void setYcentre(double yc){
		Ycentre = yc;
		setArea();
	}

	void setDist(double d){
		dist = d;
		setArea();
	}
};

public ref class ODE2D : public ODE {
  public:
   ODE2D(double w, double h) {
	  n = 2; //так как двумерная
	  traces = gcnew List<array<array<Double>^>^>();
	  rps = gcnew array<RightPart^>(n); //правая часть системы
	  x0 = gcnew array<Double>(n); //начальная точка
	  iterCount = 1000;
	  step = 0.01;

	  cc = gcnew CoordChanger(w, h);
	  bmp = gcnew Bitmap(w, h);
	  fill(Color::White);
  }


   void setDist(double d) {
	   cc->setDist(d);

	   fill(Color::White);
   }

  void fill(Color c) {
	  for(int i = 0; i < bmp->Width; i++)
		  for(int j = 0; j < bmp->Height; j++)
			 bmp->SetPixel(i, j, c);
  }
  //построение траектории
  virtual Bitmap^ updateBitmap() override {
     Point S;
	 PointD W;
	 
	 List<array<array<Double>^>^>::Enumerator e = traces->GetEnumerator();
	 array<array<Double>^>^ x;

	 while(e.MoveNext()){
		 x = e.Current;
		 for(int i = 0; i < x->Length; i++){
		   W.X = x[i][0];
		   W.Y = x[i][1];
           cc->ToScreen(W, S);
		   if(cc->isInner(W))
			 bmp->SetPixel(S.X, S.Y, Color::Black);
		}
	 }

	 return bmp;
   }
 
   virtual void setX0(... array<Double>^ arr) override {
	  Point S((int)arr[0], (int)arr[1]); //экранные координаты
	  PointD W; //мировые координаты

	  cc->ToWorld(S, W);
	  
	  x0[0] = W.X;
	  x0[1] = W.Y;
   }
   Bitmap^ getBmp() { return bmp; }
 private:
	CoordChanger ^cc;
    Bitmap ^bmp;
};

//инициализация правой части системы
public ref class VanDerPolSpec : public ODE2D {
  public: 
	VanDerPolSpec(double w, double h, double a, double b) : ODE2D(w, h) {
	  rps[0] = gcnew RP1();
	  rps[1] = gcnew RP2(a, b);
	}			
};