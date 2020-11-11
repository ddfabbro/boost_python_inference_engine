# Expose DLL that depends on Inference Engine to Python using Boost.Python

> Original question posted on [Intel Community](https://community.intel.com/t5/Intel-Distribution-of-OpenVINO/Expose-DLL-that-depends-on-Inference-Engine-to-Python-using/m-p/1227403#M21412)

I have created a DLL for Windows that depends on Inference Engine and now I want to develop a python package that uses this DLL.

To create a python package that can access the functions of this DLL I am using [Boost.Python](https://www.boost.org/doc/libs/1_63_0/libs/python/doc/html/tutorial/index.html).

I have used Boost.Python before to develop python packages that depends on OpenCV and never had any issues.

However, when I create a DLL that depends on the Inference Engine I get runtime error. Specifically for MKLDNNPlugin.dll

The error message is 

```

RuntimeError: Failed to create plugin C:\local\Release\MKLDNNPlugin.dll for device CPU
Please, check your environment
Cannot load library 'C:\local\Release\MKLDNNPlugin.dll': 126 from cwd: C:\local\Release

```

I have seen many issues reported with "Cannot load library MKLDNNPlugin.dll"

Among the solutions are:


- run setupvars.bat

- copy MKLDNNPlugin.dll to the same location as my output DLL


Unfortunately, none of these have worked.


I have tried wrapping in a DLL one of the simplest Inference Engine samples available, namely [hello_classification](https://github.com/openvinotoolkit/openvino/blob/master/inference-engine/samples/hello_classification/main.cpp) and uploaded [a minimal, complete and verified example](https://github.com/ddfabbro/boost_python_inference_engine/tree/master) to Github

---

### My environment:

- Visual Studio 2019
- OpenVINO  2020.3 (Installed from precompiled binaries)
- OpenCV 4.3 (Comes with OpenVINO 2020.3)
- Boost  1.71
- Python 3.7



