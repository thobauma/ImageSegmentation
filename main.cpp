#include <iostream>

#include "bitmap.hpp"
#include "graph.hpp"
#include "prGraph.hpp"
#define DEBUG

void ShowResult(const Bitmap& original, Graph& graph, const std::string& filename){
    Bitmap resbmp = graph.graphToBitmap();
    std::string originalF = "./images/original/"+filename+"Original.bmp";
    original.Save(originalF);
    #ifdef DEBUG
        std::cout << "original saved" << std::endl;
    #endif
    std::string resultF = "./images/segmented/"+filename+"Seg.bmp";
    resbmp.Save(resultF);
    #ifdef DEBUG
        std::cout << "segmented image saved" << std::endl;
    #endif

}

void test(std::vector<std::string>& files)
{
    for(auto filename: files)
    {   
        std::cout << std::endl << filename << ": " << std::endl;
        Bitmap bitmap = readpgm("./images/" + filename + ".pgm");
        Graph g(bitmap);
        g.minCut();
        ShowResult(bitmap, g, filename);
    }
}

int main(){
    // std::cout << "image name (e.g. test1, test2, test3)?" << std::endl;
    // std::string filename;
    // #ifdef DEBUG
    //     filename = "debug";
    // #else
    //     std::cin >> filename;
    // #endif
    // Bitmap bitmap = readpgm("./images/" + filename + ".pgm");
    // #ifdef DEBUG
    //     std::cout << std::endl << "bitmap initalized" << std::endl;
    // #endif
    // Graph g(bitmap);
    // #ifdef DEBUG
    //     std::cout << "graph initialized" << std::endl;
    //     // g.printTest();
    // #endif
    // g.minCut();
    // #ifdef DEBUG
    //     // g.printTest();
    //     std::cout << "minCut done" << std::endl;
    // #endif
    // ShowResult(bitmap, g, filename);

    std::vector<std::string> files = {"bird", "debug", "peppers", "sailboat", "test1","test2", "test3"};
    test(files);
    return 0;
}
