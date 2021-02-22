#ifndef ASSET_LOADER_H_
#define ASSET_LOADER_H_
#include "GameOptions.h"
#include "Parsers/CfgFile.h"
#include "Parsers/TgxFile.h"
#include "Parsers/Gm1File.h"
#include "Parsers/AniFile.h"
#include "Parsers/MlbFile.h"
#include "Parsers/TexFile.h"
#include "Rendering/BinkVideo.h"
#include "ghc/filesystem.hpp"
#include <type_traits>
#include <string>
#include <unordered_map>

class AssetLoader {
 public:
  // using declarations
  using Path = ghc::filesystem::path;
  using Tgx = Sourcehold::Parsers::TgxFile;
  using Gm1 = Sourcehold::Parsers::Gm1File;
  using Ani = Sourcehold::Parsers::AniFile;
  using Bik = Sourcehold::Rendering::BinkVideo;
  using Cfg = Sourcehold::Parsers::CfgFile;
  using Mlb = Sourcehold::Parsers::MlbFile;
  using Tex = Sourcehold::Parsers::TexFile;
  // typedef for a shared_ptr
  template <typename AssetType>
  using AssetHandle = std::shared_ptr<AssetType>;

  AssetLoader();

  // Loads an asset and returns a handle to it
  template <typename AssetType>
  AssetHandle<AssetType> Load(const Path& file);

  // Caches an asset
  template <typename AssetType>
  void LoadAndCache(const Path& file);

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

  AssetHandle<Tgx> LoadTgx(const Path& file);
  AssetHandle<Gm1> LoadGm1(const Path& file);
  AssetHandle<Ani> LoadAni(const Path& file);
  AssetHandle<Bik> LoadBik(const Path& file);
  void CacheTgx(const Path& file);
  void CacheGm1(const Path& file);
  void CacheAni(const Path& file);
  void CacheBik(const Path& file);

  template <typename AssetType>
  using Cache = std::unordered_map<std::string, std::weak_ptr<AssetType>>;

  Cache<Tgx> tgx_cache_;
  Cache<Gm1> gm1_cache_;
  Cache<Ani> ani_cache_;
  Cache<Bik> bik_cache_;
};
template <typename AssetType>
AssetLoader::AssetHandle<AssetType> AssetLoader::Load(const Path& file) {
  if (std::is_same<AssetType, Tgx>::value) {
    return LoadTgx(file);
  } else if (std::is_same<AssetType, Gm1>::value) {
    return LoadGm1(file);
  } else if (std::is_same<AssetType, Ani>::value) {
    return LoadAni(file);
  } else if (std::is_same<AssetType, Bik>::value) {
    return LoadBik(file);
  } else {
    static_assert(true, "Loading unsupported Filetype");
  }
}
template <typename AssetType>
void AssetLoader::LoadAndCache(const Path& file) {
  if (std::is_same<AssetType, Tgx>::value) {
    return CacheTgx(file);
  } else if (std::is_same<AssetType, Gm1>::value) {
    return CacheGm1(file);
  } else if (std::is_same<AssetType, Ani>::value) {
    return CacheAni(file);
  } else if (std::is_same<AssetType, Bik>::value) {
    return CacheBik(file);
  } else {
    static_assert(true, "Caching unsupported Filetype");
  }
}
#endif  // ASSET_LOADER_H_
