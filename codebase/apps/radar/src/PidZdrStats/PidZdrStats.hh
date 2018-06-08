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
/////////////////////////////////////////////////////////////
// PidZdrStats.hh
//
// PidZdrStats object
//
// Mike Dixon, RAP, NCAR, P.O.Box 3000, Boulder, CO, 80307-3000, USA
//
// June 2018
//
///////////////////////////////////////////////////////////////

#ifndef PidZdrStats_H
#define PidZdrStats_H

#include "Args.hh"
#include "Params.hh"
#include <string>
#include <Radx/Radx.hh>
#include <Radx/RadxVol.hh>
class RadxFile;
using namespace std;

class PidZdrStats {
  
public:

  // constructor
  
  PidZdrStats (int argc, char **argv);

  // destructor
  
  ~PidZdrStats();

  // run 

  int Run();

  // data members

  int OK;

protected:
private:

  string _progName;
  char *_paramsPath;
  Args _args;
  Params _params;
  vector<string> _readPaths;

  /////////////////////////////////////////
  // input data

  RadxVol _readVol;

  int _nGates;
  double _radxStartRange;
  double _radxGateSpacing;

  double _radarHtMeters;

  string _radarName;
  double _radarLatitude;
  double _radarLongitude;
  double _radarAltitude;
  
  // These are pointers into the input Radx object.
  // This memory is managed by the Radx class and should not be freed
  // by the calling class.

  const Radx::si32 *_pid;
  const Radx::fl32 *_rhohv;
  const Radx::fl32 *_temp;

  Radx::si32 _pidMiss;
  Radx::fl32 _rhohvMiss;
  Radx::fl32 _tempMiss;

  // methods
  
  int _runFilelist();
  int _runArchive();
  int _runRealtime();
  void _setupRead(RadxFile &file);
  int _processFile(const string &filePath);

  int _processVol();
  int _processRay(RadxRay *ray);
  int _writeResults();

};

#endif