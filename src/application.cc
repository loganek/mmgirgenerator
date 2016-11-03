#include "application.h"
#include "dependencyresolver.h"
#include "defsprinter.h"
#include "repositoryloader.h"

#include <iostream>

namespace GirGen {

Application::Application()
    : context("[GIR_PACKAGE...]"),
      nspace_collection(std::make_shared<NamespaceCollection>())
{
    context.set_main_group(group);
}

int Application::run(int &argc, char **&argv)
{
    try
    {
      context.parse(argc, argv);
    }
    catch (const Glib::Error& ex)
    {
        std::cerr << argv[0] << ": " << ex.what() << std::endl;
        std::cerr << "Try '" << argv[0] << " --help' for more information." << std::endl;
        return 1;
    }
    catch (const std::runtime_error& ex)
    {
        std::cerr << ex.what() << std::endl;
        return 1;
    }

    load_package_list();

    load_namespaces();

    print_definitions();

    return 0;
}

void Application::load_package_list()
{
    DependencyResolver resolver(group.gir_path);

    for (const auto& package : group.packages)
    {
        resolver.resolve_dependencies(package);
    }

    packages = resolver.get_packages();
}

void Application::load_namespaces()
{
    RepositoryLoader loader;
    for (const auto& package : packages)
    {
        std::string gir_file = Glib::build_path(G_DIR_SEPARATOR_S, Glib::ArrayHandle<std::string>(std::vector<std::string>{group.gir_path, package})) + ".gir";
        loader.parse_gir_file(gir_file, nspace_collection);
    }
}

void Application::print_definitions()
{
    for (const auto& nspace : *nspace_collection)
    {
        if (nspace.first != group.req_namespace && group.namespace_mapping[nspace.first] != group.req_namespace)
        {
            continue;
        }

        DefsPrinter printer(nspace.first, nspace_collection, group.namespace_mapping);

        if (group.print_enum)
        {
            printer.print_enums();
        }
        if (group.print_vfunc)
        {
            printer.print_virtual_methods();
        }
        if (group.print_signal)
        {
            printer.print_signals();
        }
        if (group.print_property)
        {
            printer.print_properties();
        }
        if (group.print_function)
        {
            printer.print_free_functions();
        }
    }
}

}
