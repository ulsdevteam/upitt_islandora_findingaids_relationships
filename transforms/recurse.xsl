<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" xmlns:ead="urn:isbn:1-931666-22-9"
    xmlns:mods="http://www.loc.gov/mods/v3" xmlns:ns2="http://www.w3.org/1999/xlink"
    exclude-result-prefixes="ead mods ns2 xsl" version="1.0">
    <xsl:output method="text" indent="no" encoding="utf-8"/>
<xsl:variable name="decrement">1</xsl:variable>
<xsl:template match="/">
    <xsl:apply-templates select="//ead:container | //ead:c" />
</xsl:template>

    <xsl:template name="container" match="//ead:container | //ead:c">
        <xsl:param name="containers" select="//ead:container" />
        <xsl:if test="current()[@type='file'] | current()[@type='folder'] | current()[@type='Folder'] | current()[@type='Folders'] | current()[@type='Item'] | current()[@level='item'] | current()[@type='oversize'] | current()[@type='Oversize'] | current()[@type='volume'] | current()[@type='Volume'] | current()[@type='Volumes']">
                <xsl:value-of select="normalize-space(//ead:eadid)" />
                <xsl:text>&#09;</xsl:text>
                <xsl:value-of select="normalize-space(//ead:titleproper)" />
                <xsl:text>&#09;</xsl:text>
                <xsl:text>Box</xsl:text>
                <xsl:text>&#09;</xsl:text>
                <xsl:call-template name="box_recurse">
                    <xsl:with-param name="the_c" select="$containers" />
                    <xsl:with-param name="index" select="position()" />
                </xsl:call-template>
                <xsl:text>&#09;</xsl:text>
                <xsl:value-of select="@type" />
                <xsl:text>&#09;</xsl:text>
                <xsl:value-of select="."/>
                <xsl:text>&#09;</xsl:text>
                <xsl:value-of select="normalize-space(../ead:unittitle)" />
                <xsl:text>&#09;</xsl:text>
                <xsl:value-of select="..//ead:unitdate" />
                <xsl:text>&#10;</xsl:text>
            </xsl:if>
    </xsl:template>

    <xsl:template name="box_recurse">
        <xsl:param name="the_c" />
        <xsl:param name="index" />
        <xsl:choose>
            <xsl:when test="$the_c[$index][@type='Box']">
                <xsl:value-of select="$the_c[$index]"/>
            </xsl:when>
            <xsl:when test="$index - $decrement != 0">
                <xsl:call-template name="box_recurse">
                    <xsl:with-param name="the_c" select="$the_c" />
                    <xsl:with-param name="index" select="$index - $decrement" />
                </xsl:call-template>
            </xsl:when>
            <xsl:otherwise>
                <xsl:text>0</xsl:text>
            </xsl:otherwise>
        </xsl:choose>

    </xsl:template>
</xsl:stylesheet>