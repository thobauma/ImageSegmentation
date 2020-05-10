#include <iostream>
#include <iomanip> // for setprecision

#include "bitmap.hpp"
#include "graph.hpp"
// #define DEBUG

void ShowResult(const Bitmap& original, Graph& graph, const std::string& filename){
    Bitmap resbmp = graph.graphToBitmap();
    // std::stringstream s;
    // s << filename << ".bmp";
    std::string originalF = "./images/"+filename+"Original.bmp";
    original.Save(originalF);
    #ifdef DEBUG
        std::cout << "original saved" << std::endl;
    #endif
    std::string resultF = "./images/"+filename+"Seg.bmp";
//    std::string resultF = "hello.bmp";
    resbmp.Save(resultF);
    #ifdef DEBUG
        std::cout << "segmented image saved" << std::endl;
    #endif

}


int main(){
    std::cout << std::setprecision(3); // don't remove
    // std::cout << "image name (e.g. bird, peppers or sailboat)?";
    std::cout << "image name (e.g. test1, test2, test3)?";
    std::string filename;
    #ifdef DEBUG
        filename = "test3";
    #else
        std::cin >> filename;
    #endif
    Bitmap bitmap = readpgm("./images/" + filename + ".pgm");
    #ifdef DEBUG
        std::cout << std::endl << "bitmap initalized" << std::endl;
    #endif
    Graph g(bitmap);
    #ifdef DEBUG
        std::cout << "graph initialized" << std::endl;
    #endif
     g.minCut();
    #ifdef DEBUG
        std::cout << "minCut done" << std::endl;
    #endif
    ShowResult(bitmap, g, filename);

    return 0;
}
