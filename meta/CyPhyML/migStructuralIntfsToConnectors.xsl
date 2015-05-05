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
		
	<!-- Change these to connectors -->
	<xsl:template match="@kind">
	  <xsl:attribute name="kind">
		<xsl:choose>
		  <xsl:when test=". = 'StructuralInterface'">
			<xsl:text>Connector</xsl:text>
		  </xsl:when>
		  <xsl:otherwise>
			<xsl:value-of select="." />
		  </xsl:otherwise>
		</xsl:choose>
	  </xsl:attribute>
	</xsl:template>
	
	<xsl:template match="@role">
	  <xsl:attribute name="role">
		<xsl:choose>
		  <xsl:when test=". = 'StructuralInterface'">
			<xsl:text>Connector</xsl:text>
		  </xsl:when>
		  <xsl:otherwise>
			<xsl:value-of select="." />
		  </xsl:otherwise>
		</xsl:choose>
	  </xsl:attribute>
	</xsl:template>
	
	<!-- delete these attributes of StructuralInterfaces -->
	<xsl:template match="model[@kind='StructuralInterface']/attribute[@kind='Style']" />
	<xsl:template match="model[@kind='StructuralInterface']/attribute[@kind='Role']" />
	<xsl:template match="model[@kind='StructuralInterface']/attribute[@kind='IgnoreInterface']" />
		
	<!-- whack these elements completely -->
	<xsl:template match="connection[@role='JoinStructures']" />
	<xsl:template match="atom[@kind='StructuralInterfaceForwarder']" />
</xsl:stylesheet>