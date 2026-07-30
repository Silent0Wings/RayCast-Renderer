#include "helper.cpp"

void scene_file()
{
    space s;
    s.loadFromFile("../scene_export.txt");
    s.addObject(object(primitive::suzane, 10));
    s.launchThreadedCameraSplit();
}

void test()
{
    space s;
    object obj(primitive::suzane, 10);
    obj.enableGrid(20);
    s.addObject(obj);

    MeshReader reader;
    s.loadReader("../scene_export.txt", reader);
    s.loadCameraFromFile(reader);

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