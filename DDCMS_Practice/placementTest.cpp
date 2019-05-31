#include<iostream>
#include <cmath>

int main(){

#define EDM_ML_DEBUG


  double waferSize_ = 167.4408;
  double waferSepar_ = 1.;
   int nCells_= 12;
  
  static const double sqrt3 = std::sqrt(3.0);
  double rM  = 0.5*(waferSize_ + waferSepar_);
  double RM2 = rM/sqrt3;
  double R   = waferSize_/(3.0*nCells_);
  double r   = 0.5*R*sqrt3;


 int counter = 0;
  for (int u=0; u<2*nCells_; ++u) {
    for (int v=0; v<2*nCells_; ++v) {
      if (((v-u) < nCells_) && (u-v) <= nCells_) {
      counter++;
	int    n2 = nCells_/2;
	double yp = (u-0.5*v-n2)*2*r;
	double xp = (1.5*(v-nCells_)+1.0)*R;
	int    cell(0);
	if      ((u == 0)           && (v == 0))           cell = 7;
	else if ((u == 0)           && (v == nCells_-1))   cell = 8;
	else if ((u == nCells_)     && (v == 2*nCells_-1)) cell = 9;
	else if ((u == 2*nCells_-1) && (v == 2*nCells_-1)) cell = 10;
	else if ((u == 2*nCells_-1) && (v == nCells_-1))   cell = 11;
	else if ((u == nCells_)     && (v == 0))           cell = 12;
	else if (u == 0)                                   cell = 1;
	else if ((v-u) == (nCells_-1))                     cell = 4;
	else if (v == (2*nCells_-1))                       cell = 2;
	else if (u == (2*nCells_-1))                       cell = 5;
	else if ((u-v) == nCells_)                         cell = 3;
	else if (v == 0)                                   cell = 6;
//	DDTranslation tran(xp,yp,0);
	//int copy = (cellType_*100 + v)*100 + u;
	//cpv.position(DDName(cellNames_[cell]), glog, copy, tran, rot);
#ifdef EDM_ML_DEBUG
	std::cout  <<" at : " << xp <<" : " << yp << std::endl;
#endif
      }
    }

  }
  std::cout << "Counter : " << counter << std::endl;
  
}
