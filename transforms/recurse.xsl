<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" xmlns:ead="urn:isbn:1-931666-22-9"
    xmlns:mods="http://www.loc.gov/mods/v3" xmlns:ns2="http://www.w3.org/1999/xlink"
    exclude-result-prefixes="ead mods ns2 xsl" version="1.0">
    <xsl:output method="text" indent="no" encoding="utf-8"/>

<xsl:template match="/">
    <xsl:apply-templates select="//ead:did[ead:container[not(@parent)]]" />
</xsl:template>

    <xsl:template match="ead:did[ead:container[not(@parent)]]">
                <xsl:value-of select="normalize-space(/ead:ead/ead:eadheader/ead:filedesc/ead:titlestmt/ead:titleproper/ead:num)" />
                <xsl:text>&#09;</xsl:text>
                <xsl:value-of select="normalize-space(/ead:ead/ead:eadheader/ead:filedesc/ead:titlestmt/ead:titleproper)" />
                <xsl:text>&#09;</xsl:text>
                <xsl:call-template name="capitalize">
                  <xsl:with-param name="input">
                    <xsl:value-of select="ead:container[not(@parent)][1]/@type" />
                  </xsl:with-param>
                </xsl:call-template>
                <xsl:text>&#09;</xsl:text>
                <xsl:value-of select="normalize-space(ead:container[not(@parent)][1])" />
                <xsl:text>&#09;</xsl:text>
                <xsl:call-template name="capitalize">
                  <xsl:with-param name="input">
                    <xsl:value-of select="ead:container[@parent][1]/@type" />
                  </xsl:with-param>
                </xsl:call-template>
                <xsl:text>&#09;</xsl:text>
                <xsl:value-of select="normalize-space(ead:container[@parent][1])" />
                <xsl:text>&#09;</xsl:text>
                <xsl:value-of select="normalize-space(ead:unittitle)" />
                <xsl:text>&#09;</xsl:text>
                <xsl:value-of select="normalize-space(.//ead:unitdate)" />
                <xsl:text>&#10;</xsl:text>
    </xsl:template>

  <xsl:template name="capitalize">
    <xsl:param name="input" />
    <xsl:variable name="lowerchars" select="'abcdefghijklmnopqrstuvwxyz'" />
    <xsl:variable name="upperchars" select="'ABCDEFGHIJKLMNOPQRSTUVWXYZ'" />
    <xsl:value-of select="concat(translate(substring($input, 1, 1), $lowerchars, $upperchars), substring($input, 2))" />
  </xsl:template>

</xsl:stylesheet>
