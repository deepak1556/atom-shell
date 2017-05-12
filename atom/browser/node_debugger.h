// Copyright (c) 2014 GitHub, Inc.
// Use of this source code is governed by the MIT license that can be
// found in the LICENSE file.

#ifndef ATOM_BROWSER_NODE_DEBUGGER_H_
#define ATOM_BROWSER_NODE_DEBUGGER_H_

#include <memory>
#include <string>

#include "base/memory/weak_ptr.h"
#include "base/threading/thread.h"
#include "net/test/embedded_test_server/stream_listen_socket.h"
#include "v8/include/v8-debug.h"
#include "vendor/node/deps/uv/include/uv.h"
#include "vendor/node/src/inspector_agent.h"

namespace atom {

// Add support for node's "--debug" switch.
class NodeDebugger {
 public:
  explicit NodeDebugger(node::Environment* env);
  virtual ~NodeDebugger();

 private:
  std::unique_ptr<node::inspector::Agent> agent_;

  base::WeakPtrFactory<NodeDebugger> weak_factory_;

  DISALLOW_COPY_AND_ASSIGN(NodeDebugger);
};

}  // namespace atom

#endif  // ATOM_BROWSER_NODE_DEBUGGER_H_
