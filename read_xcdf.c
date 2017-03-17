/*  Example of wrapping the cos function from math.h using the Numpy-C-API. */

#include <Python.h>
#include <numpy/arrayobject.h>

#include <xcdf/utility/XCDFUtility.h>
#include <xcdf/utility/EventSelectExpression.h>
#include <xcdf/XCDFDefs.h>
#include <xcdf/config.h>

#include <vector>
#include <set>

#include <boost/shared_ptr.hpp>

using namespace std;

struct XCDFFieldReadBuffer{
public:
  virtual ~XCDFFieldReadBuffer(){}

  virtual void Read() = 0;

  virtual void* data() = 0;

  virtual const char* name() = 0;

  virtual unsigned size() = 0;
  
  virtual unsigned nentries() = 0;

  virtual int type() = 0;
};

template <class T>
int get_numpy_type();

template<>
int get_numpy_type<double>(){return NPY_DOUBLE;}

template<>
int get_numpy_type<int64_t>(){return NPY_INT64;}

template<>
int get_numpy_type<uint64_t>(){return NPY_UINT64;}

template<class T> 
struct TypedXCDFFieldReadBuffer : public XCDFFieldReadBuffer{

  TypedXCDFFieldReadBuffer(XCDFField<T>& fieldRead) : field(fieldRead){}

  virtual ~TypedXCDFFieldReadBuffer(){}

  virtual void Read(){
    buffer.push_back(field.At(0));
  }

  virtual void* data(){
    return buffer.size() > 0 ? &(buffer[0]) : NULL;
  }

  virtual const char* name(){
    return field.GetName().c_str();
  }

  virtual unsigned size(){
    return buffer.size() * sizeof(T);
  }

  virtual unsigned nentries(){
    return buffer.size();
  }

  virtual int type(){
    return get_numpy_type<T>();
  }

  XCDFField<T>& field; 
  vector<T> buffer;
};

struct visitor{
  template <class T>
  void operator()(XCDFField<T>& t){
    buffers.push_back(boost::shared_ptr<XCDFFieldReadBuffer>(new TypedXCDFFieldReadBuffer<T>(t)));
  }
  
  vector<boost::shared_ptr<XCDFFieldReadBuffer> > buffers;

  void Read(){
    for(vector<boost::shared_ptr<XCDFFieldReadBuffer> >::iterator iter = 
	  buffers.begin() ; 
	iter != buffers.end(); 
	iter++){
      (*iter)->Read();
    }
  }
};

// the main read_xcdf function exported to python
static PyObject* read_xcdf(PyObject* self, PyObject* args)
{
  char* infileName;
  if(!PyArg_ParseTuple(args,"s",& infileName))
    return NULL;

  XCDFFile f(infileName,"r");

  visitor v;
  f.ApplyFieldVisitor(v);

  while(f.Read()){
    v.Read();
  }

  PyObject *returned = PyDict_New();


  for(unsigned i = 0 ; i < v.buffers.size() ; i++){
    PyObject *out_array;

    void* data = malloc(v.buffers[i]->size());

    memcpy(data,v.buffers[i]->data(),v.buffers[i]->size());

    npy_intp m = v.buffers[i]->nentries();
    out_array = PyArray_SimpleNewFromData(1,&m,v.buffers[i]->type(),data);

    PyObject* key= PyString_FromString(v.buffers[i]->name());
    PyDict_SetItem(returned, key, out_array);

  }


  return returned;

}

/*  define functions in module */
static PyMethodDef CosMethods[] =
{
     {"read_xcdf", read_xcdf, METH_VARARGS,
         "evaluate the cosine on a numpy array"},
     {NULL, NULL, 0, NULL}
};

/* module initialization */
PyMODINIT_FUNC
initread_xcdf(void)
{
     (void) Py_InitModule("read_xcdf", CosMethods);
     /* IMPORTANT: this must be called */
     import_array();
}
