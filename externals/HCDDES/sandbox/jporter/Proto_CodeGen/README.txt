Platform-Specific Code Generator

This requires the ctemplate library -- a zipfile should be in 3rdParty.

Rewritten from TTP_CodeGen.
Hopefully the file descriptions give a little outline of the idea --

EDPCG.h - macros to iterate over UDM containers (from TTP_CodeGen).
EsMoL.[h,cpp] - UDM Generated source (since these were generated the names have been changed to ESMOL.*)
EsMoL_xsd.h - More UDM Generated source 

UdmEngine.[h,cpp] - UDM engine

All of these are unfinished:
PFCGMain.cpp - main (stolen from TTP_CodeGen)
PF_CodeGen.[h,cpp] - model parser - (rewritten from TTP_CodeGen - in progress)

GenBuilder.[h,cpp] - model-specific interface - called by model parser
  GeneratorBase -- common interface to all of the model-specific stuff (abstract class)
  GenBuilder -- outer layer directly callable by the model parser 
     uses a macro to pass calls through to the PF-specific objects
  PFGenerator -- base for the PF-specific implementations
TTPGenerator.[h,cpp] - implementation for TTP tools template output
ConfigKeeper.[h,cpp] - thin wrapper on the Google template calls -- called by GenBuilder and friends

Templates:
TTPTemplates -- templates for TTP tools
   ttp_build.tpl - template for TTP_BUILD files
   ttp_plan.tpl - template for TTP_PLAN files
   ttp_main.tpl - draft template for main (code generation)
   ttp_main_*.tpl - the latest idea was to split main into little pieces that could be handled separately,
      then call them sequentially during generation
      
TrueTimeTemplates -- templates for TrueTime simulations
   There are no templates yet, only a copy of Peter Humke's example to start from.
   
Other stuff -- 

SchedGen -- generator for scheduler (ignore) -- it was going to be part of PF_CodeGen, but it's too different
GMEComp -- GME component code for later -- allow the interpreter to be invoked from GME
