#include "AssetLoader.h"
#include "Logger.h"
#include "GameOptions.h"
AssetLoader::AssetLoader() {
  using namespace Sourcehold::System;
  try {
    CreateAssetFolders(game_options_gk.data_directory_path_);
    LoadCfg(config_file_path_);
    LoadMlb(data_folder_ / "stronghold.mlb");
    LoadTex(data_folder_ / "sh.tex");
  } catch (std::exception& e) {
    std::cout << e.what() << std::endl;
    std::exit(1);
  }
}
void AssetLoader::CreateAssetFolders(
    const Path& data_directory_path_) noexcept {
  using namespace Sourcehold::System;
  using namespace FileSystem;
  data_folder_ = data_directory_path_;
  saves_folder_ = GetDocumentsPath();
  saves_folder_ /= "Stronghold/Saves/";

  Logger::message(GAME) << "Creating asset folders .." << std::endl;

  try {
    CreateFolder(data_folder_ / "campaigns/");
    CreateFolder(saves_folder_);
  } catch (std::exception& e) {
    std::cout << e.what() << std::endl;
  }

  config_file_path_ = saves_folder_ / "../stronghold.cfg";
}

void AssetLoader::LoadCfg(const Path& file) {
  using namespace Sourcehold::System;

  using namespace FileSystem;
  Logger::message(GAME) << "Loading config file .." << std::endl;
  if (!DoesFileExist(file)) {
    Logger::message(GAME) << "Missing. Creating new config file .."
                          << std::endl;
    cfg_.SetDefaultValues();
    cfg_.WriteToDisk(file);
  } else {
    if (!cfg_.LoadFromDisk(file)) {
      throw std::runtime_error("Failed to load config file");
    }
  }
}
void AssetLoader::LoadMlb(const Path& file) {
  using namespace Sourcehold::System;
  if (!mlb_.LoadFromDisk(file)) {
    throw std::runtime_error("Failed to load stronghold.mlb");
  }
}

void AssetLoader::LoadTex(const Path& file) {
  using namespace Sourcehold::System;
  if (!tex_.LoadFromDisk(file)) {
    throw std::runtime_error("Failed to load sh.tex");
  }
}

//// Gets a reference to the underlying asset cache
//// TODO(seidl): if we are moving to a version with if constexpr, change this
// template <>
// AssetLoader::Cache_t<AssetLoader::Tgx>& AssetLoader::GetCache() noexcept {
//  return tgx_cache_;
//}
// template <>
// AssetLoader::Cache_t<AssetLoader::Gm1>& AssetLoader::GetCache() noexcept {
//  return gm1_cache_;
//}
// template <>
// AssetLoader::Cache_t<AssetLoader::Ani>& AssetLoader::GetCache() noexcept {
//  return ani_cache_;
//}
// template <>
// AssetLoader::Cache_t<AssetLoader::Bik>& AssetLoader::GetCache() noexcept {
//  return bik_cache_;
//}
//
