#include "BoundaryPointEditor.hh"
#include "PolarManager.hh"

#include <iostream>
#include <fstream>
#include <iterator>
#include <QApplication>

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

void BoundaryPointEditor::setTool(BoundaryToolType tool)
{
	if (tool != currentTool)
		clear();
	currentTool = tool;
}

BoundaryToolType BoundaryPointEditor::getCurrentTool()
{
	return(currentTool);
}


vector<Point> BoundaryPointEditor::getWorldPoints()
{
	return(points);
}

void BoundaryPointEditor::checkToMovePointToOriginIfVeryClose(Point &pt)
{
	if (points.size() > 2 && pt.distanceTo(points[0].x, points[0].y) < CLOSE_DISTANCE)
	{
		pt.x = points[0].x;
		pt.y = points[0].y;
	}
}

void BoundaryPointEditor::addPoint(int x, int y)
{
	currentTool == BoundaryToolType::polygon;

	if (!isAClosedPolygon())
	{
		Point pt;
		pt.x = x;
		pt.y = y;

		checkToMovePointToOriginIfVeryClose(pt);

		points.push_back(pt);
	//	coutPoints();
	}
}

void BoundaryPointEditor::insertPoint(int x, int y)
{
	Point pt;
	pt.x = x;
	pt.y = y;
	int nearestPtIndex = getNearestPointIndex(x, y, points);
	Point nearestPt = points[nearestPtIndex];

	//remember that the last point is just a duplicate of the first point in a closed polygon, so just ignore this point

	//figure out which of the two possible segments (x,y) is closer to
	int afterPtIndex = nearestPtIndex+1;
	if (afterPtIndex >= points.size()-1)
		afterPtIndex = 0;
	float afterPtDistance = pt.distToLineSegment(nearestPt.x, nearestPt.y, points[afterPtIndex].x, points[afterPtIndex].y);

	int beforePtIndex = nearestPtIndex-1;
	if (beforePtIndex < 0)
		beforePtIndex = points.size()-2;
	float beforePtDistance = pt.distToLineSegment(nearestPt.x, nearestPt.y, points[beforePtIndex].x, points[beforePtIndex].y);

	cout << "INSERT POINT! nearestPtIndex=" << nearestPtIndex << ", insert point at " << x << "," << y << endl;
	cout << "afterPtDist=" << afterPtDistance << ", beforePtDist=" << beforePtDistance << ", beforePtIndex=" << beforePtIndex << ", afterPtIndex=" << afterPtIndex << endl;

	if (afterPtDistance <= beforePtDistance)
	{
		if (afterPtIndex == 0)
			points.insert(points.begin() + points.size()-1, pt);
		else
			points.insert(points.begin() + afterPtIndex, pt);
	}
	else
	{
		if (beforePtIndex == points.size()-1)
			points.insert(points.begin() + points.size()-2, pt);
		else
			points.insert(points.begin() + beforePtIndex+1, pt);
	}
}

void BoundaryPointEditor::delNearestPoint(int x, int y)
{
  int nearestPtIndex = getNearestPointIndex(x, y, points);
  if (nearestPtIndex == 0 || nearestPtIndex == points.size()-1)
  {
  	eraseLastPoint();
    points.erase(points.begin());  //erase first point

    //now add closing (last) point that is identical to the new first point
    appendFirstPointAsLastPoint();
  }
  else
    points.erase(points.begin() + nearestPtIndex);
	cout << "delete nearest point to " << x << "," << y << endl;
}

void BoundaryPointEditor::appendFirstPointAsLastPoint()
{
  Point pt;
  pt.x = points[0].x;
  pt.y = points[0].y;
  points.push_back(pt);
}

void BoundaryPointEditor::eraseLastPoint()
{
	points.erase(points.begin() + points.size()-1);
}

int BoundaryPointEditor::getNearestPointIndex(int x, int y, vector<Point> &pts)
{
	int nearestPointIndex;
	float nearestDistance = 99999;

	for (int i=0; i < pts.size(); i++)
	{
		float distance = pts[i].distanceTo(x, y);
		if (distance < nearestDistance)
		{
			nearestDistance = distance;
			nearestPointIndex = i;
		}
	}
	return(nearestPointIndex);
}

void BoundaryPointEditor::moveNearestPointTo(int x, int y)
{
	int nearestPointIndex = getNearestPointIndex(x, y, points);
	points[nearestPointIndex].x = x;
	points[nearestPointIndex].y = y;
	if (nearestPointIndex == 0)
	{
		points[points.size()-1].x = x;
		points[points.size()-1].y = y;
	}
}

bool BoundaryPointEditor::isOverAnyPoint(int x, int y)
{
	for (int i=0; i < points.size(); i++)
		if (points[i].distanceTo(x, y) < CLOSE_DISTANCE)
			return(true);
	return(false);
}

bool BoundaryPointEditor::isAClosedPolygon()
{
	return(currentTool == BoundaryToolType::polygon && (points.size() > 2) && points[0].equals(points[points.size()-1]));
}

