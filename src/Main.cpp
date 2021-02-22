#include <iostream>
#include "Commandline.h"
#include "StartGame.h"

// Initialize GameOptions here
// TODO(seidl):
// kind of ugly, find a better solution
GameOptions game_options_gk;

int main(int argc, char** argv) {
  try {
    game_options_gk = getGameOptions(argc, argv);
  } catch (std::exception& e) {
    std::cerr << e.what() << '\n';
    return EXIT_FAILURE;
  }

  return StartGame(game_options_gk);
}

#if SOURCEHOLD_MINGW == 1 && 0

#include <windows.h>
#include <string>
#include <vector>

/* Windows specific entry point */
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevIns, LPSTR lpszArgument,
                   int iShow) {
  /* Convert argument list */
  int w_argc = 0;
  LPWSTR* w_argv = CommandLineToArgvW(GetCommandLineW(), &w_argc);
  if (w_argv) {
    std::vector<std::string> argv_buf;
    argv_buf.reserve(w_argc);

    for (int i = 0; i < w_argc; ++i) {
      int w_len = lstrlenW(w_argv[i]);
      int len =
          WideCharToMultiByte(CP_ACP, 0, w_argv[i], w_len, NULL, 0, NULL, NULL);
      std::string s;
      s.resize(len);
      WideCharToMultiByte(CP_ACP, 0, w_argv[i], w_len, &s[0], len, NULL, NULL);
      argv_buf.push_back(s);
    }

    std::vector<char*> argv;
    argv.reserve(argv_buf.size());
    for (std::vector<std::string>::iterator i = argv_buf.begin();
         i != argv_buf.end(); ++i)
      argv.push_back((char*)i->c_str());

    int code = main(argv.size(), &argv[0]);

    LocalFree(w_argv);
    return code;
  }

  int code = main(0, NULL);

  LocalFree(w_argv);
  return code;
}

#endif
