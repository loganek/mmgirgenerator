#include "dependencyresolver.h"

#include <libxml++/libxml++.h>
#include <glibmm.h>

namespace GirGen {

DependencyResolver::DependencyResolver(const std::string &gir_path)
    : gir_path(gir_path)
{
}

bool DependencyResolver::is_in_vector(const std::string &package_name) const
{
    return std::find(packages.begin(), packages.end(), package_name) != packages.end();
}

void DependencyResolver::resolve_dependencies(const std::string &package)
{
    if (is_in_vector(package))
    {
        return;
    }

    // TODO glibmm: ArrayHandler initializer list constructor
    std::string path = Glib::build_path(G_DIR_SEPARATOR_S, Glib::ArrayHandle<std::string>(std::vector<std::string>{gir_path, package})) + ".gir";

    for (auto pkg : get_include_list(path))
    {
        resolve_dependencies(pkg);
    }

    packages.push_back(package);
}


std::vector<std::string> DependencyResolver::get_include_list(const std::string &gir_file)
{
    xmlpp::DomParser parser;
    parser.parse_file(gir_file);

    xmlpp::Node* root = parser.get_document()->get_root_node();
    auto nodes = root->find("/*[name()='repository']/*[name()='include']");

    std::vector<std::string> pkgs;

    for (const auto& node : nodes)
    {
        auto element = dynamic_cast<const xmlpp::Element*>(node);
        pkgs.push_back(element->get_attribute_value("name") + "-" + element->get_attribute_value("version"));
    }

    return pkgs;
}

}
