# boost_python_inference_engine

## Instructions using Visual Studio 2019

1. Open `boost_python_openVINO.sln` in Visual Studio 2019 and build solution to generate `_py_hello_classification.dll`
2. In `boost_python_inference_engine\x64\Release`, rename `_py_hello_classification.dll` to `_py_hello_classification.pyd`
3. Download [`alexnet.bin`](https://drive.google.com/file/d/1aXO3nvnBzVOruBeHukfbLOlwQ2__BEfc/view?usp=sharing) and copy it to `boost_python_inference_engine\deploy`
4. Copy all files in `boost_python_inference_engine\deploy` to `boost_python_inference_engine\x64\Release`
5. In `boost_python_inference_engine\x64\Release`, run `hello_classification.py` using Python 3.7
