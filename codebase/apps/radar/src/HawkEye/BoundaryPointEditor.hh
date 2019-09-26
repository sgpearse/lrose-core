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

  float distToLineSegment(float x1, float y1, float x2, float y2)
  {
    // A - the standalone point (x, y)
    // B - start point of the line segment (x1, y1)
    // C - end point of the line segment (x2, y2)
    // D - the crossing point between line from A to BC
    float AB = distBetween(x, y, x1, y1);
    float BC = distBetween(x1, y1, x2, y2);
    float AC = distBetween(x, y, x2, y2);

    // Heron's formula
    float s = (AB + BC + AC) / 2;
    float area = (float) sqrt(s * (s - AB) * (s - BC) * (s - AC));

    // but also area == (BC * AD) / 2
    // BC * AD == 2 * area
    // AD == (2 * area) / BC
    // TODO: check if BC == 0
    float AD = (2 * area) / BC;

    return AD;
  }

  float distBetween(float x, float y, float x1, float y1)
  {
    float xx = x1 - x;
    float yy = y1 - y;

    return (float) sqrt(xx * xx + yy * yy);
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
	void insertPoint(int x, int y);
	void delNearestPoint(int x, int y);
	void draw(WorldPlot worldPlot, QPainter &painter);
	bool isOverAnyPoint(int worldX, int worldY);
	void moveNearestPointTo(int worldX, int worldY);
	bool isPolygonFinished();
	void clear();
	void save(string path);
	void load(string path);

  private:
	BoundaryPointEditor(){};
	int getNearestPointIndex(int x, int y);
	float getNearestDistToLineSegment(int x, int y, int segmentPtIndex1, int segmentPtIndex2);
	void coutPoints();
	void drawPointBox(WorldPlot worldPlot, QPainter &painter, Point point);
	void checkToMovePointToOriginIfVeryClose(Point &pt);

	float CLOSE_DISTANCE = 10;
	QPushButton *_clearBtn;
	QPushButton *_saveBtn;
	vector<Point> points;
	static BoundaryPointEditor* m_pInstance;

	void ReadFromFile(vector<Point> &x, const string &file_name);
};


#endif
