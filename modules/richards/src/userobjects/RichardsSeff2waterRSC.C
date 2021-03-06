/*****************************************/
/* Written by andrew.wilkins@csiro.au    */
/* Please contact me if you make changes */
/*****************************************/

//  Rogers-Stallybrass-Clements version of effective saturation of water phase.
//  valid for residual saturations = 0, and viscosityOil = 2*viscosityWater.  (the "2" is important here!).
// C Rogers, MP Stallybrass and DL Clements "On two phase filtration under gravity and with boundary infiltration: application of a Backlund transformation" Nonlinear Analysis Theory Methods and Applications 7 (1983) 785--799.
//
#include "RichardsSeff2waterRSC.h"

template<>
InputParameters validParams<RichardsSeff2waterRSC>()
{
  InputParameters params = validParams<RichardsSeff>();
  params.addRequiredParam<Real>("oil_viscosity", "Viscosity of oil (gas) phase.  It is assumed this is double the water-phase viscosity");
  params.addRequiredParam<Real>("scale_ratio", "This is porosity/permeability/beta^2, where beta may be chosen by the user (RSC define beta<0, but MOOSE only uses beta^2, so its sign is irrelevant).  It has dimensions [time]");
  params.addRequiredParam<Real>("shift", "effective saturation is a function of (Pc - shift)");
  params.addClassDescription("Rogers-Stallybrass-Clements version of effective saturation for the water phase, valid for residual saturations = 0, and viscosityOil = 2*viscosityWater.  seff_water = 1/Sqrt(1 + Exp(Pc - shift)/scale)), where scale = 0.25*scale_ratio*oil_viscosity");
  return params;
}

RichardsSeff2waterRSC::RichardsSeff2waterRSC(const std::string & name, InputParameters parameters) :
  RichardsSeff(name, parameters),
  _oil_viscosity(getParam<Real>("oil_viscosity")),
  _scale_ratio(getParam<Real>("scale_ratio")),
  _shift(getParam<Real>("shift")),
  _scale(0.25*_scale_ratio*_oil_viscosity)
{}



Real
RichardsSeff2waterRSC::seff(std::vector<VariableValue *> p, unsigned int qp) const
{
  Real pc = (*p[1])[qp] - (*p[0])[qp];
  return RichardsSeffRSC::seff(pc, _shift, _scale);
}

std::vector<Real>
RichardsSeff2waterRSC::dseff(std::vector<VariableValue *> p, unsigned int qp) const
{
  Real pc = (*p[1])[qp] - (*p[0])[qp];
  std::vector<Real> answer(2);
  answer[1] = RichardsSeffRSC::dseff(pc, _shift, _scale);
  answer[0] = -answer[1];
  return answer;
}

std::vector<std::vector<Real> >
RichardsSeff2waterRSC::d2seff(std::vector<VariableValue *> p, unsigned int qp) const
{
  Real pc = (*p[1])[qp] - (*p[0])[qp];
  std::vector<std::vector<Real> > answer(2);
  answer[0].resize(2);
  answer[1].resize(2);
  answer[1][1] = RichardsSeffRSC::d2seff(pc, _shift, _scale);
  answer[0][1] = -answer[1][1];
  answer[1][0] = -answer[1][1];
  answer[0][0] = answer[1][1];
  return answer;
}

