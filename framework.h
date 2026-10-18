#pragma once

#include "targetver.h"
#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files
#include <windows.h>
// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

#include <commctrl.h>
#pragma comment(lib, "comctl32.lib")

// my includes
#include <cstdio>

#define MAX_LOADSTRING 100
#define INIT_SHAPES_COUNT 16

#define SHAPE_THICKNESS 5
#define LINEOO_RADIUS 5

#define BUFFER_SIZE 256

#define IDC_SHAPE_TOOLBAR 10001