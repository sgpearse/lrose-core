
#include <vector>
#include <iostream>

#include "SoloFunctionsModel.hh"
#include "RemoveAcMotion.cc" // This comes from an external library
#include "BoundaryPointEditor.hh"


#include <Radx/RadxField.hh>
#include <Radx/RadxRay.hh>
#include <Radx/RadxCfactors.hh>
#include <Radx/RadxGeoref.hh>
#include <toolsa/LogStream.hh>
#include <Solo/SoloFunctionsApi.hh>

using namespace std;


SoloFunctionsModel::SoloFunctionsModel() {
  _boundaryMask = NULL;
  _boundaryMaskSet = false;
  _boundaryMaskLength = 0;
}

// call this for each new ray, since the azimuth changes each time the ray changes
void SoloFunctionsModel::SetBoundaryMask(RadxVol *vol,
					 int rayIdx, int sweepIdx) {

  _boundaryMaskSet = true;

  if (_boundaryMask != NULL) {
    delete[] _boundaryMask;
  }

  SetBoundaryMaskOriginal(vol, rayIdx, sweepIdx);

  /* fixed boundary ... 
  size_t nGates = 2000;
  _boundaryMaskLength = nGates;
  _boundaryMask = new short[nGates];
    for (size_t i = 0; i<nGates; i++)
      _boundaryMask[i] = 0;
    for (size_t i = 500; i<nGates; i++)
      _boundaryMask[i] = 1;
  end fixed boundary ... */

  /*
  _boundaryMaskLength = 2000;
  _boundaryMask = new short[_boundaryMaskLength];
  for (int i=0; i<_boundaryMaskLength; i++)
    _boundaryMask[i] = 3;
  */
}


