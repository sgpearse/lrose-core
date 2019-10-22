
#include "toolsa/LogStream.hh"
#include "PpiBeamController.hh"
#include "DisplayFieldController.hh"
#include "ColorMap.hh"
#include "ColorBar.hh"


PpiBeamController::PpiBeamController(PpiBeamModel *ppiBeamModel)
{
  LOG(DEBUG) << "enter";
  _current = ppiBeamModel;
  LOG(DEBUG) << "exit";
}

PpiBeamController::~PpiBeamController() {

}

void PpiBeamController::addBeam(const RadxRay *ray,
			   const float start_angle,
			   const float stop_angle,
			   const std::vector< std::vector< double > > &beam_data,
                          DisplayFieldController *displayFieldController)
{
  // TODO: send the controller or the model?
  _beams->addBeam(ray, start_angle, stop_angle, beam_data, displayFieldController);

}




