<?xml version="1.0" ?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
<xsl:template match="scene">
<TinySG version="0">
	<xsl:text>&#x0A;</xsl:text> <!-- LINE FEED -->
	<xsl:text>&#x09;</xsl:text> <!-- TAB -->
    <node name="_WORLD_">
    	<xsl:text>&#x0A;</xsl:text> <!-- LINE FEED -->
		<xsl:text>&#x09;</xsl:text> <!-- TAB -->
		<xsl:text>&#x09;</xsl:text> <!-- TAB -->
        <position value="0 0 0" />
        <xsl:text>&#x0A;</xsl:text> <!-- LINE FEED -->
		<xsl:text>&#x09;</xsl:text> <!-- TAB -->
		<xsl:text>&#x09;</xsl:text> <!-- TAB -->
        <orientation format="AngleAxis" angle="0" axis="1 0 0" />
    	<xsl:text>&#x0A;</xsl:text> <!-- LINE FEED -->
		<xsl:text>&#x09;</xsl:text> <!-- TAB -->
		<xsl:for-each select="space">
			<xsl:call-template name="_processSpace"/>
		</xsl:for-each>
		<xsl:text>&#x0A;</xsl:text> <!-- LINE FEED -->
		<xsl:text>&#x09;</xsl:text> <!-- TAB -->
    </node>
    
    <xsl:for-each select="space/body">
    	<xsl:variable name="WORLD" select="_WORLD_" />
    	<xsl:variable name="currBodyName" select="@name" />
    	<xsl:variable name="currBodyParent" select="@parent" />
    	<xsl:choose> <xsl:when test="child::transform">
			<!-- Body has one or more relative transformation steps to its containing body.
				 Therefore we may need to create some intermediate nodes which capture this offset. -->
			<xsl:choose> <xsl:when test="count(child::transform/*) &gt; 1">
				<xsl:for-each select="transform/*">
					<xsl:variable name="tempName" select="concat($currBodyName,'_inter_',position())" />
					<xsl:variable name="tempParent" select="concat($currBodyName,'_inter_',position()-1)" />
					<xsl:text>&#x0A;</xsl:text> <!-- LINE FEED -->
					<xsl:text>&#x09;</xsl:text> <!-- TAB -->
    				<xsl:element name="node">
    					<xsl:choose> <xsl:when test="position() = last()">
							<xsl:attribute name="name"><xsl:value-of select="$currBodyName" /></xsl:attribute>
							<xsl:attribute name="parent"><xsl:value-of select="$tempParent" /></xsl:attribute>
    					</xsl:when> <xsl:otherwise>
							<xsl:attribute name="name"><xsl:value-of select="$tempName" /></xsl:attribute>
							<xsl:choose> <xsl:when test="position() = 1">
								<xsl:attribute name="parent">
									<xsl:choose> <xsl:when test="contains($currBodyParent, 'world')">
										<xsl:text>_WORLD_</xsl:text>
									</xsl:when> <xsl:otherwise>
										<xsl:value-of select="$currBodyParent" />
									</xsl:otherwise> </xsl:choose>
								</xsl:attribute>
							</xsl:when> <xsl:otherwise>
								<xsl:attribute name="parent"><xsl:value-of select="$tempParent" /></xsl:attribute>
							</xsl:otherwise> </xsl:choose>
						</xsl:otherwise> </xsl:choose>
    					
    					<xsl:choose> <xsl:when test="name() = 'translation'">
							<xsl:call-template name="_processTranslation" />
						</xsl:when> <xsl:otherwise>
							<xsl:call-template name="_processRotation" />
						</xsl:otherwise> </xsl:choose>
		    			
		    			<xsl:text>&#x0A;</xsl:text> <!-- LINE FEED -->
						<xsl:text>&#x09;</xsl:text> <!-- TAB -->
		    		</xsl:element>
    			</xsl:for-each>
			</xsl:when> <xsl:otherwise>
				<xsl:for-each select="transform/*">
					<xsl:text>&#x0A;</xsl:text> <!-- LINE FEED -->
					<xsl:text>&#x09;</xsl:text> <!-- TAB -->
					<xsl:element name="node">
						<xsl:attribute name="name"><xsl:value-of select="$currBodyName" /></xsl:attribute>
						<xsl:attribute name="parent">
							<xsl:choose> <xsl:when test="contains($currBodyParent, 'world')">
								<xsl:text>_WORLD_</xsl:text>
							</xsl:when> <xsl:otherwise>
								<xsl:value-of select="$currBodyParent" />
							</xsl:otherwise> </xsl:choose>
						</xsl:attribute>
						<xsl:choose> <xsl:when test="name() = 'translation'">
							<xsl:call-template name="_processTranslation" />
						</xsl:when> <xsl:otherwise>
							<xsl:call-template name="_processRotation" />
						</xsl:otherwise> </xsl:choose>
		    			<xsl:text>&#x0A;</xsl:text> <!-- LINE FEED -->
						<xsl:text>&#x09;</xsl:text> <!-- TAB -->
		    		</xsl:element>
		    	</xsl:for-each>
			</xsl:otherwise> </xsl:choose>
		</xsl:when> <xsl:otherwise>
			<xsl:text>&#x0A;</xsl:text> <!-- LINE FEED -->
			<xsl:text>&#x09;</xsl:text> <!-- TAB -->
			<xsl:element name="node">
				<xsl:attribute name="name"><xsl:value-of select="$currBodyName" /></xsl:attribute>
				<xsl:attribute name="parent">
					<xsl:choose> <xsl:when test="contains($currBodyParent, 'world')">
						<xsl:text>_WORLD_</xsl:text>
					</xsl:when> <xsl:otherwise>
						<xsl:value-of select="$currBodyParent" />
					</xsl:otherwise> </xsl:choose>
				</xsl:attribute>
    			<xsl:text>&#x0A;</xsl:text> <!-- LINE FEED -->
				<xsl:text>&#x09;</xsl:text> <!-- TAB -->
    		</xsl:element>
		</xsl:otherwise> </xsl:choose>
		
		
    	<xsl:for-each select="geometry">
	    	<xsl:variable name="currGeomName" select="@name" />
    		<xsl:choose> <xsl:when test="child::transform">
				<!-- This section doesn't quite work right!!! To fix it, if there were N transform steps 
				you would need to create intermediate nodes for the first N-1 steps. Then the last node
				would need to contain the attached geometry information. -->
				<xsl:choose> <xsl:when test="count(child::transform/*) &gt; 1">
					<xsl:for-each select="transform/*">
						<xsl:variable name="tempName" select="concat($currGeomName,'_inter_',position())" />
						<xsl:variable name="tempParent" select="concat($currGeomName,'_inter_',position()-1)" />
						<xsl:text>&#x0A;</xsl:text> <!-- LINE FEED -->
						<xsl:text>&#x09;</xsl:text> <!-- TAB -->
	    				<xsl:element name="node">
	    					<xsl:choose> <xsl:when test="position() = last()">
								<xsl:attribute name="name"><xsl:value-of select="$currGeomName" /></xsl:attribute>
								<xsl:attribute name="parent"><xsl:value-of select="$tempParent" /></xsl:attribute>
	    					</xsl:when> <xsl:otherwise>
								<xsl:attribute name="name"><xsl:value-of select="$tempName" /></xsl:attribute>
								<xsl:choose> <xsl:when test="position() = 1">
									<xsl:attribute name="parent"><xsl:value-of select="$currBodyName" /></xsl:attribute>
								</xsl:when> <xsl:otherwise>
									<xsl:attribute name="parent"><xsl:value-of select="$tempParent" /></xsl:attribute>
								</xsl:otherwise> </xsl:choose>
							</xsl:otherwise> </xsl:choose>
	    					
	    					<xsl:choose> <xsl:when test="name() = 'translation'">
								<xsl:call-template name="_processTranslation" />
							</xsl:when> <xsl:otherwise>
								<xsl:call-template name="_processRotation" />
							</xsl:otherwise> </xsl:choose>
			    			
			    			<xsl:if test="position()=last()">
			    				<xsl:call-template name="_processGeometry"/>
			    			</xsl:if>
			    			
			    			<xsl:text>&#x0A;</xsl:text> <!-- LINE FEED -->
							<xsl:text>&#x09;</xsl:text> <!-- TAB -->
			    		</xsl:element>
	    			</xsl:for-each>
				</xsl:when> <xsl:otherwise>
					<xsl:text>&#x0A;</xsl:text> <!-- LINE FEED -->
					<xsl:text>&#x09;</xsl:text> <!-- TAB -->
					<xsl:element name="node">
						<xsl:attribute name="name"><xsl:value-of select="$currGeomName" /></xsl:attribute>
						<xsl:attribute name="parent"><xsl:value-of select="$currBodyName" /></xsl:attribute>
						<xsl:for-each select="transform/*">
							<xsl:choose> <xsl:when test="name() = 'translation'">
								<xsl:call-template name="_processTranslation" />
							</xsl:when> <xsl:otherwise>
								<xsl:call-template name="_processRotation" />
							</xsl:otherwise> </xsl:choose>
						</xsl:for-each>
						<xsl:call-template name="_processGeometry"/>
		    			<xsl:text>&#x0A;</xsl:text> <!-- LINE FEED -->
						<xsl:text>&#x09;</xsl:text> <!-- TAB -->
		    		</xsl:element>
				</xsl:otherwise> </xsl:choose>
			</xsl:when> <xsl:otherwise>
				<xsl:text>&#x0A;</xsl:text> <!-- LINE FEED -->
				<xsl:text>&#x09;</xsl:text> <!-- TAB -->
				<xsl:element name="node">
					<xsl:attribute name="name"><xsl:value-of select="$currGeomName" /></xsl:attribute>
					<xsl:attribute name="parent"><xsl:value-of select="$currBodyName" /></xsl:attribute>
					<xsl:call-template name="_processGeometry"/>
	    			<xsl:text>&#x0A;</xsl:text> <!-- LINE FEED -->
					<xsl:text>&#x09;</xsl:text> <!-- TAB -->
	    		</xsl:element>
			</xsl:otherwise> </xsl:choose>
    	</xsl:for-each>
    </xsl:for-each>
