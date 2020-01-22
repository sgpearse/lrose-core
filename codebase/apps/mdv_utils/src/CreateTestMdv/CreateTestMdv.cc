// *=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=* 
// ** Copyright UCAR (c) 1990 - 2016                                         
// ** University Corporation for Atmospheric Research (UCAR)                 
// ** National Center for Atmospheric Research (NCAR)                        
// ** Boulder, Colorado, USA                                                 
// ** BSD licence applies - redistribution and use in source and binary      
// ** forms, with or without modification, are permitted provided that       
// ** the following conditions are met:                                      
// ** 1) If the software is modified to produce derivative works,            
// ** such modified software should be clearly marked, so as not             
// ** to confuse it with the version available from UCAR.                    
// ** 2) Redistributions of source code must retain the above copyright      
// ** notice, this list of conditions and the following disclaimer.          
// ** 3) Redistributions in binary form must reproduce the above copyright   
// ** notice, this list of conditions and the following disclaimer in the    
// ** documentation and/or other materials provided with the distribution.   
// ** 4) Neither the name of UCAR nor the names of its contributors,         
// ** if any, may be used to endorse or promote products derived from        
// ** this software without specific prior written permission.               
// ** DISCLAIMER: THIS SOFTWARE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS  
// ** OR IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED      
// ** WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.    
// *=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=* 
///////////////////////////////////////////////////////////////
// CreateTestMdv object
//
// Mike Dixon, EOL, NCAR, P.O.Box 3000, Boulder, CO, 80307-3000, USA
//
// Jan 2020
//
///////////////////////////////////////////////////////////////
//
// CreateTestMdv creates a dummy MDV file for testing purposes.
//
////////////////////////////////////////////////////////////////

#include <cerrno>
#include <toolsa/mem.h>
#include <toolsa/ucopyright.h>
#include <toolsa/Path.hh>
#include <toolsa/DateTime.hh>
#include <Mdv/MdvxField.hh>
#include "CreateTestMdv.hh"
using namespace std;

const fl32 CreateTestMdv::_missingFloat = -9999.0;

// Constructor

CreateTestMdv::CreateTestMdv(int argc, char **argv)

{

  isOK = true;

  // set programe name

  _progName = (char *) "CreateTestMdv";
  ucopyright(_progName.c_str());
  
  // get command line args

  if (_args.parse(argc, argv, _progName)) {
    cerr << "ERROR: " << _progName << endl;
    cerr << "Problem with command line args" << endl;
    isOK = false;
    return;
  }

  // get TDRP params
  
  _paramsPath = (char *) "unknown";
  if (_params.loadFromArgs(argc, argv, _args.override.list,
			   &_paramsPath)) {
    cerr << "ERROR: " << _progName << endl;
    cerr << "Problem with TDRP parameters" << endl;
    isOK = false;
  }

  return;

}

// destructor

CreateTestMdv::~CreateTestMdv()

{

}

//////////////////////////////////////////////////
// Run

int CreateTestMdv::Run ()
{

  DsMdvx mdvx;
  if (_createMdvx(mdvx)) {
    cerr << "ERROR - CreateTestMdv::Run" << endl;
    cerr << "  creating mdvx object" << endl;
    return -1;
  }
  
  if (_writeMdvx(mdvx)) {
    cerr << "ERROR - CreateTestMdv::Run" << endl;
    cerr << "  writing mdvx file" << endl;
    return -1;
  }

  return 0;

}

///////////////////////////////
// create mdvx object

int CreateTestMdv::_createMdvx(DsMdvx &mdvx)

{
  
  time_t dataTime = time(NULL);
  
  if (_params.debug) {
    cerr << "Creating file at time: "
         << DateTime::strm(dataTime) << endl;
  }
  
  // initialize the master header
  
  _initMasterHeader(mdvx, dataTime);
  
  // add the fields
  
  int nFields = _params.output_fields_n;
  for (int ifield = 0; ifield < nFields; ifield++) {
    Params::output_field_t &ofld = _params._output_fields[ifield];
    _addField(mdvx, ofld);
  }

  return 0;

}

///////////////////////////////
// write MDV file

int CreateTestMdv::_writeMdvx(DsMdvx &mdvx)

{
  
  return 0;

}

///////////////////////////////
// add a field

void CreateTestMdv::_addField(DsMdvx &mdvx,
                              const Params::output_field_t &ofld)

