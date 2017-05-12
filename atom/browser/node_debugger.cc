// Copyright (c) 2014 GitHub, Inc.
// Use of this source code is governed by the MIT license that can be
// found in the LICENSE file.

#include "atom/browser/node_debugger.h"

#include <string>

#include "base/bind.h"
#include "base/command_line.h"
#include "base/strings/string_number_conversions.h"
#include "base/strings/stringprintf.h"
#include "base/strings/utf_string_conversions.h"
#include "content/public/browser/browser_thread.h"
#include "net/test/embedded_test_server/tcp_listen_socket.h"
//#include "gin/public/v8_platform.h"
#include "libplatform/libplatform.h"

#include "atom/common/node_includes.h"

namespace atom {

NodeDebugger::NodeDebugger(node::Environment* env)
    : agent_(new node::inspector::Agent(env)),
      weak_factory_(this) {
  base::CommandLine* cmd = base::CommandLine::ForCurrentProcess();
  if (cmd->HasSwitch("inspect")) {
    node::DebugOptions options;
    options.EnableDebugAgent(node::DebugAgentType::kInspector);
    agent_->Start(v8::platform::CreateDefaultPlatform(0), nullptr, options);
  }
}

NodeDebugger::~NodeDebugger() {
  //thread_.Stop();
}

}  // namespace atom
