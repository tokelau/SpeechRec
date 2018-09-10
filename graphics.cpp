#include <fstream>
#include <iostream>
#include "graphics.h"

void drawData(string fileName, int pointsCount, int channels, int64_t deep) {
	graph g;
	fprintf_s(gPlot, "set terminal png font Arial 8 size %d, %d\n", g.W, g.H);
	// Устанавливаем границы графика.
	fprintf_s(gPlot, "set xrange [0:%d]\n", pointsCount);
	fprintf_s(gPlot, "set yrange [%d:%d]\n", -deep, deep);
	// Добавляем рюшечки
	fprintf(gPlot, "set grid x y\n");
	fprintf(gPlot, "set xlabel \"Sample(num)\"\n");
	fprintf(gPlot, "set ylabel \"Amplitude (Volume)\"\n");

	//оставить size_t?
	for (size_t i = 0; i < channels; i++) {
		string input = "plot \"graphs/" + fileName + "/data_" + to_string(i) + ".dat\" with lines \n";
		string output = "set output \"graphs/" + fileName + "/data_" + to_string(i) + ".png\"\n";
		
		fprintf(gPlot, output.c_str());
		fprintf(gPlot, input.c_str());
	}
}

void drawAmp(std::string fileName, int channels, int x, int y) {
	graph g;
	fprintf_s(gPlot, "set terminal png font Arial 8 size %d, %d\n", g.W, g.H);
	// Устанавливаем границы графика.
	fprintf_s(gPlot, "set xrange [%d:%d]\n", -x, x);
	fprintf_s(gPlot, "set yrange [0:%d]\n", y);
	// Добавляем рюшечки для графика.
	fprintf(gPlot, "set grid x y\n");
	fprintf(gPlot, "set xlabel \"Amplitude (Volume)\"\n");
	fprintf(gPlot, "set ylabel \"Numes\"\n");

	for (size_t i = 0; i < channels; i++) {
		string input = "plot \"graphs/" + fileName + "/amp_" + to_string(i) + ".dat\" with lines \n";
		string output = "set output \"graphs/" + fileName + "/amp_" + to_string(i) + ".png\"\n";

		fprintf(gPlot, output.c_str());
		fprintf(gPlot, input.c_str());
	}
}

void drawSqData(string fileName, int pointsCount, int channels, int64_t deep) {
	graph g;
	fprintf_s(gPlot, "set terminal png font Arial 8 size %d, %d\n", g.W, g.H);
	// Устанавливаем границы графика
	fprintf_s(gPlot, "set xrange [0:%d]\n", pointsCount);
	fprintf_s(gPlot, "set yrange [0:%d]\n", deep);
	// Добавляем рюшечки
	fprintf(gPlot, "set grid x y\n");
	fprintf(gPlot, "set xlabel \"Sample(num)\"\n");
	fprintf(gPlot, "set ylabel \"Amplitude (Volume)\"\n");

	for (size_t i = 0; i < channels; i++) {
		string input = "plot \"graphs/" + fileName + "/sq_data_" + to_string(i) + ".dat\" with lines \n";
		string output = "set output \"graphs/" + fileName + "/sq_data_" + to_string(i) + ".png\"\n";
		fprintf(gPlot, output.c_str());
		fprintf(gPlot, input.c_str());
	}

}

void drawSqAmp(string fileName, int channels, int x, int y) {
	graph g;
	fprintf_s(gPlot, "set terminal png font Arial 8 size %d, %d\n", g.W, g.H);
	//Устанавливаем границы графика.
	fprintf_s(gPlot, "set xrange [0:%d]\n", x);
	fprintf_s(gPlot, "set yrange [0:%d]\n", y);
	// Добавляем рюшечки для графика.
	fprintf(gPlot, "set grid x y\n");
	fprintf(gPlot, "set xlabel \"Amplitude (Volume)\"\n");
	fprintf(gPlot, "set ylabel \"Numes\"\n");

	for (size_t i = 0; i < channels; i++) {
		string input = "plot \"graphs/" + fileName + "/sq_amp_" + to_string(i) + ".dat\" with lines \n";
		string output = "set output \"graphs/" + fileName + "/sq_amp_" + to_string(i) + ".png\"\n";

		fprintf(gPlot, output.c_str());
		fprintf(gPlot, input.c_str());
	}
}
