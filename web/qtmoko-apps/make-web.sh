#!/bin/sh
cp index.html.header index.html

for f in *.xml
do
	out=${f%.xml}.html
	echo generating $out 
	xsltproc package.xsl $f > $out
	xsltproc package-item.xsl $f >> index.html
done

cat index.html.footer >> index.html
