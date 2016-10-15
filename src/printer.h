#ifndef _GIR_GEN_PRINTER_H_
#define _GIR_GEN_PRINTER_H_

#include "repository.h"

namespace GirGen {

class Printer
{
 protected:
  Repository repository;
  
 public:
  Printer(const Repository& repository) : repository(repository) {}
  virtual ~Printer() {}

  virtual void print_classes() = 0;
  virtual void print_methods() = 0;
  virtual void print_functions() = 0;
};

}

#endif /* _GIR_GEN_PRINTER_H_ */
