//
//  main.cpp
//  TSP
//
//  Created by Marty on 12/3/13.
//  Copyright (c) 2013 Evans_Ulrich. All rights reserved.
//

#include <iostream>
#include "Splitter.cpp"
#include <math.h>
#include <vector>
#include <fstream>

using namespace std;

#pragma mark - struct defs

struct CityCoord {
	float x, y;
};

struct TwoCities {
	int indexA, indexB;
};

#pragma mark - function declarations
int calculateDistance(CityCoord coord1, CityCoord coord2);
void readCitiesFile(string fileName);

#pragma mark - variable declarations
vector<CityCoord> cityCoords;

#pragma mark - function implementations
int main(int argc, const char * argv[])
{
	readCitiesFile("example-input-1.txt");

    return 0;
}

int calculateDistance(CityCoord coord1, CityCoord coord2)
{
	return int(round(sqrt( pow(coord1.x - coord2.x, 2) + pow(coord1.y - coord2.y, 2))));
}

void readCitiesFile(string fileName)
{
	vector<string> objectFileNames;
	ifstream fileStream(fileName);
	string line;

	fileStream.clear();
	if (fileStream.is_open())
	{
		getline(fileStream, line);
		Splitter split(line, " ");

		// get names of object files
		while (fileStream.good())
		{
			getline(fileStream, line);
			split.reset(line, " ");
			string tmpFileName = split[0];
			if (tmpFileName.back() == '\r')
			{
				tmpFileName.pop_back();
			}

			objectFileNames.push_back(tmpFileName);
		}
	}
	else
	{
		cout << "\nCouldn't read file\n";
		exit(1);
	}

}
