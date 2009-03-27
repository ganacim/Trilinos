#include "PB_BlockPreconditionerFactory.hpp"

#include "Thyra_DefaultPreconditioner.hpp"

using namespace Thyra;

namespace PB {

LinearOp BlockPreconditionerFactory::buildPreconditionerOperator(LinearOp & lo) const
{
   // get the blocked linear operator
   RCP<LinearOpBase<double> > loA = Teuchos::rcp_const_cast<Thyra::LinearOpBase<double> >(lo);
   BlockedLinearOp A = Teuchos::rcp_dynamic_cast<Thyra::PhysicallyBlockedLinearOpBase<double> >(loA);

   return buildPreconditionerOperator(A);
}

//! is this operator compatiable with the preconditioner factory?
bool BlockPreconditionerFactory::isCompatible(const Thyra::LinearOpSourceBase<double> &fwdOpSrc) const
{
   RCP<const Thyra::PhysicallyBlockedLinearOpBase<double> > A 
         = Teuchos::rcp_dynamic_cast<const Thyra::PhysicallyBlockedLinearOpBase<double> >(fwdOpSrc.getOp());
   return A!=Teuchos::null;
}

//! create an instance of the preconditioner
RCP<Thyra::PreconditionerBase<double> > BlockPreconditionerFactory::createPrec() const
{
   return rcp(new Thyra::DefaultPreconditioner<double>());
}

//! initialize a newly created preconditioner object
void BlockPreconditionerFactory::initializePrec(const RCP<const LinearOpSourceBase<double> > & ASrc,
                    PreconditionerBase<double> * prec,
                    const ESupportSolveUse supportSolveUse) const
{
   // get the blocked linear operator
   RCP<LinearOpBase<double> > loA = Teuchos::rcp_const_cast<Thyra::LinearOpBase<double> >(ASrc->getOp());
   BlockedLinearOp A = Teuchos::rcp_dynamic_cast<Thyra::PhysicallyBlockedLinearOpBase<double> >(loA);
 
   // build the preconditioner
   const RCP<const LinearOpBase<double> > M = buildPreconditionerOperator(A);

   // must first cast that to be initialized
   DefaultPreconditioner<double> & dPrec = Teuchos::dyn_cast<DefaultPreconditioner<double> >(*prec);
   dPrec.initializeUnspecified(M);
}

//! wipe clean a already initialized preconditioner object
void BlockPreconditionerFactory::uninitializePrec(PreconditionerBase<double> * prec, 
                      RCP<const LinearOpSourceBase<double> > * fwdOpSrc,
                      ESupportSolveUse *supportSolveUse) const
{
   // what do I do here?
   TEST_FOR_EXCEPT_MSG(true,"\"BlockPreconditionerFactory::uninitializePrec not implemented\"");
}

// for ParameterListAcceptor
///////////////////////////////////////////////////////////////////////

//! Set parameters from a parameter list and return with default values.
void BlockPreconditionerFactory::setParameterList(const RCP<ParameterList> & paramList)
{
   paramList_ = paramList;
}

//! Get the parameter list that was set using setParameterList().
RCP< ParameterList > BlockPreconditionerFactory::getNonconstParameterList()
{
   return paramList_;
}

//! Unset the parameter list that was set using setParameterList(). 
RCP< ParameterList > BlockPreconditionerFactory::unsetParameterList()
{
   RCP<ParameterList> _paramList = paramList_;
   paramList_ = Teuchos::null;
   return _paramList;
}

} // end namespace PB
