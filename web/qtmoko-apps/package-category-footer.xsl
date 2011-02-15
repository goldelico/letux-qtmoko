<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">

<xsl:output method="html"/>

<xsl:template match="package">
<xsl:element name="html"/>
<head>
<title><xsl:apply-templates select="@category"/></title>
</head>
<xsl:element name="body"/>
<img src="qtmoko.svg" width="320" border="0" alt="QtMoko" />
<h1><xsl:apply-templates select="@category"/> category</h1>
</xsl:template>

<xsl:template match="@category">
  <xsl:apply-templates/>
</xsl:template>

</xsl:stylesheet>