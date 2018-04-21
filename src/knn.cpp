/*
Copyright 2018 Dennis Rohde

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include <boost/python.hpp>

#include "knn_tester.hpp"
#include "csv_reader.hpp"
#include "random.hpp"

using namespace boost::python;
namespace np = boost::python::numpy;

BOOST_PYTHON_MODULE(KNNTest)
{
    Py_Initialize();
    np::initialize();
    
    def("read_csv", read_csv<double>);
    
    class_<Tuple<double>>("Tuple", init<unsigned long>())
        .def(init<np::ndarray>())
        .def("__str__", &Tuple<double>::as_str)
        //.def("__iter__", iterator<std::vector<double>>())
        .add_property("numpy_array", &Tuple<double>::as_ndarray)
        .def("get", &Tuple<double>::get)
        .add_property("dimensions", &Tuple<double>::dimensions)
    ;
 
    class_<Relation<double>>("Relation", init<>())
        .def(init<np::ndarray>())
        .def("__str__", &Relation<double>::as_str)
        //.def("__iter__", iterator<std::vector<Tuple<double>>>())
        .add_property("numpy_array", &Relation<double>::as_ndarray)
        .def("get", &Relation<double>::get)
        .add_property("size", &Relation<double>::size)
        .add_property("dimensions", &Relation<double>::dimensions)
    ;
    
    class_<KNN_Graph<double>::Adjacency_List>("Adjacency_List", init<>())
        .def("get", &KNN_Graph<double>::Adjacency_List::get)
        .def("__iter__", iterator<std::vector<typename KNN_Graph<double>::index_type>>())
        .add_property("length", &KNN_Graph<double>::Adjacency_List::length)
    ;
    
    class_<KNN_Graph<double>>("KNN_Graph", init<optional<unsigned int>>())
        .def("__str__", &KNN_Graph<double>::as_str)
        .def("build", static_cast<void (KNN_Graph<double>::*)(const KNN_Graph<double>::vertices_type&)>(&KNN_Graph<double>::build))
        .def("build", static_cast<void (KNN_Graph<double>::*)(const np::ndarray&)>(&KNN_Graph<double>::build))
        .add_property("k", &KNN_Graph<double>::get_k)
        .add_property("number_vertices", &KNN_Graph<double>::number_vertices)
        .add_property("number_edges", &KNN_Graph<double>::number_edges)
        .add_property("number_wrongly_connected_vertices", &KNN_Graph<double>::number_wrongly_connected_vertices)
        .add_property("dimensions", &KNN_Graph<double>::dimensions)
        .def("get_vertex", &KNN_Graph<double>::get_vertex)
        .def("get_neighbors", &KNN_Graph<double>::get_neighbors)
        .def("set_edges", &KNN_Graph<double>::edges_from_ndarray)
        .def("sort", &KNN_Graph<double>::sort)
        .def("epsilon", &KNN_Graph<double>::epsilon)
        .add_property("vertices", range(&KNN_Graph<double>::vertices_begin, &KNN_Graph<double>::vertices_end))
    ;
    
    class_<KNN_Graph_Exact<double>, bases<KNN_Graph<double>>>("KNN_Graph_Exact", init<optional<unsigned int>>())
        .def("build", static_cast<void (KNN_Graph_Exact<double>::*)(const KNN_Graph_Exact<double>::vertices_type&)>(&KNN_Graph_Exact<double>::build))
        .def("build", static_cast<void (KNN_Graph_Exact<double>::*)(const np::ndarray&)>(&KNN_Graph_Exact<double>::build))
    ;

    class_<Uniform_Random_Tuple_Generator<double>>("Uniform_Random_Tuple_Generator", init<std::uint64_t, optional<unsigned int>>())
        .def("get", &Uniform_Random_Tuple_Generator<double>::get)
    ;
    
    class_<Query_Oracle<double>>("Query_Oracle", init<PyObject*>());
    
    class_<KNN_Tester<double>>("KNN_Tester")
        .def_readwrite("c1", &KNN_Tester<double>::c1)
        .def_readwrite("c2", &KNN_Tester<double>::c2)
        .def("test", &KNN_Tester<double>::test)
        .def("c1_approximate", &KNN_Tester<double>::c1_approximate)
    ;
    
    class_<KNN_Tester_Oracle<double>, bases<KNN_Tester<double>>>("KNN_Tester_Oracle", init<Query_Oracle<double>>())
        .def("test", &KNN_Tester_Oracle<double>::test)
    ;
    
    class_<KNN_Improver<double>, bases<KNN_Tester<double>>>("KNN_Improver")
        .def("improve", &KNN_Improver<double>::improve)
    ;
}
