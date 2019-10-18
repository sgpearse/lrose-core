
#include "toolsa/LogStream.hh"
#include "DisplayFieldController.hh"
#include "DisplayField.hh"
#include "ColorMap.hh"
#include "ColorBar.hh"


DisplayFieldController::DisplayFieldController(DisplayFieldModel *displayFieldModel)
{
  LOG(DEBUG) << "enter";
  _current = displayFieldModel;
  LOG(DEBUG) << "exit";
}

DisplayFieldController::~DisplayFieldController() {

  // free  _workingCopies;
}


vector<string>  DisplayFieldController::getFieldNames() {
  // TODO: this should be in the model ... 
  vector<string> fieldNames;
  for (vector<DisplayField *>::iterator fieldItr = _fields.begin(); fieldItr != _fields.end(); fieldItr++) {
    DisplayField *field = *fieldItr;
    fieldNames.push_back(field->getName());
  }
 
  return fieldNames;

} 

size_t DisplayFieldController::getNFields() {
  return _current->getNFields();
}  

DisplayField *DisplayFieldController::getField(size_t fieldIdx) {
  return _current->getField(fieldIdx);
}  

DisplayField *DisplayFieldController::getField(string fieldName) {
  return _current->getField(fieldName);
}  

size_t DisplayFieldController::getFieldIdx(string fieldName) {
  return _current->getFieldIdx(fieldName);
}  


DisplayField *DisplayFieldController::getSelectedField() {
  return _current->getField(_selectedFieldIdx);
} 

void DisplayFieldController::setSelectedField(string fieldName) {
  LOG(DEBUG) << "enter " << fieldName;
  _selectedFieldName = fieldName;
  LOG(DEBUG) << "exit";
} 

void DisplayFieldController::setSelectedField(size_t fieldIdx) {
  if ((fieldIdx >= 0) && (fieldIdx < _current->getNFields()))
    _selectedFieldIdx = fieldIdx;
  else 
    throw ArgumentException(...);
} 

DisplayField *DisplayFieldController::getFiltered(size_t ifield)
{
  return _current->getFiltered(ifield);
}

/*
bool DisplayFieldController::getChanges() {

   //show();
   return false;

}
*/

void DisplayFieldController::setForLocationClicked(double value, const string &text) {
  _current->setForLocationClicked(value, text);
  // TODO: move this to the model
  for (size_t ii = 0; ii < nFields; ii++) {
    _fields[ii]->setSelectValue(value);
    _fields[ii]->setDialogText(text);
  }
}

void DisplayFieldController::setForLocationClicked(string fieldName, double value, const string &text) {
  _current->setForLocationClicked(fieldName, value, text);
  // TODO: move this to the model
  for (size_t ii = 0; ii < nFields; ii++) {
    if (fieldName == _fields[ii]->getName()) {
      _fields[ii]->setSelectValue(value);
      _fields[ii]->setDialogText(text);
    }
  }
}

ColorMap *DisplayFieldController::_getOriginalColorMap(string fieldName) {

  LOG(DEBUG) << "looking for field ...";
  LOG(DEBUG) << fieldName;

  // find the field or return NULL if not found
  ColorMap *colorMap = NULL;

  bool found = false;
  vector<DisplayField *>::iterator it;
  it = _fields.begin(); 
  while (it != _fields.end() && !found) {
    DisplayField *field = *it;

    string name = field->getName();
    LOG(DEBUG) << "comparing to ...";
    LOG(DEBUG) << name;
    if (name.compare(fieldName) == 0) {
      LOG(DEBUG) << "found color map";
      found = true;
      const ColorMap &colorMapOriginal = field->getColorMap();
      colorMap = new ColorMap(colorMapOriginal);
    }
    it++;
  }
  if (!found) {
    LOG(ERROR) << fieldName;
    LOG(ERROR) << "ERROR - field not found";                                                           
  } 
  return colorMap;
}

// make a working copy of the colorMaps ... 
// return NULL if not found
ColorMap *DisplayFieldController::getColorMap(string fieldName) {
  
  LOG(DEBUG) << "entry " << fieldName;

  return _current->getColorMap(fieldName);
  /*
  ColorMap *workingCopyColorMap = NULL;

  // first, look in the working copies
  map<string, ColorMap *>::iterator it = _workingCopies.find(fieldName);
  if (it != _workingCopies.end()) {
    LOG(DEBUG) << "found in the workingCopies";
    workingCopyColorMap = it->second; 
  } else {
    LOG(DEBUG) << " need to make a copy";
    // if no working copy, then make one and insert it into list
    //if (!found) {
    workingCopyColorMap = _getOriginalColorMap(fieldName);
    if (workingCopyColorMap != NULL) {
      _workingCopies[fieldName] = workingCopyColorMap;
    }
  }
  setSelectedField(fieldName);

  workingCopyColorMap->print(cout);
  
  LOG(DEBUG) << "exit";
  
  return workingCopyColorMap;
  */
}

