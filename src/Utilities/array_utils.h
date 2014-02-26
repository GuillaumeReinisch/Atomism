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


#ifndef ATOMISM_ARRAY_DECL_H
#define ATOMISM_ARRAY_DECL_H

#include <array_utils_decl.h>
#include <array>

namespace std
{
  

  template <typename T, typename T2,int N>
  inline
  std::array<T,N>
  pow (const std::array<T,N>& in, const std::array<T2,N>& n) {
    
      std::array<T> out=in;
      const size_t size = in.size();
      for (size_t i=0; i != size; ++i)
          out[i] = pow(in[i], n);
      return out;
  }

  template <typename T,int N>
  inline
  std::array<T,N>
  max (const std::array<T,N>& a, const std::array<T,N>& b) {
    
      using std::max;

      const size_t size = a.size();
      std::array<T,N> out(size);
      for (size_t i=0; i != size; ++i)
          out[i] = max(a[i], b[i]);
      return out;
  }

  template <typename T,int N>
  inline
  std::array<T,N>
  min (const std::array<T,N>& a, const std::array<T,N>& b) {
    
      using std::min;

      const size_t size = a.size();
      std::valarray<T> out(size);
      for (size_t i=0; i != size; ++i)
          out[i] = min(a[i], b[i]);
      
      return out;
  }


} // end namespace std


namespace atomism
{

  template <typename T,int N,int M>
  inline
  std::array< std::array<T,M>, N> zero() {
   
      std::array< std::array<T,M>, N> out;
      for(size_t i=0; i<N;i++) 
	  for(size_t j=0;j<M;j++)
	      out[i][j]=zero<T>();
      return out;
  };
  
  template <typename T,int N>
  inline
  std::array<T,N>
  zero_clone(const std::array<T,N>& example) {
    
      if (example.size()){
          
	  std::array<T,N> out;
	  out.fill( zero_clone(example(0)));
	  return out;
      }
      else
          return std::array<T,N>();
}

  template <typename T1, typename T2,int N>
  inline
  void
  zero_clone(std::array<T,N>& output, const  std::array<T2,N>& example) {
    
      const std::size_t sz = example.size();
      
      for (std::size_t i=0; i != sz; ++i)
          zero_clone(output[i], example[i]);
  }

  template <typename T, typename Scalar,int N>
  inline
  std::array<T,N>
  constant_clone(const std::valarray<T>& example, const Scalar& value) {
    
      if (example.size()){
          
	  std::array<T,N> out;
	  out.fill( value);
	  return out;
      }
      else
          return std::array<T,N>();
  }

  template <typename T,int N>
  inline
  void
  init_clone(std::array<T,N>& output, const std::array<T,N>& example) {
    
      const std::size_t sz = example.size();
      for (std::size_t i=0; i != sz; ++i)
           init_clone(output[i], example[i]);
  }


}