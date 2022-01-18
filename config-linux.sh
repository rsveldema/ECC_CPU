rm -rf build
mkdir build

export workspace=`pwd`

cmake -B $workspace/build \
        -DCMAKE_BUILD_TYPE=Ninja \
        -DCMAKE_CXX_COMPILER=g++-11 \
        -DCMAKE_CXX_FLAGS="-fcoroutines" \
        -DCMAKE_BUILD_TYPE=Debug \
        -DCMAKE_CXX_FLAGS_DEBUG="-g2 -O0"