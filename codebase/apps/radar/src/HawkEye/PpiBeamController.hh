#ifndef PPIBEAMCONTROLLER_H
#define PPIBEAMCONTROLLER_H

#include "PpiBeam.hh"

class PpiBeamController
{

public:

  PpiBeamController();
  ~PpiBeamController();

  void add(PpiBeam *);
  

private:
 
  vector<PpiBeam *> _beams;
  //  vector<PpiBeam *> _working;
 
  

};

#endif
