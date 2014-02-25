
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

/* Vectorized operations. Needs indexing operators and iterators
 * GPU // definition of the functions are in the vexCL file
 */
 

namespace atomism {
    
   
    
    //-----------------------------------------------------------------------------
    //-----------------------------------------------------------------------------
    
    template<
    typename ScalarType,
    typename VectorType>
    ScalarType sum(const VectorType& x) {
        
        ScalarType s(0);
        for(size_t i=0;i<x.size();i++) s+=x[i];
        return s;
    }
    
    //-----------------------------------------------------------------------------
    //-----------------------------------------------------------------------------
    
    template<
    typename ScalarType,
    typename VectorType,
    typename MatrixType>
    VectorType multiply(const MatrixType& mat,
                        const VectorType& vec) {
        
        ATOMISM_LOGGIN();
        
        VectorType result = zero_clone(vec);
        
        for(size_t i=0;i<x.size();i++)
            for(size_t j=0;j<x.size();j++)
                result[i]+=mat[i][j]*vec[i];
        
        ATOMISM_LOGOUT();
        return result;
    }
    
    //-----------------------------------------------------------------------------
    //-----------------------------------------------------------------------------
    
    template<
    typename ScalarType,
    typename VectorType>
    inline
    ScalarType innerProduct(const vex::vector<ScalarType>& x,
                            const vex::vector<ScalarType>& y) {
        
        ATOMISM_LOGGIN();
        ScalarType s(0);
        for(size_t i=0;i<x.size();i++) s+=x[i]*y[i];
        ATOMISM_LOGOUT();
        return s;
    }
    
    //-----------------------------------------------------------------------------
    //-----------------------------------------------------------------------------
    
    template<
    typename ScalarType,
    typename PositionVectorType,
    typename Vector3dType>
    inline
    void translate(PositionVectorType& coors,
                   const Vector3dType& trans ) {
        
        ATOMISM_LOGGIN();
        for( it: coors )
            for( size_t i=0;i<3;i++) it[i] += trans[i];
        ATOMISM_LOGOUT();
    }
    
    //-----------------------------------------------------------------------------
    //-----------------------------------------------------------------------------
    
    template<
    typename ScalarType,
    typename PositionVectorType,
    typename Matrix3DType>
    inline
    void rotate(PositionVectorType& coors,
                const Matrix3DType& rot ) {
        
        ATOMISM_LOGGIN();
        for( it: coors )
            *it = multiply(rot ,(*it));
        ATOMISM_LOGOUT();
    }
    
    //-----------------------------------------------------------------------------
    //-----------------------------------------------------------------------------
    
    template<
    typename ScalarType,
    typename Vector3dType,
    typename PositionVectorType >
    inline
    Vector3dType sumOutterProduct( const PositionVectorType& coors0,
                                   const PositionVectorType& coors1 ) {
        
        ATOMISM_LOGGIN();
        Vector3dType sum(0);
        
        for( size_t i = 0;i <coors0.size(); i++ ){
            
            sum[0] += coors0[i][1]*coors1[i][2] - coors0[i][2]*coors1[i][1];
            sum[1] += coors0[i][0]*coors1[i][2] - coors0[i][2]*coors1[i][0];
            sum[2] += coors0[i][0]*coors1[i][1] - coors0[i][1]*coors1[i][0];
        }
        
        ATOMISM_LOGOUT();
        return sum(tmp);
    }
    
    //-----------------------------------------------------------------------------
    //-----------------------------------------------------------------------------
    
    template<
    typename ScalarType,
    typename VectorType,
    typename VectorPositionType,
    >
    inline
    Vector3dType computeLinearMomentum(const VectorPositionType& coors0,
                                       const VectorPositionType& coors1,
                                       const VectorType& masses){
        
        ATOMISM_LOGGIN();
        
        VectorPositionType linearMomentum = zero_clone(coors0);
        
        for( size_t i = 0;i <coors0.size(); i++ ){
            
            linearMomentum[i][0] = masses[i] * ( coors1[i][0] - coors0[i][0] ) ;
            linearMomentum[i][1] = masses[i] * ( coors1[i][1] - coors0[i][1] ) ;
            linearMomentum[i][2] = masses[i] * ( coors1[i][2] - coors0[i][2] ) ;
        }
        
        ATOMISM_LOGOUT();
    }
    
    //-----------------------------------------------------------------------------
    //-----------------------------------------------------------------------------
    
    template< typename ScalarType,typename Vector3dType >
    inline
    Vector3dType computeLinearMomentum(const Matrix3D& rot,
                                       const VectorPositionType& coors,
                                       const VectorType& masses,
                                       VectorPositionType& linearMomentum){
        
        ATOMISM_LOGGIN();
        ATOMISM_EXCEPT_IF([&]() { return coors.size()!=masses.size(); },
                          "unconsistent size between coordinates and masses");
        
        VectorPositionType linearMomentum = zero_clone(coors);
        
        for( size_t i =0; i <coors0.size(); i++ )
            
            for(size_t j=0; j<3;j++)
                linearMomentum[i][j] = masses[i] * (rot[j][0] * coors[i][0] +
                                                    rot[j][1] * coors[i][1] +
                                                    rot[j][2] * coors[i][2] - coors[i][j] );
        ATOMISM_LOGOUT();
    }
    
    //-----------------------------------------------------------------------------
    //-----------------------------------------------------------------------------
    
    template< typename ScalarType,typename Vector3dType,typename Matrix3dType >
    inline
    Vector3DType computeAngularMomentum<ScalarType,vex::multivector<ScalarType,3>,Vector3dType,Matrix3dType>(
                                                                                                             const Matrix3DType& rot,
                                                                                                             const VectorPositionType& coors,
                                                                                                             const VectorType& masses){
        
        ATOMISM_LOGGIN();
        
        vex::multivector<ScalarType,3> linearMomentum;
        
        computeLinearMomentum( rot, coors, masses, linearMomentum);
        Vector3DType J = sumOutterProducts(coors,linearMomentum);
        
        ATOMISM_LOGOUT();
        return J;
    }
    
}