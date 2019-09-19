#ifndef BoundaryPointEditor_HH
#define BoundaryPointEditor_HH

#include "GeneralDefinitions.hh"
#include "WorldPlot.hh"

#include <QPaintEngine>
#include <QPushButton>
#include <QBrush>
#include <list>

using namespace std;

struct Point
{
   int x;
   int y;

   float distanceTo(int x2, int y2)
   {
       float dx = (float)(x2 - x);
       float dy = (float)(y2 - y);
       return sqrt(dx*dx + dy*dy);
   }

   bool equals(Point other)
   {
	   return(x == other.x && y == other.y);
   }
};

/*
 * BoundaryPointEditor.hh
 *
 *  Created on: Sep 5, 2019
 *      Author: jeff smith
 *  Implemented as a singleton and stores the points for a boundary
 */
class BoundaryPointEditor
{
  public:
	static BoundaryPointEditor* Instance();
	void addPoint(int x, int y);
	void draw(WorldPlot worldPlot, QPainter &painter);
	bool isOverAnyPoint(int worldX, int worldY);
	void moveNearestPointTo(int worldX, int worldY);
	bool isPolygonFinished();
	void clear();
	void save(string path);
	void load(string path);

  private:
	BoundaryPointEditor(){};
	void coutPoints();
	void drawHandle(WorldPlot worldPlot, QPainter &painter, Point point);
	void checkToMovePointToOriginIfVeryClose(Point &pt);

	float CLOSE_DISTANCE = 10;
	QPushButton *_clearBtn;
	QPushButton *_saveBtn;
	vector<Point> points;
	static BoundaryPointEditor* m_pInstance;

	void ReadFromFile(vector<Point> &x, const string &file_name);
};


#endif
