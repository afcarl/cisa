#include "gsminterface.h"
#include "Eigen/Core"
#include "exception.h"

using namespace Eigen;

PyObject* GSM_new(PyTypeObject* type, PyObject* args, PyObject* kwds) {
	PyObject* self = type->tp_alloc(type, 0);

	if(self)
		reinterpret_cast<GSMObject*>(self)->gsm = 0;

	return self;
}



int GSM_init(GSMObject* self, PyObject* args, PyObject* kwds) {
	char* kwlist[] = {"dim", "num_scales", 0};
	int dim;
	int num_scales = 10;

	// read arguments
	if(!PyArg_ParseTupleAndKeywords(args, kwds, "i|i", kwlist,
		&dim, &num_scales))
		return -1;

	// create actual GSM instance
	self->gsm = new GSM(dim, num_scales);

	return 0;
}



void GSM_dealloc(GSMObject* self) {
	// delete actual GSM instance
	delete self->gsm;

	// delete GSM object
	self->ob_type->tp_free(reinterpret_cast<PyObject*>(self));
}



PyObject* GSM_dim(GSMObject* self, PyObject*, void*) {
	return PyInt_FromLong(self->gsm->dim());
}



PyObject* GSM_num_scales(GSMObject* self, PyObject*, void*) {
	return PyInt_FromLong(self->gsm->numScales());
}



PyObject* GSM_scales(GSMObject* self, PyObject*, void*) {
	return PyArray_FromMatrixXd(self->gsm->scales());
}



int GSM_set_scales(GSMObject* self, PyObject* value, void*) {
	if(!PyArray_Check(value)) {
		PyErr_SetString(PyExc_TypeError, "Scales should be of type `ndarray`.");
		return -1;
	}

	try {
		self->gsm->setScales(PyArray_ToMatrixXd(value));

	} catch(Exception exception) {
		PyErr_SetString(PyExc_RuntimeError, exception.message());
		return -1;
	}

	return 0;
}



PyObject* GSM_variance(GSMObject* self, PyObject*, PyObject*) {
	try {
		return PyFloat_FromDouble(self->gsm->variance());
	} catch(Exception exception) {
		PyErr_SetString(PyExc_RuntimeError, exception.message());
		return 0;
	}
}



PyObject* GSM_normalize(GSMObject* self, PyObject*, PyObject*) {
	try {
		self->gsm->normalize();
	} catch(Exception exception) {
		PyErr_SetString(PyExc_RuntimeError, exception.message());
		return 0;
	}

	Py_INCREF(Py_None);
	return Py_None;
}



PyObject* GSM_train(GSMObject* self, PyObject* args, PyObject* kwds) {
	char* kwlist[] = {"data", "max_iter", "tol", 0};

	PyObject* data;
	int max_iter = 100;
	double tol = 1e-5;

	// read arguments
	if(!PyArg_ParseTupleAndKeywords(args, kwds, "O|id", kwlist, &data, &max_iter, &tol))
		return 0;

	// make sure data is stored in NumPy array
	if(!PyArray_Check(data)) {
		PyErr_SetString(PyExc_TypeError, "Data has to be stored in a NumPy array.");
		return 0;
	}

	try {
		if(self->gsm->train(PyArray_ToMatrixXd(data), max_iter, tol)) {
			Py_INCREF(Py_True);
			return Py_True;
		} else {
			Py_INCREF(Py_False);
			return Py_False;
		}
	} catch(Exception exception) {
		PyErr_SetString(PyExc_RuntimeError, exception.message());
		return 0;
	}

	return 0;
}



PyObject* GSM_posterior(GSMObject* self, PyObject* args, PyObject* kwds) {
	char* kwlist[] = {"data", 0};

	PyObject* data;

	// read arguments
	if(!PyArg_ParseTupleAndKeywords(args, kwds, "O", kwlist, &data))
		return 0;

	// make sure data is stored in NumPy array
	if(!PyArray_Check(data)) {
		PyErr_SetString(PyExc_TypeError, "Data has to be stored in a NumPy array.");
		return 0;
	}

	try {
		return PyArray_FromMatrixXd(self->gsm->posterior(PyArray_ToMatrixXd(data)));
	} catch(Exception exception) {
		PyErr_SetString(PyExc_RuntimeError, exception.message());
		return 0;
	}
}



