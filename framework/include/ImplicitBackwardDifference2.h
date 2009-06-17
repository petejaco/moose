#ifndef IMPLICITBD2
#define IMPLICITBD2

#include "Kernel.h"


//Forward Declarations
class ImplicitBackwardDifference2;

template<>
Parameters valid_params<ImplicitBackwardDifference2>();

class ImplicitBackwardDifference2 : public Kernel
{
public:

  ImplicitBackwardDifference2(std::string name,
                              Parameters parameters,
                              std::string var_name,
                              std::vector<std::string> coupled_to=std::vector<std::string>(0),
                              std::vector<std::string> coupled_as=std::vector<std::string>(0));

protected:
  virtual Real computeQpResidual();

  virtual Real computeQpJacobian();
  
};
#endif //IMPLICITBD2
