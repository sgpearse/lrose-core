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
////////////////////////////////////////////////////////
// QPESumVectors2Mdv.hh
//
// QPESumVectors2Mdv object
//
// Dan Megenhardt, RAL, NCAR
// P.O.Box 3000, Boulder, CO, 80307-3000, USA
//
// December 2012
//
///////////////////////////////////////////////////////////////
//
// QPESumVectors2Mdv reads netCDF data and writes Mdv files
//
///////////////////////////////////////////////////////////////////////

#ifndef QPESumVectors2Mdv_H
#define QPESumVectors2Mdv_H

#include <string>
#include <didss/DsInputPath.hh>
#include <didss/LdataInfo.hh>
#include <Mdv/DsMdvx.hh>
#include <Mdv/MdvxField.hh>
#include <Ncxx/Nc3File.hh>
#include <set>
#include "Args.hh"
#include "Params.hh"
using namespace std;

////////////////////////
// This class

class QPESumVectors2Mdv {
  
public:

  // constructor

  QPESumVectors2Mdv (int argc, char **argv);

  // destructor
  
  ~QPESumVectors2Mdv();

  // run 

  int Run();

  // data members

  bool isOK;

protected:
  Nc3File *_ncFile;
  
private:

  string _progName;
  char *_paramsPath;
  Args _args;
  Params _params;
  int _numVars;
  int _numDims;
  DsInputPath *fileTrigger;

  void _clear();
  int _processData(char *inputPath); 
  int _writeData(DsMdvx &mdv);
};

#endif

