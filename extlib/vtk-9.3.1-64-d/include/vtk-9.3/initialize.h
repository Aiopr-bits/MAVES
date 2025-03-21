// initialize.h
#pragma once

#ifdef _WIN32
#ifdef BUILDING_DLL
#define INIT_API __declspec(dllexport)
#else
#define INIT_API __declspec(dllimport)
#endif
#else
#define INIT_API
#endif

extern "C" INIT_API void initialize();