#pragma once
#if defined(ENGINE_EXPORT)
#define API __declspec(dllexport)
#else
#define API __declspec(dllimport)
#endif


#if !defined(Delete)
#define Delete(x) delete x; x = nullptr
#endif

#if !defined(DeleteSafe)
#define DeleteSafe(x) if (x != nullptr) { Delete(x); }
#endif