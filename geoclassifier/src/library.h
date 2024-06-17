#ifndef GEOCLASSIFIER_LIBRARY_H
#define GEOCLASSIFIER_LIBRARY_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct ArgsStruct {
    double** points;
    double** clusters;
    int pointsCount;
    int clustersCount;
    int* clustersBsCount;
} ArgsStruct;

const char* GetClassifiedPointsCh(const char* pointsData);

void FreeMemory(const char* ptr);

#ifdef __cplusplus
}
#endif
#endif //GEOCLASSIFIER_LIBRARY_H
