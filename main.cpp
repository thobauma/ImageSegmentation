#include <iostream>
#include <chrono>
#include <map>
#include "bitmap.hpp"
#include "graph.hpp"

using filesContainer = std::vector<std::string>;


void ShowResult(const Bitmap& original, Graph& graph, const std::string& filename, bool printOriginal){
    if(printOriginal){
        std::string originalF = "./cx_out/"+filename+"Original.bmp";
        original.Save(originalF);
    }
    Bitmap resbmp = graph.graphToBitmap();
    std::string resultF = "./cx_out/"+filename+"Seg.bmp";
    resbmp.Save(resultF);

}


void compareAlgs()
{   
    filesContainer files = {"bird", "debug", "peppers", "sailboat", "test1", "test2", "test3"};
    filesContainer options = {"bird", "debug", "peppers", "sailboat", "test1", "test2", "test3", "test", "real", "all"};
    std::string input;
    // optionsContainer optionsMap;
    // initOptionsMap(optionsMap, files);

    std::cout << std::endl
        << "Enter: test1, test2, test3, bird, peppers, sailboat" << std::endl
        << "Or for multiple:" << std::endl
        << "test for test1, test2, test3," << std::endl
        << "real for bird, peppers, sailboat" << std::endl
        << "all for all pictures" << std::endl;
    std::cin >> input;

    auto opt = std::find(options.begin(), options.end(), input);

    if(opt == options.end())
    {
        std::cerr << "invalid input" << std::endl;
        exit(1);
    }
    filesContainer testFiles;
    if(std::find(files.begin(), files.end(), input) != files.end())
        testFiles={input};
    else if(input == "all")
        testFiles = files;
    else if(input == "test")
        testFiles = {"test1","test2", "test3"};
    else if(input == "real")
        testFiles = {"bird", "peppers", "sailboat"};

    for(auto filename: testFiles)
    {   
        std::cout << std::endl <<"*******  " <<filename <<"  *******" << std::endl;
        const Bitmap bitmap = readpgm("./images/" + filename + ".pgm");
        std::string originalF = "./cx_out/"+filename+"Original.bmp";
        bitmap.Save(originalF);
        std::cout <<std::endl<< "----- Edmunds Karp -----" << std::endl;
        auto startEdmundsKarp = std::chrono::high_resolution_clock::now();
        Graph ekG(bitmap);
        ekG.minCut();
        auto endEdmundsKarp = std::chrono::high_resolution_clock::now();
        ShowResult(bitmap, ekG, filename+"EK", 0);
        auto runTimeEdmundsKarp = std::chrono::duration_cast<std::chrono::duration<double>>(endEdmundsKarp - startEdmundsKarp).count();
        std::cout << "runtime: " << runTimeEdmundsKarp << " seconds"<< std::endl;

        std::cout <<std::endl<< "----- Push-Relabel -----" << std::endl;
        auto startPushRelabel = std::chrono::high_resolution_clock::now();
        Graph prG(bitmap);
        prG.prMinCut();
        auto endPushRelabel = std::chrono::high_resolution_clock::now();
        ShowResult(bitmap, ekG, filename+"PR", 0);
        auto runTimePushRelabel = std::chrono::duration_cast<std::chrono::duration<double>>(endPushRelabel - startPushRelabel).count();
        std::cout << "runtime: " << runTimePushRelabel << " seconds"<< std::endl;
        std::cout << std::endl;
    }
}

void testEdmundsKarp(std::string filename)
{
    Bitmap bitmap = readpgm("./images/" + filename + ".pgm");

    std::cout << std::endl <<"*******  " <<filename <<"  *******" << std::endl;
    std::cout << "----- Edmunds Karp -----" << std::endl;

    auto startTime = std::chrono::high_resolution_clock::now();
    
    Graph g(bitmap);
    g.minCut();

    auto endTime = std::chrono::high_resolution_clock::now();
    auto runTime = std::chrono::duration_cast<std::chrono::duration<double>>(endTime - startTime).count();
    std::cout << "runtime: " << runTime << " seconds"<< std::endl;

    ShowResult(bitmap, g, filename+"EK", 1);
}

void testPushRelabel(std::string filename)
{
    Bitmap bitmap = readpgm("./images/" + filename + ".pgm");

    std::cout << std::endl <<"*******  " <<filename <<"  *******" << std::endl;
    std::cout << "----- Push-Relabel -----" << std::endl;
    
    auto startTime = std::chrono::high_resolution_clock::now();
    
    Graph g(bitmap);
    g.prMinCut();

    auto endTime = std::chrono::high_resolution_clock::now();
    auto runTime = std::chrono::duration_cast<std::chrono::duration<double>>(endTime - startTime).count();
    std::cout << "runtime: " << runTime << " seconds"<< std::endl;

    ShowResult(bitmap, g, filename+"PR", 1);
}

int main(){

    // compareAlgs();

    std::string filename;
    std::cout << "image name (e.g. test1, test2, test3, bird, peppers, sailboat)?" << std::endl;
    std::cin >> filename;
    testEdmundsKarp(filename);
    testPushRelabel(filename);

    return 0;
}
