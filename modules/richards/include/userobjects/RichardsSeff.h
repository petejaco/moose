/*****************************************/
/* Written by andrew.wilkins@csiro.au    */
/* Please contact me if you make changes */
/*****************************************/

#ifndef RICHARDSSEFF_H
#define RICHARDSSEFF_H

#include "GeneralUserObject.h"

class RichardsSeff;


template<>
InputParameters validParams<RichardsSeff>();

/**
 * Base class for effective saturation as a function of porepressure(s)
 * The functions seff, dseff and d2seff must be over-ridden in the derived class
 */
class RichardsSeff : public GeneralUserObject
{
 public:
  RichardsSeff(const std::string & name, InputParameters parameters);

  void initialize();
  void execute();
  void finalize();

  /**
   * effective saturation as a function of porepressure(s) at given quadpoint of the element
   * @param p the porepressure(s).  Eg (*p[0])[qp] is the zeroth pressure evaluated at quadpoint qp
   * @param the quad point of the element to evaluate effective saturation at.
   */
  virtual Real seff(std::vector<VariableValue *> p, unsigned int qp) const = 0;

  /**
   * derivative(s) of effective saturation as a function of porepressure(s) at given quadpoint of the element
   * If there are three porepressures, this will return at length-3 vector (dSeff/dP[0], dSeff/dP[1], dSeff/dP[2])
   * @param p the porepressure(s).  Eg (*p[0])[qp] is the zeroth pressure evaluated at quadpoint qp
   * @param the quad point of the element to evaluate the derivative at
   */
  virtual std::vector<Real> dseff(std::vector<VariableValue *> p, unsigned int qp) const = 0;

  /**
   * second derivative(s) of effective saturation as a function of porepressure(s) at given quadpoint of the element
   * d2seff[m][n] = d^2 Seff/dP[m]/dP[n]
   * @param p the porepressure(s).  Eg (*p[0])[qp] is the zeroth pressure evaluated at quadpoint qp
   * @param the quad point of the element to evaluate the derivative at
   */
  virtual std::vector<std::vector<Real> > d2seff(std::vector<VariableValue *> p, unsigned int qp) const = 0;

};

#endif // RICHARDSSEFF_H
