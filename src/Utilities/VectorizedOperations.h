


namespace atomism {
    
    
    template<
    class ScalarType,
    size_t N,
    size_t M,
    class MatrixType,
    class VectorType >
    void multiply( const MatrixType& mat, const VectorType& vec) {
        
        #error multiply not implemented for the templated types
    }
 
    template<class ScalarType,
    size_t N,
    size_t M>
    void multiply<ScalarType,N, M,eigen::Matrix<ScalarType,N,M>,eigen::Matrix<ScalarType,N,1> >(
                                            const eigen::Matrix<ScalarType,N,M>& mat,
                                            const eigen::Matrix<ScalarType,N,1>& vec,
                                            vex::vector& result) {
        
        result = mat * vec;
    }
    
    // dynamic allocated size
    
    template<
    class ScalarType,
    class MatrixType,
    class VectorType >
    void multiply( const MatrixType& mat, const VectorType& vec) {
        
#error multiply not implemented for the templated types
    }
    // vexCl
    
    template<class ScalarType>
    void multiply<ScalarType,vex::vector<ScalarType>, vex::vectorScalarType >(
                                                                                  const vex::vector<ScalarType>& mat,
                                                                                  const vex::vector<ScalarType>& vec,
                                                                                  vex::vector<ScalarType>& result) {
        
        result = vex::reduce<SUM>( vex::extents[N][M],
                                  mat * reshape( vec, extents[n][m], extents[1] ),
                                  1
                                  );
    }
    
    // eigen
    
    template<class ScalarType>
    void multiply<ScalarType,
                  eigen::Matrix<ScalarType,Dynamic,Dynamic>,
                  eigen::Matrix<ScalarType,Dynamic,1> >( const eigen::Matrix<ScalarType,Dynamic,Dynamic>& mat,
                                                         const eigen::Matrix<ScalarType,1,Dynamic>& vec,
                                                         eigen::Matrix<ScalarType,1,N>& result) {
        
        result = mat * vec;
    }
    
    //-----------------------------------------------------------------------------
    //-----------------------------------------------------------------------------
    
    template<
    class ScalarType,
    class VectorType >
    ScalarType innerProduct(const VectorType& x, const VectorType& y) {
        
        #error multiply not implemented for the templated types
        return sum(r);
    }
    
    template<class ScalarType>
    ScalarType innerProduct<ScalarType,vex::vector<ScalarType> >(const vex::vector<ScalarType>& x,
                                                                 const vex::vector<ScalarType>& y) {
        
        VectorType r = x * y ;
        return sum(r);
    }
    
    template<class ScalarType>
    ScalarType innerProduct<ScalarType,eigen::Matrix<ScalarType,Dynamic,1> >(
                                                                const eigen::Matrix<ScalarType,Dynamic,1>& x,
                                                                const eigen::Matrix<ScalarType,Dynamic,1>& y) {
        
        VectorType r = x.dot(y) ;
        return sum(r);
    }
    //-----------------------------------------------------------------------------
    //-----------------------------------------------------------------------------
    
    template<
    class VectorPositionType,
    class Vector3dType >
    inline
    Vector3d translate(VectorPositionType& coors,
                       const Vector3dType& trans ) {
        
        ATOMISM_LOGGIN();
        coors = std::tie(coors(0) + trans[0],
                         coors(1) + trans[1],
                         coors(2) + trans[2]);
        ATOMISM_LOGOUT();
    }
    
    //-----------------------------------------------------------------------------
    //-----------------------------------------------------------------------------
    
    template<
    class VectorPositionType,
    class Matrix3DType >
    inline
    Vector3d rotate(VectorPositionType& coors,
                    const Matrix3DType& rot ) {
        
        ATOMISM_LOGGIN();
        
        coors = std::tie(coors(0) * rot[0][0] + coors(1) * rot[0][1] + coors(2) * rot[0][2] ,
                         coors(0) * rot[1][0] + coors(1) * rot[1][1] + coors(2) * rot[1][2] ,
                         coors(0) * rot[2][0] + coors(1) * rot[2][1] + coors(2) * rot[2][2]
                         );
        
        ATOMISM_LOGOUT();
    }
    
    //-----------------------------------------------------------------------------
    //-----------------------------------------------------------------------------
    
    template< class VectorPositionType >
    inline
    Vector3d sumOutterProduct(const VectorPositionType& coors0,
                              const VectorPositionType& coors1 ) {
        ATOMISM_LOGGIN();
        
        VectorPositionType tmp = std::tie(coors0(1)*coors1(2) - coors0(2)*coors1(1),
                                          coors0(0)*coors1(2) - coors0(2)*coors1(0),
                                          coors0(0)*coors1(1) - coors0(1)*coors1(0));
        
        ATOMISM_LOGOUT();
        return sum(tmp);
    }
    
    //-----------------------------------------------------------------------------
    //-----------------------------------------------------------------------------
    
    template<
    class VectorType,
    class VectorPositionType >
    inline
    Vector3d computeLinearMomentum(const VectorPositionType& coors0,
                                   const VectorPositionType& coors1,
                                   const VectorType& masses){
        ATOMISM_LOGGIN();
        
        linearMomentum = std::tie(masses*( coors1(0) - coors0(0) ),
                                  masses*( coors1(1) - coors0(1) ),
                                  masses*( coors1(2) - coors0(2) ));
        ATOMISM_LOGOUT();
    }
    
    //-----------------------------------------------------------------------------
    //-----------------------------------------------------------------------------
    
    template<class VectorType,
    class VectorPositionType,
    class Matrix3D>
    inline
    void computeLinearMomentum( Matrix3D& rot,
                               const VectorPositionType& coors,
                               const VectorType& masses,
                               VectorPositionType& linearMomentum){
        ATOMISM_LOGGIN();
        ATOMISM_EXCEPT_IF([&]() { return coors.size()!=masses.size(); },
                          "unconsistent size between coordinates and masses");
        
        linearMomentum = std::tie(masses*( rot[0][0] * coors0(0) + rot[0][1]* coors0(1) + rot[0][2]* coors0(2) - coors0(0)),
                                  masses*( rot[1][0] * coors0(0) + rot[1][1]* coors0(1) + rot[1][2]* coors0(2) - coors0(1)),
                                  masses*( rot[2][0] * coors0(0) + rot[2][1]* coors0(1) + rot[2][2]* coors0(2) - coors0(2)) );
        ATOMISM_LOGOUT();
    }
    
    //-----------------------------------------------------------------------------
    //-----------------------------------------------------------------------------
    
    template<class VectorType,
    class VectorPositionType,
    class Vector3D,
    class Matrix3D>
    inline
    void computeAngularMomentum(const Matrix3D& rot,
                                const VectorPositionType& coors,
                                const VectorType& masses){
        ATOMISM_LOGGIN();
        
        Coordinates3D<vectorType> linearMomentum;
        
        computeLinearMomentum( rot, coors, masses, linearMomentum);
        Vector3D J = sumOutterProducts(coors,linearMomentum);
        
        ATOMISM_LOGOUT();
        return J;
    }
    
}