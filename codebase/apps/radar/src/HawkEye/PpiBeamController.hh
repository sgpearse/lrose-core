#ifndef PPIBEAMCONTROLLER_H
#define PPIBEAMCONTROLLER_H


class PpiBeamController
{

public:

  PpiBeamController();
  ~PpiBeamController();

  void add(PpiBeam *);
  

private:
 
  vector<PpiBeam *> _current;
  vector<PpiBeam *> _working;
 
  

};

#endif
