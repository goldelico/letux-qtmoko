<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">

<xsl:output method="html"/>

<xsl:template match="package">
<xsl:text disable-output-escaping="yes">&lt;html&gt;</xsl:text>
<head>
<title><xsl:apply-templates select="@category"/></title>
</head>
<xsl:text disable-output-escaping="yes">&lt;body&gt;</xsl:text>
<img src="qtmoko.svg" width="320" border="0" alt="QtMoko" />
<h1><xsl:apply-templates select="@category"/> category</h1>
</xsl:template>

<xsl:template match="@category">
  <xsl:apply-templates/>
</xsl:template>

</xsl:stylesheet>