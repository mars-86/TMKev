#ifndef TMKEV_TMKEV_H
#define TMKEV_TMKEV_H
#pragma once

#include "keys.h"

#ifdef __linux__
#include "x11ev.h"
#elif _WIN32
#define WINVER 0x0500
#include "winev.h"
#endif // __linux__

#endif // !TMKEV_TMKEV_H
