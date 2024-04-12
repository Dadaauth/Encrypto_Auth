#include <Python.h>

#include "../main.h"

static PyObject *method_encrypt(PyObject *self, PyObject *args)
{
    char *filepath, *password = NULL;
    int bytes_encrypted = -1;

    /* Parse arguments */
    if (!PyArg_ParseTuple(args, "ss", &filepath, &password)) {
        return NULL;
    }

    bytes_encrypted = encrypt(filepath, password);
    return PyLong_FromLong(bytes_encrypted);
}

static PyObject *method_decrypt(PyObject *self, PyObject *args)
{
    char *filepath, *password = NULL;
    int bytes_decrypted = -1;

    /* Parse arguments */
    if (!PyArg_ParseTuple(args, "ss", &filepath, &password)) {
        return NULL;
    }

    bytes_decrypted = decrypt(filepath, password);
    return PyLong_FromLong(bytes_decrypted);
}


static PyMethodDef EncryptoMethods[] = {
    {"encrypt", method_encrypt, METH_VARARGS, "Python interface for the encrypt function written in C."},
    {"decrypt", method_decrypt, METH_VARARGS, "Python interface for the decrypt function written in C."},
    {NULL, NULL, 0, NULL}
};

static struct PyModuleDef encrypto_module = {
    PyModuleDef_HEAD_INIT,
    "encrypto",
    "Python interface for the encrypto program written in C.",
    -1,
    EncryptoMethods
};


PyMODINIT_FUNC PyInit_encrypto_auth(void){
    return PyModule_Create(&encrypto_module);
}