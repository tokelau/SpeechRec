#ifndef WAVE_H
#define WAVE_H

#include <iostream>
#include <fstream>
#include <string>
#include <stdint.h>
#include "graphics.h"

struct header {
	/*�������� ������� �RIFF� � ASCII ��������� 
	(0x52494646 � big-endian �������������). 
	�������� ������� RIFF-�������.*/
	char chunkId[4];

	/*	��� ���������� ������ �������, ������� 
	� ���� �������. ����� ������, ��� ������ 
	����� � 8, �� ����, ��������� ���� chunkId
	� chunkSize.*/
	unsigned long chunkSize;

	/*�������� ������� �WAVE�
	(0x57415645 � big-endian �������������)*/
	char format[4];

	/*�������� ������� �fmt �
	(0x666d7420 � big-endian �������������)*/
	char subchunk1Id[4];

	/*16 ��� ������� PCM. ��� ���������� ������ 
	����������, ������� � ���� �������.*/
	unsigned long subchunk1Size;

	/*����� ������, ������ ������ ����� �������� ����� http://audiocoding.ru/wav_formats.txt
	��� PCM = 1 (�� ����, �������� �����������).
	��������, ������������ �� 1, ���������� ��������� ������ ������.*/
	unsigned short audioFormat;

	/*���������� �������. ���� = 1, ������ = 2 � �.�.*/
	unsigned short numChannels;

	/*������� �������������. 8000 ��, 44100 �� � �.�.*/
	unsigned long sampleRate;

	/*���������� ����, ���������� �� ������� ���������������.*/
	unsigned long byteRate;

	/*���������� ���� ��� ������ ������, ������� ��� ������.*/
	unsigned short blockAlign;

	/*���������� ��� � ������. ��� ���������� ��������� ���
	�������� ��������. 8 ���, 16 ��� � �.�.*/
	unsigned short bitsPerSample;

	/*	�������� ������� �data� (0x64617461 � big-endian 
	�������������)*/
	char subchunk2Id[4];

	/*���������� ���� � ������� ������.*/
	uint32_t subchunk2Size;
};

class Record {
	header header;
	std::string fileName;
	int framesByChannel;
	float durationInSeconds;
	int16_t* data; //������ � �������
	int64_t deep; //������� �����������, ��� ���������� ��������
	void writeData(); //������� �������� ��������� � ����
	void writeSqData(); //������� ������������� �������� ��������� � ����
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