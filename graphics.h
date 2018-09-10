#ifndef  _GR_H
#define _GR_H

#include <Windows.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <math.h>

using namespace std;

#ifdef _WIN32
static	FILE *gPlot = _popen("gnuplot -persist", "w");
#else
static	FILE *gPlot = popen("gnuplot -persist", "w");
#endif

struct graph {
	int W = 1000*2, H = 300*2;
};

//амплитуда от времени
void drawData(string fileName, int pointsСount, int channels, int64_t deep);
//гистограмма (количество от амплитуды)
void drawAmp(string fileName, int channels, int x, int y);
//амплитуда от времени без отрицательных величин 
void drawSqData(string fileName, int pointsCount, int channels, int64_t deep);
//гистограмма без отрицательных величин
void drawSqAmp(string fileName, int channels, int x, int y);
	//_pclose(gPlot);


#endif // ! _GR_H

