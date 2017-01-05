#include "Python.h"

#include "ul_sign/ul_sign.h"

static PyObject *
_creat_sign_fs64(PyObject *self, PyObject *args)
{
	char *buf;
	int len;
	unsigned int sign1 = 0;
	unsigned int sign2 = 0;
    
	int ret;

    if (!PyArg_ParseTuple(args, "si", &buf, &len))
        return NULL;
		
	ret = creat_sign_fs64(buf, len, &sign1, &sign2);
    
    return Py_BuildValue("(ii)", sign1,sign2);
}


static PyMethodDef Methods[] = {
    {"creat_sign_fs64", _creat_sign_fs64, METH_VARARGS,""},
    {NULL, NULL, 0, NULL}
};

PyMODINIT_FUNC
initul_sign(void)
{
    (void) Py_InitModule("ul_sign", Methods);
}


