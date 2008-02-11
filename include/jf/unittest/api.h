// -*- C++ -*-

// Copyright (C) 2008 Joerg Faschingbauer

// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License
// as published by the Free Software Foundation; either version 2.1 of
// the License, or (at your option) any later version.

// This library is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.

// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
// USA

#ifndef HAVE_JF_UNITTEST_API_H
#define HAVE_JF_UNITTEST_API_H

#ifdef _MSC_VER
#  ifdef JF_UNITTEST_EXPORTS
#     define JF_UNITTEST_API __declspec(dllexport)
#  else
#     define JF_UNITTEST_API __declspec(dllimport)
#  endif
#  define JF_UNITTEST_LOCAL
   // warning C4251: 'AnyClass::m_member' : class 'std::vector<_Ty>' needs to have dll-interface to be used by clients of class 'AnyClass'
#  pragma warning (disable: 4251)
   // warning C4127: conditional expression is constant
#  pragma warning (disable: 4127)
#elif defined _GNUC_
#  ifdef JF_UNITTEST_EXPORTS
#     if _GNUC_ > 4
#        define JF_UNITTEST_API    __attribute__ ((visibility("default")))
#        define JF_UNITTEST_LOCAL  __attribute__ ((visibility("hidden")))
#     elif _GNUC_ > 3
#        define JF_UNITTEST_API __attribute__ ((visibility("default")))
#        define JF_UNITTEST_LOCAL
#     endif
#  else
#     define JF_UNITTEST_API
#     define JF_UNITTEST_LOCAL
#  endif
#else
#  define JF_UNITTEST_API
#  define JF_UNITTEST_LOCAL
#endif

#endif // HAVE_JF_UNITTEST_API_H