void BoundaryPointEditor::coutPoints()
{
	cout << points.size() << " total boundary editor points: " << endl;

	for (int i=0; i < points.size(); i++)
		cout << "(" << points[i].x << " " << points[i].y << ") ";
	cout << endl;
}

void BoundaryPointEditor::draw(WorldPlot worldPlot, QPainter &painter)
{
	painter.setPen(Qt::yellow);
	bool isFinished = isAClosedPolygon();

	for (int i=1; i < points.size(); i++)
	{
		worldPlot.drawLine(painter, points[i-1].x, points[i-1].y, points[i].x, points[i].y);
		if (isFinished && currentTool == BoundaryToolType::polygon)
		  drawPointBox(worldPlot, painter, points[i]);
	}

}

void BoundaryPointEditor::drawPointBox(WorldPlot worldPlot, QPainter &painter, Point point)
{
	double x = point.x;
	double y = point.y;

	int size = 6 * pointBoxScale;
	worldPlot.fillRectangle(painter, *yellowBrush, x-(size/2), y-(size/2), size, size);
}

bool BoundaryPointEditor::updateScale(double xRange)
{
	float newPointBoxScale = xRange / 450;  //450 is the default range/size if no radar data has been loaded
	bool doUpdate = (newPointBoxScale != pointBoxScale);
	pointBoxScale = newPointBoxScale;
	return(doUpdate);  //only do an update if this value has changed
}

void BoundaryPointEditor::clear()
{
	cout << "clear points" << endl;
	points.clear();
}

bool BoundaryPointEditor::setCircleRadius(int value)
{
	circleRadius = value;
	bool resizeExistingCircle = (currentTool == BoundaryToolType::circle && points.size() > 1);
	if (resizeExistingCircle)  //resize existing circle
		makeCircle(circleOrigin.x, circleOrigin.y);
	return(resizeExistingCircle);
}

void BoundaryPointEditor::setSmartBrushRadius(int value)
{
	smartBrushRadius = value;
}

void BoundaryPointEditor::addToSmartBrushShape(int x, int y)
{
	float distToPrevPt = points.size() == 0 ? 0 : smartBrushLastOrigin.distanceTo(x, y);

	if (points.size() == 0)
	{
		int temp = circleRadius;
		circleRadius = smartBrushRadius;
		makeCircle(x, y);
		circleRadius = temp;
		smartBrushLastOrigin.x = x;
		smartBrushLastOrigin.y = y;
	}
	else if (distToPrevPt > 10) //initial circle exists so create second circle and merge them
	{
		reorderPointsSoStartingPointIsOppositeOfXY(x, y);

		//next, delete all the existing points that fall inside the new circle whose origin is (x,y)
		int indexToInsertAt = erasePointsCloseToXY(x, y, smartBrushRadius);
		mergePoints.clear();

		//now insert the new points
		for (double angle=2*PI; angle > 0; angle -= 0.4)
		{
			Point pt;
			pt.x = x + (float)smartBrushRadius * cos(angle);
			pt.y = y + (float)smartBrushRadius * sin(angle);

			//is this new point outside the previous circle? if so, add insert it!
			if (pt.distanceTo(smartBrushLastOrigin.x, smartBrushLastOrigin.y) > smartBrushRadius)
				mergePoints.push_back(pt);
		}

		int nearestIndex = getNearestPointIndex(points[indexToInsertAt].x, points[indexToInsertAt].y, mergePoints);
		nearestIndex++;
		for (int i=0; i < mergePoints.size(); i++)
		{
			int mergePointsIndexToUse = i+nearestIndex;
			if (mergePointsIndexToUse >= mergePoints.size())
				mergePointsIndexToUse -= mergePoints.size();
			points.insert(points.begin() + indexToInsertAt++, mergePoints[mergePointsIndexToUse]);
		}

		smartBrushLastOrigin.x = x;
		smartBrushLastOrigin.y = y;
	}
}

void BoundaryPointEditor::reorderPointsSoStartingPointIsOppositeOfXY(int x, int y)
{
	//get furthest point from (x,y) and make that point[0]
	eraseLastPoint();

	int furthestIndex = getFurthestPtIndex(x, y);
//cout << "furthestIndex=" << furthestIndex << endl;

	vector<Point> tempPoints;
	for (int i=0; i < points.size(); i++)
	{
		int tempIndex = furthestIndex + i;
		if (tempIndex > points.size()-1)
			tempIndex -= points.size();
		tempPoints.push_back(points[tempIndex]);
	}

	points.swap(tempPoints);  //now assign this newly ordered list of points to "points"
	appendFirstPointAsLastPoint();

	//now free memory from tempPoints by swapping with empty vector
	vector<Point> tempVector;
	tempPoints.swap(tempVector);
}

