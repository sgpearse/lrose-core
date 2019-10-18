#ifndef FIELDRENDERERCONTROLLER_H
#define FIELDRENDERERCONTROLLER_H


class FieldRendererController
{

public:

  FieldRendererController();
  ~FieldRendererController();

  void add(FieldRenderer *);
  

private:
 
  vector<FieldRenderer *> _current;
  vector<FieldRenderer *> _working;
 
  

};

#endif
