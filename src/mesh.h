#ifndef __MESH_FEM2A__
#define __MESH_FEM2A__

#include <vector>
#include <string>

namespace FEM2A {

    struct vertex {
        double x ;
        double y ;
    } ;
    typedef vertex vec2 ;

    class Mesh {
        public:
            Mesh() ;

            int nb_vertices() const ;
            int nb_edges() const ;
            int nb_triangles() const ;

            vertex get_vertex( int vertex_index ) const ;
            vertex get_edge_vertex( int edge_index, int vertex_local_index ) const ;
            vertex get_triangle_vertex( int triangle_index, int vertex_local_index ) const ;

            int get_edge_vertex_index( int edge_index, int vertex_local_index ) const ;
            int get_triangle_vertex_index( int triangle_index, int vertex_local_index ) const ;

            int get_vertex_attribute( int vertex_index ) const ;
            int get_edge_attribute( int edge_index ) const ;
            int get_triangle_attribute( int triangle_index ) const ;

            int get_bdr_attr_max() const ;
            int get_attr_max() const ;

            /**
             * \brief  Sets the attribute of a triangle (if border is false) or of a segment
             *         (if border is true) to attribute_index if the function region applied
             *         to all vertices is stricty positive.
             *         Overwrites the previous region attributes.
             *
             * \param region Function used to define a region
             * \param attribute_index The index of the region to set
             * \param border Apply to triangles if false, to edges if true
             */
            void set_attribute( double (*region)(vertex), int attribute_index, bool border ) ;

            bool load( const std::string& file_name ) ;
            bool save( const std::string& file_name ) const ;

        private:
            std::vector< vertex > vertices_ ;
            std::vector< int > edges_ ;
            std::vector< int > triangles_ ;
            std::vector< int > vertex_attributes_ ;
            std::vector< int > edge_attributes_ ;
            std::vector< int > triangle_attributes_ ;

            int bdr_attr_max_ ;
            int attr_max_ ;
    } ;

    void save_solution( const std::vector<double>& x, const std::string& filename ) ;

}
#endif
