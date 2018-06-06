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
// SpolWarnSms.cc
//
// Mike Dixon, EOL, NCAR, P.O.Box 3000, Boulder, CO, 80307-3000, USA
//
// June 2018
//
///////////////////////////////////////////////////////////////
//
// SpolWarnSms reads status XML monitoring data from SPDB,
// for the S-band and K-band.
// It checks the status in the XML, and determines if
// a warning message should be sent
//
////////////////////////////////////////////////////////////////

#include <iostream>
#include <fstream>
#include <cerrno>
#include <cmath>
#include <cstdio>
#include <unistd.h>
#include <sys/stat.h>
#include <ctime>
#include <toolsa/DateTime.hh>
#include <toolsa/uusleep.h>
#include <toolsa/file_io.h>
#include <toolsa/pmu.h>
#include <toolsa/Path.hh>
#include <toolsa/TaStr.hh>
#include <toolsa/TaXml.hh>
#include <dsserver/DsLdataInfo.hh>
#include "SpolWarnSms.hh"

using namespace std;

// Constructor

SpolWarnSms::SpolWarnSms(int argc, char **argv)
  
{

  isOK = true;

  // set programe name
  
  _progName = "SpolWarnSms";
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
    return;
  }
  
  // init process mapper registration
  
  PMU_auto_init((char *) _progName.c_str(),
                _params.instance,
                PROCMAP_REGISTER_INTERVAL);

}

// destructor

SpolWarnSms::~SpolWarnSms()

{

  // unregister process

  PMU_auto_unregister();

}

//////////////////////////////////////////////////
// Run

int SpolWarnSms::Run ()
{
  
  PMU_auto_register("Run");
  
  while (true) {
    
    PMU_auto_register("Getting status");
    time_t now = time(NULL);

    // read status, append to the message

    string warningMsgSband;
    if (_params.monitor_the_sband) {
      if (_readStatus(now,
                      _params.sband_monitoring_spdb_url,
                      _params.sband_spdb_margin_secs,
                      _params._sband_xml_entries,
                      _params.sband_xml_entries_n,
                      warningMsgSband)) {
        cerr << "ERROR - SpolWarnSms::Run" << endl;
        cerr << "  Problems monitoring the sband" << endl;
        warningMsgSband = "#Sband down#";
      }
    }

    string warningMsgKband;
    if (_params.monitor_the_kband) {
      if (_readStatus(now,
                      _params.kband_monitoring_spdb_url,
                      _params.kband_spdb_margin_secs,
                      _params._kband_xml_entries,
                      _params.kband_xml_entries_n,
                      warningMsgKband)) {
        cerr << "ERROR - SpolWarnSms::Run" << endl;
        cerr << "  Problems monitoring the kband" << endl;
        warningMsgKband = "#Kband down#";
      }
    }
    
    // combine messages

    string warningMsg;
    if (warningMsgSband.size() > 0 || warningMsgKband.size() > 0) {
      warningMsg += "SPOL-WARNING:";
      warningMsg += warningMsgSband;
      warningMsg += warningMsgKband;
    }

    // write message to dir for SMS
    
    if (warningMsg.size() > 0) {
      if (_params.debug) {
        cerr << "===================== warning message ============================" << endl;
        cerr << warningMsg << endl;
        cerr << "==================================================================" << endl;
      }
      if (_params.write_warnings_to_dir) {
        _writeMessageToDir(now, warningMsg);
      }
    }

    // write to SPDB?

    if (_params.write_warnings_to_spdb) {
      if (warningMsg.size() > 0) {
        _writeMessageToSpdb(now, warningMsg);
      } else {
        string msg("No warnings");
        _writeMessageToSpdb(now, msg);
      }
    }
    
    // sleep
    
    umsleep(_params.monitoring_interval_secs * 1000);

  } // while
  
  return 0;
  
}

/////////////////////////////
// read the status
// update warning msg

int SpolWarnSms::_readStatus(time_t now,
                             char *spdbUrl,
                             int marginSecs,
                             Params::xml_entry_t *entries,
                             int nEntries,
                             string &warningMsg)

