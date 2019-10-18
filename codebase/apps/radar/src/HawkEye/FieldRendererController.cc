



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
