#include "mesh.h"
#include <cassert>
#include <fstream>
#include <iostream>

namespace FEM2A {

    Mesh::Mesh()
        : bdr_attr_max_( 0 ), attr_max_( 0 )
    {

    }
    int Mesh::nb_vertices() const
    {
        return vertices_.size();
    }
    int Mesh::nb_edges() const
    {
        return edges_.size() / 2;
    }
    int Mesh::nb_triangles() const
    {
        return triangles_.size() / 3;
    }

    vertex Mesh::get_vertex( int vertex_index ) const
    {
        assert( vertex_index < vertices_.size() );
        return vertices_[vertex_index];
    }
    vertex Mesh::get_edge_vertex( int edge_index, int vertex_local_index ) const
    {
        assert( edge_index < edges_.size() / 2 );
        assert( vertex_local_index < 2 );
        return vertices_[edges_[2 * edge_index + vertex_local_index]];
    }
    vertex Mesh::get_triangle_vertex( int triangle_index, int vertex_local_index ) const
    {
        assert( triangle_index < triangles_.size() / 3 );
        assert( vertex_local_index < 3 );
        return vertices_[triangles_[3 * triangle_index + vertex_local_index]];
    }

    int Mesh::get_edge_vertex_index( int edge_index, int vertex_local_index ) const
    {
        assert( edge_index < edges_.size() / 2 );
        assert( vertex_local_index < 2 );
        return edges_[2 * edge_index + vertex_local_index];
    }
    int Mesh::get_triangle_vertex_index( int triangle_index, int vertex_local_index ) const
    {
        assert( triangles_.size() != 0 );
        assert( triangle_index < triangles_.size() / 3 );
        assert( vertex_local_index < 3 );
        return triangles_[3 * triangle_index + vertex_local_index];
    }

    int Mesh::get_vertex_attribute( int vertex_index ) const
    {
        assert( vertex_index < vertices_.size() );
        return vertex_attributes_[vertex_index];
    }
    int Mesh::get_edge_attribute( int edge_index ) const
    {
        assert( edge_index < edges_.size() / 2 );
        return edge_attributes_[edge_index];

    }
    int Mesh::get_triangle_attribute( int triangle_index ) const
    {
        assert( triangle_index < triangles_.size() / 3 );
        return triangle_attributes_[triangle_index];
    }

    void Mesh::set_attribute( double (*region)(vertex), 
            int attribute_index, bool border ) {
        if (border) {
            for (int e = 0; e < nb_edges(); ++e) {
                vertex v1 = get_edge_vertex(e, 0);
                vertex v2 = get_edge_vertex(e, 1);
                if (region(v1) > 0 && region(v2) > 0) {
                    edge_attributes_[e] = attribute_index;
                }
            }
            return;
        }
        for (int t = 0; t < nb_triangles(); ++t) {
            vertex v1 = get_triangle_vertex(t, 0);
            vertex v2 = get_triangle_vertex(t, 1);
            vertex v3 = get_triangle_vertex(t, 2);
            if (region(v1) > 0 && region(v2) > 0 && region(v3) > 0) {
                triangle_attributes_[t] = attribute_index;
            }
        }
    }

    int Mesh::get_bdr_attr_max() const
    {
        return bdr_attr_max_;
    }
    int Mesh::get_attr_max() const
    {
        return attr_max_;
    }

    enum input_flag {
        HEADER, DIMENSION, VERTICES, TRIANGLES, EDGES, NO_FLAG
    };

    int check_flag( const std::string& keyword )
    {
        if( keyword == "Dimension" ) {
            return DIMENSION;
        } else if( keyword == "Vertices" ) {
            return VERTICES;
        } else if( keyword == "Triangles" ) {
            return TRIANGLES;
        } else if( keyword == "Edges" ) {
            return EDGES;
        } else {
            return NO_FLAG;
        }
    }

