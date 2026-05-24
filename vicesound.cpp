//
// vicesound.cpp
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

#include "vicesound.h"
#include "defs.h"
extern "C" unsigned long circle_get_ticks(void);
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern "C" {
#include <strings.h>
}

#include <circle/sched/scheduler.h>

ViceSound::ViceSound(CVCHIQDevice *pVCHIQDevice,
                     TVCHIQSoundDestination Destination)
    : ViceSoundBaseDevice(pVCHIQDevice, SAMPLE_RATE, CHUNK_SIZE, Destination) {
  src_pos = 0;
  src_size = 0;
  src_buffer = 0;
  bytes_buffered = 0;
}

ViceSound::~ViceSound(void) {}

boolean ViceSound::Playback(int volume, int channels) {
  assert(!IsActive());
  num_channels = channels;
  SetVolume(volume);
  SetChannels(channels);
  return Start();
}

boolean ViceSound::PlaybackActive(void) const { return IsActive(); }

void ViceSound::CancelPlayback(void) { Cancel(); }

void ViceSound::SetControl(int nVolume, TVCHIQSoundDestination Destination) {
  ViceSoundBaseDevice::SetControl(nVolume, Destination);
}

unsigned ViceSound::AddChunk(s16 *pBuffer, unsigned nChunkSize) {
  // Rate-limit writes to at most SAMPLE_RATE samples/second using wall clock.
  // This prevents the GPU audio hardware buffer from slowly filling when
  // VICE runs slightly faster than real-time (~0.1% drift on Pi Zero 2W).
  // A 2048-sample cushion permits normal bursting without audible effect.
  static unsigned long rate_start_us = 0;
  static unsigned long rate_total = 0;
  if (rate_start_us == 0) {
    rate_start_us = circle_get_ticks();
  } else {
    unsigned long elapsed_us = circle_get_ticks() - rate_start_us;
    unsigned long allowed = (unsigned long long)SAMPLE_RATE * elapsed_us / 1000000 + 2048;
    if (rate_total >= allowed) {
      return 0;
    }
  }

  src_pos = 0;
  src_buffer = pBuffer;
  unsigned original = nChunkSize;

  while (nChunkSize > 0) {
    src_size = nChunkSize;
    if (src_size > CHUNK_SIZE)
      src_size = CHUNK_SIZE;

    WriteChunk();
    src_pos += src_size;
    nChunkSize -= src_size;
  }

  rate_total += original;
  return 0;
}

unsigned ViceSound::GetChunk(s16 *pBuffer, unsigned nChunkSize) {

  assert(pBuffer != 0);
  assert(nChunkSize > 0);
  assert((nChunkSize & 1) == 0);

  // VICE expects us to 'block' if our buffer is full. But
  // this shouldn't happen.
  while (bytes_buffered >= FRAG_SIZE * NUM_FRAGS * BYTES_PER_SAMPLE) {
    CScheduler::Get()->Yield();
  }

  if (src_buffer == 0 || src_size == 0) {
    // Nothing to give? Give a silent packet.
    memset(pBuffer, 0, FRAG_SIZE * BYTES_PER_SAMPLE);
    src_size = FRAG_SIZE;
  } else {
    // We always give VC4 nChunkSize packets.
    memcpy(pBuffer, src_buffer + src_pos, src_size * BYTES_PER_SAMPLE);
  }
  return src_size;
}

// Callback from VC to let us know how much is currently buffered.
void ViceSound::AmountBufferedBytes(unsigned nBytes) {
  bytes_buffered = nBytes;
}

// Call from vice to ask us how much space is left in our buffer.
// Return value is in samples.
unsigned ViceSound::BufferSpaceSamples() {
  int left = FRAG_SIZE * NUM_FRAGS - bytes_buffered / BYTES_PER_SAMPLE / num_channels;
  return left < 0 ? 0 : left;
}
