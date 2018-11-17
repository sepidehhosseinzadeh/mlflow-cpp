Project (mlflow)
cmake_minimum_required (VERSION 3.4)
set(CMAKE_C_COMPILER "/usr/bin/g++")
add_compile_options(-std=c++14)
#add_compile_options(-O2)
add_compile_options(-g)
add_compile_options(-DNOMINMAX -DNDEBUG)

option(WITH_PQXX "WITH_PQXX" ON)

set(MLFLOW_CPP ../../../mlflow-0.7/mlflow/cpp/client/src/main/cpp)
set(ECOPIA_ZEBRA ../../../)

# src .cxx
aux_source_directory (${MLFLOW_CPP}/org/mlflow/tracking SRC)
aux_source_directory (${MLFLOW_CPP}/org/mlflow/tracking/creds SRC)
aux_source_directory (${MLFLOW_CPP}/org/mlflow/api/proto SRC)
aux_source_directory (${MLFLOW_CPP}/org/mlflow/api/proto/scalapb SRC)
# zebra
aux_source_directory (${ECOPIA_ZEBRA}/src/config SRC)
aux_source_directory (${ECOPIA_ZEBRA}/src/ginger SRC)
aux_source_directory (${ECOPIA_ZEBRA}/src/core SRC)
aux_source_directory (${ECOPIA_ZEBRA}/src/json SRC)
aux_source_directory (${ECOPIA_ZEBRA}/src/basics SRC)
aux_source_directory (${ECOPIA_ZEBRA}/src/utils SRC)
aux_source_directory (${ECOPIA_ZEBRA}/src/awslib SRC)
aux_source_directory (${ECOPIA_ZEBRA}/src/network SRC)
aux_source_directory (${ECOPIA_ZEBRA}/src/combinatorics SRC)
aux_source_directory (${ECOPIA_ZEBRA}/proto/cpp SRC)

# gcc -I .h
include_directories (${MLFLOW_CPP}/org/mlflow/api/proto)
include_directories (${MLFLOW_CPP}/org/mlflow/api/proto/scalapb)
include_directories (/usr/local/include)
include_directories (/usr/include/gdal)
include_directories (/usr/include/cppconn)
include_directories (/usr/local/include/cppconn)
include_directories (/usr/local/include/geos)
include_directories (/usr/local/lib/libzip/include)
include_directories (/usr/include/openssl)
include_directories (${MLFLOW_CPP})

# zebra
include_directories (${ECOPIA_ZEBRA}/include)
include_directories (${ECOPIA_ZEBRA}/proto/cpp)

# gcc -L
link_directories (/usr/local/lib)
link_directories (/usr/lib)
link_directories (/usr/local/lib/linux/intel64)
link_directories (/usr/lib/x86_64-linux-gnu)
link_directories (/usr/local/cuda/lib64)

# gcc -l
link_libraries (glog)
link_libraries (gflags)
link_libraries (raft)
link_libraries (curl)
link_libraries (gdal)
link_libraries (geos)
link_libraries (mysqlcppconn)
link_libraries (pthread)
link_libraries (protobuf)
link_libraries (boost_filesystem)
link_libraries (boost_system)
link_libraries (boost_serialization)
link_libraries (boost_date_time)
link_libraries (boost_thread)
link_libraries (aws-cpp-sdk-core)
link_libraries (aws-cpp-sdk-s3)
link_libraries (crypto)
link_libraries (zip)
link_libraries (z)

if (WITH_PQXX)
  include_directories (/usr/include/pqxx)
  link_libraries (pq)
  link_libraries (pqxx)
  add_definitions("-D__PQXX_ECOPIA__")
endif(WITH_PQXX)

add_definitions("-D__LINUX__")
add_definitions("-DL1D_CACHE_LINE_SIZE=64")

add_library(mlflow SHARED ${SRC})

#INSTALL(FILES add.h DESTINATION include)
INSTALL(TARGETS mlflow LIBRARY DESTINATION lib)

file (GLOB APP_SOURCES ./test/*.cpp)
foreach (app_source ${APP_SOURCES})
  get_filename_component(bin_name ${app_source} NAME_WE)
  message("adding executable: " ${bin_name})
  link_libraries (mlflow)
  add_executable (${bin_name} ${app_source})
endforeach (app_source ${APP_SOURCES})