PyObject* GSM_sample(GSMObject* self, PyObject* args, PyObject* kwds) {
	char* kwlist[] = {"num_samples", 0};

	int num_samples = 1;

	if(!PyArg_ParseTupleAndKeywords(args, kwds, "|i", kwlist, &num_samples))
		return 0;

	try {
		return PyArray_FromMatrixXd(self->gsm->sample(num_samples));
	} catch(Exception exception) {
		PyErr_SetString(PyExc_RuntimeError, exception.message());
		return 0;
	}
}



PyObject* GSM_sample_posterior(GSMObject* self, PyObject* args, PyObject* kwds) {
	char* kwlist[] = {"data", 0};

	PyObject* data;

	// read arguments
	if(!PyArg_ParseTupleAndKeywords(args, kwds, "O", kwlist, &data))
		return 0;

	// make sure data is stored in NumPy array
	if(!PyArray_Check(data)) {
		PyErr_SetString(PyExc_TypeError, "Data has to be stored in a NumPy array.");
		return 0;
	}

	try {
		return PyArray_FromMatrixXd(self->gsm->samplePosterior(PyArray_ToMatrixXd(data)));
	} catch(Exception exception) {
		PyErr_SetString(PyExc_RuntimeError, exception.message());
		return 0;
	}
}



PyObject* GSM_loglikelihood(GSMObject* self, PyObject* args, PyObject* kwds) {
	char* kwlist[] = {"data", 0};

	PyObject* data;

	// read arguments
	if(!PyArg_ParseTupleAndKeywords(args, kwds, "O", kwlist, &data))
		return 0;

	// make sure data is stored in NumPy array
	if(!PyArray_Check(data)) {
		PyErr_SetString(PyExc_TypeError, "Data has to be stored in a NumPy array.");
		return 0;
	}

	try {
		return PyArray_FromMatrixXd(self->gsm->logLikelihood(PyArray_ToMatrixXd(data)));
	} catch(Exception exception) {
		PyErr_SetString(PyExc_RuntimeError, exception.message());
		return 0;
	}
}



PyObject* GSM_energy(GSMObject* self, PyObject* args, PyObject* kwds) {
	char* kwlist[] = {"data", 0};

	PyObject* data;

	// read arguments
	if(!PyArg_ParseTupleAndKeywords(args, kwds, "O", kwlist, &data))
		return 0;

	// make sure data is stored in NumPy array
	if(!PyArray_Check(data)) {
		PyErr_SetString(PyExc_TypeError, "Data has to be stored in a NumPy array.");
		return 0;
	}

	try {
		return PyArray_FromMatrixXd(self->gsm->energy(PyArray_ToMatrixXd(data)));
	} catch(Exception exception) {
		PyErr_SetString(PyExc_RuntimeError, exception.message());
		return 0;
	}
}



PyObject* GSM_energy_gradient(GSMObject* self, PyObject* args, PyObject* kwds) {
	char* kwlist[] = {"data", 0};

	PyObject* data;

	// read arguments
	if(!PyArg_ParseTupleAndKeywords(args, kwds, "O", kwlist, &data))
		return 0;

	// make sure data is stored in NumPy array
	if(!PyArray_Check(data)) {
		PyErr_SetString(PyExc_TypeError, "Data has to be stored in a NumPy array.");
		return 0;
	}

	try {
		return PyArray_FromMatrixXd(self->gsm->energyGradient(PyArray_ToMatrixXd(data)));
	} catch(Exception exception) {
		PyErr_SetString(PyExc_RuntimeError, exception.message());
		return 0;
	}
}