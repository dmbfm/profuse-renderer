#if !defined(__TG_WASM_H)
#define __TG_WASM_H

#include "tg_types.h"

#ifdef __wasm__
#define export __attribute__((used,visibility("default")))
#define export_named(name) __attribute__((used, visibility("default"), export_name(#name)))
#else
#define export
#define export_named(name)
#endif
#endif
