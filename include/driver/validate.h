#pragma once

#include "po/argument_parser.h"
#include <string>
#include <string_view>

namespace WasmEdge {
namespace Driver {

#include "po/argument_parser.h"
#include <string_view>

struct DriverValidateOptions {
  DriverValidateOptions()
      : InputPath(WasmEdge::PO::Description("Wasm file to validate"),
                  WasmEdge::PO::MetaVar("WASM")) {}

  WasmEdge::PO::Option<::std::string> InputPath;

  void add_option(WasmEdge::PO::ArgumentParser &Parser) noexcept {
    Parser.add_option(InputPath);
  }
};

int Validate(const DriverValidateOptions &Opts);

} // namespace Driver
} // namespace WasmEdge 
