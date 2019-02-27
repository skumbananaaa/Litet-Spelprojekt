#pragma once
#if defined(ENGINE_EXPORT)
#define API __declspec(dllexport)
#else
#define API __declspec(dllimport)
#endif

#define NUM_CLIP_DISTANCES 8

#if !defined(Delete)
#define Delete(x) delete x; x = nullptr
#endif

#if !defined(DeleteArr)
#define DeleteArr(x) delete[] x; x = nullptr
#endif

#if !defined(DeleteSafe)
#define DeleteSafe(x) if (x != nullptr) { Delete(x); }
#endif

#if !defined(DeleteArrSafe)
#define DeleteArrSafe(x) if (x != nullptr) { DeleteArr(x); }
#endif

#define FORWARD_RENDER_PATH
//#define DEFERRED_RENDER_PATH

#define PRINT_CPU_DEBUG_DATA

#define MAX_NUM_BONES_PER_VERT 4
#define MAX_NUM_BONES 60
#define SHADOW_SIZE 2048
#define REFLECTION_SIZE 512