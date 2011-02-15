<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">

<xsl:output method="text"/>

<xsl:template match="package">
<xsl:text>&#xa;</xsl:text>
<xsl:apply-templates select="tag"/>
</xsl:template>

<xsl:template match="tag">
  <xsl:apply-templates/>
  <xsl:text>&#xa;</xsl:text>
</xsl:template>

</xsl:stylesheet>