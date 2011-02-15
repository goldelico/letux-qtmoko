#!/bin/sh
cp index.html.header index.html

rm -f category-*.html
echo "#!/bin/sh" > write-category-footers.sh
echo "#!/bin/sh" > write-categories.sh
echo "#!/bin/sh" > write-packages.sh

for f in *.xml
do
	out=${f%.xml}.html
	echo generating $out

    # html for package
	xsltproc package.xsl $f > $out

    # add package to all app list
    echo "xsltproc package-item.xsl $f >> index.html" >> write-packages.sh

    # add package in categories
    category=`xsltproc package-category.xsl $f`

    if [ ! -n "$category" ]
    then
        category="unknown"
    fi

    echo category $category
    category_html="category-${category}.html"
    echo category $category_html

    if [ -f "$category_html" ]
    then
        xsltproc package-item.xsl $f >> $category_html
    else
        xsltproc package-category-header.xsl $f >> $category_html
        xsltproc package-item.xsl $f >> $category_html
        echo "echo \"</body></html>\" >> $category_html" >> write-category-footers.sh
        echo "xsltproc category-item.xsl $f >> index.html" >> write-categories.sh
    fi

done

chmod +x write-category-footers.sh
./write-category-footers.sh
rm -f ./write-category-footers.sh

chmod +x write-categories.sh
./write-categories.sh
rm -f ./write-categories.sh

echo "<hr/>" >> index.html

chmod +x write-packages.sh
./write-packages.sh
rm -f ./write-packages.sh

cat index.html.footer >> index.html
