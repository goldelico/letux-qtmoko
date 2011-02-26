<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">

<xsl:output method="html"/>

<xsl:template match="package">
<xsl:element name="a">
<xsl:attribute name="href">
<xsl:apply-templates select="@name"/>
<xsl:text>.html</xsl:text>
</xsl:attribute>
<xsl:attribute name="style">text-decoration: none</xsl:attribute>
<table cellpadding="16">
<tr>
<td>
<xsl:element name="img">
<xsl:attribute name="src">
<xsl:apply-templates select="@name"/>
<xsl:text>60.png</xsl:text>
</xsl:attribute>
<xsl:attribute name="border">0</xsl:attribute>
<xsl:attribute name="width">60</xsl:attribute>
<xsl:attribute name="height">80</xsl:attribute>
</xsl:element>
</td>
<td>
<h2><xsl:apply-templates select="@title"/></h2>
<p><xsl:apply-templates select="@summary"/></p>
</td>
</tr>
</table>
</xsl:element>
</xsl:template>

<xsl:template match="@title">
  <xsl:apply-templates/>
</xsl:template>

<xsl:template match="@summary">
  <xsl:apply-templates/>
</xsl:template>

<xsl:template match="@name">
  <xsl:apply-templates/>
</xsl:template>

<xsl:template match="@version">
  <xsl:apply-templates/>
</xsl:template>

</xsl:stylesheet>