// call this for each new ray, since the azimuth changes each time the ray changes
void SoloFunctionsModel::SetBoundaryMaskOriginal(RadxVol *vol,
					 int rayIdx, int sweepIdx) {
  
  LOG(DEBUG) << "enter";
  LOG(DEBUG) << " radIdx=" << rayIdx
	     << " sweepIdx=" << sweepIdx;
 
  short *boundary;

  // TODO: make this a call to BoundaryPointModel?
  BoundaryPointEditor *bpe = BoundaryPointEditor::Instance();
  // TODO:   vector<Point> boundaryPoints = bpe->getWorldPoints();
  // vector<Point> myPoints = BoundaryPointEditor::Instance()->getBoundaryPoints("/media/sf_lrose/ncswp_SPOL_RHI_.nc", 0, 4, "Boundary1");  TODO

  /*
  // Test data ...  works
  Point p1, p2, p3, p4;
  p1.x =  1000; p1.y = 1000;
  p2.x = -1000; p2.y = 1000;
  p3.x = -1000; p3.y =-1000;
  p4.x =  1000; p4.y =-1000;
  */

  // Test data ...
  Point p1, p2, p3, p4;
  int baseValue = 100000;
  p1.x =  baseValue; p1.y = baseValue;
  p2.x = -baseValue; p2.y = baseValue;
  p3.x = -baseValue; p3.y =-baseValue;
  p4.x =  baseValue; p4.y =-baseValue;

  /*
  p1.x = -50; p1.y =  50;
  p2.x =  50; p2.y =  50;
  p3.x =  50; p3.y = -50;
  p4.x = -50; p4.y = -50;
  */
  vector<Point> boundaryPoints;
  boundaryPoints.push_back(p1);
  boundaryPoints.push_back(p2);
  boundaryPoints.push_back(p3);
  boundaryPoints.push_back(p4);
  // end Test data

  //map boundaryPoints to a list of short/boolean the same size as the ray->datafield->ngates
  int nBoundaryPoints = boundaryPoints.size();
  LOG(DEBUG) << "nBoundaryPoints = " << nBoundaryPoints;

  // if we have less than three points, then it is NOT a boundary
  if (nBoundaryPoints < 3) 
    return;
 
  short *xpoints = new short[nBoundaryPoints];
  short *ypoints = new short[nBoundaryPoints];

  // convert data models  ...  
  vector<Point>::iterator it;
  int i = 0;
  for (it = boundaryPoints.begin(); it != boundaryPoints.end(); it++) {
    xpoints[i] = (short) it->x;
    ypoints[i] = (short) it->y;
    i += 1;
  }

  //  we want the boundary algorithm to be outside of the individual f(x).  Because it applies to 
  //       all the f(x) in a script
  
  // Wait! The SoloFunctionsApi doesn't store state!!!! So, we need to 
  // pass around the boundary mask.  
  // get the boundary mask for this ray

  SoloFunctionsApi soloFunctionsApi;

  // TODO: Actually, I want to move this boundary stuff to a couple levels up 
  // because the boundary is for a particular ray; then the boundary
  // can be used with multiple functions ?? maybe NOT!

  // wrestle this information out of the ray and radar volume ...
  float radar_origin_latitude = vol->getLatitudeDeg();
  float radar_origin_longitude = vol->getLongitudeDeg();
  float radar_origin_altitude = vol->getAltitudeKm() * 1000.0;
  float boundary_origin_tilt = 0.0;
  float boundary_origin_latitude = radar_origin_latitude; // 0.0;
  float boundary_origin_longitude = radar_origin_longitude; // 0.0;
  float boundary_origin_altitude = radar_origin_altitude; // 0.0;
  
  // =======

  vol->loadRaysFromFields();

  const RadxField *field;
  /*
  field = vol->getFieldFromRay(fieldName);
  if (field == NULL) {
    LOG(DEBUG) << "no RadxField found in volume";
    throw "No data field with name " + fieldName;;
  }
  */

  //  get the ray for this field 
  // for the boundary mask, all we care about is the geometry of the ray,
  // NOT the data values for each field.
  const vector<RadxRay *>  &rays = vol->getRays();
  if (rays.size() > 1) {
    LOG(DEBUG) <<  "ERROR - more than one ray; expected only one";
  }
  RadxRay *ray = rays.at(rayIdx);
  if (ray == NULL) {
    LOG(DEBUG) << "ERROR - ray is NULL";
    throw "Ray is null";
  } 

  //field = ray->getField(fieldName);
  size_t nGates = ray->getNGates(); 
  LOG(DEBUG) << "there are nGates " << nGates;

  // =======

  _boundaryMaskLength = nGates;

  float gateSize = ray->getGateSpacingKm() * 1000.0;
  float distanceToCellNInMeters = ray->getStartRangeKm() * 1000.0;
  float azimuth = ray->getAzimuthDeg();
  // need to do some conversions here ...
  // TODO: get these from SoloLibrary::dd_math.h
  int radar_scan_mode = 1; // PPI;
  int radar_type = 0; // GROUND; 
 
  float tilt_angle = 0.0; // TODO: It should be this ... ray->getElevationDeg();
  float rotation_angle = 0.0; 

  _boundaryMask = new short[_boundaryMaskLength];
 
  if (_boundaryMaskLength != 1832)
    LOG(DEBUG) << "HERE";

  soloFunctionsApi.GetBoundaryMask(xpoints, ypoints, nBoundaryPoints,
                         radar_origin_latitude,
                         radar_origin_longitude,
                         radar_origin_altitude,
                         boundary_origin_tilt,
                         boundary_origin_latitude,
                         boundary_origin_longitude,
                         boundary_origin_altitude,
                                           nGates,
                                           gateSize,
                                           distanceToCellNInMeters,
                                           azimuth,
                                           radar_scan_mode,
                                           radar_type,
                                           tilt_angle,
						  rotation_angle,
_boundaryMask);
 

  /*
  short *GetBoundaryMask(short *xpoints, short *ypoints, int npoints,
                         //float radar_origin_x,                                                                                     
                         //  float radar_origin_y,                                                                                   
                         //  float radar_origin_z,                                                                                   
                         float radar_origin_latitude,
                         float radar_origin_longitude,
                         float radar_origin_altitude,
                         float boundary_origin_tilt,
                         // float boundary_origin_x,                                                                                 
                         // float boundary_origin_y,                                                                                 
                         // float boundary_origin_z,                                                                                 
                         float boundary_origin_latitude,
                         float boundary_origin_longitude,
                         float boundary_origin_altitude,
                                           int nGates,
                                           float gateSize,
                                           float distanceToCellNInMeters,
                                           float azimuth,
                                           int radar_scan_mode,
                                           int radar_type,
                                           float tilt_angle,
                                           float rotation_angle);

  short *boundaryMask = soloFunctionsApi.GetBoundaryMask(boundaryList, // is this boundary_name?
						     &radar_origin,
						     &boundary_origin,
				nGates, // for this ray
				gateSize, // for this ray
				distanceToCellNInMeters, // for this ray
				azimuth, // ray azimuth
						     radar_scan_mode,
						     radar_type,
						     tilt_angle,
						     rotation_angle);
  */
  // _boundaryMask = boundaryMask;
  printBoundaryMask();

  delete[] xpoints;
  delete[] ypoints;

  LOG(DEBUG) << "exit"; 

}

