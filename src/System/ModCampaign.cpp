#ifdef SCRIPT_SUPPORT

  #include "System/ModCampaign.h"
  #include "System/FileUtil.h"
  #include "System/Logger.h"

  #include <duktape.h>
  #include <cassert>
  #include <istream>

using namespace Sourcehold;
using namespace System;

std::vector<ModCampaign> military, eco;

class JSONConfig {
  duk_context *ctx{nullptr};

 public:
  JSONConfig(const ghc::filesystem::path &path) {
    ctx = duk_create_heap(NULL, NULL, NULL, (void *)0xCAFEBABE, my_fatal);
    if (!ctx) {
      throw std::bad_alloc();
    }

    std::ifstream ifs(path.string(), std::ios::in);
    if (!ifs.is_open()) {
      throw std::runtime_error("Failed to open JSON config file!");
    }

    std::string file((std::istreambuf_iterator<char>(ifs)),
                     std::istreambuf_iterator<char>());

    ifs.close();

    duk_push_string(ctx, file.c_str());
    duk_json_decode(ctx, -1);
  }

  std::string GetStringProp(const std::string &prop) {
    if (!duk_get_prop_string(ctx, -1, prop.c_str())) {
      duk_pop(ctx);
      throw std::runtime_error("No such string property");
    }

    std::string pr(duk_safe_to_string(ctx, -1));
    duk_pop(ctx);

    return pr;
  }

  int32_t GetIntProp(const std::string &prop) {
    if (!duk_get_prop_string(ctx, -1, prop.c_str())) {
      duk_pop(ctx);
      throw std::runtime_error("No such int property");
    }

    int32_t pr(duk_to_int32(ctx, -1));
    duk_pop(ctx);

    return pr;
  }

  ~JSONConfig() {
    if (ctx)
      duk_destroy_heap(ctx);
  }

 protected:
  static void my_fatal(void *udata, const char *msg) {
    (void)udata;

    Logger::error(GAME) << "Duktape error: " << msg << std::endl;
    abort();
  }
};

void System::LoadModCampaigns(const ghc::filesystem::path &base) {
  /* Folder structure:
   *
   * campaigns/MyModCampaign/
   * ├── manifest.json
   * ├── campaign.js
   * ├── maps/
   */
  auto folders = GetDirectoryRecursive(base, "", false, true);
  for (const auto &folder : folders) {
    ghc::filesystem::path maps(folder / "maps/"),
        manifest(folder / "manifest.json"), campaign(folder / "campaign.js");

    if (!DoesFileExist(maps) || !IsFolder(maps) || !DoesFileExist(manifest) ||
        !DoesFileExist(campaign)) {
      Logger::error(GAME) << "Campaign at '" << folder
                          << "' does not contain all the necessary files! "
                             "Please check the wiki!"
                          << std::endl;
      return;
    }

    // Read the manifest //
    try {
      JSONConfig conf(manifest);

      ModCampaign info;
      info.type = (ModCampaign::Type)conf.GetIntProp("type");
      info.title = conf.GetStringProp("title");
      info.author = conf.GetStringProp("author");
      info.comment = conf.GetStringProp("comment");

      Logger::success(GAME) << "Registered campaign: '" << info.title << "' by "
                            << info.author << "!" << std::endl;

      if (info.type == 0) {
        military.push_back(info);
      }
      else if (info.type == 1) {
        eco.push_back(info);
      }
      else {
        Logger::warning(GAME) << "Campaign '" << info.title
                              << " has an invalid type!" << std::endl;
      }
    }
    catch (std::exception &ex) {
      Logger::warning(GAME) << "Failed to read campaign manifest: '" << folder
                            << "'!" << std::endl;
    }
  }
}

std::vector<ModCampaign> *System::GetMilitaryCampaigns() {
  return &military;
}

std::vector<ModCampaign> *System::GetEcoCampaigns() {
  return &eco;
}

bool System::HasMilitaryModCampaigns() {
  return military.size() > 0;
}

bool System::HasEcoModCampaigns() {
  return eco.size() > 0;
}

#endif
