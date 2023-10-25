/*
 * Global.h
 *
 *  Created on: 2011-9-26
 *      Author: Junhao Gan
 */
#ifndef GLOBAL_H_
#define GLOBAL_H_

#ifndef GLOBAL_CPP
#define DECLARE_EXTERN extern
#define EXTERN_INIT(x)
#else
#define DECLARE_EXTERN
#define EXTERN_INIT(x) x
#endif

#define FAILIF(b) {if (b) {fprintf(stderr, "FAILIF triggered on line %d, file %s.\n", __LINE__, __FILE__); exit(1);}}
#define ASSERT(b) {if (!(b)) {fprintf(stderr, "ASSERT failed on line %d, file %s.\n", __LINE__, __FILE__); exit(1);}}

/**
 * Global variables
 */
// Dataset size
DECLARE_EXTERN int nPoints EXTERN_INIT(= 0);
// Data point's dimensionality
DECLARE_EXTERN int pointsDimension EXTERN_INIT(= 0);
// Dataset pointer
DECLARE_EXTERN PPointT *dataSetPoints EXTERN_INIT(= NULL);
// Query set pointer
DECLARE_EXTERN PPointT *querySetPoints EXTERN_INIT(= NULL);
// The maximum coordinate value
DECLARE_EXTERN double maxCoordinate EXTERN_INIT(= -RAND_MAX);
// The minimum coordinate value
DECLARE_EXTERN double minCoordinate EXTERN_INIT(= RAND_MAX);
// The maximum sqrLength value
DECLARE_EXTERN double maxSqrLength EXTERN_INIT(= -RAND_MAX);
// The minimum sqrLength value
DECLARE_EXTERN double minSqrLength EXTERN_INIT(= RAND_MAX);

#endif /* GLOBAL_H_ */
