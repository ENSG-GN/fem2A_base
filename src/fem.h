#pragma once

#include "mesh.h"
#include "solver.h"

#include <assert.h>
#include <string>
#include <vector>


namespace FEM2A {

    /**
     * \brief Structure used to store a quadrature, which is a set of
     *        weights and points.
     */
    struct Quadrature {

        /* Methods */
        int nb_points() const ;
        vertex point( int i ) const ;
        double weight( int i ) const ;

        /**
         * \brief Gets an instance of Quadrature to integrate polynomials
         *        of degree order (= 0, 2, 4 or 6).
         *
         * \param order Order of the polynomials which will be exactly
         *              integrated by the quadrature
         * \param border If true, quadrature of the segment [0,1]; else,
         *               quadrature of the triangle (0,0)(1,0)(0,1)
         */
        static Quadrature get_quadrature( int order, bool border = false ) ;

        /* Data */
        std::vector< double > wxy_ ;

    } ;

    /**********************************************************/
    /*             _______ ____  _____   ____                 */
    /*            |__   __/ __ \|  __ \ / __ \                */
    /*               | | | |  | | |  | | |  | |               */
    /*               | | | |  | | |  | | |  | |               */
    /*               | | | |__| | |__| | |__| |               */
    /*               |_|  \____/|_____/ \____/                */
    /*                                                        */
    /* All the classes and methods below are not implemented. */
    /* It's your work to do it. Good luck !                   */
    /**********************************************************/

    /**
     * \brief ElementMapping is a class used to represent the mapping
     *        of an element from its reference space to the world space.
     *        Reference elements:
     *          segment: [0,1]
     *          triangle: (0,0) (1,0) (0,1)
     */
    class ElementMapping {
        public:
            /**
             * \brief Builds a mapping from the element i of a mesh M
             *
             * \param M The Mesh
             * \param border If true, the element is an edge.
             *               If false, the element is a triangle.
             * \param i The index of the element in the mesh M
             */
            ElementMapping( const Mesh& M, bool border, int i ) ;

            /**
             * \brief Transforms a point from the reference element
             *        to the world space.
             * \param x_r Position in the reference element
             * \return the position in the world space
             */
            vertex transform( vertex x_r ) const ;

            /**
             * \brief Computes the jacobian matrix of the mapping.
             * \param x_r Position in the reference space
             * \return the jacobian matrix of the mapping
             */
            DenseMatrix jacobian_matrix( vertex x_r ) const ;

            /**
             * \brief  Computes the determinant of the jacobian matrix
             *         of the mapping.
             * \param x_r Position in the reference space
             * \return the determinant of the jacobian matrix
             */
            double jacobian( vertex x_r ) const ;

        private:
            bool border_ ;
            std::vector< vertex > vertices_ ;
    } ;

    /**
     * \brief ShapeFunctions is a class that defines the interpolation
     *        functions on the reference triangle (if dim == 2) or on
     *        the reference segment (if dim == 1).
     *        Only defined for order 1 -> The functions are linear.
     *        Reference elements:
     *          segment: [0,1]
     *          triangle: (0,0) (1,0) (0,1)
     */
    class ShapeFunctions {
        public:
            /**
             * \brief Constructor of the ShapeFunctions
             * \param dim 1 for reference segment, 2 for reference triangle
             * \param order Should be 1 (linear functions only)
             */
            ShapeFunctions( int dim, int order ) ;

            /**
             * \brief Number of shape functions
             * \return 2 if segment, 3 if triangle
             */
            int nb_functions() const ;

            /**
             * \brief Evaluates the i-th shape function at a given reference
                      point.
             * \param i The index of the shape function
             * \param x_r Position in the reference space
             * \return the value of the i-th shape function at x_r
             */
            double evaluate( int i, vertex x_r ) const ;

            /**
             * \brief Evaluates the gradient of the i-th shape function at a
             *        given reference point.
             * \param i The index of the shape function
             * \param x_r Position in the reference space
             * \return the gradient of the i-th shape function at x_r
             */
            vec2 evaluate_grad( int i, vertex x_r ) const ;

        private:
            int dim_ ;
            int order_ ;
    } ;

    /****************************/
    /* Finite Element functions */
    /****************************/

    /**
     * \brief Computes the elementary matrix Ke associated
     *        to a triangle defined by its ElementMapping.
     *
     * \param[in] elt_mapping The mapping of the considered triangle
     * \param[in] reference_functions The shape functions on the
                                      reference triangle
     * \param[in] quadrature The quadrature on the reference triangle
     * \param[in] coefficient The function associated to the diffusion
     *                        coefficient k(x,y)
     * \param[out] Ke The result
     */
    void assemble_elementary_matrix(
        const ElementMapping& elt_mapping,
        const ShapeFunctions& reference_functions,
        const Quadrature& quadrature,
        double (*coefficient)(vertex),
        DenseMatrix& Ke ) ;

