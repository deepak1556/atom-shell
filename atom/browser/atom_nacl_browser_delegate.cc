// Copyright (c) 2015 GitHub, Inc.
// Use of this source code is governed by the MIT license that can be
// found in the LICENSE file.

#include "atom/browser/atom_nacl_browser_delegate.h"

#include <vector>

#include "atom/common/chrome_version.h"
#include "base/files/file_path.h"
#include "base/path_service.h"
#include "base/strings/string_split.h"
#include "brightray/browser/brightray_paths.h"
#include "chrome/browser/renderer_host/pepper/chrome_browser_pepper_host_factory.h"
#include "url/gurl.h"

using content::BrowserPpapiHost;

namespace atom {

namespace {

void OnKeepalive(
    const BrowserPpapiHost::OnKeepaliveInstanceData& instance_data,
    const base::FilePath& profile_dir) {
  // NoOp
}

}  // namespace

AtomNaClBrowserDelegate::AtomNaClBrowserDelegate() {
}

AtomNaClBrowserDelegate::~AtomNaClBrowserDelegate() {
}

void AtomNaClBrowserDelegate::ShowMissingArchInfobar(
    int render_process_id, int render_view_id) {
  LOG(ERROR) << "Missing architecture for pid: " << render_process_id;
}

bool AtomNaClBrowserDelegate::DialogsAreSuppressed() {
  return true;
}

bool AtomNaClBrowserDelegate::GetCacheDirectory(
    base::FilePath* cache_dir) {
  return PathService::Get(brightray::DIR_CACHE, cache_dir);
}

bool AtomNaClBrowserDelegate::GetPluginDirectory(
    base::FilePath* plugin_dir) {
  base::FilePath user_dir;
  if (!GetUserDirectory(&user_dir))
    return false;
  *plugin_dir = user_dir.Append(FILE_PATH_LITERAL("plugins"));
  return true;
}

bool AtomNaClBrowserDelegate::GetPnaclDirectory(
    base::FilePath* pnacl_dir) {
  base::FilePath plugin_dir;
  if (!GetPluginDirectory(&plugin_dir))
    return false;
  *pnacl_dir = plugin_dir.Append(FILE_PATH_LITERAL("pnacl"));
  return true;
}

bool AtomNaClBrowserDelegate::GetUserDirectory(
    base::FilePath* user_dir) {
  return PathService::Get(brightray::DIR_USER_DATA, user_dir);
}

std::string AtomNaClBrowserDelegate::GetVersionString() const {
  return CHROME_VERSION_STRING;
}

ppapi::host::HostFactory* AtomNaClBrowserDelegate::CreatePpapiHostFactory(
    BrowserPpapiHost* ppapi_host) {
  return new chrome::ChromeBrowserPepperHostFactory(ppapi_host);
}

bool AtomNaClBrowserDelegate::MapUrlToLocalFilePath(
    const GURL& url,
    bool is_blocking,
    const base::FilePath& profile_dir,
    base::FilePath* path) {
  if (url.scheme() == "chrome" && url.host() == "pnacl-translator") {
    base::FilePath pnacl_dir;
    if (!GetPnaclDirectory(&pnacl_dir))
      return false;
    std::vector<std::string> tokens =
        base::SplitString(url.path(), "/",
                          base::TRIM_WHITESPACE,
                          base::SPLIT_WANT_NONEMPTY);
    *path = pnacl_dir.Append(FILE_PATH_LITERAL(tokens[1]));
    return true;
  }
  return false;
}

void AtomNaClBrowserDelegate::SetDebugPatterns(
    const std::string& debug_patterns) {
  // NoOp
}

bool AtomNaClBrowserDelegate::URLMatchesDebugPatterns(
    const GURL& manifest_url) {
  return true;
}

content::BrowserPpapiHost::OnKeepaliveCallback
AtomNaClBrowserDelegate::GetOnKeepaliveCallback() {
  return base::Bind(&OnKeepalive);
}

bool AtomNaClBrowserDelegate::IsNonSfiModeAllowed(
    const base::FilePath& profile_dir,
    const GURL& manifest_url) {
  return false;
}

}  // namespace atom
