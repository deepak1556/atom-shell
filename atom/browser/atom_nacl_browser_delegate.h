// Copyright (c) 2015 GitHub, Inc.
// Use of this source code is governed by the MIT license that can be
// found in the LICENSE file.

#ifndef ATOM_BROWSER_ATOM_NACL_BROWSER_DELEGATE_H_
#define ATOM_BROWSER_ATOM_NACL_BROWSER_DELEGATE_H_

#include <string>

#include "components/nacl/browser/nacl_browser_delegate.h"

namespace atom {

class AtomNaClBrowserDelegate : public NaClBrowserDelegate {
 public:
  AtomNaClBrowserDelegate();
  ~AtomNaClBrowserDelegate() override;

  // NaClBrowserDelegate:
  void ShowMissingArchInfobar(
      int render_process_id,
      int render_view_id) override;
  bool DialogsAreSuppressed() override;
  bool GetCacheDirectory(base::FilePath* cache_dir) override;
  bool GetPluginDirectory(base::FilePath* plugin_dir) override;
  bool GetPnaclDirectory(base::FilePath* pnacl_dir) override;
  bool GetUserDirectory(base::FilePath* user_dir) override;
  std::string GetVersionString() const override;
  ppapi::host::HostFactory* CreatePpapiHostFactory(
      content::BrowserPpapiHost* ppapi_host) override;
  bool MapUrlToLocalFilePath(const GURL& url,
                        bool is_blocking,
                        const base::FilePath& profile_dir,
                        base::FilePath* path) override;
  void SetDebugPatterns(
      const std::string& debug_patterns) override;
  bool URLMatchesDebugPatterns(
      const GURL& manifest_url) override;
  content::BrowserPpapiHost::OnKeepaliveCallback
  GetOnKeepaliveCallback() override;
  bool IsNonSfiModeAllowed(
      const base::FilePath& profile_dir,
      const GURL& manifest_url) override;

 private:
  DISALLOW_COPY_AND_ASSIGN(AtomNaClBrowserDelegate);
};

}  // namespace atom

#endif  // ATOM_BROWSER_ATOM_NACL_BROWSER_DELEGATE_H_
