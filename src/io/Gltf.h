/* SPDX-License-Identifier: GPL-3.0-or-later */
#pragma once

#include <string>

namespace Animorph {
class Mesh;
}

void IoGltfWrite(const Animorph::Mesh &mesh, const std::string &fileName);
