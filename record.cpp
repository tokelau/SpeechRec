#include <iostream>
#include <fstream>
#include <vector>
#include <stdint.h>
#include "record.h"

void Record::read() {
	/*заменить на какую-то одну функцию о всеми похожими проверками
	* try-catch?? */
	if (fileName.empty()) {
		std::cout << "Call init() function to initialize name of WAV file." << std::endl;
		return;
	}
	std::string name = (std::string("sounds/") + fileName + std::string(".wav"));
	std::ifstream in(name, std::ios::binary);
	if (!in.good()) {
		std::cout << "Failed file." << std::endl;
		return;
	}
	in.read((char*)(&header), sizeof(header));

	if (header.audioFormat != 1) {
		std::cout << "The sample data for the wave file is stored wrong." << std::endl;
		return;
	}

	framesByChannel = (float)header.subchunk2Size / (header.bitsPerSample / 8 ) / header.numChannels;

	////какую лучше подобрать стркутуру????
	////vector, double*
	//выделяем память
	data = (int16_t*)malloc(sizeof(int16_t) * framesByChannel * header.numChannels);
	for (int i = 0; i < framesByChannel * header.numChannels; i++) data[i] = 0;
	max = min = 0;
	for (int i = 0; i < 1500 || in.good(); i++) {
		in.read((char*)(&data[i]), 2);
		//std::cout << i << ": ";
		//showBits(data[i]);
		if (data[i] > max) max = data[i];
		//if (data[i] < min) min = data[i];
		//if (data[i] != 0) 
		//	std::cout << i << " " << (int)data[i] << std::endl;
		//if (data[i] != 0) std::cout << i << " " << (int)data[i] << std::endl;
	}
	//std::cout << max << std::endl;
	//std::cout << header.blockAlign << std::endl;
	if (header.bitsPerSample == 8) {
		deep = 300;
	}
	else if (header.bitsPerSample == 16) {
		deep = 33000;
	}
	else if (header.bitsPerSample == 24) {
		deep = 16800000;
	}
	else if (header.bitsPerSample == 32) {
		deep = 4300000000;
	}

	durationInSeconds = (float) header.subchunk2Size / (header.bitsPerSample / 8) / header.numChannels / header.sampleRate; 
	in.close();
}

void Record::showInf() {
	if (!header.chunkSize) {
		std::cout << "Header is empty" << std::endl;
		return;
	}
	std::cout << "Frames by channel: " << framesByChannel << std::endl;
	std::cout << "Channels: " << header.numChannels << std::endl;
	std::cout << "Friequency: " << header.sampleRate << std::endl;
	std::cout << "Deep in bytes: " << header.bitsPerSample << std::endl; 
	std::cout << "Size: " << header.subchunk2Size << std::endl;
	//std::cout << "Duration in seconds: " << durationInSeconds << std::endl;
}

//void Record::showBits(int num) {
//	int count = 0;
//	for (int i = sizeof(int)*8; i >= 1; i--) {
//		//std::cout << (1 << i) << std::endl;
//		std::cout << ((num & (1 << (i - 1))) ? 1 : 0);
//		count++;
//		if (count == 4) {
//			std::cout << ' ';
//			count = 0;
//		}
//	}
//	std::cout << std::endl;
//}

void Record::writeData() {
	for (size_t i = 1; i <= header.numChannels; i++) {
		std::string out_name = std::string("graphs/") + fileName + "/data_" + to_string(i-1) + std::string(".dat");
		std::ofstream out(out_name);
		for (size_t j = 1; j <= framesByChannel; j++) {
			out << ((j - 1)*header.numChannels + i - 1) << " " << data[(j-1)*header.numChannels + i - 1] << '\n';
		}
		out.close();
	}
}

