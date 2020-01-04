// SPDX-License-Identifier: GPL-3.0-or-later

#include "Version.h"
#include "VersionConfig.h"

#include <fmt/format.h>

const auto relVer = fmt::format(FMT_STRING("{}.{}.{}"),
                                g_project_version_major,
                                g_project_version_minor,
                                g_project_version_patch);

const auto devVer = fmt::format(FMT_STRING("{}-dev + {}"),
                                relVer,
                                fmt::string_view(g_project_git_commit_hash, 7));

const std::string prettyVersion() {
	if constexpr(g_project_version_devel) {
		return devVer;
	} else {
		return relVer;
	}
}
