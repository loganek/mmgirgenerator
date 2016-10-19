#include "dependencyresolver.h"

#include <libxml++/libxml++.h>
#include <glibmm.h>

namespace GirGen {

DependencyResolver::DependencyResolver(const std::string &gir_path, const std::string &initial_repository)
    : gir_path(gir_path),
      initial_repository(initial_repository)
{
}

bool DependencyResolver::is_in_vector(const std::string &repository_name) const
{
    return std::find(packages.begin(), packages.end(), repository_name) != packages.end();
}

std::vector<std::string> DependencyResolver::resolve_dependencies()
{
    packages.clear();

    resolve_package_dependency(initial_repository);

    return packages;
}

void DependencyResolver::resolve_package_dependency(const std::string &package)
{
    if (is_in_vector(package))
    {
        return;
    }

    // TODO glibmm: ArrayHandler initializer list constructor
    std::string path = Glib::build_path(G_DIR_SEPARATOR_S, Glib::ArrayHandle<std::string>(std::vector<std::string>{gir_path, package})) + ".gir";

    for (auto repo : get_include_list(path))
    {
        resolve_package_dependency(repo);
    }

    packages.push_back(package);
}

std::vector<std::string> DependencyResolver::get_include_list(const std::string &gir_file)
{
    xmlpp::DomParser parser;
    parser.parse_file(gir_file);

    xmlpp::Node* root = parser.get_document()->get_root_node();
    auto nodes = root->find("/*[name()='repository']/*[name()='include']");

    std::vector<std::string> repos;

    for (const auto& node : nodes)
    {
        auto element = dynamic_cast<const xmlpp::Element*>(node);
        repos.push_back(element->get_attribute_value("name") + "-" + element->get_attribute_value("version"));
    }

    return repos;
}

}
