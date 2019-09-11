#ifndef BoundaryPointEditor_HH
#define BoundaryPointEditor_HH

#include "GeneralDefinitions.hh"
#include "WorldPlot.hh"

#include <QPaintEngine>
#include <QPushButton>
#include <list>

using namespace std;

struct point
{
   int x;
   int y;

   float distanceTo(int x2, int y2)
   {
       float dx = (float)(x2 - x);
       float dy = (float)(y2 - y);
       return sqrt(dx*dx + dy*dy);
   }
};

/*
 * BoundaryPointEditor.hh
 *
 *  Created on: Sep 5, 2019
 *      Author: jeffsmith
 *  Implemented as a singleton and stores the points for a boundary
 */
class BoundaryPointEditor
{
  public:
	static BoundaryPointEditor* Instance();
	void addPoint(int x, int y);
	void clear();
	void drawLines(WorldPlot worldPlot, QPainter &painter);

  private:
	BoundaryPointEditor(){};
	void coutPoints();
	void checkToMovePointToOriginIfVeryClose(point &pt);

	QPushButton *_clearBtn;
	vector<point> points;
	static BoundaryPointEditor* m_pInstance;
};


#endif