void Record::writeSqData() {
	for (size_t i = 1; i <= header.numChannels; i++) {
		std::string out_name = std::string("graphs/") + fileName + "/sq_data_" + to_string(i - 1) + std::string(".dat");
		std::ofstream out(out_name);
		for (size_t j = 1; j <= framesByChannel; j++) {
			if (data[(j - 1)*header.numChannels + i - 1] < 0)
				out << ((j - 1)*header.numChannels + i - 1) << " " << -data[(j - 1)*header.numChannels + i - 1] << '\n';
				
			else 
				out << ((j - 1)*header.numChannels + i - 1) << " " << data[(j - 1)*header.numChannels + i - 1] << '\n';
		}
		out.close();
	}
}

void Record::writeAmp(int N) {
	y_amp = 0;
	for (size_t i = 1; i <= header.numChannels; i++) {
		int maxAmp, minAmp;
		maxAmp = -deep;
		minAmp = deep;

		std::string out_name = std::string("graphs/") + fileName + "/amp_" + to_string(i - 1) + std::string(".dat");
		std::ofstream out(out_name);
		
		/*находим максимальную и минимальную амплитуду, чтобы поделить на бины*/
		for (int j = 1; j <= framesByChannel; j++) {
			if (data[(j - 1)*header.numChannels + i - 1] > maxAmp) {
				maxAmp = data[(j - 1)*header.numChannels + i - 1];
			}
			else if (data[(j - 1)*header.numChannels + i - 1] < minAmp) {
				minAmp = data[(j - 1)*header.numChannels + i - 1];
			}
		}

		N = N == 0 ? (maxAmp - minAmp) + 1 : N;
		int bin = (maxAmp - minAmp + 1) / N + 1;

		int* amp_num = new int[N];
		for (int j = 0; j < N; j++) {
			amp_num[j] = 0;
		}

		/*считаем количество амплитуд*/
		for (int j = 1; j <= framesByChannel; j++) {
			int index = data[(j - 1)*header.numChannels + i - 1];
			index = index < 0 ? (index + -1 * minAmp) / bin + 1 : (index + -1 * minAmp) / bin;
			//std::cout << index << std::endl;
			amp_num[index] += 1;
		}

		for (int j = 1; j <= (maxAmp - minAmp) + 1; j++) {
			int index = (j - 1) / bin;
			out << j + minAmp << " " << amp_num[index] << "\n";

			if (amp_num[index] > y_amp) {
				y_amp = amp_num[index];
			}
		}

		x_amp = -minAmp > maxAmp ? minAmp : maxAmp;
		out.close();
	}
}

void Record::writeSqAmp(int N) {
	y_amp = 0;
	for (size_t i = 1; i <= header.numChannels; i++) {
		int maxAmp, minAmp;
		maxAmp = -deep;
		minAmp = 0;

		std::string out_name = std::string("graphs/") + fileName + "/sq_amp_" + to_string(i - 1) + std::string(".dat");
		std::ofstream out(out_name);

		/*находим максимальную амплитуду, чтобы поделить на бины*/
		for (int j = 1; j <= framesByChannel; j++) {
			if (data[(j - 1)*header.numChannels + i - 1] > maxAmp) {
				maxAmp = data[(j - 1)*header.numChannels + i - 1];
			}
		}

		N = N == 0 ? (maxAmp - minAmp) + 1 : N;
		int bin = (maxAmp - minAmp + 1) / N + 1;

		int* amp_num = new int[N];
		for (int j = 0; j < N; j++) {
			amp_num[j] = 0;
		}

		/*считаем количество амплитуд*/
		for (int j = 1; j <= framesByChannel; j++) {
			int index = data[(j - 1)*header.numChannels + i - 1];
			index = index < 0 ? -index : index;
			index = (index + -1 * minAmp) / bin;
			//std::cout << index << std::endl;
			amp_num[index] += 1;
		}

		for (int j = 0; j < N; j++) {
			//int index = j / bin;
			out << j << " " << amp_num[j] << "\n";

			if (amp_num[j] > y_amp) {
				y_amp = amp_num[j];
			}
		}

		x_amp = maxAmp;
		out.close();
	}
}