{
  
  // fill in field header and vlevel header
  
  Mdvx::field_header_t fhdr;
  MEM_zero(fhdr);
  Mdvx::vlevel_header_t vhdr;
  MEM_zero(vhdr);

  MdvxProj proj;
  _initProjection(fhdr);
  
  fhdr.proj_origin_lat = _params.grid_origin_lat;
  fhdr.proj_origin_lon = _params.grid_origin_lon;
  fhdr.nx = _params.grid_xy_geom.nx;
  fhdr.ny = _params.grid_xy_geom.ny;
  fhdr.nz = _params.grid_xy_geom.nz;
  fhdr.grid_dx = _params.grid_geom.dx;
  fhdr.grid_dy = _params.grid_geom.dy;
  fhdr.grid_minx = _params.grid_geom.minx;
  fhdr.grid_miny = _params.grid_geom.miny;

  if (_params.vlevels_n > 1) {
    fhdr.grid_dz = _params._vlevels[1] - _params._vlevels[0];
    fhdr.grid_minz = _params._vlevels[0];
  } else if (_params.vlevels_n > 0) {
    fhdr.grid_dz = 1.0;
    fhdr.grid_minz = _params._vlevels[0];
  } else {
    fhdr.grid_dz = 1.0;
    fhdr.grid_minz = 0.0;
  }
      
  // int npointsPlane = fhdr.nx * fhdr.ny;
    
  fhdr.compression_type = Mdvx::COMPRESSION_NONE;
  fhdr.transform_type = Mdvx::DATA_TRANSFORM_NONE;
  fhdr.scaling_type = Mdvx::SCALING_NONE;

  fhdr.native_vlevel_type = _params.vlevel_type;
  fhdr.vlevel_type = _params.vlevel_type;
  fhdr.dz_constant = false;

  fhdr.bad_data_value = _missingFloat;
  fhdr.missing_data_value = _missingFloat;
  
  fhdr.encoding_type = Mdvx::ENCODING_FLOAT32;
  fhdr.data_element_nbytes = sizeof(fl32);
  fhdr.volume_size = fhdr.nx * fhdr.ny * fhdr.nz * sizeof(fl32);

  // vlevel header
  
  for (int iz = 0; iz < fhdr.nz; iz++) {
    vhdr.type[iz] = _params.vlevel_type;
    vhdr.level[iz] = _params._vlevels[iz];
  }

  // create field
  
  MdvxField *field = new MdvxField(fhdr, vhdr, data);

  // set name etc
  
  field->setFieldName(_params._fields[fieldNum].name);
  field->setFieldNameLong(_params._fields[fieldNum].name_long);
  field->setUnits(_params._fields[fieldNum].units);
  field->setTransform(_params._fields[fieldNum].transform);
  
  field->convertRounded((Mdvx::encoding_type_t) _params.output_encoding,
                        (Mdvx::compression_type_t) _params.output_compression);

  // add field to mdvx object

  mdvx.addField(field);

  int nBytesExpected = 0;

  switch (_params.input_encoding) {
    case Params::ENCODING_INT8:
      nBytesExpected = nxyz * nFields * sizeof(ui08);
      break;
    case Params::ENCODING_INT16:
      nBytesExpected = nxyz * nFields * sizeof(ui16);
      break;
    case Params::ENCODING_FLOAT32:
      nBytesExpected = nxyz * nFields * sizeof(fl32);
      break;
  }

  if (nBytesFile < nBytesExpected) {
    cerr << "ERROR - CreateTestMdv::_processFile" << endl;
    cerr << "  File not large enough: " << input_path << endl;
    cerr << "  n bytes expected: " << nBytesExpected << endl;
    cerr << "  n bytes in file: " << nBytesFile << endl;
    inFile.fclose();
    return -1;
  }
  
  // read in input data

  ui08 *inBuf = new ui08[nBytesExpected];
  if (inFile.fread(inBuf, 1, nBytesExpected) != nBytesExpected) {
    int errNum = errno;
    cerr << "ERROR - CreateTestMdv::_processFile" << endl;
    cerr << "  Cannot read input file: " << input_path << endl;
    cerr << "  " << strerror(errNum) << endl;
    delete[] inBuf;
    inFile.fclose();
    return -1;
  }

  // close input file

  inFile.fclose();

  // byte swap input buffer if needed

  if (_params.byte_swap_input_data) {
    switch (_params.input_encoding) {
      case Params::ENCODING_INT16:
        SWAP_array_16((ui16 *) inBuf, nBytesExpected);
        break;
      case Params::ENCODING_FLOAT32:
        SWAP_array_32((ui32 *) inBuf, nBytesExpected);
        break;
      default: {}
    }
  }

  // create output file object, initialize master header

  DsMdvx mdvx;
  _initMasterHeader(mdvx, dataTime);

  // load up field data

  for (int ifield = 0; ifield < nFields; ifield++) {

    fl32 *data = new fl32[nxyz];
    ui08 *buf = NULL; 
    switch (_params.input_encoding) {
      case Params::ENCODING_INT8:
        buf = inBuf + (nxyz * ifield * sizeof(ui08));
        break;
      case Params::ENCODING_INT16:
        buf = inBuf + (nxyz * ifield * sizeof(ui16));
        break;
      case Params::ENCODING_FLOAT32:
        buf = inBuf + (nxyz * ifield * sizeof(fl32));
        break;
    }
    
    _loadFieldData(buf, nx, ny, nz, nxy, nxyz,
                   _params._fields[ifield].scale,
                   _params._fields[ifield].bias,
                   _params._fields[ifield].missing_val,
                   _params._fields[ifield].min_val,
                   _params._fields[ifield].max_val,
                   data);
    
    // add field to output object

    _addField(mdvx, ifield, data);

    // free up

    delete[] data;

  } // ifield

  // free up input buffer

  delete[] inBuf;

  // write out file
  
  if (_writeOutput(mdvx)) {
    return -1;
  }

  return 0;

}