// return the temporary name for the new field in the volume
string SoloFunctionsModel::ZeroMiddleThird(string fieldName,  RadxVol *vol,
					   int rayIdx, int sweepIdx,
					   string newFieldName) {
  LOG(DEBUG) << "entry with fieldName ... " << fieldName << " radIdx=" << rayIdx
	     << " sweepIdx=" << sweepIdx;
  cerr << "inside SoloFunctionsModel::RemoveAircraftMotion" << endl;

  // gather data from context -- most of the data are in a DoradeRadxFile object

  // TODO: convert the context RadxVol to DoradeRadxFile and DoradeData format;
  //RadxVol vol = context->_vol;
  // make sure the radar angles have been calculated.

  //  vol->loadFieldsFromRays();
  vol->loadRaysFromFields();

  
  const RadxField *field;
  /*
  field = vol->getFieldFromRay(fieldName);
  if (field == NULL) {
    LOG(DEBUG) << "no RadxField found in volume";
    throw "No data field with name " + fieldName;;
  }
  */

  //  get the ray for this field 
  const vector<RadxRay *>  &rays = vol->getRays();
  if (rays.size() > 1) {
    LOG(DEBUG) <<  "ERROR - more than one ray; expected only one";
  }
  RadxRay *ray = rays.at(rayIdx);
  if (ray == NULL) {
    LOG(DEBUG) << "ERROR - ray is NULL";
    throw "Ray is null";
  } 
  
  field = ray->getField(fieldName);
  size_t nGates = ray->getNGates(); 
  cerr << "there are nGates " << nGates;
  const float *data = field->getDataFl32();
  float *newData = new float[nGates];
  for (int i=0; i<10; i++)
    newData[i] = data[i];   
  for (int i=10; i<30; i++)
    newData[i] = 0;
  for (int i=30; i<nGates; i++)
    newData[i] = data[i];   

  // insert new field into RadxVol                                                                             
  cerr << "result = ";
  for (int i=0; i<50; i++)
    cerr << newData[i] << ", ";
  cerr << endl;

  // I have the ray, can't I just add a field to it?

  // ========
  /*
  RadxRay *newRay = new RadxRay();

  newRay->setVolumeNumber(1);
  newRay->setSweepNumber(sweepIdx);
  newRay->setRayNumber(rayIdx);
  */
  //      const string name = "VEL";                                                                  
  //const string units = "m/s";                                                                       
  Radx::fl32 missingValue = Radx::missingFl32; // -999;
  //      const Radx::si16 *data = &rawData[0];                                                       
  //  double scale = 1.0;
  //double offset = 0.0;
  bool isLocal = false;

  //RadxField *newField = new RadxField(newFieldName, "m/s");
  //newField->copyMetaData(*field);
  //newField->addDataFl32(nGates, newData);
  RadxField *field1 = ray->addField(newFieldName, "m/s", nGates, missingValue, newData, isLocal);
  //  RadxField *field1 = newRay->addField(newFieldName, "m/s", nGates, missingValue, newData, scale, offset, isLocal);

  string tempFieldName = field1->getName();

  /*
  // to avoid this warning ...                                                                        
  // WARNING - Range geom has not been set on ray                                                     
  double startRangeKm = 3.0;
  double gateSpacingKm = 5.0;
  newRay->setRangeGeom(startRangeKm, gateSpacingKm);
  */

  //vol->addRay?Field?();  // apparently this is not needed!

  // STOPPED HERE ...  Kinda close ... adds fields VEL_xyz, VEL_xyz2, ... VEL_xyz4 one for each sweep, 
  // and the missing value is wonky 

  // ============

  //  return newData;

  return tempFieldName;
}

