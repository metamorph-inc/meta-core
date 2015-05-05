pyxbgen -u ./iFAB/manufacturingDetails.xsd -m _manufacturingDetails
sleep 1
rm -rf iFAB/lib
mkdir iFAB/lib
mv *.py iFAB/lib/
