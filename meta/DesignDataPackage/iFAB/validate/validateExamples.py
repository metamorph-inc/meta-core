from validate import validate

result = validate("../assemblyExample.xml", "../assemblyDetails.xsd")
print(result.message)

result = validate("../castingExample.xml", "../manufacturingDetails.xsd")
print(result.message)

result = validate("../machinedExample.xml", "../manufacturingDetails.xsd")
print(result.message)

result = validate("../plateSheetExample.xml", "../manufacturingDetails.xsd")
print(result.message)

result = validate("../purchasedExample.xml", "../manufacturingDetails.xsd")
print(result.message)

result = validate("../wireHarnessExample.xml", "../specialDetails.xsd")
print(result.message)

result = validate("../ddmhInputExample.xml", "../ddmhInput.xsd")
print(result.message)

result = validate("../ddmhOutputExample.xml", "../ddmhOutput.xsd")
print(result.message)

result = validate("../BOMExample.xml", "../BOM.xsd")
print(result.message)

result = validate("../hudatExample.xml", "../hudat.xsd")
print(result.message)

result = validate("../armorExample.xml", "../armor.xsd")
print(result.message)