    bool Mesh::load( const std::string& file_name )
    {
        std::string line;
        std::ifstream ifs( file_name.c_str(), std::ifstream::in );
        if( ifs.is_open() ) {
            int flag = NO_FLAG;
            double trash;
            int dim;
            while( std::getline( ifs, line ) ) {
                std::string keyword;
                ifs >> keyword;
                flag = check_flag( keyword );
                if( flag == DIMENSION ) {
                    std::getline( ifs, line );
                    ifs >> dim;
                    assert( dim == 3 || dim == 2 );
                    flag = NO_FLAG;
                }
                if( flag == VERTICES ) {
                    std::getline( ifs, line );
                    int nb_vertices;
                    ifs >> nb_vertices;
                    vertices_.resize( nb_vertices );
                    vertex_attributes_.resize( nb_vertices );
                    for( int v = 0; v < nb_vertices; v++ ) {
                        std::getline( ifs, line );
                        if( dim == 3) {
                        ifs >> vertices_[v].x >> vertices_[v].y >> trash
                            >> vertex_attributes_[v];
                        }
                        if(dim == 2 ) {
                            ifs >> vertices_[v].x >> vertices_[v].y
                                >> vertex_attributes_[v];
                        }
                    }
                    flag = NO_FLAG;
                }
                if( flag == TRIANGLES ) {
                    std::getline( ifs, line );
                    int nb_triangles;
                    ifs >> nb_triangles;
                    triangles_.resize( 3 * nb_triangles );
                    triangle_attributes_.resize( nb_triangles );
                    for( int tr = 0; tr < nb_triangles; tr++ ) {
                        std::getline( ifs, line );
                        ifs >> triangles_[3 * tr] >> triangles_[3 * tr + 1]
                            >> triangles_[3 * tr + 2] >> triangle_attributes_[tr];
                        if( triangle_attributes_[tr] > attr_max_ ) {
                            attr_max_ = triangle_attributes_[tr];
                        }
                    }
                    for( int tr = 0; tr < triangles_.size(); tr++ ) {
                        triangles_[tr] -= 1;
                    }

                    flag = NO_FLAG;
                }
                if( flag == EDGES ) {
                    std::getline( ifs, line );
                    int nb_edges;
                    ifs >> nb_edges;
                    edges_.resize( nb_edges * 2 );
                    edge_attributes_.resize( nb_edges );
                    for( int ed = 0; ed < nb_edges; ed++ ) {
                        std::getline( ifs, line );
                        ifs >> edges_[2 * ed] >> edges_[2 * ed + 1]
                            >> edge_attributes_[ed];
                        if( edge_attributes_[ed] > bdr_attr_max_ ) {
                            bdr_attr_max_ = edge_attributes_[ed];
                        }
                    }
                    for( int ed = 0; ed < edges_.size(); ed++ ) {
                        edges_[ed] -= 1;
                    }
                    flag = NO_FLAG;
                }
            }
            ifs.close();
        } else {
            std::cout << "Error while opening the file, check the name" << std::endl;
            return false;
        }

        return true;
    }

    bool Mesh::save( const std::string& file_name ) const
    {
        std::ofstream ofs( file_name.c_str() );

        ofs << "MeshVersionFormatted 2" << std::endl;
        ofs << "Dimension" << std::endl;
        ofs << "2" << std::endl;

        ofs << "Vertices" << std::endl;
        ofs << nb_vertices() << std::endl;
        for( int v = 0; v < nb_vertices(); v++ ) {
            ofs << get_vertex( v ).x << " " << get_vertex( v ).y << " "
                << get_vertex_attribute( v ) << std::endl;
        }

        ofs << "Edges" << std::endl;
        ofs << nb_edges() << std::endl;
        for( int e = 0; e < nb_edges(); e++ ) {
            ofs << get_edge_vertex_index( e, 0 ) + 1 << " "
                << get_edge_vertex_index( e, 1 ) + 1 << " " <<  get_edge_attribute( e )
                << std::endl;
        }

        ofs << "Triangles" << std::endl;
        ofs << nb_triangles() << std::endl;
        for( int tr = 0; tr < nb_triangles(); tr++ ) {
            ofs << get_triangle_vertex_index( tr, 0 ) + 1 << " "
                << get_triangle_vertex_index( tr, 1 ) + 1 << " "
                << get_triangle_vertex_index( tr, 2 ) + 1 << " "
                << get_triangle_attribute( tr ) << std::endl;
        }
        ofs << "End" << std::endl;

        return true;
    }

    void save_solution( const std::vector< double >& x, const std::string& filename )
    {
        std::ofstream medit_bb( filename.c_str(), std::ios::out | std::ios::trunc );

        medit_bb << " 2 1 " << x.size() << " 2" << std::endl;
        for( int i = 0; i < x.size(); i++ ) {
            medit_bb << x[i] << std::endl;
        }
    }

}
