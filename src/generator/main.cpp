#include <cstring>
#include <iostream>
#include <fstream>
#include <vector>
#include "simpleshapes.hpp"
#include "patch.hpp"
#include "utils/tinyxml2.h"

using std::cout;
using std::endl;
using std::string;
using namespace SimpleShapes;
using namespace tinyxml2;

namespace FLAGS {
    static bool printXML = false;


    void readFlag(char *flag) {
        if (!strcmp(flag, "-xml"))
            FLAGS::printXML = true;
    }

    std::vector<char *> clearFlags(int argc, char **argv) {
        std::vector<char *> args;
        for (int i = 0; i < argc; i++) {
            if (argv[i]) {
                if (argv[i][0] == '-') {
                    readFlag(argv[i]);
                } else args.push_back(argv[i]);
            }
        }
        return args;
    }
};


void createXML(char *string) {
    XMLDocument xmlDoc;
    XMLNode *pRoot = xmlDoc.NewElement("scene");
    xmlDoc.InsertFirstChild(pRoot);
    XMLElement *gelem = xmlDoc.NewElement("group");
    XMLElement *bpelem = xmlDoc.NewElement("models");
    XMLElement *pElement = xmlDoc.NewElement("model");
    pElement->SetAttribute("file", string);
    bpelem->InsertEndChild(pElement);
    gelem->InsertEndChild(bpelem);
    pRoot->InsertEndChild(gelem);
    xmlDoc.InsertEndChild(pRoot);

    for (unsigned int i = strlen(string); i > 0; i--) {
        if (string[i] == '.') {
            string[i] = '\0';
            break;
        }
    }
    char *saving = strcat(string, ".xml");
    xmlDoc.SaveFile(saving);
}


//generator flags <Primitive> <PrimitiveOptions?> <Output_File>
//min valid value for argc is 4
int main(int argc, char **argv) {

    std::vector<char *> args = FLAGS::clearFlags(argc - 1, argv + 1);
    unsigned int argCount = args.size();
    char *fileName = args[argCount - 1];
    std::ofstream output;

    if (argCount > 0) {
        string prim = string(args[0]);
        if (prim == "plane") {
            if (argCount == 3) {
                output.open(fileName);
                mkPlane(atof(args[1])).writeStringTo(output);
            } else cout << "Error in Plane handler's parameters" << endl;
        } else if (prim == "box") {
            if (argCount == 5) {
                output.open(fileName);
                mkBox(atof(args[1]), atof(args[3]), atof(args[2]), 1).writeStringTo(output);
            } else if (argCount == 6) {
                output.open(fileName);
                mkBox(atof(args[1]), atof(args[3]), atof(args[2]), atoi(args[4])).writeStringTo(output);
            } else cout << "Error in Box handler's parameters" << endl;
        } else if (prim == "sphere") {
            if (argCount == 5) {
                output.open(fileName);
                mkSphere(atof(args[1]), atoi(args[2]), atoi(args[3])).writeStringTo(output);
            } else cout << "Error in Sphere handler's parameters" << endl;
        } else if (prim == "cone") {
            if (argCount == 6) {
                output.open(fileName);
                mkCone(atof(args[2]), atof(args[1]), atoi(args[3]), atoi(args[4])).writeStringTo(output);
            } else cout << "Error in Cone handler's parameters" << endl;
        } else
            //if (prim == "torus") {
            //if (argCount == 6) {
            //    output.open(fileName);
            //    mkTorus(atof(args[1]), atof(args[2]), atof(args[3]), atof(args[4])).writeStringTo(output);
            //} else cout << "Error in Torus handler's parameters" << endl;
            //}
            //else
        if (prim == "bezier") {
            BezierPrimitive res = BezierPrimitive();
            res.readBezierPatches(args[1]);
            output.open(fileName);
            res.generateBezierPatches(atoi(args[2])).writeStringTo(output);
        } else cout << "Error reading given comand arguments" << endl;
    } else cout << "Error on comand arguments" << endl;

    if (output.is_open())
        output.close();
    if (FLAGS::printXML)
        createXML(fileName);

}