
#ifndef _LHCORE_C_H_
#define _LHCORE_C_H_

#ifdef _WIN32

#define DLLEXPORT __declspec(dllexport)
#define qword unsigned _int64

#else 

#define DLLEXPORT
#define qword unsigned long long

#endif /* _WIN32 */

#ifndef __cplusplus
#define CEXPORT DLLEXPORT
#else
#define CEXPORT extern "C" DLLEXPORT
#endif

CEXPORT void lhcoreGenerateMap( int *map, int size, int coastwidth, int trees,
                               int lakes, int grave_fields, int drygrass_fields,
                               int trees_in_forest, int lake_area, int gravel_area,
                               int drygrass_area );

CEXPORT void lhcoreSaveMapToFile( int *map, int size, const char *filename );

#endif /* _LHCORE_C_H_ */