//
// (c) Copyright 2017 DESY,ESS
//
// This file is part of h5cpp.
//
// This library is free software; you can redistribute it and/or modify it
// under the terms of the GNU Lesser General Public License as published
// by the Free Software Foundation; either version 2.1 of the License, or
// (at your option) any later version.
//
// This library is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty ofMERCHANTABILITY
// or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
// License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this library; if not, write to the
// Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor
// Boston, MA  02110-1301 USA
// ===========================================================================
//
// Author: Eugen Wintersberger <eugen.wintersberger@desy.de>
// Created on: Sep 11, 2017
//
#pragma once

#include "group_view.hpp"
#include "../windows.hpp"

namespace hdf5 {
namespace node {

class Link;

class DLL_EXPORT LinkView : public GroupView
{
  public:
    LinkView(Group &group);
    LinkView(const LinkView &) = default;
    LinkView() = delete;

    bool exists(const std::string &name,
                const property::LinkAccessList &lapl=property::LinkAccessList()) const;

    Link operator[](size_t index) const;
    Link operator[](const std::string &name) const;

};

} // namespace node
} // namespace hdf5