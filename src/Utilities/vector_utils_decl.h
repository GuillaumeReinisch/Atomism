/*
 <one line to give the library's name and an idea of what it does.>
 Copyright (C) 2013  Guillaume <email>
 
 This library is free software; you can redistribute it and/or
 modify it under the terms of the GNU Lesser General Public
 License as published by the Free Software Foundation; either
 version 2.1 of the License, or (at your option) any later version.
 
 This library is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 Lesser General Public License for more details.
 
 You should have received a copy of the GNU Lesser General Public
 License along with this library; if not, write to the Free Software
 Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */


#ifndef ATOMISM_VECTOR_DECL_H
#define ATOMISM_VECTOR_DECL_H

#include <metaprogramming_decl.h>
#include <Exceptions.h>
#include <vector>


  

namespace atomism{

  
  template <typename T>
  inline
  size_t noOfElements(const std::vector<T>& out);
  
  template <typename T>
  inline
  size_t noOfElements(const std::array<std::vector<T>,3>& out);
  
  template <typename T>
  inline
  void allocate(std::vector<T>& out,size_t n);
  
  template <typename T>
  inline
  void allocate(std::array<std::vector<T>&,3>& out,size_t n);
  
  template<typename T1,typename T2>
  std::vector<T1> operator* (const std::vector<T1>& x,const std::vector<T2>& y);
  
  template<typename T>
  inline
  std::vector<T> sin(const std::vector<T>& x);
  
  template<typename T>
  inline
  std::vector<T> cos(const std::vector<T>& x);
  
  template <typename T>
  inline
  std::vector<T> zero_clone(const std::vector<T>& example);
  
  template <typename T>
  inline
  std::vector<T> constant_clone(const std::vector<T>& example,const T& v);
  
  template <typename T>
  inline
  void init_constant(std::vector<T>& example,const T& v);
  
  template <typename T>
  inline
  void init_range(std::vector<T>& example,const T& min,const T& max);
  
  
 /*
  template <typename S>
  inline
  std::array< std::array<S,N>,M>
  zero_clone(std::array< std::array<S,N>,M>&& example);
  
  template <typename T1, typename T2>
  inline
  void zero_clone(std::array<T1>& output, const std::array<T2>& example);

  template <typename T>
  inline
  void set_zero(std::array<T>& output);
  
  template <typename T, typename Scalar>
  inline
  T constant_clone(const std::array<T>& example, const Scalar& value);

  template <typename T>
  inline
  void init_clone(std::array<T>& output, const std::array<T>& example);

  template <typename T>
  inline
  void init_constant(std::array<T>& output, const T& example);
*/
  
} // end namespace Atomism

#endif //ANTIOCH_METAPROGRAMMING_DECL_H
