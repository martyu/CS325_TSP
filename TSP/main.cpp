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
#include <list>
#include <fstream>

using namespace std;


#pragma mark - struct defs

struct CityCoord {
	int x, y;
};

struct TwoCities {
	int cityA, cityB;
};

struct CityDistance {
	int distance;
	bool used; // set to true when the city has been added to the path.
};

#pragma mark - function declarations
int calculateDistance(CityCoord coord1, CityCoord coord2);
void readCitiesFile(string fileName);
TwoCities calculateDistancesBetweenCities();
TwoCities closestTwoCities(int cityA, int cityB);
void makePath(TwoCities startingCities);
int distanceBetweenCities(int cityIndexA, int cityIndexB, bool forceOriginal);
void removeCity(int cityIndex);
int calculatePathDistance();
void printPath();
bool pathHasCity(int city);

#pragma mark - variable declarations
vector<CityCoord> cityCoords;
vector<vector<CityDistance>> cityDistances;
list<int> path;
int pathDistance;

#pragma mark - function implementations
int main(int argc, const char * argv[])
{
	readCitiesFile("example-input-1.txt");

	// calculate all the distances between two cities, and find the two nearest neighbors
	TwoCities startingCities = calculateDistancesBetweenCities();

	// make the path
	makePath(startingCities);

	printPath();

	printf("distance: %i\n", calculatePathDistance());

    return 0;
}

void printPath()
{
	list<int>::const_iterator iterator;
	for (iterator = path.begin(); iterator != path.end(); ++iterator)
	{
		printf("%i\n", *iterator);
	}
}

bool pathHasCity(int city)
{
	bool hasCity = false;

	list<int>::const_iterator iterator;
	for (iterator = path.begin(); iterator != path.end(); ++iterator)
	{
		if (*iterator == city)
			hasCity = true;
	}

	return hasCity;
}

int calculatePathDistance()
{
	vector<int> path_vector;

	int distance = 0;
	list<int>::const_iterator iterator;
	for (iterator = path.begin(); iterator != path.end(); iterator++)
	{
		path_vector.push_back(*iterator);
	}

	for (int i = 0; i < path_vector.size()-1; i++)
	{
		distance += distanceBetweenCities(path_vector[i], path_vector[i+1], true);
	}

	distance += distanceBetweenCities(path_vector.front(), path_vector.back(), true);

	return distance;
}

// set force original to true when calculating path distance.
int distanceBetweenCities(int cityIndexA, int cityIndexB, bool forceOriginal)
{
	// the bigger city index has to be first
	if (cityDistances[max(cityIndexA, cityIndexB)][min(cityIndexA, cityIndexB)].used && !forceOriginal)
		return INT_MAX;
	else
		return cityDistances[max(cityIndexA, cityIndexB)][min(cityIndexA, cityIndexB)].distance;
}

void makePath(TwoCities startingCities)
{
	TwoCities currentTwoCities = startingCities;

	path.push_back(currentTwoCities.cityA);
	path.push_back(currentTwoCities.cityB);

	cityDistances[max(currentTwoCities.cityA, currentTwoCities.cityB)][min(currentTwoCities.cityA, currentTwoCities.cityB)].used = true;

	while (path.size() < cityCoords.size())
	{
		if (pathHasCity(currentTwoCities.cityA) && pathHasCity(currentTwoCities.cityB))
		{
			printf("oops\n");
		}

		// find the closest cities to the current two cities we're looking at.
		TwoCities citiesNearestCurrentCities = closestTwoCities(currentTwoCities.cityA, currentTwoCities.cityB);
		int cityNearestToA = citiesNearestCurrentCities.cityA;
		int cityNearestToB = citiesNearestCurrentCities.cityB;

		cityDistances[max(currentTwoCities.cityA, cityNearestToA)][min(currentTwoCities.cityA, cityNearestToA)].used = true;
		cityDistances[max(currentTwoCities.cityB, cityNearestToB)][min(currentTwoCities.cityB, cityNearestToB)].used = true;


		if (distanceBetweenCities(currentTwoCities.cityA, cityNearestToA, false) < distanceBetweenCities(currentTwoCities.cityB, cityNearestToB, false))
		{
			path.push_front(cityNearestToA);
			removeCity(currentTwoCities.cityA);
			currentTwoCities.cityA = cityNearestToA;
		}
		else
		{
			path.push_back(cityNearestToB);
			removeCity(currentTwoCities.cityB);
			currentTwoCities.cityB = cityNearestToB;
		}
	}
}

