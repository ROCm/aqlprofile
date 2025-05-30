// MIT License
//
// Copyright (c) 2017-2025 Advanced Micro Devices, Inc.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.


#ifndef TEST_CTRL_TEST_HSA_H_
#define TEST_CTRL_TEST_HSA_H_

#include <string>

#include "ctrl/test_aql.h"
#include "ctrl/test_kernel.h"
#include "util/hsa_rsrc_factory.h"
#include "util/perf_timer.h"

// Class implements HSA test
class TestHsa : public TestAql {
 public:
  // Instantiate HSA resources
  static HsaRsrcFactory* HsaInstantiate(const uint32_t agent_ind = agent_id_);
  static void HsaShutdown();
  static void SetQueue(hsa_queue_t* queue) { hsa_queue_ = queue; }
  static uint32_t HsaAgentId() { return agent_id_; }
  static std::string HsaAgentName() { return agent_info_->gfxip; }

  // Constructor
  explicit TestHsa(TestKernel* test) : test_(test), name_(test->Name()), symb_(test->SymbName()) {
    total_time_taken_ = 0;
    setup_time_taken_ = 0;
    dispatch_time_taken_ = 0;
    hsa_exec_ = {};
  }

  // Get methods for Agent Info, HAS queue, HSA Resourcse Manager
  const AgentInfo* GetAgentInfo() { return agent_info_; }
  hsa_queue_t* GetQueue() { return hsa_queue_; }
  HsaRsrcFactory* GetRsrcFactory() { return hsa_rsrc_; }

  // Initialize application environment including setting
  // up of various configuration parameters based on
  // command line arguments
  // @return bool true on success and false on failure
  bool Initialize(int argc, char** argv);

  // Setup application parameters for exectuion
  // @return bool true on success and false on failure
  bool Setup();

  // Run the BinarySearch kernel
  // @return bool true on success and false on failure
  bool Run();

  // Verify against reference implementation
  // @return bool true on success and false on failure
  bool VerifyResults();

  // Print to console the time taken to execute kernel
  void PrintTime();

  // Release resources e.g. memory allocations
  // @return bool true on success and false on failure
  bool Cleanup();

  virtual bool RunSdma(size_t sdma_size);
  // test SDMA of @sdma_size bytes.

  // To get test name
  // @return test name
  const char* Name() { return name_.c_str(); }

 private:
  typedef TestKernel::mem_descr_t mem_descr_t;
  typedef TestKernel::mem_map_t mem_map_t;
  typedef TestKernel::mem_it_t mem_it_t;

  // Test object
  TestKernel* test_;

  // Path of Brig file
  std::string brig_path_obj_;

  // Used to track time taken to run the sample
  double total_time_taken_;
  double setup_time_taken_;
  double dispatch_time_taken_;

  // Handle of signal
  hsa_signal_t hsa_signal_;

  // Handle of Kernel Code Descriptor
  hsa_executable_symbol_t kernel_code_desc_;

  // Instance of timer object
  uint32_t setup_timer_idx_;
  uint32_t dispatch_timer_idx_;
  PerfTimer hsa_timer_;

  // Instance of Hsa Resources Factory
  static HsaRsrcFactory* hsa_rsrc_;

  // GPU id
  static uint32_t agent_id_;

  // Handle to an Hsa Gpu Agent
  static const AgentInfo* agent_info_;

  // Handle to an Hsa Queue
  static hsa_queue_t* hsa_queue_;

  // Test kernel name
  const std::string name_;

  // Test kernel symboll name
  const std::string symb_;

  // Kernel executable
  hsa_executable_t hsa_exec_;
};

#endif  // TEST_CTRL_TEST_HSA_H_
