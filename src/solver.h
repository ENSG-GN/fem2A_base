#pragma once

#include "mesh.h"

namespace FEM2A {

    /**
     * \brief DenseMatrix is useful to store (small) matrices and to
     *        compute basic operations on 2x2 matrices.
     *
     * In practice, call set_size() to set the matrix size then set()
     * or add() to set/modify the matrix coefficients.
     */
    class DenseMatrix {
        public:
            int height() const;
            int width() const;
            void set_size( int height, int width ) ;
            void set( int i, int j, double v ) ;
            void add( int i, int j, double v ) ;
            double get( int i, int j ) const ;
            void print() const ;
            DenseMatrix transpose() const;

            /* Methods for 2x2 DenseMatrix */
            double det_2x2() const ;
            DenseMatrix invert_2x2() const ;
            vec2 mult_2x2_2( vec2 x ) const ;

        private:
            std::vector< double > data_ ;
            int height_ ;
            int width_ ;
    } ;

    /**
     * \brief SparseMatrix is used to store (large) matrices mainly
     *        composed of zeros. Only the non-zero coefficients are
     *        stored (see the CSR -compressed row storage- format).
     */
    class SparseMatrix {
        public:
            SparseMatrix( int nb_rows ) ;
            int nb_rows() const ;

            /**
             * \brief Adds val to the (i,j) coefficient if it exists
             * or creates a new entry with val if it does not exist.
             * This is equivalent to M(i,j) += val.
             * \param i row index
             * \param j column index
             * \param val value to add
             */
            void add( int i, int j, double val ) ;

            /**
             * \brief Multiplies the (i,j) coefficient by val (should
             * exists). This is equivalent to M(i,j) *= val
             * \param i row index
             * \param j column index
             * \param val multiplier
             */
            void multiply( int i, int j, double val ) ;

            /**
             * \param i row index
             * \return a reference to a vector containing the indices
             * of the non-zero coefficients in row i.
             */
            const std::vector< int > & get_cols_at_line( int i ) const ;

            /**
             * \param i row index
             * \return a reference to a vector containing the values
             * of the non-zero coefficients in row i.
             */
            const std::vector< double > & get_vals_at_line( int i ) const ;

            void print() const ;

        private:
            std::vector< std::vector< int > > cols_at_line_ ;
            std::vector< std::vector< double > > val_at_line_ ;
    } ;

    /**
     * \return the scalar product between two vectors.
     */
    double dot( vec2 x, vec2 y ) ;

    /**
     * \brief  Solve the linear system Ax=b
     *
     * \param A a square sparse matrix
     * \param b the right hand side vector
     * \param x the solution
     *
     * \return true if the solver has converged.
     */
    bool solve(
            const SparseMatrix& A,
            const std::vector<double>& b,
            std::vector<double>& x);

    /**
     * \brief Basic test of the OpenNL library
     * \return true if it works.
     */
    bool test_opennl() ;

}
