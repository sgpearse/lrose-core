#ifndef SOLOFUNCTIONSMODEL_H
#define SOLOFUNCTIONSMODEL_H

#include <stdio.h>

#include <vector>
#include <iostream>

#include "Radx/RadxVol.hh"

using namespace std;


class SoloFunctionsModel
{


public:
  SoloFunctionsModel() {}

  vector<double> RemoveAircraftMotion(vector<double>, RadxVol *vol);
  vector<double> RemoveAircraftMotion(string fieldName, RadxVol *vol);
  vector<double> RemoveAircraftMotion(string fieldName, RadxVol *vol,
				      int rayIdx, int sweepIdx);


  float *ZeroMiddleThird(string fieldName,  RadxVol *vol,
			 int rayIdx, int sweepIdx,
			 string newFieldName);

  void SetBoundaryMask(RadxVol *vol, int rayIdx, int sweepIdx);
 
private:

  //  SpreadSheetModel *dataModel;
  short *_boundaryMask;

};


#endif