// return the temporary name for the new field in the volume
string SoloFunctionsModel::ZeroInsideBoundary(string fieldName,  RadxVol *vol,
					   int rayIdx, int sweepIdx,
					   string newFieldName) {
  LOG(DEBUG) << "entry with fieldName ... " << fieldName << " radIdx=" << rayIdx
	     << " sweepIdx=" << sweepIdx;

  vol->loadRaysFromFields();
  
  const RadxField *field;

  //  get the ray for this field 
  const vector<RadxRay *>  &rays = vol->getRays();
  if (rays.size() > 1) {
    LOG(DEBUG) <<  "ERROR - more than one ray; expected only one";
  }
  RadxRay *ray = rays.at(rayIdx);
  if (ray == NULL) {
    LOG(DEBUG) << "ERROR - ray is NULL";
    throw "Ray is null";
  } 

  // get the data (in) and create space for new data (out)  
  field = ray->getField(fieldName);
  size_t nGates = ray->getNGates(); 

  float *newData = new float[nGates];

  // =======
  // data, _boundaryMask, and newData should have all the same dimensions = nGates
  SoloFunctionsApi soloFunctionsApi;

  //  if (!_boundaryMaskSet) 
  //  _boundaryMask = NULL;

  // =======

  if (_boundaryMaskSet) {

    // verify dimensions on data in/out and boundary mask
    if (nGates > _boundaryMaskLength)
      throw "Error: boundary mask and field gate dimension are not equal (SoloFunctionsModel)";

    cerr << "there are nGates " << nGates;
    const float *data = field->getDataFl32();
  
    // perform the function ...
    soloFunctionsApi.ZeroInsideBoundary(data, _boundaryMask, newData, nGates);
  /*
    if (_boundaryMaskLength != nGates) 
      throw "Error: boundaryMaskLength not equal to nGates (ZeroInsideBoundary)";
    for (int i=0; i<_boundaryMaskLength; i++) {
      if (_boundaryMask[i]) 
	newData[i] = data[i];
      else
	newData[i] = 0.0;
    }
  */
  } else {  // no _boundaryMaskSet
    cerr << "there are nGates " << nGates;
    const float *data = field->getDataFl32();

  
    /*  This is for a fixed boundary; and it works fine!
    short *fixedBnd = new short[nGates];
    for (size_t i = 0; i<nGates; i++)
      fixedBnd[i] = 0;
    for (size_t i = 500; i<nGates; i++)
      fixedBnd[i] = 1;
    soloFunctionsApi.ZeroInsideBoundary(data, fixedBnd, newData, nGates);
    delete[] fixedBnd;
    */

    soloFunctionsApi.ZeroInsideBoundary(data, NULL, newData, nGates);


    /*
    for (int i=0; i<nGates; i++) {
	newData[i] = 0.0;
    }
    */
  }
  /*
  for (int i=0; i<10; i++)
    newData[i] = data[i];   
  for (int i=10; i<30; i++)
    newData[i] = 0;
  for (int i=30; i<nGates; i++)
    newData[i] = data[i];   
  */



  // insert new field into RadxVol                                                                             
  cerr << "result = ";
  for (int i=0; i<50; i++)
    cerr << newData[i] << ", ";
  cerr << endl;

  Radx::fl32 missingValue = Radx::missingFl32; 
  bool isLocal = false;

  //RadxField *newField = new RadxField(newFieldName, "m/s");
  //newField->copyMetaData(*field);
  //newField->addDataFl32(nGates, newData);
  RadxField *field1 = ray->addField(newFieldName, "m/s", nGates, missingValue, newData, isLocal);

  string tempFieldName = field1->getName();

  /*
  // to avoid this warning ...                                                                        
  // WARNING - Range geom has not been set on ray                                                     
  double startRangeKm = 3.0;
  double gateSpacingKm = 5.0;
  newRay->setRangeGeom(startRangeKm, gateSpacingKm);
  */

  return tempFieldName;
}


