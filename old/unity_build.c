#ifdef __wasm__
#define TOOLBOX_NO_STD_LIB
#endif

#define TOOLBOX_RUN_TESTS

#ifdef PROFUSE_DEBUG
#define TOOLBOX_DEBUG
#endif

// TODO: For now hardcode that we'll use OpenGL. Later on support other backends 
// and specify GL versions.
#define PROFUSE_GL

#include "coffee/unity_build.c"
#include "toolbox/unity_build.c"
#include "main.c"

