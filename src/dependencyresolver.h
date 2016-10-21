#ifndef _GIR_GEN_DEPENDENCY_RESOLVER_H_
#define _GIR_GEN_DEPENDENCY_RESOLVER_H_

#include <vector>
#include <string>

namespace GirGen {

class DependencyResolver
{
    std::string gir_path;
    std::vector<std::string> packages;

    static std::vector<std::string> get_include_list(const std::string &gir_file);
    bool is_in_vector(const std::string &package_name) const;

public:
    DependencyResolver(const std::string &gir_path);
    virtual ~DependencyResolver() {}

    void resolve_dependencies(const std::string &package);
    std::vector<std::string>  get_packages() const { return packages; }
};

}

#endif /* _GIR_GEN_DEPENDENCY_RESOLVER_H_ */
