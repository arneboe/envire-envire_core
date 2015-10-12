/*
 * TransformTreeTypes.hpp
 *
 *  Created on: Sep 16, 2015
 *      Author: aboeckmann
 *
 *  This file is needed to break a circular header dependency between
 *  TransformTree and FrameEventArgs.
 */

#ifndef SRC_TRANSFORMTREETYPES_HPP_
#define SRC_TRANSFORMTREETYPES_HPP_


#include <envire_core/items/Frame.hpp> /** Frames are for the Vertex **/
#include <envire_core/items/Transform.hpp> /** Transform are the Edges **/
#include <envire_core/items/Environment.hpp> /** Environment is the graph property **/

#include <boost/graph/directed_graph.hpp>

//HACK to gain access to private members in labeled_graph
//This is necessary because there is a bug in boost and a workaround needs to be
//implemented while we wait for boost to fix it
#define private protected
#include <boost/graph/labeled_graph.hpp>
#undef private
#include <string>
#include <unordered_map>
#include <unordered_set>

namespace envire { namespace core
{

    /**@brief Frame Property
     * boost access tag for the Frame property
     */
    struct FrameProperty
    {
        Frame frame;
        FrameProperty() : frame("envire::core::frame::default_name"){}
        FrameProperty(const FrameId& frameId): frame(frameId){}
    };

    /**@brief Transform Property
     * boost access tag for the Transform property
     */
    struct TransformProperty
    {
        Transform transform;
    };


    /**@brief The Transform Graph type
    *
    * The basic class for the Transform Graph
    *
    */
    typedef boost::directed_graph<FrameProperty, TransformProperty, envire::core::Environment> TransformGraphBase;
    typedef boost::labeled_graph<TransformGraphBase, FrameId> LabeledTransformGraph;
    typedef LabeledTransformGraph::vertex_descriptor vertex_descriptor;
    typedef LabeledTransformGraph::edge_descriptor edge_descriptor;
    typedef LabeledTransformGraph::edge_iterator edge_iterator;
    typedef LabeledTransformGraph::vertex_iterator vertex_iterator;
    typedef LabeledTransformGraph::vertices_size_type vertices_size_type;
    typedef LabeledTransformGraph::edges_size_type edges_size_type;
    typedef LabeledTransformGraph::degree_size_type degree_size_type;
    typedef LabeledTransformGraph::out_edge_iterator out_edge_iterator;
    typedef LabeledTransformGraph::in_edge_iterator in_edge_iterator;
    
    /**A map that shows the parent->children relation between vertices in a tree.
       The key is the parent.*/
    using VertexMap = std::unordered_map<vertex_descriptor, std::unordered_set<vertex_descriptor>>;
}}
#endif /* SRC_TRANSFORMTREETYPES_HPP_ */