// make a working copy of the colorMaps ... 
// return NULL if not found
void DisplayFieldController::setColorMap(string fieldName, ColorMap newColorMap) {
  
  LOG(DEBUG) << "entry " << fieldName;

  DisplayField *field = getField(fieldName);
  field->replaceColorMap(newColorMap);

  LOG(DEBUG) << "exit";
}


ColorMap *DisplayFieldController::colorMapMaxChanged(double newValue) {
  LOG(DEBUG) << "entry " << newValue;
  LOG(DEBUG) << "_selectedFieldName " << _selectedFieldName;

  ColorMap *workingVersion = getColorMap(_selectedFieldName);

  if (newValue != workingVersion->rangeMax()) {
    /* create new ColorMap because just setting the range doesn't work
    string currentColorMapName = workingVersion->getName();
    LOG(DEBUG) << "current ColorMap name " << currentColorMapName;
    ColorMap *newColorMap = new ColorMap(workingVersion->rangeMin(), newValue,
					 currentColorMapName);
    delete workingVersion;
    workingVersion = newColorMap;
    _workingCopies[_selectedFieldName] = newColorMap;
    */
    workingVersion->setRangeMax(newValue);
  }
  LOG(DEBUG) << "colorMap after max changed";
  workingVersion->print(cout);
  LOG(DEBUG) << "exit";
  return workingVersion;
}

ColorMap *DisplayFieldController::colorMapMinChanged(double newValue) {
  LOG(DEBUG) << "entry " << newValue;
  LOG(DEBUG) << "_selectedFieldName " << _selectedFieldName;

  ColorMap *workingVersion = getColorMap(_selectedFieldName);

  if (newValue != workingVersion->rangeMin()) {
    workingVersion->setRangeMin(newValue);
  }
  LOG(DEBUG) << "colorMap after min changed";
  workingVersion->print(cout);
  LOG(DEBUG) << "exit";
  return workingVersion;
}

void DisplayFieldController::setColorMapMinMax(string fieldName, double min, double max) {
  LOG(DEBUG) << "entry " << fieldName << ", min=" << min << ", max=" << max;
  _current->setColorMapMinMax(fieldName, min, max);
  LOG(DEBUG) << "exit";
}


void DisplayFieldController::colorMapChanged(string newColorMapName) {
  LOG(DEBUG) << "enter";
  // change the ColorMap for the currently selected field
  ColorMap *workingVersion = getColorMap(_selectedFieldName);

  // maintain the current min, max, step, center points
  ColorMap *colorMap;
  colorMap = new ColorMap(workingVersion->rangeMin(), 
				       workingVersion->rangeMax(), newColorMapName);
  //  newColorMap->setRangeMax(workingVersion->rangeMax());  
  //newColorMap->setRangeMin(workingVersion->rangeMin());  
  // currently only using built in names
  setColorMap(_selectedFieldName, colorMap);

  
  LOG(DEBUG) << "exit";
}

bool DisplayFieldController::backgroundChanged(string fieldName) {
  LOG(DEBUG) << fieldName;
  LOG(DEBUG) << "background changed";
  return false;
}

string DisplayFieldController::getGridColor() {
  return _gridColor;
}

void DisplayFieldController::setGridColor(string colorName) {
  LOG(DEBUG) << "enter " << colorName;
  _gridColor = colorName;
  LOG(DEBUG) << "exit";
}

string DisplayFieldController::getEmphasisColor() {
  return _emphasisColor;
}

void DisplayFieldController::setEmphasisColor(string colorName) {
  _emphasisColor = colorName;
}

string DisplayFieldController::getAnnotationColor() {
  return _annotationColor;
}

void DisplayFieldController::setAnnotationColor(string colorName) {
  _annotationColor = colorName;
}

string DisplayFieldController::getBackgroundColor() {
  return _backgroundColor;
}

void DisplayFieldController::setBackgroundColor(string colorName) {
  _backgroundColor = colorName;
}