{
  
  if (_params.debug) {
    cerr << "==>> Checking status, time: " << DateTime::strm(now) << endl;
  }

  // read in status
  
  DsSpdb spdb;
  if (_params.debug >= Params::DEBUG_VERBOSE) {
    spdb.setDebug();
  }

  if (spdb.getLatest(spdbUrl, marginSecs, 0, 0)) {
    cerr << "ERROR - SpolWarnSms::_readStatus()" << endl;
    cerr << "  Calling getLatest for url: " << spdbUrl << endl;
    cerr << "  margin (secs): " << marginSecs << endl;
    cerr << spdb.getErrStr() << endl;
    return -1;
  }
  
  if (_params.debug >= Params::DEBUG_VERBOSE) {
    cerr << "=======>> Got status" << endl;
    cerr << "url: " << spdbUrl << endl;
    cerr << "Prod label: " << spdb.getProdLabel() << endl;
    cerr << "Prod id:    " << spdb.getProdId() << endl;
    cerr << "N Chunks:   " << spdb.getNChunks() << endl;
  }
  
  const vector<Spdb::chunk_t> &chunks = spdb.getChunks();
  int nChunks = (int) chunks.size();
  if (nChunks < 1) {
    cerr << "ERROR - SpolWarnSms::_readStatus()" << endl;
    cerr << "  No chunks returned from SPDB" << endl;
    cerr << "  Calling getLatest for url: " << spdbUrl << endl;
    cerr << "  margin (secs): " << marginSecs << endl;
    return -1;
  }

  // set the xml string from the chunk data
  
  const Spdb::chunk_t &latest = chunks[chunks.size() - 1];
  time_t validTime = latest.valid_time;
  if (now - validTime > marginSecs) {
    cerr << "ERROR - SpolWarnSms::_readStatus()" << endl;
    cerr << "  Data too old" << endl;
    cerr << "  Calling getLatest for url: " << spdbUrl << endl;
    cerr << "  margin (secs): " << marginSecs << endl;
    cerr << "  now: " << DateTime::strm(now) << endl;
    cerr << "  status valid time: " << DateTime::strm(validTime) << endl;
    return -1;
  }

  string statusXml((const char *) latest.data);
  if (_params.debug >= Params::DEBUG_EXTRA) {
    cerr << "===>> time: " << DateTime::strm(validTime) << endl;
    cerr << "==================== STATUS XML =====================" << endl;
    cerr << statusXml;
    cerr << "=====================================================" << endl;
  }
  
  // check status, append to warning message as needed
  
  for (int ii = 0; ii < nEntries; ii++) {
    
    const Params::xml_entry_t &entry = entries[ii];
    
    switch (entry.entry_type) {
      
      case Params::XML_ENTRY_BOOLEAN:
        _handleBooleanEntry(now, statusXml, entry, warningMsg);
        break;
        
      case Params::XML_ENTRY_NUMBER:
        _handleNumberEntry(now, statusXml, entry, warningMsg);
        break;
        
    } // switch (entry.entry_type)
    
  } // ii

  return 0;

}

/////////////////////////////////
// write message to dir, for SMS

int SpolWarnSms::_writeMessageToDir(time_t now,
                                    const string &warningMsg)
  
{

  if (_params.debug) {
    cerr << "==>> writing warning message, now: " << DateTime::strm(now) << endl;
  }
  
  // compute dir path

  DateTime ntime(now);
  char outputDir[MAX_PATH_LEN];
  sprintf(outputDir, "%s%s%.4d%.2d%.2d",
          _params.warning_message_dir,
          PATH_DELIM,
          ntime.getYear(), ntime.getMonth(), ntime.getDay());
  
  // create day directory as needed

  if (ta_makedir_recurse(outputDir)) {
    int errNum = errno;
    cerr << "ERROR - SpolWarnSms::_writeMessageToDir" << endl;
    cerr << "  Cannot make directory for output files: " << outputDir << endl;
    cerr << strerror(errNum) << endl;
    return -1;
  }

  // compute file path

  char outputPath[MAX_PATH_LEN];
  sprintf(outputPath, "%s%swarning_message_%.2d%.2d%.2d.txt",
          outputDir,
          PATH_DELIM,
          ntime.getHour(), ntime.getMin(), ntime.getSec());
  
  // write message to path
  
  FILE *out = fopen(outputPath, "w");
  if (out == NULL) {
    int errNum = errno;
    cerr << "ERROR - SpolWarnSms::_writeMessageToDir" << endl;
    cerr << "  Cannot open file for writing: " << outputPath << endl;
    cerr << strerror(errNum) << endl;
    return -1;
  }

  fprintf(out, "%s\n", warningMsg.c_str());
  fclose(out);
  

  if (_params.debug >= Params::DEBUG_EXTRA) {
    cerr << "Wrote message to file: " << outputPath << endl;
  }

  return 0;

}

/////////////////////////////
// update the SPDB data base

int SpolWarnSms::_writeMessageToSpdb(time_t now,
                                     const string &warningMsg)

