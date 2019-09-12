#include "BoundaryPointEditor.hh"
#include <iostream>

/*
 * BoundaryPointEditor.cc
 *
 *  Created on: Sep 5, 2019
 *      Author: jeff smith
 */

// Global static pointer used to ensure a single instance of the class.
BoundaryPointEditor* BoundaryPointEditor::m_pInstance = NULL;


BoundaryPointEditor* BoundaryPointEditor::Instance()
{
   if (!m_pInstance)   // Only allow one instance of class to be generated.
      m_pInstance = new BoundaryPointEditor;
   return m_pInstance;
}

void BoundaryPointEditor::checkToMovePointToOriginIfVeryClose(point &pt)
{
	if (points.size() > 1 && pt.distanceTo(points[0].x, points[0].y) < 10)
	{
		pt.x = points[0].x;
		pt.y = points[0].y;
	}
}

void BoundaryPointEditor::addPoint(int x, int y)
{
	point pt;
	pt.x = x;
	pt.y = y;

	checkToMovePointToOriginIfVeryClose(pt);

	points.push_back(pt);
	coutPoints();
}

void BoundaryPointEditor::coutPoints()
{
	cout << points.size() << " total boundary editor points: " << endl;

	for (int i=0; i < points.size(); i++)
		cout << "(" << points[i].x << " " << points[i].y << ") ";
	cout << endl;
}

void BoundaryPointEditor::drawLines(WorldPlot worldPlot, QPainter &painter)
{
	painter.setPen(Qt::yellow);
	for (int i=1; i < points.size(); i++)
		worldPlot.drawLine(painter, points[i-1].x, points[i-1].y, points[i].x, points[i].y);

/* test code
	double x1 = 0;
	double x2 = 0;
	double y1 = 0;
	double y2 = 140;
	cout << "drawing line from (" << x1 << "," << y1 << ") to (" << x2 << "," << y2 << ")" << endl;
	painter.setPen(Qt::yellow);
	worldPlot.drawLine(painter, x1, y1, x2, y2);
	worldPlot.drawLine(painter, x1+1, y1, x2+1, y2);
*/
}

void BoundaryPointEditor::clear()
{
	cout << "clear points" << endl;
	points.clear();
}

void BoundaryPointEditor::save()
{
	cout << "save boundary" << endl;
}
