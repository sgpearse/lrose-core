

#ifndef SCRIPTEDITORCONTROLLER_HH
#define SCRIPTEDITORCONTROLLER_HH

#include <QObject>
#include "ScriptEditorView.hh"
#include "ScriptEditorModel.hh"
#include "SoloFunctionsModel.hh"
//#include "PolarWidget.hh"
//#include <Radx/RadxVol.hh>
#include "SoloFunctions.hh"

#include <vector>

class ScriptEditorView;

using namespace std;

class ScriptEditorController : public QObject
{

  Q_OBJECT

public:

  ScriptEditorController(ScriptEditorView *view);
  ScriptEditorController(ScriptEditorView *view, ScriptEditorModel *model);

  vector<string> getFieldNames();
  //vector<float> *getData(string fieldName);
  //void setData(string fieldName, vector<float> *data);

  void open(string fileName);

  ScriptEditorModel *getDataModel() {return _currentModel;};
  void volumeUpdated();

signals:
  void volumeChanged(); // const RadxVol &radarDataVolume);

public slots:
  void needFieldNames();
  // void needDataForField(string fieldName, int r, int c);
  void getVolumeChanges();

private:

  ScriptEditorModel *_currentModel;
  ScriptEditorView *_currentView;
  SoloFunctions *_soloFunctions;

};


#endif // SCRIPTEDITORCONTROLLER_H
