/*
 * Audio.cpp
 *
 *  Created on: 2011-9-26
 *      Author: Junhao Gan
 */

#include "headers.h"

//Read the Audio Bianry Dataset
void readAudioBinaryDataSet(char *filename, int n, int d) {
	ifstream fin(filename, ios::binary);
	if (!fin) {
		printf("File open error!\n");
		return;
	}

	float temp;

	unsigned header[3]; /* entry size, row, col */
	fin.read((char *) header, sizeof(header));

	int id = 0;
	PPointT p;

	//printf("%d %d %d\n", header[0], header[1], header[2]);

	FAILIF(NULL == (dataSetPoints = (PPointT*)malloc(n * sizeof(PPointT))));

	//	float *test;
	//	FAILIF(NULL == (test = (float*)malloc (sizeof(float) * n * d)));
	//	fin.read((char*) test, sizeof(float) * n * d);
	//
	//	for (int i = 0; i < 10; i++)
	//		printf("%f ", test[i]);
	//	printf("\n");

	while (!fin.eof()) {
		FAILIF(NULL == (p = (PPointT)malloc(sizeof(PointT))));
		FAILIF(NULL == (p->coordinates = (double*)malloc(d * sizeof(double))));
		for (int i = 0; i < d; i++) {
			fin.read((char*) (&temp), sizeof(float));
			p->coordinates[i] = temp;

			// Record the maximum coordinate value.
			if (p->coordinates[i] > maxCoordinate)
				maxCoordinate = p->coordinates[i];
			// Record the minimum coordinate value.
			if (p->coordinates[i] < minCoordinate)
				minCoordinate = p->coordinates[i];
		}
		p->index = id + 1;
		dataSetPoints[id] = p;
		id++;
	}
	fin.close();

	printf("%lf %lf %lf\n", maxCoordinate, minCoordinate, maxCoordinate - minCoordinate);
	//printf("Read over!\n");
}
