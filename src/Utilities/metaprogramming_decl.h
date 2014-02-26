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


#ifndef ATOMISM_METAPROGRAMMING_DECL_H
#define ATOMISM_METAPROGRAMMING_DECL_H

#include <Logger.h>
#include <Exceptions.h>

namespace atomism
{
  

  // A function for zero-initializing a matrix numeric types
  template < typename T, int N , int M = 1 >
  inline
  T zero();
  
  // A function for zero-initializing vectorized numeric types
  // while resizing them to match the example input
  template <typename T>
  inline
  T zero_clone(const T& example);

  // A function for zero-initializing vectorized numeric types
  // while resizing them to match the example input
  template <typename T1, typename T2>
  inline
  void zero_clone(T1& output, const T2& example);

  // A function for zero-setting vectorized numeric types
  template <typename T>
  inline
  void set_zero(T& output);
  
  // A function for initializing vectorized numeric types to a
  // constant // while resizing them to match the example input
  template <typename T, typename Scalar>
  inline
  T constant_clone(const T& example, const Scalar& value);

  // A function for initializing vectorized numeric types
  // while resizing them to match the example input
  template <typename T>
  inline
  void init_clone(T& output, const T& example);

  // A function for initializing numeric vector types.  Resizes the
  // contents of vector-of-vector types but does not resize the outer
  // vector.
  template <typename Vector, typename Scalar>
  inline
  void init_constant(Vector& output, const Scalar& example);

  // A function for summing vector's elements
  template<typename Scalar, typename Vector>
  inline
  Scalar sum(const Vector& x);
    
  // dense matrix vector multiplication 
  template<typename VectorType, typename MatrixType>
  inline
  VectorType multiply(const MatrixType& mat, const VectorType& vec);
  
  // vector's inner product 
  template<typename Scalar, typename Vector >
  inline
  Scalar innerProduct(const Vector& x, const Vector& y);
    
  // translate all positions by a constant vector 
  template< typename Positions, typename Vector3d>
  inline
  void translate(Positions& coors, const Vector3d& trans );
    
  // rotate all positions by a constant rotation matrix 
  template<typename PositionVectorType, typename Matrix3DType>
  inline
  void rotate(PositionVectorType& coors, const Matrix3DType& rot );
    
  // sum outter product of the two Positions vector
  template<typename Vector3d, typename Positions >
  inline
  Vector3d totalOutterProduct( const Positions& coors0, const Positions& coors1 );
    
  // compute the linear momentum between 2 sets of position,
  // using the masses vector
  template<typename Vector, typename Vector3d, typename Positions>
  inline
  Vector3d totalLinearMomentum(const Positions& coors0, const Positions& coors1,
                               const Vector& masses);
    
  // compute the linear momentum of each elements when
  // rotated by a particular rotation matrix
  template< typename Matrix3D,typename Vector, typename Positions>
  inline
  void linearMomentum( Positions& out, const Positions& coors,
		      const Vector& masses, const Matrix3D& rot
 		    );
    
  
  template< typename Matrix3D,typename Vector, typename Positions>
  inline
  Vector totalAngularMomentum( const Matrix3D& rot, const Positions& coors, const Vector& masses);
  
} // end namespace Atomism

#endif //ANTIOCH_METAPROGRAMMING_DECL_H
