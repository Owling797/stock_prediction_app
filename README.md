# Stock_prediction_app
A small project as part of the technopark program

## Requirements:
* Boost
* QT

## Run
**1. Specify the path to boost in CMakeLists.txt**

Example:
```C++
find_package(Boost 1.76.0)

SET(BOOST_INCLUDE  "C:/boost/include/boost-1_76")
SET(BOOST_LIB "C:/boost/lib")
```

**2. Run server_short.py, wait ~20s then run CMakeLists.txt**
