iFAB Foundry XML Schemas
------------------------
Copyright 2012-2013 The Pennsylvania State University

------------
Introduction
------------
This folder contains the XML schemas for data files used internally and
externally by iFAB Foundry.  The available schemas include:

  common.xsd               - Common elements and types used across all XML
                             schemas
  materials.xsd            - Standard list of manufacturable materials
  manufacturingDetails.xsd - Manufacturing/purchasing information for
                             individual parts
  assemblyDetails.xsd      - Specifies how individual parts are joined/
                             attached together
  specialDetails.xsd       - Cost model inputs for special components,
                             such as wire harnesses, control boxes,
                             hydraulics, armor plates, etc.
  wireHarness.xsd          - Cost model inputs for wire harnesses
  controlBox.xsd           - Cost model inputs for control/electrical boxes
  hydraulicAssembly.xsd    - Cost model inputs for hydraulic assemblies
  armor.xsd                - Cost model inputs for armor plates
  BOM.xsd                  - Contains the entire bill-of-materials (BOM)
                             for the design, including any manufacturing,
                             assembly, or special details
  ddmhInput.xsd            - Input format for the MLI DDMH library
  ddmhOutput.xsd           - Output format from the MLI DDMH library
  hudat.xsd                - Output from HuDAT describing ballistic welds
  procurement.xsd          - Defines procurement information for purchased
                             components

Also contained in this folder are example XML files for these schemas.

------------------
Validating Schemas
------------------
Go to the validate folder and run 'python validateExamples.py' (requires
Python 2.7).  All of the examples should conform to their schema.