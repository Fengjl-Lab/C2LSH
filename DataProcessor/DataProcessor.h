/*
 * DataProcessor.h
 *
 *  Created on: 2011-9-26
 *      Author: Junhao Gan
 */

#ifndef DATAPROCESSOR_H_
#define DATAPROCESSOR_H_

#include "headers.h"

/**
 * Point Structure
 */
typedef struct _PointT {
	int index; // the index of this point in the dataset list of points
	double *coordinates;
} PointT, *PPointT;

// Generate a random integer in [rangeStart, rangeEnd]
int genRandomInt(int rangeStart, int rangeEnd);

/*
 * Generate random integer dataset in the range of [0, <scale>].
 * <filename> the output file path
 * <n>  the dataset size
 * <d>  the dimensionality of data
 * <qn> the query set size
 */
void generateRandInt(char* filename, int n, int d, int qn, int scale);

// Read a Point from file
PPointT readPoint(FILE *fileHandle);

// Read a Dataset with Format "id <pointsDimension> coordinates"
void readDataSetFromFile(char *filename, int n, int d);

// Output the Ground Truth File
void writeGroundTruth(char* queryFilePath, char* groundTruthFilePath, int nQueries);

/**
 * Process a dataset by randomly removing <num> points and using them to form a query set.
 * Please note that the dataset should have been read by Funtion
 * void readDataSetFromFile(char *filename, int n, int d).
 */
void processDataSet(char *filename, int num, double scale = 1.0);

#endif /* DATAPROCESSOR_H_ */
