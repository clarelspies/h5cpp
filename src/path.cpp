//
// (c) Copyright 2017 DESY,ESS
//
// This file is part of h5pp.
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
// Created on: Aug 24, 2017
//

#include <h5cpp/path.hpp>
#include <boost/algorithm/string.hpp>

namespace hdf5 {

//
// private functions used for path construction - they are not exported to the
// ABI of the library
namespace {

bool is_absolute_path_string(const std::string &str)
{
  if(str[0] == '/')
    return true;
  else
    return false;
}

void sanitize(std::list<std::string>& list)
{
  for (auto i = list.begin(); i != list.end();)
  {
    if (*i == ".")
      i = list.erase(i);
    else
      ++i;
  }
}

std::list<std::string> str_to_list(const std::string &str)
{
  std::list<std::string> result;
  auto string_start = str.begin();
  auto string_end = str.end();

  //ignore a leading / in the input string
  if(is_absolute_path_string(str)) string_start++;

  if(string_start == string_end)
    return result;

  //ignore a trailing / from the input string
  if(str.back()=='/') string_end--;

  std::string buffer(string_start,string_end);
  boost::split(result,buffer,
               boost::is_any_of("/"),boost::token_compress_on);

  sanitize(result);
  return result;
}

} // end of anonymous name space

//=============================================================================
void Path::from_string(const std::string &str)
{
  absolute_ = is_absolute_path_string(str);
  link_names_ = str_to_list(str);
}

std::string Path::to_string() const
{
  if (!is_absolute() && link_names_.empty())
    return ".";
  return (is_absolute() ? "/" : "")
      + boost::algorithm::join(link_names_, "/");
}

Path::Path():
    absolute_(false),
    link_names_()
{}

Path::Path(const std::string &str):
    absolute_(false),
    link_names_()
{
  from_string(str);
}

size_t Path::size() const
{
  return link_names_.size();
}

Path::const_iterator Path::begin() const
{
  return link_names_.begin();
}

Path::const_iterator Path::end() const
{
  return link_names_.end();
}

Path::iterator Path::begin()
{
  return link_names_.begin();
}

Path::iterator Path::end()
{
  return link_names_.end();
}

Path::reverse_iterator Path::rbegin()
{
  return link_names_.rbegin();
}

Path::reverse_iterator Path::rend()
{
  return link_names_.rend();
}

Path::const_reverse_iterator Path::rbegin() const
{
  return link_names_.rbegin();
}

Path::const_reverse_iterator Path::rend() const
{
  return link_names_.rend();
}

void Path::push_front(const value_type &link_name)
{
  link_names_.push_front(link_name);
}

Path::value_type Path::pop_front()
{
  value_type result = link_names_.front();
  link_names_.pop_front();
  return result;
}

void Path::push_back(const Path::value_type &link_name)
{
  link_names_.push_back(link_name);
}

Path::value_type Path::pop_back()
{
  value_type result = link_names_.back();
  link_names_.pop_back();
  return result;
}

bool Path::is_absolute() const noexcept
{
  return absolute_;
}

void Path::set_absolute(bool v) noexcept
{
  absolute_ = v;
}

bool Path::is_root() const
{
  if(is_absolute() && link_names_.empty())
    return true;
  else
    return false;
}

Path operator+(const std::string &link_name,const Path &path)
{
  return Path(link_name) + path;
}

Path operator+(const Path &path,const std::string &link_name)
{
  return path + Path(link_name);
}

Path operator+(const Path &lhs,const Path &rhs)
{
  Path result(lhs);
  std::copy(rhs.begin(),rhs.end(),std::back_inserter(result));
  return result;
}

std::ostream &operator<<(std::ostream &stream,const Path &path)
{
  return stream<<static_cast<std::string>(path);
}

Path::value_type Path::front() const
{
  if(is_root())
    return "/";

  return link_names_.front();
}

Path::value_type Path::back() const
{
  if(is_root())
    return "/";

  return link_names_.back();
}

std::string Path::name() const
{
  if (link_names_.size())
    return link_names_.back();
  return ".";
}

Path Path::parent() const
{
  if(is_root())
    return *this;

  Path p(*this);

  if (p.link_names_.size())
    p.link_names_.pop_back();
  return p;
}

bool operator==(const Path &lhs, const Path &rhs)
{
  return (lhs.absolute_ == rhs.absolute_) && (lhs.link_names_ == rhs.link_names_);
}

bool operator!=(const Path &lhs, const Path &rhs)
{
  return !(lhs == rhs);
}


} // namespace hdf5
