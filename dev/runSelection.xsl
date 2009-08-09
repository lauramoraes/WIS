<?xml version="1.0" encoding="ISO-8859-1"?>

<xsl:stylesheet version="1.0"
xmlns:xsl="http://www.w3.org/1999/XSL/Transform">

<xsl:template match="/">
  <html>
  <body>
    <h2>Runs</h2>
    <table border="1">
    <tr bgcolor="#9acd32">
      <th align="left">Run Number</th>
      <th align="left">Run Type</th>
      <th align="left">Run Date</th>
      <th align="left">Events</th>
      <th align="left">Status</th>
      <th align="left">Histogram Creation Time</th>

    </tr>
    <xsl:for-each select="catalog/cd">
    <tr>
      <td><xsl:value-of select="title"/></td>
      <td><xsl:value-of select="artist"/></td>
    </tr>
    </xsl:for-each>
    </table>
  </body>
  </html>
</xsl:template>

</xsl:stylesheet>
