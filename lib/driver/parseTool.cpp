// SPDX-License-Identifier: Apache-2.0
// SPDX-FileCopyrightText: 2019-2024 Second State INC

#include "driver/parse.h"
#include <filesystem>
#include <iostream>
#include <string>
#include <vector>

// STRICT C API USE
#include "wasmedge/wasmedge.h"

namespace WasmEdge {
namespace Driver {

int Parse(struct DriverParseOptions &Opts) noexcept {
  // 1. Check if file exists
  // FIX: Use 'WasmName' instead of 'InputPath'
  if (!std::filesystem::exists(Opts.WasmName.value())) {
    std::cerr << "Error: Input file not found: " << Opts.WasmName.value()
              << std::endl;
    return EXIT_FAILURE;
  }

  // 2. Setup C API Contexts
  WasmEdge_ConfigureContext *Conf = WasmEdge_ConfigureCreate();
  WasmEdge_LoaderContext *Loader = WasmEdge_LoaderCreate(Conf);
  WasmEdge_ASTModuleContext *Module = nullptr;

  // 3. Load and Parse using C API
  // FIX: Use 'WasmName' to get the string
  std::string PathStr = Opts.WasmName.value();
  WasmEdge_Result Res =
      WasmEdge_LoaderParseFromFile(Loader, &Module, PathStr.c_str());

  // 4. Check for Errors
  if (!WasmEdge_ResultOK(Res)) {
    std::cerr << "Parse error: " << WasmEdge_ResultGetMessage(Res) << std::endl;
    std::cerr << "Code: " << WasmEdge_ResultGetCode(Res) << std::endl;

    WasmEdge_LoaderDelete(Loader);
    WasmEdge_ConfigureDelete(Conf);
    return EXIT_FAILURE;
  }

  std::cout << "WASM file \"" << PathStr << "\" parsed successfully."
            << std::endl;

  // 5. Print Statistics
  std::cout << "  Import count:   "
            << WasmEdge_ASTModuleListImports(Module, NULL, 0) << std::endl;
  std::cout << "  Export count:   "
            << WasmEdge_ASTModuleListExports(Module, NULL, 0) << std::endl;

  // 6. Cleanup
  WasmEdge_ASTModuleDelete(Module);
  WasmEdge_LoaderDelete(Loader);
  WasmEdge_ConfigureDelete(Conf);

  return EXIT_SUCCESS;
}

} // namespace Driver
} // namespace WasmEdge