{

  if (_params.debug) {
    cerr << "==>> updating SPDB" << endl;
  }
  
  DsSpdb spdb;
  if (_params.debug >= Params::DEBUG_VERBOSE) {
    spdb.setDebug();
  }
  spdb.addPutChunk(0, now, now + _params.monitoring_interval_secs,
                   warningMsg.size() + 1, warningMsg.c_str());
  
  if (spdb.put(_params.warning_spdb_url,
               SPDB_ASCII_ID, SPDB_ASCII_LABEL)) {
    cerr << "ERROR - SpolWarnSms::_writeMessageToSpdb" << endl;
    cerr << spdb.getErrStr() << endl;
    return -1;
  }
  
  if (_params.debug >= Params::DEBUG_EXTRA) {
    cerr << "Wrote SPDB data to: " << _params.warning_spdb_url << endl;
  }

  return 0;

}

///////////////////////////////////////////
// handle a boolean entry in the status xml

int SpolWarnSms::_handleBooleanEntry(time_t now,
                                     const string &statusXml,
                                     const Params::xml_entry_t &entry,
                                     string &warningMsg)
  
{
  
  // get tag list
  
  vector<string> tags;
  TaStr::tokenize(entry.xml_tags, "<>", tags);
  if (tags.size() == 0) {
    // no tags
    cerr << "WARNING - SpolWarnSms::_handleBooleanEntry" << endl;
    cerr << "  No tags found: " << entry.xml_tags << endl;
    return -1;
  }
  
  // read through the outer tags in status XML
  
  string buf(statusXml);
  for (size_t jj = 0; jj < tags.size(); jj++) {
    string val;
    if (TaXml::readString(buf, tags[jj], val)) {
      cerr << "WARNING - SpolWarnSms::_handleBooleanEntry" << endl;
      cerr << "  Bad tags found in status xml, expecting: "
           << entry.xml_tags << endl;
      return -1;
    }
    buf = val;
  }

  // get the boolean value

  bool bval;
  if (TaXml::readBoolean(buf, bval)) {
    cerr << "ERROR - SpolWarnSms::_handleBooleanEntry" << endl;
    cerr << "  Cannot read bool value, buf: " << buf << endl;
    return -1;
  }

  // check value

  if (bval == entry.ok_boolean) {
    // no problem with this entry
    return 0;
  }
  
  // create message for this entry

  string msg("#");
  msg += entry.label;
  msg += ":";
  msg += buf;
  msg += "#";
  if (_params.debug >= Params::DEBUG_VERBOSE) {
    cerr << "Adding msg: " << msg << endl;
  }

  // add to the warning message

  warningMsg += msg;

  return 0;

}

///////////////////////////////////////////
// handle a number entry in the status xml

int SpolWarnSms::_handleNumberEntry(time_t now,
                                    const string &statusXml,
                                    const Params::xml_entry_t &entry,
                                    string &warningMsg)
  
{
  
  // get tag list
  
  vector<string> tags;
  TaStr::tokenize(entry.xml_tags, "<>", tags);
  if (tags.size() == 0) {
    // no tags
    cerr << "WARNING - SpolWarnSms::_handleNumberEntry" << endl;
    cerr << "  No tags found: " << entry.xml_tags << endl;
    return -1;
  }
  
  // read through the outer tags in status XML
  
  string buf(statusXml);
  for (size_t jj = 0; jj < tags.size(); jj++) {
    string val;
    if (TaXml::readString(buf, tags[jj], val)) {
      cerr << "WARNING - SpolWarnSms::_handleNumberEntry" << endl;
      cerr << "  Bad tags found in status xml, expecting: "
           << entry.xml_tags << endl;
      return -1;
    }
    buf = val;
  }

  // get the numerical value

  double dval;
  if (TaXml::readDouble(buf, dval)) {
    cerr << "WARNING - SpolWarnSms::_handleNumberEntry" << endl;
    cerr << " Cannot read numerical value from: " << buf << endl;
    return -1;
  }
  
  // check value

  if (dval >= entry.valid_lower_limit &&
      dval <= entry.valid_lower_limit) {
    // no problem with this entry
    return 0;
  }
  
  // create message for this entry
  
  string msg("#");
  msg += entry.label;
  msg += ":";
  msg += buf;
  msg += "#";
  if (_params.debug >= Params::DEBUG_VERBOSE) {
    cerr << "Adding msg: " << msg << endl;
  }

  // add to the warning message

  warningMsg += msg;

  return 0;

}
