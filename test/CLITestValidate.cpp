// SPDX-License-Identifier: Apache-2.0
// SPDX-FileCopyrightText: 2019-2024 Second State INC

#include "driver/validate.h"
#include "gtest/gtest.h"
#include <filesystem>
#include <fstream>

using namespace WasmEdge;
using namespace WasmEdge::Driver;

class CLITest : public ::testing::Test {
protected:
  void SetUp() override {
    // Prepare a minimal valid wasm file for validation
    wasmFile = std::filesystem::temp_directory_path() / "test_valid.wasm";
    std::ofstream ofs(wasmFile, std::ios::binary);
    // Write a minimal valid wasm binary header
    const unsigned char wasmHeader[] = {0x00, 0x61, 0x73, 0x6D,
                                        0x01, 0x00, 0x00, 0x00};
    ofs.write(reinterpret_cast<const char *>(wasmHeader), sizeof(wasmHeader));
    ofs.close();
  }
  void TearDown() override { std::filesystem::remove(wasmFile); }
  std::filesystem::path wasmFile;
};

TEST_F(CLITest, ValidateValidWasm) {
  DriverValidateOptions opts;
  opts.InputPath.value() = wasmFile.string();
  EXPECT_EQ(Validate(opts), EXIT_SUCCESS);
}

TEST_F(CLITest, ValidateInvalidWasm) {
  DriverValidateOptions opts;
  opts.InputPath.value() = "nonexistent_file.wasm";
  EXPECT_EQ(Validate(opts), EXIT_FAILURE);
}
