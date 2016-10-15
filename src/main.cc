#include "defsprinter.h"
#include "repositoryloader.h"
#include <glib-object.h>

#include <dlfcn.h>

#include <iostream>


int main(int, char**)
{
    /*if (argc < 2) {
        std::cout << "expected argument" << std::endl;
        return 1;
    }

    std::string input_file = argv[1];

    std::shared_ptr<GirGen::Loader> loader = std::make_shared<GirGen::XmlLoader>(input_file);

    GirGen::Repository repo = loader->load_repository();

    std::shared_ptr<GirGen::Printer> printer = std::make_shared<GirGen::DefsPrinter>(repo);

    printer->print_methods();
*/

    GirGen::RepositoryLoader loader;

    // GStreamer core
    loader.parse_gir_file("/usr/share/gir-1.0/Gst-1.0.gir");
    loader.parse_gir_file("/usr/share/gir-1.0/GstBase-1.0.gir");
    loader.parse_gir_file("/usr/share/gir-1.0/GstCheck-1.0.gir");
    loader.parse_gir_file("/usr/share/gir-1.0/GstNet-1.0.gir");
    loader.parse_gir_file("/usr/share/gir-1.0/GstController-1.0.gir");

    // gst-plugin-base
    loader.parse_gir_file("/usr/share/gir-1.0/GstRtsp-1.0.gir");
    loader.parse_gir_file("/usr/share/gir-1.0/GstAudio-1.0.gir");
    loader.parse_gir_file("/usr/share/gir-1.0/GstFft-1.0.gir");
    loader.parse_gir_file("/usr/share/gir-1.0/GstApp-1.0.gir");
    loader.parse_gir_file("/usr/share/gir-1.0/GstAllocators-1.0.gir");
    loader.parse_gir_file("/usr/share/gir-1.0/GstRtp-1.0.gir");
    loader.parse_gir_file("/usr/share/gir-1.0/GstTag-1.0.gir");
    loader.parse_gir_file("/usr/share/gir-1.0/GstSdp-1.0.gir");
    loader.parse_gir_file("/usr/share/gir-1.0/GstPbutils-1.0.gir");
    loader.parse_gir_file("/usr/share/gir-1.0/GstVideo-1.0.gir");


    for (auto ns : loader.namespaces) {
        GirGen::DefsPrinter printer(ns.second);
        //printer.print_enums();
        printer.print_virtual_methods();
        //std::cout << ns.first << std::endl; continue;
        for (auto enumeration : ns.second->enumerations) {
            //std::cout << enumeration->name << std::endl;
            //std::cout << enumeration->documentation << std::endl;
        }
    }

  return 0;
}
