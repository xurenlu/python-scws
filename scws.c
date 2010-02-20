#include <Python.h>
#include "scws.h"
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#define	XMODE_SHOW_ATTR		0x01
#define	XMODE_DICT_MEM		0x02
#define	XMODE_DO_STAT		0x04
#define	XMODE_STAT_FILE		0x08
#define	XMODE_NO_TIME		0x10

typedef struct {
    PyObject_HEAD
    scws_t * st;
} PyScws;

scws_t s;
static PyObject * scws_scws_new(PyObject * self,PyObject * args){
    if (!(s = scws_new())) {
        return NULL;
    }
    return Py_BuildValue("i",1);
}
static PyObject * scws_set_xdb(PyObject * self,PyObject * args){
    const char * xdb;
    if (!PyArg_ParseTuple(args, "s",&xdb)){
        return NULL;
    }
    scws_set_dict(s, xdb, SCWS_XDICT_XDB);
    return Py_BuildValue("i",1);
}
static PyObject * scws_scws_set_charset(PyObject * self,PyObject * args){
    const char * charset;

    if (!PyArg_ParseTuple(args, "s",&charset)){
        return NULL;
    }
    scws_set_charset(s, charset);
    return Py_BuildValue("i",1);
}

static PyObject * scws_scws_set_rule(PyObject * self,PyObject * args){
    const char * rule;

    if (!PyArg_ParseTuple(args, "s",&rule)){
        return NULL;
    }
    scws_set_rule(s, rule);
    return Py_BuildValue("i",1);
}

/**
 * 得到解析好的字符串
 * */
static PyObject * scws_get_res(PyObject * self,PyObject * args){
    const char *text;
    int sts;


    if (!PyArg_ParseTuple(args, "s",&text))
        return NULL;

    scws_res_t res, cur;
    scws_send_text(s, text, strlen(text));
    PyObject * v;
    int i=0;
    int total=0;
    long idf;
    scws_res_t head;
    v=PyList_New(0);//strlen(text));
    double d;
    while (res = cur = scws_get_result(s))
    {
        while(cur!=NULL){
            printf("total:cur->len:%d\n",cur->len);
            printf("Word: %.*s/%s (IDF = %4.2f)\n", cur->len, text+cur->off, cur->attr, cur->idf);
            PyList_Append(v,Py_BuildValue("(O,O,d)",
                PyString_FromStringAndSize(text+cur->off,cur->len),
                PyString_FromString(cur->attr),
                cur->idf));
            cur = cur->next;
        }
    }
    printf("try to free:\n");
    scws_free_result(res);
    printf("freed:\n");

    //sts = system(command);
    return Py_BuildValue("O",v);
}
static PyObject * 
scws_scws_free(PyObject * self,PyObject * args){
    scws_free(s);
    return Py_BuildValue("i", 1);
}
static PyMethodDef ScwsMethods[] = {
    {"scws_set_rule",  scws_scws_set_rule, METH_VARARGS, ""},
    {"scws_set_xdb",  scws_set_xdb, METH_VARARGS, ""},
    {"scws_set_charset",  scws_scws_set_charset, METH_VARARGS, ""},
    {"scws_new",  scws_scws_new, METH_VARARGS, ""},
    {"scws_free",  scws_scws_free, METH_VARARGS, ""},
    {"get_res",  scws_get_res, METH_VARARGS, ""},
    {NULL, NULL, 0, NULL}        /* Sentinel */
};
    PyMODINIT_FUNC
init_scws(void)
{
    PyObject * m;
    m=Py_InitModule("_scws", ScwsMethods);
    //ADD_INT(m,XMODE_SHOW_ATTR);
//    PyModule_AddObject(m,"scws_t",(PyObject * ) & PystType);

}

