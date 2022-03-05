/*
 * Z-buffer for 'morphdemo'
 *
 * The declaration for the Z-buffer is in a separate file so that its
 * location in memory can be precisely controlled using the spec file.
 *
 */
#include <ultra64.h>

#include "bumpmap.h"

unsigned short zbuffer[SCREEN_WD*SCREEN_HT];


