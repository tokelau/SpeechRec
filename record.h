#ifndef WAVE_H
#define WAVE_H

#include <iostream>
#include <fstream>
#include <string>
#include <stdint.h>
#include "graphics.h"

struct header {
	/*Содержит символы “RIFF” в ASCII кодировке 
	(0x52494646 в big-endian представлении). 
	Является началом RIFF-цепочки.*/
	char chunkId[4];

	/*	Это оставшийся размер цепочки, начиная 
	с этой позиции. Иначе говоря, это размер 
	файла – 8, то есть, исключены поля chunkId
	и chunkSize.*/
	unsigned long chunkSize;

	/*Содержит символы “WAVE”
	(0x57415645 в big-endian представлении)*/
	char format[4];

	/*Содержит символы “fmt “
	(0x666d7420 в big-endian представлении)*/
	char subchunk1Id[4];

	/*16 для формата PCM. Это оставшийся размер 
	подцепочки, начиная с этой позиции.*/
	unsigned long subchunk1Size;

	/*Аудио формат, полный список можно получить здесь http://audiocoding.ru/wav_formats.txt
	Для PCM = 1 (то есть, Линейное квантование).
	Значения, отличающиеся от 1, обозначают некоторый формат сжатия.*/
	unsigned short audioFormat;

	/*Количество каналов. Моно = 1, Стерео = 2 и т.д.*/
	unsigned short numChannels;

	/*Частота дискретизации. 8000 Гц, 44100 Гц и т.д.*/
	unsigned long sampleRate;

	/*Количество байт, переданных за секунду воспроизведения.*/
	unsigned long byteRate;

	/*Количество байт для одного сэмпла, включая все каналы.*/
	unsigned short blockAlign;

	/*Количество бит в сэмпле. Так называемая “глубина” или
	точность звучания. 8 бит, 16 бит и т.д.*/
	unsigned short bitsPerSample;

	/*	Содержит символы “data” (0x64617461 в big-endian 
	представлении)*/
	char subchunk2Id[4];

	/*Количество байт в области данных.*/
	uint32_t subchunk2Size;
};

class Record {
	header header;
	std::string fileName;
	int framesByChannel;
	float durationInSeconds;
	int16_t* data; //массив с данными
	int64_t deep; //глубина кодирования, для построения графиков
	void writeData(); //запишет значения параметра в файл
	void writeSqData(); //запишет положительные значения параметра в файл
	void writeAmp(int N);
	void writeSqAmp(int N);
	int max, min;
	int x_amp, y_amp;
public:
	Record() { }
	Record(std::string _fileName) { init(_fileName);  read(); }
	void init(std::string _fileName) { fileName = _fileName; }
	void read();
	void showInf();
	//void showBits(int num);
	void makeData() {
		writeData();
		drawData(this->fileName, framesByChannel, header.numChannels, deep);
	}
	void makeSqData() {
		writeSqData();
		drawSqData(fileName, framesByChannel, header.numChannels, deep);
	}
	void makeAmp(int N = 0) {
		writeAmp(N);
		drawAmp(this->fileName, header.numChannels, x_amp, y_amp);
	}
	void makeSqAmp(int N = 0) {
		writeSqAmp(N);
		drawSqAmp(this->fileName, header.numChannels, x_amp, y_amp);
	}
};

#endif