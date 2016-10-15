#ifndef _GIR_GEN_LOADER_H_
#define _GIR_GEN_LOADER_H_

#include "repository.h"

namespace GirGen {

class Loader
{
 public:
  virtual ~Loader() {}
  
  virtual Repository load_repository() = 0;
};

}

#endif /* _GIR_GEN_LOADER_H_ */
