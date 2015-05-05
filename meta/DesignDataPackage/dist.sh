find ../../../../ddp/specs/component -type f -exec rm {} \;
cp -r release/* ../../../../ddp/specs/component
find ../../../../ddp/specs/component -iname ".DS_Store" -exec rm {} \;
