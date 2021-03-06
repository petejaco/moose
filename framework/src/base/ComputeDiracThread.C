/****************************************************************/
/*               DO NOT MODIFY THIS HEADER                      */
/* MOOSE - Multiphysics Object Oriented Simulation Environment  */
/*                                                              */
/*           (c) 2010 Battelle Energy Alliance, LLC             */
/*                   ALL RIGHTS RESERVED                        */
/*                                                              */
/*          Prepared by Battelle Energy Alliance, LLC           */
/*            Under Contract No. DE-AC07-05ID14517              */
/*            With the U. S. Department of Energy               */
/*                                                              */
/*            See COPYRIGHT for full restrictions               */
/****************************************************************/

#include "ComputeDiracThread.h"

//Moose Includes
#include "ParallelUniqueId.h"
#include "DiracKernel.h"
#include "Problem.h"
#include "NonlinearSystem.h"
#include "MooseVariable.h"

// libmesh includes
#include "libmesh/threads.h"

ComputeDiracThread::ComputeDiracThread(FEProblem & feproblem,
                                       NonlinearSystem & system,
                                       SparseMatrix<Number> * jacobian) :
    ThreadedElementLoop<DistElemRange>(feproblem, system),
    _jacobian(jacobian),
    _sys(system)
{}

// Splitting Constructor
ComputeDiracThread::ComputeDiracThread(ComputeDiracThread & x, Threads::split split) :
    ThreadedElementLoop<DistElemRange>(x, split),
    _jacobian(x._jacobian),
    _sys(x._sys)
{}

ComputeDiracThread::~ComputeDiracThread()
{
}

void
ComputeDiracThread::pre()
{
  // Force TID=0 because we run this object _NON THREADED_
  // Take this out if we ever get Dirac's working with threads!
  _tid = 0;
}

void
ComputeDiracThread::onElement(const Elem * elem)
{
  bool has_dirac_kernels_on_elem = _fe_problem.reinitDirac(elem, _tid);

  if (has_dirac_kernels_on_elem)
  {
    // Only call reinitMaterials() if one or more DiracKernels has
    // actually called getMaterialProperty().  Loop over all the
    // DiracKernels and check whether this is the case.
    bool need_reinit_materials = false;
    for (std::vector<DiracKernel *>::const_iterator dirac_kernel_it = _sys._dirac_kernels[_tid].all().begin();
         dirac_kernel_it != _sys._dirac_kernels[_tid].all().end();
         ++dirac_kernel_it)
    {
      // If any of the DiracKernels have had getMaterialProperty()
      // called, we need to reinit Materials.
      if ((*dirac_kernel_it)->getMaterialPropertyCalled())
      {
        need_reinit_materials = true;
        break;
      }
    }

    if (need_reinit_materials)
      _fe_problem.reinitMaterials(_subdomain, _tid, /*swap_stateful=*/false);

    for (std::vector<DiracKernel *>::const_iterator dirac_kernel_it = _sys._dirac_kernels[_tid].all().begin();
        dirac_kernel_it != _sys._dirac_kernels[_tid].all().end();
        ++dirac_kernel_it)
    {
      DiracKernel * dirac = *dirac_kernel_it;

      if (dirac->hasPointsOnElem(elem))
      {
        if (_jacobian == NULL)
          dirac->computeResidual();
        else
        {
          dirac->subProblem().prepareShapes(dirac->variable().number(), _tid);
          dirac->computeJacobian();
        }
      }
    }

    // Note that we do not call swapBackMaterials() here as they were
    // never swapped in the first place.  This avoids messing up
    // stored values of stateful material properties.
  }
}

void
ComputeDiracThread::postElement(const Elem * /*elem*/)
{
  Threads::spin_mutex::scoped_lock lock(Threads::spin_mtx);
  if (_jacobian == NULL)
    _fe_problem.addResidual(_tid);
  else
    _fe_problem.addJacobian(*_jacobian, _tid);
}

void
ComputeDiracThread::join(const ComputeDiracThread & /*y*/)
{
}
