// Copyright (c) 2014 GitHub, Inc.
// Use of this source code is governed by the MIT license that can be
// found in the LICENSE file.

#include "atom/app/atom_content_client.h"

#include <string>
#include <vector>

#include "atom/common/atom_version.h"
#include "atom/common/chrome_version.h"
#include "atom/common/options_switches.h"
#include "base/command_line.h"
#include "base/strings/string_split.h"
#include "base/strings/string_util.h"
#include "content/public/common/content_constants.h"
#include "content/public/common/pepper_plugin_info.h"
#include "content/public/common/user_agent.h"
#include "ppapi/shared_impl/ppapi_permissions.h"

#if defined(OS_LINUX) && !defined(DiSABLE_NACL)
#include "base/path_service.h"
#include "components/nacl/common/nacl_constants.h"
#include "components/nacl/renderer/plugin/ppapi_entrypoints.h"
#endif

namespace atom {

namespace {

bool GetNaClPluginPath(base::FilePath* path) {
  base::FilePath module;
  if (!PathService::Get(base::DIR_MODULE, &module))
    return false;
  *path = module.Append(nacl::kInternalNaClPluginFileName);
  return true;
}

content::PepperPluginInfo CreatePepperFlashInfo(const base::FilePath& path,
                                                const std::string& version) {
  content::PepperPluginInfo plugin;

  plugin.is_out_of_process = true;
  plugin.name = content::kFlashPluginName;
  plugin.path = path;
  plugin.permissions = ppapi::PERMISSION_ALL_BITS;

  std::vector<std::string> flash_version_numbers;
  base::SplitString(version, '.', &flash_version_numbers);
  if (flash_version_numbers.size() < 1)
    flash_version_numbers.push_back("11");
  // |SplitString()| puts in an empty string given an empty string. :(
  else if (flash_version_numbers[0].empty())
    flash_version_numbers[0] = "11";
  if (flash_version_numbers.size() < 2)
    flash_version_numbers.push_back("2");
  if (flash_version_numbers.size() < 3)
    flash_version_numbers.push_back("999");
  if (flash_version_numbers.size() < 4)
    flash_version_numbers.push_back("999");
  // E.g., "Shockwave Flash 10.2 r154":
  plugin.description = plugin.name + " " + flash_version_numbers[0] + "." +
      flash_version_numbers[1] + " r" + flash_version_numbers[2];
  plugin.version = JoinString(flash_version_numbers, '.');
  content::WebPluginMimeType swf_mime_type(
      content::kFlashPluginSwfMimeType,
      content::kFlashPluginSwfExtension,
      content::kFlashPluginSwfDescription);
  plugin.mime_types.push_back(swf_mime_type);
  content::WebPluginMimeType spl_mime_type(
      content::kFlashPluginSplMimeType,
      content::kFlashPluginSplExtension,
      content::kFlashPluginSplDescription);
  plugin.mime_types.push_back(spl_mime_type);

  return plugin;
}

bool CreateNaClInfo(content::PepperPluginInfo* nacl) {
  base::FilePath path;
  if (!GetNaClPluginPath(&path))
    return false;

  nacl->is_internal = true;
  nacl->path = path;
  nacl->name = nacl::kNaClPluginName;
  content::WebPluginMimeType nacl_mime_type(nacl::kNaClPluginMimeType,
                                            nacl::kNaClPluginExtension,
                                            nacl::kNaClPluginDescription);
  nacl->mime_types.push_back(nacl_mime_type);
  content::WebPluginMimeType pnacl_mime_type(nacl::kPnaclPluginMimeType,
                                             nacl::kPnaclPluginExtension,
                                             nacl::kPnaclPluginDescription);
  nacl->mime_types.push_back(pnacl_mime_type);
  nacl->internal_entry_points.get_interface = nacl_plugin::PPP_GetInterface;
  nacl->internal_entry_points.initialize_module =
      nacl_plugin::PPP_InitializeModule;
  nacl->internal_entry_points.shutdown_module = nacl_plugin::PPP_ShutdownModule;
  nacl->permissions = ppapi::PERMISSION_PRIVATE | ppapi::PERMISSION_DEV;

  return true;
}

}  // namespace

AtomContentClient::AtomContentClient() {
}

AtomContentClient::~AtomContentClient() {
}

std::string AtomContentClient::GetProduct() const {
  return "Chrome/" CHROME_VERSION_STRING;
}

std::string AtomContentClient::GetUserAgent() const {
  return content::BuildUserAgentFromProduct(
      "Chrome/" CHROME_VERSION_STRING " "
      ATOM_PRODUCT_NAME "/" ATOM_VERSION_STRING);
}

void AtomContentClient::AddAdditionalSchemes(
    std::vector<std::string>* standard_schemes,
    std::vector<std::string>* savable_schemes) {
  auto command_line = base::CommandLine::ForCurrentProcess();
  auto custom_schemes = command_line->GetSwitchValueASCII(
      switches::kRegisterStandardSchemes);
  if (!custom_schemes.empty()) {
    std::vector<std::string> schemes;
    base::SplitString(custom_schemes, ',', &schemes);
    standard_schemes->insert(standard_schemes->end(),
                             schemes.begin(),
                             schemes.end());
  }
  standard_schemes->push_back("chrome-extension");
}

void AtomContentClient::AddPepperPlugins(
    std::vector<content::PepperPluginInfo>* plugins) {
  auto command_line = base::CommandLine::ForCurrentProcess();

#if defined(OS_LINUX) && !defined(DiSABLE_NACL)
  // Create NaCl plugin info.
  content::PepperPluginInfo nacl_info;
  if (CreateNaClInfo(&nacl_info))
    plugins->push_back(nacl_info);
#endif

  // Create Flash plugin info.
  auto flash_path = command_line->GetSwitchValuePath(
      switches::kPpapiFlashPath);
  if (flash_path.empty())
    return;

  auto flash_version = command_line->GetSwitchValueASCII(
      switches::kPpapiFlashVersion);

  plugins->push_back(
      CreatePepperFlashInfo(flash_path, flash_version));
}

}  // namespace atom
