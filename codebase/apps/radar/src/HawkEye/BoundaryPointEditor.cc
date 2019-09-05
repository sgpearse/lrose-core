#include "BoundaryPointEditor.hh"
#include <iostream>

/*
 * BoundaryPointEditor.cc
 *
 *  Created on: Sep 5, 2019
 *      Author: jeffs
 */

// Global static pointer used to ensure a single instance of the class.
BoundaryPointEditor* BoundaryPointEditor::m_pInstance = NULL;


BoundaryPointEditor* BoundaryPointEditor::Instance()
{
   if (!m_pInstance)   // Only allow one instance of class to be generated.
      m_pInstance = new BoundaryPointEditor;
   return m_pInstance;
}

void BoundaryPointEditor::addPoint(int x, int y)
{
	point pt;
	pt.x = x;
	pt.y = y;
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
	for (int i=1; i < points.size(); i++)
	{
		worldPlot.drawLine(painter, points[i-1].x, points[i-1].y, points[i].x, points[i].y);
	}
}

void BoundaryPointEditor::clear()
{
	points.clear();
}