// TODO: send rayIdx, sweepIdx, OR  Radx::Float32 *data
vector<double> SoloFunctionsModel::RemoveAircraftMotion(string fieldName, RadxVol *vol,
							int rayIdx, int sweepIdx) { 

  // TODO: what is being returned? the name of the new field in the model that
  // contains the results.
  // since the std::vector<double> data has to be copied to QVector anyway, 
  // go ahead and format it as a string?
  // maybe return a pointer to std::vector<double> ?? then when presenting the data, we can convert it to string,
  // but maintain the precision in the model (RadxVol)??

  LOG(DEBUG) << "entry with fieldName ... ";
  LOG(DEBUG) << fieldName;
  cerr << "inside SoloFunctionsModel::RemoveAircraftMotion" << endl;

  // gather data from context -- most of the data are in a DoradeRadxFile object

  // TODO: convert the context RadxVol to DoradeRadxFile and DoradeData format;
  //RadxVol vol = context->_vol;
  // make sure the radar angles have been calculated.

  vol->loadFieldsFromRays();

  const RadxField *field;
  field = vol->getFieldFromRay(fieldName);
  if (field == NULL) {
    LOG(DEBUG) << "no RadxField found in volume";
    throw "No data field with name " + fieldName;;
  }
  

  //  get the ray for this field 
  const vector<RadxRay *>  &rays = vol->getRays();
  if (rays.size() > 1) {
    LOG(DEBUG) <<  "ERROR - more than one ray; expected only one";
  }
  RadxRay *ray = rays.at(rayIdx);
  if (ray == NULL) {
    LOG(DEBUG) << "ERROR - ray is NULL";
    throw "Ray is null";
  } 

  

  const RadxGeoref *georef = ray->getGeoreference();
  if (georef == NULL) {
    LOG(DEBUG) << "ERROR - georef is NULL";
    LOG(DEBUG) << "      trying to recover ...";
    vol->setLocationFromStartRay();
    georef = ray->getGeoreference();
    if (georef == NULL) {
      throw "Georef is null";
    }
  } 
 
  float vert_velocity = georef->getVertVelocity();  // fl32
  float ew_velocity = georef->getEwVelocity(); // fl32
  float ns_velocity = georef->getNsVelocity(); // fl32;

  float ew_gndspd_corr = 0.0; 
  const RadxCfactors *cfactors = ray->getCfactors();
  if (cfactors != NULL) {
    ew_gndspd_corr = cfactors->getEwVelCorr(); // ?? _gndspd_corr; // fl32;
  }
 
  float tilt = georef->getTilt(); // fl32; 
  // TODO: elevation changes with different rays/fields how to get the current one???
  float elevation = ray->getElevationDeg(); // doradeData.elevation; // fl32;

  // TODO:  look up the dataField and get the associated values
  // look through DoradeRadxFile::_ddParms for a parameter_t type that has parameter_name that matches
  // the dataField.
  // short *data; // data is in and out parameter
  if (field->getDataType() != Radx::SI16) {
    throw  "ERROR - data is not 16-bit signed integer as expected";
  } 

  //Radx::fl32 *rawData;
  //rawData = field->getDataFl32();
  //Radx::fl32 *ptr = rawData;
  cerr << " A few data values ";
  for (int i=0; i< 10; i++) {
    cerr << field->getDoubleValue(i) << ", ";
  }
  cerr << endl;
  //vector<double> data = field->getData  Radx::fl32 *getDataFl32();   
  //  double scale = 1.0 / parm.parameter_scale;
  //  double bias = (-1.0 * parm.parameter_bias) / parm.parameter_scale;

  // related to field->setTypeSi32(parm.bad_data, scale, bias)
  // RadxField::_scale;  RadxField::_offset = bias; RadxField::_missingSi32 = bad_data
  // 
  // TODO: need to find the field. How to do this????
  short bad = field->getMissingSi16(); // doradeData.bad_data;
  float parameter_scale = 1.0 / field->getScale(); // doradeData.parameter_scale; 
  float parameter_bias = -1.0 * field->getOffset() * field->getScale(); // doradeData.parameter_bias; 

  int dgi_clip_gate = field->getNPoints(); // field->num_samples; // or number_cells
  short dds_radd_eff_unamb_vel = ray->getNyquistMps(); // doradeData.eff_unamb_vel;
  int seds_nyquist_velocity = 0; // TODO: what is this value?

  cerr << "sizeof(short) = " << sizeof(short);
  if (sizeof(short) != 16) 
    throw "FATAL ERROR: short is NOT 16 bits! Exiting.";
  LOG(DEBUG) << "args: ";
  LOG(DEBUG) << "vert_velocity " << vert_velocity;
  LOG(DEBUG) <<   "ew_velocity " << ew_velocity;
  LOG(DEBUG) <<   "ns_velocity " << ns_velocity;
  LOG(DEBUG) <<   "ew_gndspd_corr " << ew_gndspd_corr;
  LOG(DEBUG) <<   "tilt " << tilt;
  LOG(DEBUG) <<   "elevation " << elevation;
  LOG(DEBUG) <<   "bad " << bad;
  LOG(DEBUG) <<   "parameter_scale " << parameter_scale;
  LOG(DEBUG) <<   "dgi_clip_gate " << dgi_clip_gate;
  LOG(DEBUG) <<   "dds_radd_eff_unamb_vel " << dds_radd_eff_unamb_vel;
  LOG(DEBUG) <<   "seds_nyquist_velocity " << "??";

  // TODO: convert the field data from Si16 to short

  SoloFunctionsApi soloFunctionsApi;

  short fakeData[3] = {0, 1, 2};  

//field->getDataSi16(), 
						     
  soloFunctionsApi.RemoveAircraftMotion(vert_velocity, ew_velocity, ns_velocity,
						     ew_gndspd_corr, tilt, elevation,
						     fakeData,
						     bad, parameter_scale, parameter_bias, dgi_clip_gate,
						     dds_radd_eff_unamb_vel, seds_nyquist_velocity,
						     _boundaryMask);
  
  //LOG(DEBUG) << " result: " << result;
  LOG(DEBUG) << " A few data values ";
  for (int i=0; i< 10; i++) {
      LOG(DEBUG) << field->getDoubleValue(i);
  }

  // TODO: We are converting from short to double!!!  <=====
  vector<double> newData; // (data, dgi_clip_gate+1);
  //  for (vector<double>::iterator it = data.begin(); it != data.end(); ++it)
  //  newData.push_back(*it * 2.0);

  LOG(DEBUG) << "exit ";

  return newData;
}



