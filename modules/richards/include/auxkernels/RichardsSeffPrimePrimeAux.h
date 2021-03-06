/*****************************************/
/* Written by andrew.wilkins@csiro.au    */
/* Please contact me if you make changes */
/*****************************************/

#ifndef RICHARDSSEFFPRIMEPRIMEAUX_H
#define RICHARDSSEFFPRIMEPRIMEAUX_H

#include "AuxKernel.h"

#include "RichardsSeff.h"

//Forward Declarations
class RichardsSeffPrimePrimeAux;

template<>
InputParameters validParams<RichardsSeffPrimePrimeAux>();

/**
 * Calculates derivative of effective saturation wrt specified porepressures
 */
class RichardsSeffPrimePrimeAux: public AuxKernel
{
public:
  RichardsSeffPrimePrimeAux(const std::string & name, InputParameters parameters);

protected:
  virtual Real computeValue();

  /**
   * The user object that defines effective saturation
   * as function of porepressure (or porepressures in the
   * multiphase situation)
   */
  const RichardsSeff & _seff_UO;

  /**
   * AuxKernel calculates d^2(Seff)/dP_wrt1 dP_wrt2
   * so wrt1 is the porepressure number to differentiate wrt to
   */
  int _wrt1;

  /**
   * AuxKernel calculates d^2(Seff)/dP_wrt1 dP_wrt2
   * so wrt2 is the porepressure number to differentiate wrt to
   */
  int _wrt2;

  /**
   * the porepressure values (this will be length N
   * where N is the number of arguments that the _seff_UO requires)
   * Eg, for twophase simulations N=2
   */
  std::vector<VariableValue *> _pressure_vals;
};

#endif // RICHARDSSEFFPRIMEPRIMEAUX_H
