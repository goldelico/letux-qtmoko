<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">

<xsl:output method="html"/>

<xsl:template match="package">
<xsl:text>  </xsl:text>
<xsl:element name="a">
<xsl:attribute name="href">
<xsl:text>category-</xsl:text>
<xsl:apply-templates select="@category"/>
<xsl:text>.html</xsl:text>
</xsl:attribute>
<xsl:apply-templates select="@category"/>
</xsl:element>
<xsl:text>  </xsl:text>
</xsl:template>

<xsl:template match="@category">
  <xsl:apply-templates/>
</xsl:template>

</xsl:stylesheet>