</TinySG>
</xsl:template>

<xsl:template match="geometry" name="_processGeometry">
	<xsl:text>&#x0A;</xsl:text> <!-- LINE FEED -->
	<xsl:text>&#x09;</xsl:text> <!-- TAB -->
	<xsl:text>&#x09;</xsl:text> <!-- TAB -->
	<xsl:element name="object">
		<xsl:attribute name="name"><xsl:value-of select="@name"/></xsl:attribute>
		<xsl:attribute name="type">ODE<xsl:value-of select="concat( translate( substring( @type, 1, 1 ),'abcdefghijklmnopqrstuvwxyz', 'ABCDEFGHIJKLMNOPQRSTUVWXYZ' ), substring( @type, 2, string-length( @type )))" /></xsl:attribute>
		<xsl:for-each select="parameter">
			<xsl:call-template name="_processParameter" />
		</xsl:for-each>
		<xsl:text>&#x0A;</xsl:text> <!-- LINE FEED -->
		<xsl:text>&#x09;</xsl:text> <!-- TAB -->
		<xsl:text>&#x09;</xsl:text> <!-- TAB -->
		<xsl:text>&#x09;</xsl:text> <!-- TAB -->
		<xsl:variable name="spaceName" select="ancestor::space/@name" />
		<xsl:element name="property">
			<xsl:attribute name="name">space</xsl:attribute>
			<xsl:attribute name="class">scene_object_ptr</xsl:attribute>
			<xsl:attribute name="value"><xsl:value-of select="$spaceName"/></xsl:attribute>
		</xsl:element>
		<xsl:text>&#x0A;</xsl:text> <!-- LINE FEED -->
		<xsl:text>&#x09;</xsl:text> <!-- TAB -->
		<xsl:text>&#x09;</xsl:text> <!-- TAB -->
	</xsl:element>