int BoundaryPointEditor::erasePointsCloseToXY(int x, int y, int thresholdDistance)
{
	bool isDone = false;
	int firstIndexErased = -1;

	while (!isDone)
	{
		int indexToErase = -1;
		for (int i=0; i < points.size(); i++)
		{
			if (points[i].distanceTo(x, y) < thresholdDistance)
			{
				indexToErase = i;
				break;
			}
		}

		if (indexToErase != -1)
		{
			points.erase(points.begin() + indexToErase);
			if (firstIndexErased == -1)
				firstIndexErased = indexToErase;
		}
		else
			isDone = true;

		points[points.size()-1].x = points[0].x;
		points[points.size()-1].y = points[0].y;
	}

	return(firstIndexErased);
}


int BoundaryPointEditor::getFurthestPtIndex(int x, int y)
{
	float maxDist = -99999;
	float indexAtMaxDist = -1;

	for (int i=0; i < points.size(); i++)
	{
		float dist = points[i].distanceTo(x, y);
		if (dist > maxDist)
		{
			maxDist = dist;
			indexAtMaxDist = i;
		}
	}

	return(indexAtMaxDist);
}

void BoundaryPointEditor::makeCircle(int x, int y)
{
	points.clear();
//	currentTool == BoundaryToolType::circle;
	circleOrigin.x = x;
	circleOrigin.y = y;

	Point firstPoint;
	//for (double angle=0; angle < 2*PI; angle+=0.4)
	for (double angle=2*PI; angle > 0; angle -= 0.4)
	{
		Point pt;
		pt.x = x + (float)circleRadius * cos(angle);
		pt.y = y + (float)circleRadius * sin(angle);
		if (angle == 2*PI)
			firstPoint = pt;
		points.push_back(pt);
	}

	points.push_back(firstPoint);
}

void BoundaryPointEditor::checkToAddOrDelPoint(int x, int y)
{
	bool isOverExistingPt = isOverAnyPoint(x, y);
	bool isShiftKeyDown = (QApplication::keyboardModifiers().testFlag(Qt::ShiftModifier) == true);
	if (isShiftKeyDown)
	{
		if (isOverExistingPt)
				BoundaryPointEditor::Instance()->delNearestPoint(x, y);
			else
				BoundaryPointEditor::Instance()->insertPoint(x, y);
	}
}

int BoundaryPointEditor::getCircleRadius()
{
	return(circleRadius);
}

int BoundaryPointEditor::getSmartBrushRadius()
{
	return(smartBrushRadius);
}

void BoundaryPointEditor::save(string path)
{
	cout << "BoundaryPointEditor, saving boundary with " << points.size() << " points to " << path << endl;

	FILE *file;
	file = fopen(path.c_str(), "wb");

	int tool;
	if (currentTool == BoundaryToolType::circle)
		tool = 0;
	else if (currentTool == BoundaryToolType::polygon)
		tool = 1;
	else
		tool = 2; //smart brush

	//write the header (five ints)
	fwrite(&tool, sizeof(int), 1, file);
	fwrite(&circleRadius, sizeof(int), 1, file);
	fwrite(&circleOrigin.x, sizeof(int), 1, file);
	fwrite(&circleOrigin.y, sizeof(int), 1, file);
	fwrite(&smartBrushRadius, sizeof(int), 1, file);

	//now write the points
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
		int headerSize = 5 * sizeof(int);
		int size = ftell(file) - headerSize;
		fseek (file, 0, SEEK_SET);
		int numPoints = size / sizeof(Point);

		//read the header
		int tool;
		fread(&tool, sizeof(int), 1, file);
		fread(&circleRadius, sizeof(int), 1, file);
		fread(&circleOrigin.x, sizeof(int), 1, file);
		fread(&circleOrigin.y, sizeof(int), 1, file);
		fread(&smartBrushRadius, sizeof(int), 1, file);

		//now read each point and add to boundary
		points.clear();
		for (int i=0; i < numPoints; i++)
		{
			int x, y;
			fread(&x, sizeof(int), 1, file);
			fread(&y, sizeof(int), 1, file);
			addPoint(x, y);
		}
		fclose (file);

		if (tool == 0)
			currentTool = BoundaryToolType::circle;
		else if (tool == 1)
			currentTool = BoundaryToolType::polygon;
		else
			currentTool = BoundaryToolType::smartBrush;
		cout << "BoundaryPointEditor, read " << points.size() << " points from " << path << endl;
	}
	else
		cout << path << " doesn't exist, skipping..." << endl;
}

/*
void BoundaryPointEditor::coutMemUsage()
{
    double vm_usage     = 0.0;
    double resident_set = 0.0;

    // the two fields we want
    unsigned long vsize;
    long rss;
    {
        std::string ignore;
        std::ifstream ifs("/proc/self/stat", std::ios_base::in);
        ifs >> ignore >> ignore >> ignore >> ignore >> ignore >> ignore >> ignore >> ignore >> ignore >> ignore
                >> ignore >> ignore >> ignore >> ignore >> ignore >> ignore >> ignore >> ignore >> ignore >> ignore
                >> ignore >> ignore >> vsize >> rss;
    }

    long page_size_kb = 1024; // in case x86-64 is configured to use 2MB pages
    vm_usage = vsize / 1024.0;
    resident_set = rss * page_size_kb;
    cout << "VM: " << vm_usage << "; RSS: " << rss << endl;
}
*/
