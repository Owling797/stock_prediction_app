# Stock_prediction_app
A small project as part of the technopark program

## What is it
This is a prototype of an educational tool for novice investors.

Using it you can analyse the change in stock prices depending on the the last few days prices.

Price dynamics are visualized.

Besides the graph of price predictions, you will see additional graphs by the processed values. The help about metrics, on what the additional graphs are built, is also available.

## Requirements:
* Boost -> get it here - https://www.boost.org/
* QT    -> here is this one - https://www.qt.io/download
* CMake  -> download - https://cmake.org/

## Run
**1. Specify the path to boost in CMakeLists.txt**

Example:
```C++
find_package(Boost 1.76.0)

SET(BOOST_INCLUDE  "C:/boost/include/boost-1_76")
SET(BOOST_LIB "C:/boost/lib")
```

**2. Run server_short.py**
```C++
cd project/server
python -u server_short.py
```
wait ~20s

**3. Run CMakeLists.txt**
```C++
cd project
cmake CMakeLists.txt
```
