#pragma once

#define TO_STR(x) #x
#define STR(x) TO_STR(x)

#ifndef PRODUCT_NAME
#define PRODUCT_NAME "octo"
#endif

#ifndef VERSION_MAJOR
#define VERSION_MAJOR 0
#endif

#ifndef VERSION_MINOR
#define VERSION_MINOR 1
#endif

#ifndef VERSION_PATCH
#define VERSION_PATCH 0
#endif

#ifndef OCTO_VERSION
#define OCTO_VERSION STR(VERSION_MAJOR) "." STR(VERSION_MINOR) "." STR(VERSION_PATCH)
#endif
