// Copyright (c) 2015 GitHub, Inc.
// Use of this source code is governed by the MIT license that can be
// found in the LICENSE file.

#ifndef ATOM_APP_ATOM_MAIN_ARGS_H_
#define ATOM_APP_ATOM_MAIN_ARGS_H_

#include <string>
#include <vector>

#include "base/strings/string16.h"
#include "base/basictypes.h"

namespace atom {

class AtomCommandLine {
 public:
  #if defined(OS_WIN)
    typedef base::string16 StringType;
  #elif defined(OS_POSIX)
    typedef std::string StringType;
  #endif

  typedef StringType::value_type CharType;
  typedef std::vector<StringType> StringVector;

  enum NoProgram { NO_PROGRAM };
  explicit AtomCommandLine(NoProgram no_program);
  ~AtomCommandLine();

  static bool Init(int argc, const CharType* const* argv);
  void InitFromArgv(int argc, const CharType* const* argv);
  static AtomCommandLine* ForCurrentProcess();
  static bool InitialisedForCurrentProcess();
  static void Reset();
  const StringVector& argv() const { return argv_; }

 private:
  AtomCommandLine();
  static AtomCommandLine* current_process_commandline_;
  StringVector argv_;

  DISALLOW_COPY_AND_ASSIGN(AtomCommandLine);
};

}  // namespace atom

#endif  // ATOM_APP_ATOM_MAIN_ARGS_H_
