#ifndef __ASSET_H
#define __ASSET_H 

#include "allocator.h"
#include "list.h"

struct AssetManager;

typedef enum {
    RawAssetNotLoaded,
    RawAssetLoading,
    RawAssetLoaded,
} RawAssetState;

typedef enum {
    RawAssetPfa,
    RawAssetPng
} RawAssetType;

typedef struct RawAsset {
    char path[512];
    int id;
    void *data;
    usize byte_length;
    RawAssetState state;
    RawAssetType type;
    struct AssetManager *manager;
} RawAsset;

typedef RawAsset* RawAssetPtr;

result_make_type(RawAssetPtr);

typedef void (*RawAssetLoadedCallback)(struct AssetManager *m, RawAsset *asset);

typedef struct AssetManager {
    Allocator *a;
    RawAsset *raw_assets;
    RawAssetLoadedCallback raw_asset_loaded_callback;

    boolean initialized;
} AssetManager;

void asset_manager_init(Allocator *a, AssetManager *manager);
int asset_manager_add_raw_asset(AssetManager *am, const char *path, RawAssetType type);
ErrorCode asset_manager_load_raw_asset(AssetManager *am, int id);

#endif /* __ASSET_H */