////////////////////////////////////////////////////////////
// Initialize projection

void CreateTestMdv::_initProjection(Mdvx::field_header_t &fhdr)
  
{

  fhdr.proj_origin_lat = _params.grid_origin_lat;
  fhdr.proj_origin_lon = _params.grid_origin_lon;

  if (_params.gridprojection == Params::PROJ_LATLON) {
    fhdr.proj_type = Mdvx::PROJ_LATLON;
  } else if (_params.gridprojection == Params::PROJ_FLAT) {
    fhdr.proj_type = Mdvx::PROJ_FLAT;
    fhdr.proj_rotation = _params.grid_rotation;
    fhdr.proj_params[0] = _params.grid_rotation;
  } else if (_params.gridprojection == Params::PROJ_LAMBERT_CONF) {
    fhdr.proj_type = Mdvx::PROJ_LAMBERT_CONF;
    fhdr.proj_params[0] = _params.grid_lat1;
    fhdr.proj_params[1] = _params.grid_lat2;
  } else if (_params.gridprojection == Params::PROJ_POLAR_STEREO) {
    fhdr.proj_type = Mdvx::PROJ_POLAR_STEREO;
    fhdr.proj_params[0] = _params.grid_tangent_lon;
    if (_params.grid_pole_is_north) {
      fhdr.proj_params[1] = 1;
    } else {
      fhdr.proj_params[1] = 0;
    }
    fhdr.proj_params[2] = _params.grid_central_scale;
  } else if (_params.gridprojection == Params::PROJ_OBLIQUE_STEREO) {
    fhdr.proj_type = Mdvx::PROJ_OBLIQUE_STEREO;
    fhdr.proj_params[0] = _params.grid_tangent_lat;
    fhdr.proj_params[1] = _params.grid_tangent_lon;
    fhdr.proj_params[2] = _params.grid_central_scale;
  } else if (_params.gridprojection == Params::PROJ_MERCATOR) {
    fhdr.proj_type = Mdvx::PROJ_MERCATOR;
  } else if (_params.gridprojection == Params::PROJ_TRANS_MERCATOR) {
    fhdr.proj_type = Mdvx::PROJ_TRANS_MERCATOR;
    fhdr.proj_params[0] = _params.grid_central_scale;
  } else if (_params.gridprojection == Params::PROJ_ALBERS) {
    fhdr.proj_type = Mdvx::PROJ_ALBERS;
    fhdr.proj_params[0] = _params.grid_lat1;
    fhdr.proj_params[1] = _params.grid_lat2;
  } else if (_params.gridprojection == Params::PROJ_LAMBERT_AZIM) {
    fhdr.proj_type = Mdvx::PROJ_LAMBERT_AZIM;
  } else if (_params.gridprojection == Params::PROJ_VERT_PERSP) {
    fhdr.proj_type = Mdvx::PROJ_VERT_PERSP;
    proj.initLambertAzim(_params.grid_persp_radius);
  }
  
  fhdr.proj_params[6] = _params.grid_false_northing;
  fhdr.proj_params[7] = _params.grid_false_easting;

}

////////////////////////////////////////////////////
// load up field data

void CreateTestMdv::_loadFieldData(ui08 *inBuf,
                               int nx, int ny, int nz, int nxy, int nxyz,
                               fl32 scale, fl32 bias, fl32 miss,
			       fl32 minVal, fl32 maxVal,
                               fl32 *data)
  
