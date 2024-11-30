## how to build on Mac

### python
* pip install grpcio-tools
* python -m grpc_tools.protoc -I./protos --python_out=. --grpc_python_out=. ./protos/helloworld.proto

### cpp
* protoc -I ./protos --grpc_out=. --plugin=protoc-gen-grpc=`which grpc_cpp_plugin` ./protos/helloworld.proto
* protoc -I ./protos --cpp_out=. ./protos/helloworld.proto
* g++ -I/usr/local/include -L/usr/local/lib `pkg-config --libs grpc++ grpc` -lgrpc++_reflection -lprotobuf -lpthread -ldl -std=c++11 *.cc
