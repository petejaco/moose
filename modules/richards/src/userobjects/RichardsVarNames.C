/*****************************************/
/* Written by andrew.wilkins@csiro.au    */
/* Please contact me if you make changes */
/*****************************************/

//  Holds maps between Richards variables (porepressure, saturations) and the variable number used by MOOSE.
//
#include "RichardsVarNames.h"

template<>
InputParameters validParams<RichardsVarNames>()
{
  InputParameters params = validParams<GeneralUserObject>();
  params.addClassDescription("Holds information on the porepressure variable names");
  params.addRequiredCoupledVar("richards_vars", "List of variables that represent the porepressures or (porepressure, saturations).  In single-phase models you will just have one (eg \'pressure\'), in two-phase models you will have two (eg \'p_water p_gas\', or \'p_water s_water\', etc.  These names must also be used in your kernels and material.");
  return params;
}

RichardsVarNames::RichardsVarNames(const std::string & name, InputParameters parameters) :
  GeneralUserObject(name, parameters),
  Coupleable(parameters, false),
  ZeroInterface(parameters),
  _num_v(coupledComponents("richards_vars")),
  _the_names(std::string())

{
  unsigned int max_moose_var_num_seen(0);

  _moose_var_num.resize(_num_v);
  _moose_var_value.resize(_num_v);
  _moose_var_value_old.resize(_num_v);
  _moose_grad_var.resize(_num_v);
  _moose_raw_var.resize(_num_v);
  for (unsigned int i=0 ; i<_num_v; ++i)
    {
      _moose_var_num[i] = coupled("richards_vars", i);
      max_moose_var_num_seen = (max_moose_var_num_seen > _moose_var_num[i] ? max_moose_var_num_seen : _moose_var_num[i]);
      _moose_var_value[i] = &coupledValue("richards_vars", i); // coupledValue returns a reference (an alias) to a VariableValue, and the & turns it into a pointer
      _moose_var_value_old[i] = (_is_transient ? &coupledValueOld("richards_vars", i) : &_zero);
      _moose_grad_var[i] = &coupledGradient("richards_vars", i);
      _moose_raw_var[i] = getVar("richards_vars", i);
      _the_names += getVar("richards_vars", i)->name() + " ";
    }
  _the_names.erase(_the_names.end() - 1, _the_names.end()); // remove trailing space

  _ps_var_num.resize(max_moose_var_num_seen + 1);
  for (unsigned int i=0 ; i<max_moose_var_num_seen+1 ; ++i)
    _ps_var_num[i] = _num_v; // NOTE: indicates that i is not a richards variable
  for (unsigned int i=0 ; i<_num_v; ++i)
    _ps_var_num[_moose_var_num[i]] = i;
}

void
RichardsVarNames::initialize()
{}

void
RichardsVarNames::execute()
{}

void RichardsVarNames::finalize()
{}



unsigned int
RichardsVarNames::num_v() const
{
  return _num_v;
}

unsigned int
RichardsVarNames::moose_var_num(unsigned int richards_var_num) const
{
  if (richards_var_num >= _moose_var_num.size())
    mooseError("The richards variable number " << richards_var_num << " is out of bounds according to the RichardsVarNames UserObject");
  return _moose_var_num[richards_var_num];
}

unsigned int
RichardsVarNames::richards_var_num(unsigned int moose_var_num) const
{
  if (moose_var_num >= _ps_var_num.size() || _ps_var_num[moose_var_num] == _num_v)
    mooseError("The moose variable with number " << moose_var_num << " is not a richards according to the RichardsVarNames UserObject");
  return _ps_var_num[moose_var_num];
}

bool
RichardsVarNames::not_richards_var(unsigned int moose_var_num) const
{
  if (moose_var_num >= _ps_var_num.size() || _ps_var_num[moose_var_num] == _num_v)
    return true;
  return false;
}

std::string
RichardsVarNames::richards_names() const
{
  return _the_names;
}

VariableValue *
RichardsVarNames::richards_vals(unsigned int richards_var_num) const
{
  return _moose_var_value[richards_var_num]; // moose_var_value is a vector of pointers to VariableValuees
}

VariableValue *
RichardsVarNames::richards_vals_old(unsigned int richards_var_num) const
{
  return _moose_var_value_old[richards_var_num];
}

VariableGradient *
RichardsVarNames::grad_var(unsigned int richards_var_num) const
{
  return _moose_grad_var[richards_var_num];
}

MooseVariable *
RichardsVarNames::raw_var(unsigned int richards_var_num) const
{
  return _moose_raw_var[richards_var_num];
}