vector<double> SoloFunctionsModel::RemoveAircraftMotion(vector<double> data, RadxVol *vol) { // SpreadSheetModel *context) {

  // TODO: what is being returned? the name of the new field in the model that
  // contains the results.
  // since the std::vector<double> data has to be copied to QVector anyway, 
  // go ahead and format it as a string?
  // maybe return a pointer to std::vector<double> ?? then when presenting the data, we can convert it to string,
  // but maintain the precision in the model (RadxVol)??

  cerr << "RemoveAircraftMotion ... "  << endl;

  // gather data from context -- most of the data are in a DoradeRadxFile object

  // TODO: convert the context RadxVol to DoradeRadxFile and DoradeData format;
  //RadxVol vol = context->_vol;
  // make sure the radar angles have been calculated.

  /*
  const RadxField *field;
  field = vol.getFieldFromRay(fieldName);
  if (field == NULL) {
    cerr << "no RadxField found " <<  endl;
    throw "No data field with name " + fieldName;;
  }
  */

  // TODO: get the ray for this field 
  const vector<RadxRay *>  &rays = vol->getRays();
  if (rays.size() > 1) {
    cerr << "ERROR - more than one ray; expected only one\n";
  }

  RadxRay *ray = rays.at(0);
  if (ray == NULL) {
    cerr << "ERROR - first ray is NULL" << endl;
  } 
  //const RadxGeoref *georef = ray->getGeoreference();

  // float vert_velocity = georef->getVertVelocity();  // fl32
  // float ew_velocity = georef->getEwVelocity(); // fl32
  // float ns_velocity = georef->getNsVelocity(); // fl32;

  float ew_gndspd_corr = 0.0; 
  const RadxCfactors *cfactors = ray->getCfactors();
  if (cfactors != NULL) {
    ew_gndspd_corr = cfactors->getEwVelCorr(); // ?? _gndspd_corr; // fl32;
  }
  LOG(DEBUG) << "ew_gndspd_corr: " << ew_gndspd_corr;
 
  // float tilt = georef->getTilt(); // fl32; 
  // TODO: elevation changes with different rays/fields how to get the current one???
  // float elevation = ray->getElevationDeg(); // doradeData.elevation; // fl32;

  // TODO:  look up the dataField and get the associated values
  // look through DoradeRadxFile::_ddParms for a parameter_t type that has parameter_name that matches
  // the dataField.
  //short *data; // data is in and out parameter
  //data = 
  //  double scale = 1.0 / parm.parameter_scale;
  //  double bias = (-1.0 * parm.parameter_bias) / parm.parameter_scale;

  // related to field->setTypeSi32(parm.bad_data, scale, bias)
  // RadxField::_scale;  RadxField::_offset = bias; RadxField::_missingSi32 = bad_data
  // 
  /* TODO: need to find the field. How to do this????
  short bad = field->getMissingSi16(); // doradeData.bad_data;
  float parameter_scale = 1.0 / field->getScale(); // doradeData.parameter_scale; 
  float parameter_bias = -1.0 * field->getOffset() * field->getScale(); // doradeData.parameter_bias; 
  int dgi_clip_gate = 7; // field->num_samples; // or number_cells
  short dds_radd_eff_unamb_vel = ray->getNyquistMps(); // doradeData.eff_unamb_vel;
  int seds_nyquist_velocity; // TODO: what is this value?

  cerr << "args: " << endl <<
    "vert_velocity " << vert_velocity << endl <<
    "ew_velocity " << ew_velocity << endl <<
    "ns_velocity " << ns_velocity << endl <<
    "ew_gndspd_corr " << ew_gndspd_corr << endl <<
    "tilt " << tilt << endl <<
    "elevation " << elevation <<
    endl;
  */
  //SoloFunctionsApi soloFunctionsApi;
  /*  int result = soloFunctionsApi.se_remove_ac_motion(vert_velocity, ew_velocity, ns_velocity,
     ew_gndspd_corr, tilt, elevation,
     data, bad, parameter_scale, parameter_bias, dgi_clip_gate,
     dds_radd_eff_unamb_vel, seds_nyquist_velocity);
  */

  // TODO: We are converting from short to double!!!  <=====
  vector<double> newData; // (data, dgi_clip_gate+1);
  for (vector<double>::iterator it = data.begin(); it != data.end(); ++it)
    newData.push_back(*it * 2.0);
  return newData;
}



void SoloFunctionsModel::printBoundaryMask() {
  cout << "Boundary Mask ... Length = " << _boundaryMaskLength << endl;
  for (int i=0; i<_boundaryMaskLength; i++)
    cout << _boundaryMask[i] << ", ";
}
