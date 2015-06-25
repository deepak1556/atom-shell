// Copyright (c) 2013 GitHub, Inc.
// Use of this source code is governed by the MIT license that can be
// found in the LICENSE file.

#include "atom/browser/javascript_environment.h"

#include <string>

#include "atom/common/atom_command_line.h"
#include "gin/array_buffer.h"

namespace atom {

JavascriptEnvironment::JavascriptEnvironment()
    : initialized_(Initialize()),
      isolate_(isolate_holder_.isolate()),
      isolate_scope_(isolate_),
      locker_(isolate_),
      handle_scope_(isolate_),
      context_(isolate_, v8::Context::New(isolate_)),
      context_scope_(v8::Local<v8::Context>::New(isolate_, context_)) {
}

bool JavascriptEnvironment::Initialize() {
  gin::IsolateHolder::LoadV8Snapshot();
  gin::IsolateHolder::Initialize(gin::IsolateHolder::kNonStrictMode,
                                 gin::ArrayBufferAllocator::SharedInstance());

  // Set max size of old heap space.
  auto args = AtomCommandLine::argv();
  for (const std::string& arg : args)
    if (!strncmp(arg.c_str(),
                 "--max_old_space_size=",
                 sizeof("--max_old_space_size=") - 1)) {
      v8::V8::SetFlagsFromString(arg.c_str(), arg.size());
      break;
    }

  return true;
}

}  // namespace atom
