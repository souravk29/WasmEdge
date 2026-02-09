// SPDX-License-Identifier: Apache-2.0
// SPDX-FileCopyrightText: 2019-2024 Second State INC

#include "driver/validate.h"
#include <api/wasmedge/wasmedge.h>
#include <filesystem>
#include <iostream>

namespace WasmEdge {
namespace Driver {

int Validate(const DriverValidateOptions &Opts) {
  // 1. Check if file exists
  if (!std::filesystem::exists(Opts.InputPath.value())) {
    std::cerr << "Error: Input file not found: " << Opts.InputPath.value()
              << std::endl;
    return EXIT_FAILURE;
  }

  // 2. Setup C API Configuration
  WasmEdge_ConfigureContext *Conf = WasmEdge_ConfigureCreate();
  WasmEdge_LoaderContext *Loader = WasmEdge_LoaderCreate(Conf);
  WasmEdge_ValidatorContext *Validator = WasmEdge_ValidatorCreate(Conf);
  WasmEdge_ASTModuleContext *Module = nullptr;

  // 3. Load and Parse
  WasmEdge_Result parseRes = WasmEdge_LoaderParseFromFile(
      Loader, &Module, Opts.InputPath.value().c_str());
  if (!WasmEdge_ResultOK(parseRes)) {
    std::cerr << "Parse error: " << WasmEdge_ResultGetMessage(parseRes)
              << std::endl;
    WasmEdge_ValidatorDelete(Validator);
    WasmEdge_LoaderDelete(Loader);
    WasmEdge_ConfigureDelete(Conf);
    return EXIT_FAILURE;
  }

  // 4. Validate
  WasmEdge_Result validateRes = WasmEdge_ValidatorValidate(Validator, Module);
  if (!WasmEdge_ResultOK(validateRes)) {
    std::cerr << "Validation failed: " << WasmEdge_ResultGetMessage(validateRes)
              << std::endl;
    WasmEdge_ASTModuleDelete(Module);
    WasmEdge_ValidatorDelete(Validator);
    WasmEdge_LoaderDelete(Loader);
    WasmEdge_ConfigureDelete(Conf);
    return EXIT_FAILURE;
  }

  std::cout << "Validation passed." << std::endl;

  // Cleanup
  WasmEdge_ASTModuleDelete(Module);
  WasmEdge_ValidatorDelete(Validator);
  WasmEdge_LoaderDelete(Loader);
  WasmEdge_ConfigureDelete(Conf);
  return EXIT_SUCCESS;
}

} // namespace Driver
} // namespace WasmEdge 
