#ifndef ASSET_LOADER_H_
#define ASSET_LOADER_H_
#include "Parsers/CfgFile.h"
//#include "Parsers/TgxFile.h"
//#include "Parsers/Gm1File.h"
#include "Parsers/AniFile.h"
#include "Parsers/MlbFile.h"
#include "Parsers/TexFile.h"
//#include "Rendering/BinkVideo.h"
#include "FileSystem.h"
#include <type_traits>
#include <string>
#include <unordered_map>
#include <algorithm>

class AssetLoader {
 public:
  // using declarations
  using Path = ghc::filesystem::path;
  using FileName = Path;
  //  using Tgx = Sourcehold::Parsers::TgxFile;
  // using Gm1 = Sourcehold::Parsers::Gm1File;
  using Ani = Sourcehold::Parsers::AniFile;
  // using Bik = Sourcehold::Rendering::BinkVideo;
  using Cfg = Sourcehold::Parsers::CfgFile;
  using Mlb = Sourcehold::Parsers::MlbFile;
  using Tex = Sourcehold::Parsers::TexFile;

  // TODO(seidl): replace with c++20 concepts
  // template <typename AssetType>
  // struct IsAsset {
  //  static constexpr bool value =               //
  //      std::is_same<AssetType, Tgx>::value     //
  //      || std::is_same<AssetType, Gm1>::value  //
  //      || std::is_same<AssetType, Ani>::value  //
  //      || std::is_same<AssetType, Bik>::value  //
  //      ;
  //};

  // typedef for a asset handle (shared ptr)
  // template <typename AssetType>
  // using AssetHandle = std::shared_ptr<AssetType>;

  //// typedef for a chache (unordered_map of weak ptrs )
  // template <typename AssetType>
  // using Cache_t = std::unordered_map<std::string, std::weak_ptr<AssetType>>;

  // Constructor
  AssetLoader();
  AssetLoader(const AssetLoader& other) = delete;
  AssetLoader& operator=(const AssetLoader& other) = delete;
  AssetLoader& operator=(AssetLoader&& other) = delete;
  ~AssetLoader() = default;

  //// Loads an asset and returns a handle to it
  // template <typename AssetType>
  // typename std::enable_if_t<IsAsset<AssetType>::value,
  // AssetHandle<AssetType>> Load(const FileName& file);

  //// Caches an asset
  // template <typename AssetType>
  // typename std::enable_if_t<IsAsset<AssetType>::value, void>  //
  // Cache(const FileName& file);

  //// Releases a cached asset
  // template <typename AssetType>
  // typename std::enable_if_t<IsAsset<AssetType>::value, void>  //
  // Release(const FileName& file);

  //// Clears a cache
  // template <typename AssetType>
  // typename std::enable_if_t<IsAsset<AssetType>::value, void>  //
  // ClearCache();

 private:
  Path data_folder_;
  Path saves_folder_;
  Path config_file_path_;

  Cfg cfg_;
  Tex tex_;
  Mlb mlb_;

  void CreateAssetFolders(const Path& data_directory_path) noexcept;
  void LoadCfg(const Path& file);
  void LoadMlb(const Path& file);
  void LoadTex(const Path& file);

  //// Gets a reference to the underlying asset cache
  // template <typename AssetType>
  //// typename std::enable_if_t<IsAsset<AssetType>::value,
  //// Cache_t<AssetType>&>  //
  // Cache_t<AssetType>& GetCache() noexcept;

  //// Gets the path prefix for the asset type
  // template <typename AssetType>
  // typename std::enable_if_t<IsAsset<AssetType>::value,
  //                          Path>  //
  // GetAssetPathPrefix() const noexcept;

  // Cache_t<Tgx> tgx_cache_;
  // Cache_t<Gm1> gm1_cache_;
  // Cache_t<Ani> ani_cache_;
  // Cache_t<Bik> bik_cache_;
};

//// Loads an asset and returns a handle to it
// template <typename AssetType>
// typename std::enable_if_t<AssetLoader::IsAsset<AssetType>::value,
//                          AssetLoader::AssetHandle<AssetType>>
// AssetLoader::Load(const FileName& file) {
//  auto& cache = GetCache<AssetType>();
//  const auto path_prefix = GetAssetPathPrefix<AssetType>();
//
//  auto asset_handle = cache[file.string()].lock();
//
//  if (!asset_handle) {
//    asset_handle = std::make_shared<AssetType>(path_prefix / file);
//  }
//
//  return asset_handle;
//}
//
//// Caches an asset
// template <typename AssetType>
// typename std::enable_if_t<AssetLoader::IsAsset<AssetType>::value, void>  //
// AssetLoader::Cache(const FileName& file) {
//  auto& cache = GetCache<AssetType>();
//  const auto& path_prefix = GetAssetPathPrefix<AssetType>();
//  cache.emplace(file, std::make_shared<AssetType>(path_prefix / file));
//}
//
//// Releases an asset from the cache
// template <typename AssetType>
// typename std::enable_if_t<AssetLoader::IsAsset<AssetType>::value, void>  //
// AssetLoader::Release(const FileName& file) {
//  auto& cache = GetCache<AssetType>();
//  auto candidate = cache.find(file.string());
//  if (candidate != std::end(cache)) {
//    cache.erase(candidate);
//  }
//}
//
//// Clears a cache
// template <typename AssetType>
// typename std::enable_if_t<AssetLoader::IsAsset<AssetType>::value, void>  //
// AssetLoader::ClearCache() {
//  auto& cache = GetCache<AssetType>();
//  cache.clear();
//}
//
//// Gets the path prefix for the asset type
// template <typename AssetType>
// typename std::enable_if<AssetLoader::IsAsset<AssetType>::value,
//                        AssetLoader::Path>::type
// AssetLoader::GetAssetPathPrefix() const noexcept {
//  if (std::is_same<AssetType, Tgx>::value) {
//    return data_folder_ / "gfx/";
//  } else if (std::is_same<AssetType, Gm1>::value) {
//    return data_folder_ / "gm/";
//  } else if (std::is_same<AssetType, Ani>::value) {
//    return data_folder_;
//  } else if (std::is_same<AssetType, Bik>::value) {
//    return data_folder_ / "binks/";
//  }
//}

#endif  // ASSET_LOADER_H_
