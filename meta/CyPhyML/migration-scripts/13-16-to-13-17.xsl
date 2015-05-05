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
		
	<!-- whack these elements completely -->
	<!-- these were used in Environments, 
		 and did not participate in connections -->
	<xsl:template match="atom[@kind='HeatPort']" />
	<xsl:template match="atom[@kind='FluidPort']" />
	<xsl:template match="atom[@kind='MultibodyFrame']" />
	<xsl:template match="atom[@kind='HydraulicPowerPort']" />
	<xsl:template match="atom[@kind='ThermalPowerPort']" />
	<xsl:template match="atom[@kind='ElectricalPowerPort']" />
	<xsl:template match="atom[@kind='TranslationalPowerPort']" />
	<xsl:template match="atom[@kind='RotationalPowerPort']" />
	<xsl:template match="atom[@kind='SoftwareCommunicationPort']" />
	<xsl:template match="atom[@kind='ExecutionContextPort']" />
	<xsl:template match="atom[@kind='ExecutionContextAssignPort']" />
	<xsl:template match="atom[@kind='BusPort']" />
	<xsl:template match="atom[@kind='MultibodyFramePowerPort']" />
	<xsl:template match="atom[@kind='InputSignalPort']" />
	<xsl:template match="atom[@kind='OutputSignalPort']" />
	<xsl:template match="atom[@kind='AggregatePort']" />
	<xsl:template match="atom[@kind='Message']" />
	<xsl:template match="atom[@kind='UsesMessage']" />
	<xsl:template match="atom[@kind='Value']" />
	<xsl:template match="atom[@kind='ModelicaSignal2Metric']" />
	<xsl:template match="atom[@kind='SimulinkModelName']" />
	<xsl:template match="atom[@kind='SignalDriver']" />
	<xsl:template match="atom[@kind='Modelica2ComponentPortComposition']" />
	<xsl:template match="atom[@kind='ModelicaPort2ComponentPortPowerFlow']" />
	<xsl:template match="atom[@kind='SimulinkModel']" />
	<xsl:template match="atom[@kind='ElectricalPort']" />
	<xsl:template match="atom[@kind='MechanicalDPort']" />
	<xsl:template match="atom[@kind='HydraulicPort']" />
	<xsl:template match="atom[@kind='ThermalPort']" />
	<xsl:template match="atom[@kind='MechanicalRPort']" />
	<xsl:template match="atom[@kind='InSignal']" />
	<xsl:template match="atom[@kind='OutSignal']" />
	<xsl:template match="atom[@kind='BondGraph']" />
	<xsl:template match="atom[@kind='ModelicaPort']" />
	<xsl:template match="atom[@kind='ModelicaSignalPort']" />
	<xsl:template match="atom[@kind='ElectricalPin']" />
	<xsl:template match="atom[@kind='RotationalFlange']" />
	<xsl:template match="atom[@kind='RealInput']" />
	<xsl:template match="atom[@kind='RealOutput']" />
	<xsl:template match="atom[@kind='IntegerInput']" />
	<xsl:template match="atom[@kind='IntegerOutput']" />
	<xsl:template match="atom[@kind='BooleanInput']" />
	<xsl:template match="atom[@kind='BooleanOutput']" />
	<xsl:template match="atom[@kind='FluidPort']" />
	<xsl:template match="atom[@kind='ModelicaMaterialSpec']" />
	<xsl:template match="atom[@kind='ModelicaOutputSignalPort']" />
	<xsl:template match="atom[@kind='ModelicaInputSignalPort']" />
	<xsl:template match="atom[@kind='MultibodyFrame']" />
	<xsl:template match="atom[@kind='ModelicaAggregateInterface']" />
	<xsl:template match="atom[@kind='ModelicaBusPortInterface']" />
	<xsl:template match="atom[@kind='ModelicaBusSignal']" />
	<xsl:template match="atom[@kind='ModelicaPortMap']" />
	<xsl:template match="atom[@kind='ModelicaElectricalPortMap']" />
	<xsl:template match="atom[@kind='ModelicaPowerPortMapBase']" />
	<xsl:template match="atom[@kind='ModelicaHeatPortMap']" />
	<xsl:template match="atom[@kind='ModelicaRotationalPortMap']" />
	<xsl:template match="atom[@kind='ModelicaTranslationalPortMap']" />
	<xsl:template match="atom[@kind='ModelicaFluidPortMap']" />
	<xsl:template match="atom[@kind='ModelicaMultibodyFramePortMap']" />
	<xsl:template match="atom[@kind='ModelicaBusPortMap']" />
	<xsl:template match="atom[@kind='BehaviorModelSignalPort']" />
	<xsl:template match="atom[@kind='IsBehaviorModelInputSignal']" />
	<xsl:template match="atom[@kind='IsBehaviorModelOutputSignal']" />
	<xsl:template match="atom[@kind='SimulinkModels']" />
</xsl:stylesheet>