{
  
  switch (_params.input_encoding) {

    case Params::ENCODING_INT8: {
      ui08 *bb = inBuf;
      for (int ii = 0; ii < nxyz; ii++, bb++) {
        if (miss == (fl32) *bb) {
          data[ii] = _missingFloat;
        } else {
          fl32 ff = *bb * scale + bias;
	  if (ff > minVal && ff < maxVal) {
	    data[ii] = ff;
	  } else {
	    data[ii] = _missingFloat;
	  }
        }
      } // ii
      break;
    }

    case Params::ENCODING_INT16: {
      ui16 *ss = (ui16 *) inBuf;
      for (int ii = 0; ii < nxyz; ii++, ss++) {
        if (miss == (fl32) *ss) {
          data[ii] = _missingFloat;
        } else {
          fl32 ff = *ss * scale + bias;
	  if (ff > minVal && ff < maxVal) {
	    data[ii] = ff;
	  } else {
	    data[ii] = _missingFloat;
	  }
        }
      } // ii
      break;
    }

    default: {
      fl32 *fff = (fl32 *) inBuf;
      for (int ii = 0; ii < nxyz; ii++, fff++) {
	fl32 ff = *fff;
        if (miss == ff) {
          data[ii] = _missingFloat;
        } else {
	  if (ff > minVal && ff < maxVal) {
	    data[ii] = ff;
	  } else {
	    data[ii] = _missingFloat;
	  }
        }
      } // ii
      break;
    }
      
  } // switch

  // re-arrange rows as required

  if (_params.input_row_ordering == Params::NORTH_ROW_FIRST) {

    fl32 *tmp = new fl32[nxyz];

    for (int iz = 0; iz < nz; iz++) {
      for (int iy = 0; iy < ny; iy++) {
        int iIn = iz * nxy + (ny - 1 - iy) * nx;
        int iOut = iz * nxy + iy * nx;
        memcpy(tmp + iOut, data + iIn, nx * sizeof(fl32));
      } // iy
    } // iz
    
    memcpy(data, tmp, nxyz * sizeof(fl32));
    delete[] tmp;
    
  }

}
  
////////////////////////////////////////////////////
// init master header

void CreateTestMdv::_initMasterHeader(DsMdvx &mdvx, time_t dataTime)

{

  // set the master header
  
  Mdvx::master_header_t mhdr;
  MEM_zero(mhdr);

  mhdr.time_begin = dataTime;
  mhdr.time_end = dataTime;
  mhdr.time_centroid = dataTime;
    
  mhdr.num_data_times = 1;
  mhdr.data_dimension = 3;
  
  mhdr.data_collection_type = -1;
  mhdr.native_vlevel_type = _params.vlevel_type;
  mhdr.vlevel_type = _params.vlevel_type;
  if (_params.specify_individual_z_levels) {
    mhdr.max_nz = _params.z_level_array_n;
  } else {
    mhdr.max_nz = _params.grid_z_geom.nz;
  }
  mhdr.vlevel_included = TRUE;
  mhdr.grid_orientation = Mdvx::ORIENT_SN_WE;
  mhdr.data_ordering = Mdvx::ORDER_XYZ;
  mhdr.max_nx = _params.grid_xy_geom.nx;
  mhdr.max_ny = _params.grid_xy_geom.ny;
  mhdr.n_fields = _params.output_fields_n;
  mhdr.n_chunks = 0;
  mhdr.field_grids_differ = FALSE;
  mhdr.sensor_lon = 0.0;
  mhdr.sensor_lat = 0.0;
  mhdr.sensor_alt = 0.0;

  mdvx.setDataSetInfo(_params.data_set_info);
  mdvx.setDataSetName(_params.data_set_name);
  mdvx.setDataSetSource(_params.data_set_source);

  mdvx.setMasterHeader(mhdr);
  
}

////////////////////////////////////////////////////
// add a field to output file

void CreateTestMdv::_addField(DsMdvx &mdvx, int fieldNum, fl32 *data)

{

}

////////////////////////////////////////////////////
// write output data
//
// returns 0 on success, -1 on failure

int CreateTestMdv::_writeOutput(DsMdvx &mdvx)

{

  // write out

  if (mdvx.writeToDir(_params.output_url)) {
    cerr << "ERROR - CreateTestMdv::_writeOutput" << endl;
    cerr << mdvx.getErrStr() << endl;
    return -1;
  }
  
  if (_params.debug) {
    cerr << "Wrote file: " << mdvx.getPathInUse() << endl;
  }

  return 0;
    
}

