//
// viceemulatorcore.cpp
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     https://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "viceemulatorcore.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "defs.h"

extern "C" {
#include "third_party/vice-3.9/src/main.h"
#include "third_party/common/semaphore.h"

extern void circle_kernel_core_init_complete(int core);
}


#include "third_party/vice-3.9/src/sid/sid.h"
#include "third_party/vice-3.9/src/resid/sid.h"
#include "third_party/vice-3.9/src/resid/filter.h"

ViceEmulatorCore::ViceEmulatorCore(CMemorySystem *pMemorySystem,
                                   int cyclesPerSecond) :
#ifdef ARM_ALLOW_MULTI_CORE
       CMultiCoreSupport(pMemorySystem),
#endif
       launch_(false), cyclesPerSecond_(cyclesPerSecond) {

  passBandFreq_ = 13230; // 60% — matches menu.c passband setting
#ifdef ARM_ALLOW_MULTI_CORE
  // VICE 3.9 reSID: sampling tables are allocated on demand; no pre-compute needed
#endif
}

ViceEmulatorCore::~ViceEmulatorCore(void) {}

void ViceEmulatorCore::RunMainVice(bool wait) {
  if (wait) {
     printf("Core waiting for launch\n");
     bool waiting = true;
     while (waiting) {
       m_Lock.Acquire();
       if (launch_)
         waiting = false;
       m_Lock.Release();
     }
  }

  // Call Vice's main_program

  // Use -soundsync 0 option for 'flexible'
  // sound sync.

  // Use -refresh 1 option to turn off the 'auto'
  // refresh which screws up badly after some time.
  // The vertical blank really messes up vice's
  // algorithm that decides to skip frames. Might
  // want to go back to using the open gl hook.
  // See arch/raspi/videoarch.c

  printf("Starting emulator main loop\n");

#if defined(RASPI_C64)
  // VICE 3.9: -soundsync and -refresh were removed
  int argc = 5;
  char *argv[] = {
      (char *)"vice", timing_option_, (char *)"-sounddev", (char *)"raspi",
      // Unless we disable the video cache, vsync is messed up
      (char *)"+VICIIvcache",
  };
#elif defined(RASPI_C128)
  int argc = 12;
  char *argv[] = {
      (char *)"vice", timing_option_, (char *)"-sounddev", (char *)"raspi",
      (char *)"-soundoutput", (char *)"1", (char *)"-soundsync", (char *)"0",
      (char *)"-refresh", (char *)"1",
      // Unless we disable the video cache, vsync is messed up
      (char *)"+VICIIvcache",
      (char *)"+VDCvcache",
  };
#elif defined(RASPI_VIC20)
  int argc = 11;
  char *argv[] = {
      (char *)"vice", timing_option_, (char *)"-sounddev", (char *)"raspi",
      (char *)"-soundoutput", (char *)"1", (char *)"-soundsync", (char *)"0",
      (char *)"-refresh", (char *)"1",
      // Unless we disable the video cache, vsync is messed up
      (char *)"+VICvcache",
  };
#elif defined(RASPI_PLUS4)
  int argc = 11;
  char *argv[] = {
      (char *)"vice", timing_option_, (char *)"-sounddev", (char *)"raspi",
      (char *)"-soundoutput", (char *)"1", (char *)"-soundsync", (char *)"0",
      (char *)"-refresh", (char *)"1",
      // Unless we disable the video cache, vsync is messed up
      (char *)"+TEDvcache",
  };
#elif defined(RASPI_PET)
  int argc = 11;
  char *argv[] = {
      (char *)"vice", timing_option_, (char *)"-sounddev", (char *)"raspi",
      (char *)"-soundoutput", (char *)"1", (char *)"-soundsync", (char *)"0",
      (char *)"-refresh", (char *)"1",
      (char *)"+CRTCvcache",
  };
#else
#error "RASPI_[model] NOT DEFINED"
#endif
  emu_machine_init(m_options->GetRasterSkip(), m_options->GetRasterSkip2());
  main_program(argc, argv);
  emu_exit();
}

// VICE 3.9: reSID Filter no longer takes a model argument; filter tables are
// initialised lazily. ComputeSamplingTable was also removed — sampling tables
// are allocated on demand by set_sampling_parameters().
void ViceEmulatorCore::ComputeResidFilter(int model) {
  // No-op in VICE 3.9
  (void)model;
}

void ViceEmulatorCore::Run(unsigned nCore) {
  assert(nCore > 0);
  switch (nCore) {
  case 1:
    RunMainVice(true);
    break;
  case 2:
#ifdef ARM_ALLOW_MULTI_CORE
    circle_kernel_core_init_complete(2);
#endif
    break;
  case 3:
#ifdef ARM_ALLOW_MULTI_CORE
    circle_kernel_core_init_complete(3);
#endif
    break;
  }

#ifdef ARM_ALLOW_MULTI_CORE
  printf("Core %d idle\n", nCore);
  asm("dsb\n\t"
      "1: wfi\n\t"
      "b 1b\n\t");
#endif
}

bool ViceEmulatorCore::Init(ViceOptions* options) {
  m_options = options;
  return Initialize();
}

void ViceEmulatorCore::LaunchEmulator(char *timing_option) {
  strncpy(timing_option_, timing_option, 8);
#ifdef ARM_ALLOW_MULTI_CORE
  m_Lock.Acquire();
  launch_ = true;
  m_Lock.Release();
#else
  RunMainVice(false);
#endif
}
