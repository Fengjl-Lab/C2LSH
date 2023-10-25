/*
 * Mnist.h
 *
 *  Created on: 2011-9-26
 *      Author: Junhao Gan
 */

#ifndef MNIST_H_
#define MNIST_H_

/**
 * Read Raw Mnist Dataset
 */
void readMnistBinaryDataSet(char *trainfilename, char *queryfilename, int n, int n2, int d);

/**
 * Output the objective formating Mnist dataset and query set.
 */
void writeMnistDatasetAndQueryset(char* filename, int targetDimension, double scale = 1.0);


#endif /* MNIST_H_ */
