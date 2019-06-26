<?xml version="1.0" encoding="utf-8"?>

<!-- This stylesheet composes base file name for output DSDL schemas. -->

<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
                version="1.0">
  <xsl:output method="text" encoding="utf-8"/>
  <xsl:include href="gen-common.xsl"/>

  <xsl:template match="/">
      <xsl:value-of select="$basename"/>
  </xsl:template>

</xsl:stylesheet>
