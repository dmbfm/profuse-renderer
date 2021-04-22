#include "asset.h"
#include "platform.h"
#include "utils.h"

void asset_manager_init(Allocator *a, AssetManager *manager) {
    manager->a           = a;
    manager->raw_assets  = list_new_cap(RawAsset, a, 512);
    manager->initialized = true;
}

int asset_manager_add_raw_asset(AssetManager *am, const char *path, RawAssetType type) {
    RawAsset asset;

    asset.type    = type;
    asset.state   = RawAssetNotLoaded;
    asset.manager = am;
    string_copy_len(asset.path, path, 512);

    int id   = list_len(am->raw_assets);
    asset.id = id;

    list_push(am->raw_assets, asset);

    return id;
}

Result(RawAssetPtr) asset_manager_get_raw_asset(AssetManager *am, int id) {
    if (id < 0 || id >= list_len(am->raw_assets)) {
        return result_error(RawAssetPtr, ERR_INDEX_OUT_OF_RANGE);
    }

    return result_ok(RawAssetPtr, &am->raw_assets[id]);
}

static void raw_asset_read_file_callback(PlatformReadFileStatus status,
                                         usize                  byte_size,
                                         void *                 data,
                                         Allocator *            allocator,
                                         void *                 user_data) {

    RawAsset *    asset   = (RawAsset *)user_data;
    AssetManager *manager = asset->manager;

    asset->state       = RawAssetLoaded;
    asset->data        = data;
    asset->byte_length = byte_size;

    manager->raw_asset_loaded_callback(manager, asset);
}

ErrorCode asset_manager_load_raw_asset(AssetManager *am, int id) {

    Result(RawAssetPtr) r = asset_manager_get_raw_asset(am, id);

    result_raise_error_code(r);

    RawAsset *asset = r.value;

    platform_read_file_async(am->a, asset->path, raw_asset_read_file_callback, (void *)asset);

    return ERR_OK;
}
