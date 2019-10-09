
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

/*
SoloFunctionsModel::SoloFunctionsModel() {

}
*/
void SoloFunctionsModel::CreateBoundary(vector<Point> vertexList, string name) {

  

  // TODO: where is the  boundary?
  short *boundary;
  vector<Point> *boundaryPoints = BoundaryPointEditor.getInstance()->getWorldPoints();
 
  int nBoundaryPoints = boundaryPoints.length();
  short *xpoints = new short[nBoundaryPoints];
  short *ypoints = new short[nBoundaryPoints];

  // convert data types ...  
  vector<Point>::iterator it;
  int i = 0;
  for (it = boundaryPoints->begin(); it != boundaryPoints->end(); it++) {
    xpoints[i] = it->x;
    ypoints[i] = it->y;
    i += 1;
  }

  // TODO: need to map boundaryPoints to a list of short/boolean the same size as the ray->datafield->ngates
  // TODO: these should be library calls ...
  // TODO: we want the boundary algorithm to be outside of the individual f(x).  Because it applies to 
  //       all the f(x) in a script
  char boundary_name[20] = "name-1";
  
  SoloFunctions soloFunctionsApi;

  // TODO: Actually, I want to move this boundary stuff to a couple levels up 
  // because the boundary is for a particular ray; then the boundary
  // can be used with multiple functions
  soloFunctionsApi.CreateBoundary(xpoints, ypoints, nBoundaryPoints, boundary_name);
  //SoloFunctionsApi soloFunctionsApi;
  //soloFunctionsApi.CreateBoundary(xpoints, ypoints, npoints, "one");



  delete xpoints;
  delete ypoints;
}

// call this for each new ray, since the azimuth changes each time the ray changes
void SoloFunctionsModel::CalculateBoundaryMask() {

  // HERE ====>>> 
  // Wait! The SoloFunctionsApi doesn't store state!!!! So, we need to 
  // pass around the boundary mask.  
  // get the boundary mask for this ray
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

  // TODO: getting to this point

  const RadxField *field;
  field = vol->getFieldFromRay(fieldName);
  if (field == NULL) {
    LOG(DEBUG) << "no RadxField found in volume";
    throw "No data field with name " + fieldName;;
  }
  

  // TODO: get the ray for this field 
  const vector<RadxRay *>  &rays = vol->getRays();
  if (rays.size() > 1) {
    LOG(DEBUG) <<  "ERROR - more than one ray; expected only one";
  }
  RadxRay *ray = rays.at(0);
  if (ray == NULL) {
    LOG(DEBUG) << "ERROR - first ray is NULL";
    throw "Ray is null";
  } 

  const RadxGeoref *georef = ray->getGeoreference();
  if (georef == NULL) {
    LOG(DEBUG) << "ERROR - georef is NULL";
    throw "Georef is null";
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

  LOG(DEBUG) << "sizeof(short) = " << sizeof(short);

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

  SoloFunctionsApi soloFunctionsApi;
  
  int result = soloFunctionsApi.RemoveAircraftMotion(vert_velocity, ew_velocity, ns_velocity,
     ew_gndspd_corr, tilt, elevation,
     field->getDataSi16(), bad, parameter_scale, parameter_bias, dgi_clip_gate,
     dds_radd_eff_unamb_vel, seds_nyquist_velocity, boundaryMask);
  
  LOG(DEBUG) << " result: " << result;
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



