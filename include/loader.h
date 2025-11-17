#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pipeline.h"

// -------------------------------------------
// Simple OBJ Loader (supports v, vn, f)
// -------------------------------------------
VertexData load_obj(const char *filename);