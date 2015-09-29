//=======================================================================
// Copyright 2001 Jeremy G. Siek, Andrew Lumsdaine, Lie-Quan Lee, 
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//=======================================================================
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/breadth_first_search.hpp>
#include <boost/pending/indirect_cmp.hpp>
#include <boost/range/iterator_range.hpp>

#include <iostream>

using namespace boost;
template < typename TimeMap, typename VertexMap >
class bfs_time_visitor:public default_bfs_visitor {
  typedef typename property_traits < TimeMap >::value_type T;
public:
  bfs_time_visitor(TimeMap tmap, VertexMap v, T & t):m_timemap(tmap), vertex_to_search(v), m_time(t)
    {
        vertex_map.insert(std::pair<long unsigned int, const char>(0, 'r'));
        vertex_map.insert(std::pair<long unsigned int, const char>(1, 's'));
        vertex_map.insert(std::pair<long unsigned int, const char>(2, 't'));
        vertex_map.insert(std::pair<long unsigned int, const char>(3, 'u'));
        vertex_map.insert(std::pair<long unsigned int, const char>(4, 'v'));
        vertex_map.insert(std::pair<long unsigned int, const char>(5, 'w'));
        vertex_map.insert(std::pair<long unsigned int, const char>(6, 'x'));
        vertex_map.insert(std::pair<long unsigned int, const char>(7, 'y'));
    }

    struct found_vertex {};

  template < typename Vertex, typename Graph >
    void discover_vertex(Vertex u, const Graph & g)
  {
    put(m_timemap, u, m_time++);
    if (u == vertex_to_search)
    {
        std::cout<<"FOUND("<<vertex_map[u]<<")\n";
        //throw found_vertex();
    }
    else
    {
        std::cout<<"STILL_TO_DISCOVER("<<vertex_map[u]<<")\n";
    }
  }

   template < typename Vertex, typename Graph >
    void finish_vertex(Vertex u, const Graph & g)
  {
    std::cout<<"NO FOUND("<<vertex_map[u]<<")\n";
  }

   template < typename Edge, typename Graph >
    void tree_edge(Edge e, const Graph & g)
  {
      VertexMap source_vertex = boost::source(e, g);
      VertexMap target_vertex = boost::target(e, g);

      std::cout<<"EDGE: "<<vertex_map[source_vertex]<<"->"<<vertex_map[target_vertex]<<"\n";

  }


  TimeMap m_timemap;
  VertexMap vertex_to_search;
  T & m_time;
  std::map<long unsigned int, const char> vertex_map;

};


int
main()
{
  using namespace boost;
  // Select the graph type we wish to use
  typedef adjacency_list < vecS, vecS, undirectedS > graph_t;
  // Set up the vertex IDs and names
  enum { r, s, t, u, v, w, x, y, N };
  const char *name = "rstuvwxy";
  // Specify the edges in the graph
  typedef std::pair < int, int >E;
  E edge_array[] = { E(r, s), E(r, v), E(s, w), E(w, r), E(w, t),
    E(w, x), E(x, t), E(t, u), E(x, y), E(u, y)
  };
  // Create the graph object
  const int n_edges = sizeof(edge_array) / sizeof(E);
#if defined(BOOST_MSVC) && BOOST_MSVC <= 1300
  // VC++ has trouble with the edge iterator constructor
  graph_t g(N);
  for (std::size_t j = 0; j < n_edges; ++j)
    add_edge(edge_array[j].first, edge_array[j].second, g);
#else
  typedef graph_traits<graph_t>::vertices_size_type v_size_t;
  graph_t g(edge_array, edge_array + n_edges, v_size_t(N));
#endif

  // Typedefs
  typedef graph_traits < graph_t >::vertex_descriptor Vertex;
  typedef graph_traits < graph_t >::vertices_size_type Size;
  typedef Size* Iiter;

  // a vector to hold the discover time property for each vertex
  std::vector < Size > dtime(num_vertices(g));

  Size time = 0;
  bfs_time_visitor < Size *, Vertex>vis(&dtime[0], vertex(x, g), time);
  std::vector<Vertex> p(num_vertices(g));

  try
  {
    breadth_first_search(g, vertex(s, g), visitor(vis));
  }catch(...)
  {
      std::cout<<"YIPUUU!!\n";
  }

  // Use std::sort to order the vertices by their discover time
  std::vector<graph_traits<graph_t>::vertices_size_type > discover_order(N);
  auto range = boost::irange(0, 8);
  std::copy(range.begin(), range.end(), discover_order.begin());
  std::sort(discover_order.begin(), discover_order.end(),
            indirect_cmp < Iiter, std::less < Size > >(&dtime[0]));

  std::cout << "order of discovery: ";
  for (int i = 0; i < N; ++i)
    std::cout << name[discover_order[i]] << " ";
  std::cout << std::endl;


  breadth_first_search(g, vertex(s, g),
    visitor(make_bfs_visitor(record_predecessors(&p[0], on_tree_edge()))));

  std::cout << "Path:" << std::endl;
  Vertex v_target = vertex(y, g);
  Vertex v_source = vertex(s, g);
  Vertex v_current = v_target;
  std::cout<<name[v_current];
  while (v_source != v_current)
  {
      v_current = p[v_current];

      std::cout<<"<-"<<name[v_current];
  }
  std::cout<<"\n";



  return EXIT_SUCCESS;
}
