args=$1

if [ "$args" == "leak" ]; then
    if [ ! -f "./build/lib/bin/libCAssignment.dylib" ]; then
        sudo sh build.sh rebuild lib
    fi
    sudo sh build.sh rebuild all
    sudo leaks --atExit --list -- ./build/exe/bin/Assignment
elif [ "$args" == "lib" ]; then
    sudo sh build.sh rebuild lib
elif [ "$args" == "exe" ]; then
    sudo sh build.sh rebuild exe
    sudo ./build/exe/bin/Assignment
elif [ "$args" == "gui" ]; then
    sudo sh build.sh rebuild gui
    # ./build/gui/bin/Assignment
else
    if [ ! -f "./build/lib/bin/libCAssignment.dylib" ]; then
        sudo sh build.sh rebuild lib
    fi
    sudo sh build.sh rebuild all
    sudo ./build/exe/bin/Assignment
fi