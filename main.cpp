#include <iostream>

#include "bitmap.hpp"
#include "graph.hpp"

void ShowResult(const Bitmap& original, Graph& graph, const std::string& filename){
    Bitmap resbmp = graph.graphToBitmap();
    std::string originalF = "./images/original/"+filename+"Original.bmp";
    original.Save(originalF);
    std::string resultF = "./images/segmented/"+filename+"Seg.bmp";
    resbmp.Save(resultF);

}

void test(std::vector<std::string>& files)
{
    for(auto filename: files)
    {   
        std::cout << std::endl << filename << ": " << std::endl;
        Bitmap bitmap = readpgm("./images/" + filename + ".pgm");
        Graph g(bitmap);
        g.prMinCut();
        // g.minCut();
        ShowResult(bitmap, g, filename);
    }
}

int main(){
    std::string filename;
        std::cout << "image name (e.g. test1, test2, test3, bird, peppers, sailboat)?" << std::endl;
        std::cin >> filename;
    Bitmap bitmap = readpgm("./images/" + filename + ".pgm");
    Graph g(bitmap);
    g.prMinCut();
    // g.minCut();
    ShowResult(bitmap, g, filename);
    // std::vector<std::string> files = {"bird", "debug", "peppers", "sailboat", "test1","test2", "test3"};
    // test(files);
    return 0;
}