</xsl:template>

<xsl:template match="geometry" name="_processSpace">
	<xsl:text>&#x0A;</xsl:text> <!-- LINE FEED -->
	<xsl:text>&#x09;</xsl:text> <!-- TAB -->
	<xsl:text>&#x09;</xsl:text> <!-- TAB -->
	<xsl:element name="object">
		<xsl:attribute name="name"><xsl:value-of select="@name"/></xsl:attribute>
		<xsl:attribute name="type">ODE<xsl:value-of select="concat( translate( substring( @type, 1, 1 ),'abcdefghijklmnopqrstuvwxyz', 'ABCDEFGHIJKLMNOPQRSTUVWXYZ' ), substring( @type, 2, string-length( @type )))" /></xsl:attribute>
		<xsl:if test="child::parameter">
			<xsl:for-each select="parameter">
				<xsl:call-template name="_processParameter" />
			</xsl:for-each>
			<xsl:text>&#x0A;</xsl:text> <!-- LINE FEED -->
			<xsl:text>&#x09;</xsl:text> <!-- TAB -->
			<xsl:text>&#x09;</xsl:text> <!-- TAB -->
		</xsl:if>
	</xsl:element>
</xsl:template>

<!-- Matches parameter tags in SceneML files and writes an equivalent TinySG property tag -->
<xsl:template match="parameter" name="_processParameter">
	<xsl:text>&#x0A;</xsl:text> <!-- LINE FEED -->
	<xsl:text>&#x09;</xsl:text> <!-- TAB -->
	<xsl:text>&#x09;</xsl:text> <!-- TAB -->
	<xsl:text>&#x09;</xsl:text> <!-- TAB -->
	<xsl:element name="property">
		<xsl:attribute name="name"><xsl:value-of select="@type"/></xsl:attribute>
		<xsl:attribute name="class">real</xsl:attribute>
		<xsl:attribute name="value"><xsl:value-of select="@value"/></xsl:attribute>
	</xsl:element>
</xsl:template>

<xsl:template match="translation" name="_processTranslation">
	<xsl:text>&#x0A;</xsl:text> <!-- LINE FEED -->
	<xsl:text>&#x09;</xsl:text> <!-- TAB -->
	<xsl:text>&#x09;</xsl:text> <!-- TAB -->
	<xsl:element name="position">
		<xsl:attribute name="value"><xsl:value-of select="@value"/></xsl:attribute>
	</xsl:element>
</xsl:template>

<xsl:template match="rotation" name="_processRotation">
	<xsl:text>&#x0A;</xsl:text> <!-- LINE FEED -->
	<xsl:text>&#x09;</xsl:text> <!-- TAB -->
	<xsl:text>&#x09;</xsl:text> <!-- TAB -->
	<xsl:element name="orientation">
		<xsl:attribute name="format"><xsl:value-of select="@type"/></xsl:attribute>
		<xsl:attribute name="value"><xsl:value-of select="@value"/></xsl:attribute>
	</xsl:element>
</xsl:template>

</xsl:stylesheet>