#include "helper.cpp"

void simple()
{
    camera cam1(800);
    object obj(primitive::cube, 0.1, point(0, 0, 1));
    space s({obj});
    s.cameras.push_back(cam1);
    s.triggerCameraRay();
    s.saveImages();
}

int main(int argc, char const *argv[])
{
    simple();
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