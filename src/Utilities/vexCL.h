


namespace atomism {
    
    
    template<
    class MatrixType,
    class VectorType >
    VectorType multiply(const MatrixType& mat, const VectorType& vec) {
    
        return mat * vec;
    }
    
    
}