    /**
     * \brief  Adds the contribution Ke of triangle t to
     *         the global matrix K.
     *
     * \param[in] M The mesh
     * \param[in] t The index of the triangle
     * \param[in] Ke The local contribution
     * \param[in,out] K The global matrix
     */
    void local_to_global_matrix(
        const Mesh& M,
        int t,
        const DenseMatrix& Ke,
        SparseMatrix& K ) ;

    /**
     * \brief Computes the elementary vector Fe associated to a
     *        triangle defined by its ElementMapping due to the
     *        source term.
     *
     * \param[in] elt_mapping The mapping of the considered triangle
     * \param[in] reference_functions The shape functions on the
                                      reference triangle
     * \param[in] quadrature The quadrature on the reference triangle
     * \param[in] source The function associated to the source term
                         f(x,y)
     * \param[out] Fe The result
     */
    void assemble_elementary_vector(
        const ElementMapping& elt_mapping,
        const ShapeFunctions& reference_functions,
        const Quadrature& quadrature,
        double (*source)(vertex),
        std::vector< double >& Fe ) ;

    /**
     * \brief Computes the elementary vector Fe associated to
     *        an edge defined by its ElementMapping due to the
     *        Neumann condition.
     *
     * \param[in] elt_mapping_1D The mapping of the considered edge
     * \param[in] reference_functions_1D The shape functions on the
                                         reference segment
     * \param[in] quadrature_1D The quadrature on the reference segment
     * \param[in] neumann The function associated to the neumann
     *                    condition h(x,y)
     * \param[out] Fe The result
     */
    void assemble_elementary_neumann_vector(
        const ElementMapping& elt_mapping_1D,
        const ShapeFunctions& reference_functions_1D,
        const Quadrature& quadrature_1D,
        double (*neumann)(vertex),
        std::vector< double >& Fe ) ;

    /**
     * \brief  Adds the contribution Fe of element i (which is a
     *         triangle if border is false or an edge if border is
     *         true) to the global vector F.
     *
     * \param[in] M The mesh
     * \param[in] border true if edge, false if triangle
     * \param[in] i The index of the element
     * \param[in] Fe The local contribution
     * \param[in,out] F The global vector
     */
    void local_to_global_vector(
        const Mesh& M,
        bool border,
        int i,
        std::vector< double >& Fe,
        std::vector< double >& F ) ;

    /**
     * \brief  Modifies the linear system with the penalty method to
     *         apply Dirichlet boundary conditions.
     *
     * \param[in] M The mesh
     * \param[in] attribute_is_dirichlet The values are imposed at vertices of
     *                                   the edges whose attribute i is such
     *                                   that attribute_is_dirichlet[i] = true
     * \param[in] values The values imposed at vertices; size must be M.nb_vertices()
     * \param[in,ou] K The global matrix
     * \param[in,ou] F The global vector
     */
    void apply_dirichlet_boundary_conditions(
        const Mesh& M,
        const std::vector< bool >& attribute_is_dirichlet, /* size: nb of attributes */
        const std::vector< double >& values, /* size: nb of DOFs */
        SparseMatrix& K,
        std::vector< double >& F ) ;

    /**
     * \brief Genereal function to solve a Poisson problem with the
     *        finite element method
     *
     * \param[in] problem_name A name assigned to problem (for logging)
     * \param[in] M the mesh with its attributes already set
     * \param[in] diffusion_coef Function used to compute the diffusion coefficient,
     *                       applied to all triangles
     * \param[in] source_term Function used to compute the value of the source
     *                    applied to all triangles whose attribute is 1
     * \param[in] dirichlet_fct Function used to compute the value of the solution
     *                      at vertices of the border edges whose attribute is 1
     * \param[in] neumann_fct Function used to compute the value of the normal component
     *                    of the gradient of the solution at border edges whose
     *                    attribute is 2
     * \param[out] solution Reference to the solution, will be filled by the function
     * \param[in] verbose Flag that can be use to print lot of details when verbose > 0
     */
    void solve_poisson_problem(
            const Mesh& M,
            double (*diffusion_coef)(vertex),
            double (*source_term)(vertex),
            double (*dirichlet_fct)(vertex),
            double (*neumann_fct)(vertex),
            std::vector<double>& solution,
            bool verbose ) ;

}

