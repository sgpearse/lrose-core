#include "BoundaryPointEditor.hh"
#include "PolarManager.hh"
#include <iostream>
#include <fstream>
#include <iterator>

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

void BoundaryPointEditor::checkToMovePointToOriginIfVeryClose(Point &pt)
{
	if (points.size() > 1 && pt.distanceTo(points[0].x, points[0].y) < 10)
	{
		pt.x = points[0].x;
		pt.y = points[0].y;
	}
}

void BoundaryPointEditor::addPoint(int x, int y)
{
	Point pt;
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
	{
		worldPlot.drawLine(painter, points[i-1].x, points[i-1].y, points[i].x, points[i].y);
		drawHandle(worldPlot, painter, points[i]);
	}
}

void BoundaryPointEditor::drawHandle(WorldPlot worldPlot, QPainter &painter, Point point)
{

}

void BoundaryPointEditor::clear()
{
	cout << "clear points" << endl;
	points.clear();
}

void BoundaryPointEditor::save(string path)
{
	cout << "BoundaryPointEditor, saving boundary with " << points.size() << " points to " << path << endl;

	FILE *file;
	file = fopen(path.c_str(), "wb");
	for (int i=0; i < points.size(); i++)
	{
		fwrite(&points[i].x, sizeof(int), 1, file);
		fwrite(&points[i].y, sizeof(int), 1, file);
	}
	fclose(file);
}

void BoundaryPointEditor::load(string path)
{
	ifstream infile(path);
	if (infile.good())
	{
		FILE *file;
		file = fopen(path.c_str(), "rb");

		//get number of points in file (from file size)
		fseek(file, 0L, SEEK_END);
		int size = ftell(file);
		fseek (file, 0, SEEK_SET);
		int numPoints = size / sizeof(Point);

		//read each point and add to boundary
		points.clear();
		for (int i=0; i < numPoints; i++)
		{
			int x, y;
			fread(&x, sizeof(int), 1, file);
			fread(&y, sizeof(int), 1, file);
			addPoint(x, y);
		}
		fclose (file);

		cout << "BoundaryPointEditor, read " << points.size() << " points from " << path << endl;
	}
	else
		cout << path << " doesn't exist, skipping..." << endl;
}

