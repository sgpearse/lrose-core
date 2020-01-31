
#include <vector>
#include <iostream>

#include <string>
#include <sstream>
#include <iterator>

#include <Radx/RadxRay.hh>
#include <toolsa/LogStream.hh>

#include "SoloFunctionsController.hh"
// #include "SpreadSheetModel.hh"

using namespace std;

SoloFunctionsController::SoloFunctionsController(RadxVol *data, QObject *parent) : QObject(parent) {
  _data = data;
  _nRays = _data->getNRays();
  _nSweeps = _data->getNSweeps();

}



template<typename Out>
void SoloFunctionsController::split(const string &s, char delim, Out result) {
  stringstream ss(s);
  string item;
  double value;
  while (getline(ss, item, delim)) {
    value = stod(item);
    *(result++) = value;
  }
}

/*
vector<string> SoloFunctions::split(const string &s, char delim) {
  vector<string> elems;
  split(s, delim, back_inserter(elems));
  return elems;
}
*/

vector<double> SoloFunctionsController::splitDouble(const string &s, char delim) {
  vector<double> elems;
  split(s, delim, back_inserter(elems));
  return elems;
}
/*
QVector<double> add(QVector<double> v, QVector<double> v2) {
  int size = v.size(); 
  if (size != v2.size()) {
    // TODO: throw exception
    // return the size of the shorter?
    QVector<double> v0(0);
    return v0;
  }
    //    QVector<double> v3 = v + v2; return v3; } // this appends one v2 elements to v                          
    QVector<double> v3(size);
    for (int i=0; i<size; i++) 
      v3[i]=v[i]+v2[i];
    return v3;
}
*/


// TODO:  parameters should be DataField ??
QString  SoloFunctionsController::REMOVE_AIRCRAFT_MOTION(QString field) { 

  //SoloFunctionsModel soloFunctionsModel;

  // the value of the field has been substituted; If the field is a vector,
  // then the QString contains all the values as a comma separated list in a string
  // parse the field data into a vector 
  //vector<string> x = split(field.toStdString(), ',');
  /*vector<double> x = splitDouble(field.toStdString(), ',');

  cerr << "list of field values: " << endl;
  for (vector<double>::iterator it = x.begin(); it != x.end(); ++it) 
    cerr << ' ' << *it;
  cerr << endl;
  */

  //  vector<double> result = soloFunctionsModel.RemoveAircraftMotion(field.toStdString(), _data);
  vector<double> result = soloFunctionsModel.RemoveAircraftMotion(field.toStdString(), _data,
								  _currentRayIdx, _currentSweepIdx);
 // ,
								  //								  _currentSweep, _currentRay);
  //  vector<double> result = soloFunctionsModel.RemoveAircraftMotion(x, dataModel);

  // TODO: what is being returned? the name of the new field in the model that
  // contains the results.
  // since the std::vector<double> data has to be copied to QVector anyway, 
  // go ahead and format it as a string?
  // maybe return a pointer to std::vector<double> ?? then when presenting the data, we can convert it to string,
  // but maintain the precision in the model (RadxVol)??
  //QString newFieldName = field + "2";
  // TODO: dataModel->addField(newFieldName.toStdString(), result);

  QString newData;
  cerr << "list of result values: " << endl;
  for (vector<double>::iterator it = result.begin(); it != result.end(); ++it) {
    cerr << ' ' << *it;
    newData.append(QString::number(*it));
    newData.append(',');
  }
  cerr << endl;

  return newData;
}

// return the name of the field in which the result is stored in the RadxVol
QString SoloFunctionsController::ZERO_MIDDLE_THIRD(QString field) { 

  //SoloFunctionsModel soloFunctionsModel;

  string tempFieldName = soloFunctionsModel.ZeroMiddleThird(field.toStdString(), _data,
						     _currentRayIdx, _currentSweepIdx,
						     field.toStdString()); // "VEL_xyz");

  // TODO: returns name of new field in RadxVol

  return QString::fromStdString(tempFieldName); // QString("zero middle result");
}


// return the name of the field in which the result is stored in the RadxVol
QString SoloFunctionsController::ZERO_INSIDE_BOUNDARY(QString field) { 

  //SoloFunctionsModel soloFunctionsModel;

  string tempFieldName = soloFunctionsModel.ZeroInsideBoundary(field.toStdString(), _data,
						     _currentRayIdx, _currentSweepIdx,
						     field.toStdString()); // "VEL_xyz");

  // TODO: returns name of new field in RadxVol

  return QString::fromStdString(tempFieldName); // QString("zero middle result");
}


// return the name of the field in which the result is stored in the RadxVol
QString SoloFunctionsController::DESPECKLE(QString field, size_t speckle_length, float bad_data,
					   size_t clip_gate) { 

  //SoloFunctionsModel soloFunctionsModel;

  string tempFieldName = soloFunctionsModel.Despeckle(field.toStdString(), _data,
						     _currentRayIdx, _currentSweepIdx,
						      speckle_length,
						      clip_gate,
						      bad_data,
						     field.toStdString());

  // TODO: returns name of new field in RadxVol

  return QString::fromStdString(tempFieldName); // QString("zero middle result");
}


