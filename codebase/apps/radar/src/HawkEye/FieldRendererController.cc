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

#include "FieldRendererController.hh"

using namespace std;

FieldRendererController::FieldRendererController()
{

}


FieldRendererController::~FieldRendererController()
{
  _fieldRenderers.clear();
}



// add a new FieldRenderer; one FieldRenderer for each field
void FieldRendererController::addFieldRenderer(FieldRenderer *fieldRenderer)
{
  LOG(DEBUG) << "enter";
  _fieldRenderers.push_back(fieldRenderer);
  LOG(DEBUG) << "exit";
}

/*
// add a Beam to each FieldRenderer
void FieldRendererController::addBeam(FieldRenderer *fieldRenderer)
{
  LOG(DEBUG) << "enter";
  _fieldRenderers.push_back(fieldRenderer);
  LOG(DEBUG) << "exit";
}
*/


void FieldRendererController::activateArchiveRendering()
{
  LOG(DEBUG) << "enter";
  LOG(DEBUG) << "_fieldRenderers.size()  = " << _fieldRenderers.size();
  for (size_t ii = 0; ii < _fieldRenderers.size(); ii++) {
    _fieldRenderers[ii]->setBackgroundRenderingOn();
  }
  LOG(DEBUG) << "exit";
}


void FieldRendererController::activateRealtimeRendering(size_t selectedField)
{

  for (size_t ii = 0; ii < _fieldRenderers.size(); ii++) {
    if (ii != selectedField) {
      _fieldRenderers[ii]->activateBackgroundRendering();
    }
  }
}

void FieldRendererController::performRendering(size_t selectedField) {
                                                                                         
  // start the rendering                                                                   
  LOG(DEBUG) << " selectedField = " << selectedField;                                    
  LOG(DEBUG) << "_fieldRenderers.size() = " << _fieldRenderers.size();                     
  for (size_t ifield = 0; ifield < _fieldRenderers.size(); ++ifield) {                     
    LOG(DEBUG) << "ifield " << ifield << " isBackgroundRendered() = "                      
       << _fieldRenderers[ifield]->isBackgroundRendered();                         
    if (ifield == selectedField ||                                                        
        _fieldRenderers[ifield]->isBackgroundRendered()) {                                 
	LOG(DEBUG) << "signaling field " << ifield << " to start";                           
      _fieldRenderers[ifield]->signalRunToStart();                                         
      }                                                                                    
  } // ifield                                                                              
                                                                                           
  // wait for rendering to complete                                                        
                                                                                           
  for (size_t ifield = 0; ifield < _fieldRenderers.size(); ++ifield) {                     
    if (ifield == selectedField ||                                                        
        _fieldRenderers[ifield]->isBackgroundRendered()) {                                 
      _fieldRenderers[ifield]->waitForRunToComplete();                                     
      }                                                                                    
  } // ifield                                                                              
  
}
