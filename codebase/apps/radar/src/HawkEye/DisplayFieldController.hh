#ifndef DISPLAYFIELDCONTROLLER_H
#define DISPLAYFIELDCONTROLLER_H

#include "DisplayFieldModel.hh"
//#include "ColorMap.hh"

class DisplayFieldController
{

public:

  DisplayFieldController(DisplayFieldModel *model);
  ~DisplayFieldController();

  vector<string> getFieldNames();
  string getSelectedField();
  void setSelectedField(string fieldName);
  //  bool getChanges();
  ColorMap *getColorMap(string fieldName);
  void setColorMap(string fieldName, ColorMap *newColorMap);
  void colorMapChanged(string newColorMapName);
  bool backgroundChanged(string fieldName);

  //  void colorMapMaxChanged(ColorMap *newColorMap);
  ColorMap *colorMapMaxChanged(double newValue);
  ColorMap *colorMapMinChanged(double newValue);

  string getGridColor();
  void setGridColor(string colorName);

  string getEmphasisColor();
  void setEmphasisColor(string colorName);

  string getAnnotationColor();
  void setAnnotationColor(string colorName);

  string getBackgroundColor();
  void setBackgroundColor(string colorName);

private:
 
  DisplayFieldModel *_current;
  DisplayFieldModel *_working;
 
  

};

#endif
