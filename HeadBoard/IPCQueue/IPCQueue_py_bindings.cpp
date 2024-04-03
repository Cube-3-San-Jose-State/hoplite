#include <pybind11/pybind11.h>
#include "IPCQueue.h"

namespace py = pybind11;

// define python bindings
PYBIND11_MODULE(IPCQueue, m) {
	py::class_<IPCQueue>(m, "IPCQueue")
		.def(py::init<const std::string>())
		.def("write", &IPCQueue::write)
		.def("read", &IPCQueue::read)
		.attr("MAX_MESSAGE_LENGTH") = IPCQueue::MAX_MESSAGE_LENGTH;
}
