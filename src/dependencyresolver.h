#ifndef _GIR_GEN_DEPENDENCY_RESOLVER_H_
#define _GIR_GEN_DEPENDENCY_RESOLVER_H_

#include <vector>
#include <string>

namespace GirGen {

class DependencyResolver
{
    std::string gir_path;
    std::string initial_repository;
    std::vector<std::string> packages;

    static std::vector<std::string> get_include_list(const std::string &gir_file);
    void resolve_package_dependency(const std::string &package);
    bool is_in_vector(const std::string &repository_name) const;

public:
    DependencyResolver(const std::string &gir_path, const std::string &initial_repository);
    virtual ~DependencyResolver() {}

    std::vector<std::string> resolve_dependencies();
};

}

#endif /* _GIR_GEN_DEPENDENCY_RESOLVER_H_ */
