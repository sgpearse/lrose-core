#ifndef BoundaryPointEditor_HH
#define BoundaryPointEditor_HH

#include "GeneralDefinitions.hh"
#include "WorldPlot.hh"

#include <QPaintEngine>
#include <QPushButton>
#include <QBrush>
#include <QColor>
#include <QSlider>
#include <list>

using namespace std;

struct Point
{
  float x;
  float y;

  void lerp(Point pt, float amount)
  {
  	x = x + amount * (float)(pt.x - x);
  	y = y + amount * (float)(pt.y - y);
  }

  float distanceTo(float x2, float y2)
  {
    float dx = (float)(x2 - x);
    float dy = (float)(y2 - y);
    return sqrt(dx*dx + dy*dy);
  }

  float distToLineSegment(float x1, float y1, float x2, float y2)
  {
  	float A = x - x1;
  	float B = y - y1;
  	float C = x2 - x1;
  	float D = y2 - y1;

  	float dot = A * C + B * D;
  	float len_sq = C * C + D * D;
  	float param = -1;
		if (len_sq != 0) //in case of 0 length line
				param = dot / len_sq;

		float xx, yy;

		if (param < 0)
		{
			xx = x1;
			yy = y1;
		}
		else if (param > 1)
		{
			xx = x2;
			yy = y2;
		}
		else
		{
			xx = x1 + param * C;
			yy = y1 + param * D;
		}

		float dx = x - xx;
		float dy = y - yy;
		float distance = sqrt(dx * dx + dy * dy);
		return distance;
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

class SegmentUtils
{
public:
	// The main function that returns true if line segment 'p1q1'
	// and 'p2q2' intersect.
	static bool doIntersect(Point p1, Point q1, Point p2, Point q2)
	{
		if ((int)p1.x == (int)q2.x && (int)p1.y == (int)q2.y)  //if two segments share common point, return false
			return(false);

	    // Find the four orientations needed for general and
	    // special cases
	    int o1 = orientation(p1, q1, p2);
	    int o2 = orientation(p1, q1, q2);
	    int o3 = orientation(p2, q2, p1);
	    int o4 = orientation(p2, q2, q1);

	    // General case
	    if (o1 != o2 && o3 != o4)
	        return true;

	    // Special Cases
	    // p1, q1 and p2 are colinear and p2 lies on segment p1q1
	    if (o1 == 0 && onSegment(p1, p2, q1)) return true;

	    // p1, q1 and q2 are colinear and q2 lies on segment p1q1
	    if (o2 == 0 && onSegment(p1, q2, q1)) return true;

	    // p2, q2 and p1 are colinear and p1 lies on segment p2q2
	    if (o3 == 0 && onSegment(p2, p1, q2)) return true;

	     // p2, q2 and q1 are colinear and q1 lies on segment p2q2
	    if (o4 == 0 && onSegment(p2, q1, q2)) return true;

	    return false; // Doesn't fall in any of the above cases
	}
private:
	// Given three colinear points p, q, r, the function checks if
	// point q lies on line segment 'pr'
	static bool onSegment(Point p, Point q, Point r)
	{
	    if ((int)q.x <= max((int)p.x, (int)r.x) && (int)q.x >= min((int)p.x, (int)r.x) &&
	    		(int)q.y <= max((int)p.y, (int)r.y) && (int)q.y >= min((int)p.y, (int)r.y))
	       return true;

	    return false;
	}

	// To find orientation of ordered triplet (p, q, r).
	// The function returns following values
	// 0 --> p, q and r are colinear
	// 1 --> Clockwise
	// 2 --> Counterclockwise
	static int orientation(Point p, Point q, Point r)
	{
	    // See https://www.geeksforgeeks.org/orientation-3-ordered-points/
	    // for details of below formula.
	    int val = ((int)q.y - (int)p.y) * ((int)r.x - (int)q.x) -
	              ((int)q.x - (int)p.x) * ((int)r.y - (int)q.y);

	    if (val == 0) return 0;  // colinear

	    return (val > 0)? 1: 2; // clock or counterclock wise
	}
};



enum class BoundaryToolType
{
  polygon,
  circle,
	brush
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
	void makeCircle(int x, int y);
	void addToBrushShape(int x, int y);
	void addPoint(int x, int y);
	void insertPoint(int x, int y);
	void delNearestPoint(int x, int y);
	void draw(WorldPlot worldPlot, QPainter &painter);
	bool isOverAnyPoint(int worldX, int worldY);
	void moveNearestPointTo(int worldX, int worldY);
	bool isAClosedPolygon();
	void checkToAddOrDelPoint(int x, int y);
	void clear();
	void save(string path);
	void load(string path);
	void setTool(BoundaryToolType tool);
	BoundaryToolType getCurrentTool();
	bool updateScale(double xRange);
	vector<Point> getWorldPoints();
	bool setCircleRadius(int value);
	void setBrushRadius(int value);
	bool getIsCircle();
	int getCircleRadius();
	int getBrushRadius();

  private:
	BoundaryPointEditor(){};
	int getNearestPointIndex(float x, float y, vector<Point> &pts);
	float getNearestDistToLineSegment(int x, int y, int segmentPtIndex1, int segmentPtIndex2);
	void coutPoints();
	void drawPointBox(WorldPlot worldPlot, QPainter &painter, Point point);
	void checkToMovePointToOriginIfVeryClose(Point &pt);
	int getClosestPtIndex(int x, int y);
	int getFurthestPtIndex(int x, int y);
	int erasePointsCloseToXY(int x, int y, int thresholdDistance);
	void reorderPointsSoStartingPointIsOppositeOfXY(int x, int y);
	void appendFirstPointAsLastPoint();
	void eraseLastPoint();
	float getMaxGapInPoints();
	bool doesLastSegmentIntersectAnyOtherSegment(Point &lastPoint);

	float CLOSE_DISTANCE = 10;
	float pointBoxScale = 1;
	Point circleOrigin;
	QPushButton *_clearBtn;
	QPushButton *_saveBtn;
  QSlider *_radiusSlider;
	int circleRadius = 50;
	int brushRadius = 20;
	int brushToolNextPtDistance = 10;
	Point brushLastOrigin;
	QBrush *yellowBrush = new QBrush(QColor(255,255,0));
	void coutMemUsage();
	vector<Point> points;
	vector<Point> mergePoints;
	static BoundaryPointEditor* m_pInstance;
	BoundaryToolType currentTool = BoundaryToolType::polygon;

	void ReadFromFile(vector<Point> &x, const string &file_name);
};


#endif
