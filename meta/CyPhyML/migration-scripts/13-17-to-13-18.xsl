<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
	<xsl:output encoding="UTF-8" method="html"/>
	
	<!-- match all nodes recursively (identity transform)-->
	<xsl:template match="@*|node()">
	  <xsl:copy>
		<xsl:apply-templates select="@*|node()"/>
	  </xsl:copy>
	</xsl:template>

	<!-- match root node -->
	<xsl:template match="/">
		<!-- Do header -->
		<xsl:text disable-output-escaping="yes">&lt;!DOCTYPE project SYSTEM "mga.dtd"&gt;

</xsl:text>

		<xsl:copy>
			<xsl:apply-templates select="@*|node()"/>
		</xsl:copy>
	</xsl:template>
		
	<!-- These are now references, so we must change the tag type -->
	<xsl:template match="atom[@kind='ModelicaParameter' or @kind='CADParameter']">
		<reference>
			<xsl:apply-templates select="@*|node()"/>
		</reference>
	</xsl:template>
    
	<xsl:template match="model[@kind='PreProcessing']"/>
	
</xsl:stylesheet>