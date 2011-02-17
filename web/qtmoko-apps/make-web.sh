#!/bin/sh
cp index.html.header index.html

rm -f category-*.html
rm -f tag-*.html
echo "#!/bin/sh" > write-category-footers.sh
echo "#!/bin/sh" > write-categories.sh
echo "#!/bin/sh" > write-tag-footers.sh
echo "#!/bin/sh" > write-tags.sh
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

    if [ -f "$category_html" ]
    then
        xsltproc package-item.xsl $f >> $category_html
    else
        xsltproc package-category-header.xsl $f >> $category_html
        xsltproc package-item.xsl $f >> $category_html
        echo "echo \"</body></html>\" >> $category_html" >> write-category-footers.sh
        echo "xsltproc category-item.xsl $f >> index.html" >> write-categories.sh
    fi

    # read and process tags
    xsltproc package-tags.xsl $f | sed 1d | while read tag ;
    do
        echo "tag $tag"
        tag_html="tag-${tag}.html"

        if [ -f "$tag_html" ]
        then
            xsltproc package-item.xsl $f >> $tag_html
        else
            echo "<html><head><meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\">" > $tag_html
            echo "<title>$tag</title>" >> $tag_html
            echo "</head><body><img src=\"qtmoko.svg\" width=\"320\" border=\"0\" alt=\"QtMoko\"><center><h1>packages tagged with $tag</h1></center>" >> $tag_html
            xsltproc package-item.xsl $f >> $tag_html
            echo "echo \"</body></html>\" >> $tag_html" >> write-tag-footers.sh
            echo "echo \" <a href=\"$tag_html\">$tag</a>\"  >> index.html"  >> write-tags.sh
        fi

    done

done

chmod +x write-category-footers.sh
./write-category-footers.sh
rm -f ./write-category-footers.sh

chmod +x write-categories.sh
./write-categories.sh
rm -f ./write-categories.sh

echo "<hr/>" >> index.html
echo "<h2>tags</h2>" >> index.html

chmod +x write-tag-footers.sh
./write-tag-footers.sh
rm -f ./write-tag-footers.sh

chmod +x write-tags.sh
./write-tags.sh
rm -f ./write-tags.sh

echo "<hr/>" >> index.html
echo "<h2>all packages</h2>" >> index.html

chmod +x write-packages.sh
./write-packages.sh
rm -f ./write-packages.sh

cat index.html.footer >> index.html