void SoloFunctionsController::applyBoundary() {
  
  //SoloFunctionsModel soloFunctionsModel;
  soloFunctionsModel.SetBoundaryMask(_data, _currentRayIdx, _currentSweepIdx);
}

void SoloFunctionsController::setCurrentSweepToFirst() {
  cerr << "entry setCurrentSweepToFirst" << endl;
  _currentSweepIdx = 0;
  cerr << "exit setCurrentSweepToFirst" << endl;

  //LOG(DEBUG) << "exit";

}


void SoloFunctionsController::setCurrentRayToFirst() {
  //cerr << "entry setCurrentRayToFirst" << endl;
  _currentRayIdx = 0;
  //applyBoundary();
  //cerr << "exit setCurrentRayToFirst" << endl;

  //LOG(DEBUG) << "exit";

}

void SoloFunctionsController::nextRay() {
  //LOG(DEBUG) << "entry";
  //cerr << "entry nextRay" << endl;
  _currentRayIdx += 1;
  //  applyBoundary();
  //cerr << "exit nextRay" << endl;
  //LOG(DEBUG) << "exit";

}

bool SoloFunctionsController::moreRays() {
  //  LOG(DEBUG) << "entry";
  //cerr << "entry moreRays" << endl;
  //  vector<RadxRay *> rays = _data->getRays();
  //size_t 
  // THIS DOES NOT WORK; it changes memory outside of its bounds
  //  const size_t nRays = _data->getNRays();

  //LOG(DEBUG) << "There are " <<  nRays << " rays";;
  //if (_currentRayIdx >= nRays)
  //  _data->loadFieldsFromRays();
  //return (_currentRayIdx < _data->getNRays()); // nRays);
  return (_currentRayIdx < _nRays);
}

void SoloFunctionsController::nextSweep() {
  //LOG(DEBUG) << "entry";
  //cerr << "entry nextSweep" << endl;
  _currentSweepIdx += 1;
  //cerr << "exit nextSweep" << endl;
  //LOG(DEBUG) << "exit";
}

bool SoloFunctionsController::moreSweeps() {
  //  LOG(DEBUG) << "entry";
  //cerr << "entry moreSweeps" << endl;
  //  const size_t nSweeps = _data->getNSweeps();
  //LOG(DEBUG) << " there are " << nSweeps << " sweeps";;
  //if (_currentSweepIdx >= nSweeps)
  //  _data->loadFieldsFromRays();

  return (_currentSweepIdx < _nSweeps);
}


void SoloFunctionsController::assign(string tempName, string userDefinedName) {
  //_data->loadFieldsFromRays(); // TODO: this is a costly function as it moves the data/or pointers
  // TODO: where are the field names kept? in the table map? can i just change that?
  // Because each RadxRay holds its own FieldNameMap,
  // TODO: maybe ...
  vector<RadxRay *> rays = _data->getRays();
  // for each ray, 
  vector<RadxRay *>::iterator it;
  for (it=rays.begin(); it != rays.end(); ++it) {
     // renameField(oldName, newName);
    (*it)->renameField(tempName, userDefinedName);
    // loadFieldNameMap
    (*it)->loadFieldNameMap();

  }
  // end for each ray
  //
  /* 
  RadxField *theField = _data->getField(tempName);
  if (theField == NULL) throw "Error: no field " + tempName + " found for " + userDefinedName + "  in data volume (SoloFunctionsController)";
  theField->setName(userDefinedName);
  theField->setLongName(userDefinedName);
  theField->setStandardName(userDefinedName);
  _data->loadRaysFromFields();
  */
}



/*
SoloFunctions::SoloFunctions() // SpreadSheetController *controller) 
{
  //_controller = controller;

}

QString SoloFunctions::cat(QString animal) 
{ 
  
  return animal.append(" instead of cat");
}


// TODO: make functions static, and pass in all values; DO NOT associate any
// data with the functions.
// - or - 
// wrap the function with the context which contains all the extraneous data needed
// 
QString SoloFunctions::REMOVE_AIRCRAFT_MOTION(QString field) 
{
  QString result(tr("|"));
  
  // find the field in the data?
  // return the first value of the field
  vector<string> fieldNames = _controller->getFieldNames();

  int c = 0;
  int r = 0;
  vector<string>::iterator it;
  for(it = fieldNames.begin(); it != fieldNames.end(); it++, c++) {
    QString the_name(QString::fromStdString(*it));
    cerr << *it << endl;
    if (the_name.compare(field) == 0) {

      vector<double> data = _controller->getData(*it);

      cerr << "found field; number of data values = " << data.size() << endl;

      for (r=0; r<20; r++) {
        string format = "%g";
        char formattedData[250];
      //    sprintf(formattedData, format, data[0]);
        sprintf(formattedData, "%g ", data[r]);
        result.append(formattedData);
      }
    }
  }
  
  return result;
}
*/
