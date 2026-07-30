#include "helper.cpp"

void scene_file()
{
    space s;
    s.loadFromFile("../scene_export.txt");
    s.launchThreadedCameraSplit();
}

int main(int argc, char const *argv[])
{
    scene_file();
    return 0;
}

// shortcut to collapse all : citrl + k + 0
// shortcut to expand all : citrl + k + j
// clean formating ctrl + k then -> ctrl + f
// clean formating ctrl + k then -> ctrl + k

// To do :
/*
-   stereo scopy
-   camera rotaion
-   optimisation
-   splitting spaces
-   quad and polygones to triangles
-   obj support
-   thread optimisation
-   display and interact with a 3d space threw input control (walk around)
-   coliision handeling

*/