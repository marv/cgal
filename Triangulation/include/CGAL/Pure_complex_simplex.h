// Copyright (c) 2009 INRIA Sophia-Antipolis (France),
//
// This file is part of CGAL (www.cgal.org); you may redistribute it under
// the terms of the Q Public License version 1.0.
// See the file LICENSE.QPL distributed with CGAL.
//
// Licensees holding a valid commercial license may use this file in
// accordance with the commercial license agreement provided with the software.
//
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
//
// $URL$
// $Id$
//
// Author(s)    : Samuel Hornus

#ifndef CGAL_TRIANGULATION_SIMPLEX_H
#define CGAL_TRIANGULATION_SIMPLEX_H

#include <CGAL/Pure_complex_ds_simplex.h>
#include <CGAL/Iterator_project.h>
#include <CGAL/Default.h>

namespace CGAL {

struct No_simplex_data {};

template< class PCTraits, typename Data_ = No_simplex_data, class TDSSimplex = Default >
class Pure_complex_simplex : public Default::Get<TDSSimplex, Pure_complex_ds_simplex<> >::type
{
    // The default type for TDSSimplex is Pure_complex_ds_simplex<> :
    typedef typename Default::Get<TDSSimplex, Pure_complex_ds_simplex<> >::type
                                                                Base;
    typedef Pure_complex_simplex<PCTraits, Data_, TDSSimplex>  Self;
public:
    typedef Data_                                       Data;
    typedef typename Base::Vertex_handle                Vertex_handle;
    typedef typename Base::Vertex_const_handle          Vertex_const_handle;
    typedef typename Base::Vertex_handle_const_iterator Vertex_handle_const_iterator;
    typedef typename Base::Simplex_const_handle         Simplex_const_handle;
    typedef typename PCTraits::Point_d                  Point;
    typedef typename PCTraits::Point_d                  Point_d;

private: // DATA MEMBERS
    Data    data_;

public:

    using Base::vertices_begin;
    using Base::vertices_end;

    template< class TDS2 >
    struct Rebind_TDS
    {
        typedef typename Base::template Rebind_TDS<TDS2>::Other TDSSimplex2;
        typedef Pure_complex_simplex<PCTraits, Data_, TDSSimplex2> Other;
    };

    Pure_complex_simplex(const int d)
        : Base(d), data_() {}

    template< typename T >
    Pure_complex_simplex(const int d, const T & t)
        : Base(d), data_(t) {}

    Pure_complex_simplex(const Self & s)
        : Base(s), data_(s.data_)  {}

    Point circumcenter() const
    {
        PCTraits pct;
        Vertex_handle_const_iterator vhit = vertices_begin();
        while( vhit != vertices_end() )
        {
            if( *vhit == Vertex_const_handle() )
            {
                CGAL_warning_msg(false, "too few points; can not compute circumcenter.");
                return Point();
            }
            ++vhit;
        }
        return pct.center_of_sphere_d_object()(points_begin(), points_end());
    }

    const Data & data() const
    {
        return data_;
    }

    Data & data()
    {
        return data_;
    }

    struct Point_from_vertex_handle
    {
        typedef Vertex_handle   argument_type;
        typedef Point           result_type;
        result_type & operator()(argument_type & x) const 
        {
            return x->point();
        }
        const result_type & operator()(const argument_type & x) const 
        {
            return x->point();
        }
    };

    typedef CGAL::Iterator_project<Vertex_handle_const_iterator, Point_from_vertex_handle,
        const Point &, const Point *> Point_const_iterator;

    Point_const_iterator points_begin() const 
        { return Point_const_iterator(Base::vertices_begin()); }
    Point_const_iterator points_end() const 
        { return Point_const_iterator(Base::vertices_end()); }

};

// FUNCTIONS THAT ARE NOT MEMBER FUNCTIONS:

std::istream &
operator>>(std::istream & is, No_simplex_data &)
{
    return is;
}

std::ostream &
operator<<(std::ostream & os, const No_simplex_data & nd)
{
    return os;
}

template < typename TDS, typename Data, typename SSP >
std::ostream &
operator<<(std::ostream & O, const Pure_complex_simplex<TDS, Data, SSP> & s)
{
    /*if( is_ascii(O) )
    {
        // os << '\n';
    }
    else {}*/
    O << s.data();
    return O;
}

template < typename TDS, typename Data, typename SSP >
std::istream &
operator>>(std::istream & I, Pure_complex_simplex<TDS, Data, SSP> & s)
{
    /*if( is_ascii(I) )
    {}
    else {}*/
    I >> s.data();
    return I;
}

} //namespace CGAL

#endif // CGAL_TRIANGULATION_SIMPLEX_H
