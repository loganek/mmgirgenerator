#ifndef _GIR_GEN_REPOSITORY_H_
#define _GIR_GEN_REPOSITORY_H_

#include "datastructures.h"
#include <vector>

namespace GirGen {

struct Repository
{
  std::vector<NamespaceInfo> namespaces;

  void clear() {
    namespaces.clear();
  }
};

}

#endif /* _GIR_GEN_REPOSITORY */
