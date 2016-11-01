#ifndef _GIR_GEN_APPLICATION_H_
#define _GIR_GEN_APPLICATION_H_

#include "mainoptiongroup.h"
#include "namespacecollection.h"

#include <glibmm.h>

namespace GirGen {

class Application
{
    Glib::OptionContext context;
    GirGen::MainOptionGroup group;

    std::vector<std::string> packages;
    std::shared_ptr<NamespaceCollection> nspace_collection;

    void load_package_list();
    void load_namespaces();
    void print_definitions();

public:
    Application();
    virtual ~Application() {}

    int run(int &argc, char **&argv);
};

}

#endif /* _GIR_GEN_APPLICATION_H_ */
