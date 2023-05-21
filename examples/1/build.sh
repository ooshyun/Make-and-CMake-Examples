args=$1
type=$2

# args
build="build"
clean="clean"
rebuild="rebuild"

# type
execute="exe"
library="lib"
gui="gui"

if [ "$type" == "$library" ]; then
    pathBuild="./build/lib/build"
    arguments="-DARGU_BUILD_TYPE:STRING=$library"
elif [ "$type" == "$execute" ]; then
    pathBuild="./build/exe/build"
    arguments="-DARGU_BUILD_TYPE:STRING=$execute"
elif [ "$type" == "$gui" ]; then
    pathBuild="./build/gui/build"
    arguments="-DARGU_BUILD_TYPE:STRING=$gui"
else
    pathBuild="./build/exe/build"
    arguments="-DARGU_BUILD_TYPE:STRING=all"
fi

build() {
    if [ ! -d "${pathBuild}" ]; then
        mkdir -p $pathBuild
    fi
    cd $pathBuild
    sudo cmake ./../../../CMakeLists.txt $arguments
    sudo make
    cd ../../../
    find ./build -type d -exec chmod 775 {} \;
    find ./build -type f -exec chmod 775 {} \;
    echo "Built project"
}

clean() {
    if [ -d "${pathBuild}" ]; then
        tmp="$pathBuild/**"
        sudo rm -rf $tmp
    fi
    echo "Cleaned build directory"
}

if [ "$args" == "$clean" ]; then
    clean
elif [ "$args" == "$rebuild" ]; then
    echo "Rebuilding project"
    clean
    build
else
    if [ ! -d "${pathBuild}" ]; then
        mkdir -p "${pathBuild}"
    fi
    build
fi

