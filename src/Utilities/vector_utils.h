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

#include <vector_utils_decl.h>

#include <math.h>
namespace atomism
{
  
  template <typename T>
  inline
  size_t noOfElements(const std::vector<T>& out){
    return out.size();
  };
  
  template <typename T>
  inline
  size_t noOfElements(const std::array<std::vector<T>,3>& out){
    
    ATOMISM_LOG();
    ATOMISM_VALUE_MISMATCH( [&](){return out[0].size();} ,
			    [&](){return out[1].size();});
    ATOMISM_VALUE_MISMATCH( [&](){return out[0].size();} ,
			    [&](){return out[2].size();});
    return out[0].size();
  };
  
  
  template<typename T1,typename T2>
  std::vector<T1> operator* (const std::vector<T1>& x,const std::vector<T2>& y){
     
      ATOMISM_VALUE_MISMATCH( [&](){return x.size();},[&](){return y.size();});
      
      std::vector<T1> v(x.size(),0);
      for( size_t i=0;i<x.size();i++) v[i]=x[i]*y[i];
      return v;
  }
  
  template<typename T>
  std::vector<T> sin(const std::vector<T>& x){
     
      std::vector<T> v(x.size(),0);
      for( size_t i=0;i<x.size();i++) v[i]=std::sin(x[i]);
      return v;
  }
  
  template<typename T>
  std::vector<T> cos(const std::vector<T>& x){
     
      std::vector<T> v(x.size(),0);
      for( size_t i=0;i<x.size();i++) v[i]=std::cos(x[i]);
      return v;
  }
  
  template <typename T>
  inline
  void allocate(std::vector<T>& out,size_t n) {
    
      ATOMISM_LOG();
      out.resize(n);
  }
  
  template <typename T>
  inline
  void allocate(std::array<std::vector<T>,3>& out,size_t n) {
    
      ATOMISM_LOG();
      out[0] = vector<T>(n);
      out[1] = vector<T>(n);
      out[2] = vector<T>(n);
  }
    
  template <typename T>
  inline
  std::vector<T>
  zero_clone(const std::vector<T>& example) {
    
      ATOMISM_LOG();
      if (example.size()){
	  std::vector<T> out(example.size(),0);
	  return out;
      }
      else{ return std::vector<T>(); }
      
  }
  
  template <typename T>
  inline
  std::vector<T> constant_clone(const std::vector<T>& example,const T& v){
    
      ATOMISM_LOG();
      if (example.size()){
	  std::vector<T> out(example.size(),v);
	  return out;
      }
      else{ return std::vector<T>(); }
       
  }
  
  template <typename T>
  inline
  void
  init_constant(std::vector<T>& example,const T& v) {
    
      ATOMISM_LOG();
      for(auto i:example) i=v;      
  }
  
  template <typename T>
  inline
  void init_range(std::vector<T>& example,const T& min,const T& max) {
    
      ATOMISM_LOG();
      ATOMISM_EXCEPT_IF( [&](){return max<min;});
      
      if( !example.size() ) return;
      for(size_t i=0;i<example.size();++i) example[i]=i*(max-min)/example.size();
  }
  
 /*
  template <typename S,int N,int M>
  inline
  std::array< std::array<S,N>,M>
  zero_clone(std::array< std::array<S,N>,M>&& example) {
    
      std::array< std::array<S,N>,M> out;
      for( size_t i=0; i<M; i++) for(size_t j=0;j<N;j++) out[i][j]=0;
      return out;
  }
  
  template <typename S,int N,int M>
  inline
  std::array< std::array<S,N>,M>&
  slice(size_t i, const std::array< std::array<S,N>,M>& matrix) {
    
      return matrix[i];
  }
  
  
  template <typename T1, typename T2,int N>
  inline
  void
  zero_clone(std::array<T1,N>& output, const  std::array<T2,N>& example) {
    
      const std::size_t sz = example.size();
      
      for (std::size_t i=0; i != sz; ++i)
          zero_clone(output[i], example[i]);
  }

  template <typename T, int N>
  inline
  std::array<T,N>
  constant_clone(const std::array<T,N>& example, const T& value) {
    
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

 */
}

#endif