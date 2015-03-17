// Copyright (c) 2013 GitHub, Inc.
// Use of this source code is governed by the MIT license that can be
// found in the LICENSE file.

#include "atom/app/atom_main_args.h"
#include "base/strings/string16.h"
#include "base/logging.h"

namespace atom {
  AtomCommandLine* AtomCommandLine::current_process_commandline_ = NULL;
  AtomCommandLine::AtomCommandLine(NoProgram no_program) : argv_() {
  }

  AtomCommandLine::~AtomCommandLine() {
  }

  bool AtomCommandLine::Init(int argc,
          const AtomCommandLine::CharType* const* argv) {
    if (current_process_commandline_) {
      return false;
    }

    current_process_commandline_ = new AtomCommandLine(NO_PROGRAM);
    current_process_commandline_->InitFromArgv(argc, argv);

    return true;
  }

  bool AtomCommandLine::InitialisedForCurrentProcess() {
    return !!current_process_commandline_;
  }

  void AtomCommandLine::Reset() {
    DCHECK(current_process_commandline_);
    delete current_process_commandline_;
    current_process_commandline_ = NULL;
  }

  void AtomCommandLine::InitFromArgv(int argc,
          const AtomCommandLine::CharType* const* argv) {
    for (int i = 0; i < argc; ++i)
      argv_.push_back(argv[i]);
  }

  AtomCommandLine* AtomCommandLine::ForCurrentProcess() {
    DCHECK(current_process_commandline_);
    return current_process_commandline_;
  }
}  // namespace atom