// call this when the city vertex has two edges.
void removeCity(int cityIndex)
{
	for (int i = cityIndex+1; i < cityDistances.size(); i++)
	{
		cityDistances[i][cityIndex].used = true;
	}

	for (int i = 0; i < cityIndex-1; i++)
	{
		cityDistances[cityIndex][i].used = true;
	}
}

// a is index of city to find nearest two neighbors of.
// find nearest two in case there's a common closest city for two cities.
TwoCities closestTwoCities(int cityA, int cityB)
{
	int pathSize = path.size();


	int closestCityDistA = INT_MAX;
	int closestCityDistB = INT_MAX;
	int closestCityIndexA = INT_MAX;
	int closestCityIndexB = INT_MAX;

/**** find closest city to city A *****/
	for (int cityANext = cityA+1; cityANext < cityDistances.size(); cityANext++)
	{
		int distBetweenAB = distanceBetweenCities(cityA, cityANext, false);
		if (distBetweenAB < closestCityDistA)
		{
			closestCityIndexA = cityANext;
			closestCityDistA = distBetweenAB;
		}
	}

	for (int cityANext = 0; cityANext < cityA-1; cityANext++)
	{
		int distBetweenAB = distanceBetweenCities(cityA, cityANext, false);
		if (distBetweenAB < closestCityDistA)
		{
			closestCityIndexA = cityANext;
			closestCityDistA = distBetweenAB;
		}
	}
/*******/

	if (cityA == 73 && cityB == 18)
	{
		printf("");
	}

/****** find closest city to city B *****/
	for (int cityBNext = cityB+1; cityBNext < cityDistances.size(); cityBNext++)
	{
		// don't add the same city as was added for city A (above)
		if (cityBNext != closestCityIndexA)
		{
			int distBetweenAB = distanceBetweenCities(cityB, cityBNext, false);
			if (distBetweenAB < closestCityDistB)
			{
				closestCityIndexB = cityBNext;
				closestCityDistB = distBetweenAB;
			}
		}
	}

	for (int cityBNext = 0; cityBNext < cityB-1; cityBNext++)
	{
		// don't add the same city as was added for city A (above)
		if (cityBNext != closestCityIndexA)
		{
			int distBetweenAB = distanceBetweenCities(cityB, cityBNext, false);
			if (distBetweenAB < closestCityDistB)
			{
				closestCityIndexB = cityBNext;
				closestCityDistB = distBetweenAB;
			}
		}
	}
/*******/

	TwoCities closestCities;
	closestCities.cityA = closestCityIndexA;
	closestCities.cityB = closestCityIndexB;

	return closestCities;
}

// returns two closest cities to start with
TwoCities calculateDistancesBetweenCities()
{
	int shortestDistance = INT_MAX;
	// starting two cities
	TwoCities closestTwoCities;

	// first array is empty.  placeholder.  since it's a 2D array, the 0 index city's distances will be included in the other's arrays.
	cityDistances.push_back(vector<CityDistance>());

	for (int i = 1; i < cityCoords.size(); i++)
	{
		cityDistances.push_back(vector<CityDistance>());
		for (int j = 0; j < i; j++)
		{
			int cityDistance = calculateDistance(cityCoords[i], cityCoords[j]);
			CityDistance city;
			city.distance = cityDistance;
			cityDistances.back().push_back(city);
			if (cityDistance < shortestDistance)
			{
				shortestDistance = cityDistance;
				closestTwoCities.cityA = i;
				closestTwoCities.cityB = j;
			}
		}
	}

	return closestTwoCities;
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
		while (line.length() > 1)
		{
			Splitter split(line, " ");
			CityCoord coords;
			float num1 = stoi(split[1]);
			float num2 = stoi(split[2]);
			coords.x = num1;
			coords.y = num2;
			cityCoords.push_back(coords);

			getline(fileStream, line);
		}
	}
	else
	{
		cout << "\nCouldn't read file\n";
		exit(1);
	}

}
