<?xml version='1.0'?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">

    <xsl:template match="/">
        <HTML>
            <HEAD>
                <TITLE>NVIDIA VGA BIOS Device Initialization (devinit) Script Opcode Specification</TITLE>
                <LINK type="text/css" rel="stylesheet" href="devinit.css"/>
            </HEAD>
            <BODY>
                <DIV class="title">NVIDIA VGA BIOS Device Initialization (devinit) Script Opcode Specification</DIV>

                <xsl:apply-templates select="devinit_spec/topic"/>

                <DIV class="index-header">Opcode Index</DIV>

                <DIV class="index-body">
                    <DIV class="index">
                        <DIV class="index-table-header">
                            <SPAN class="link">
                                Opcode
                            </SPAN>
                            <SPAN class="element">
                                Description
                            </SPAN>
                        </DIV>

                        <xsl:apply-templates select="devinit_spec/opcode" mode="opcode_index">
                            <xsl:sort data-type="text" select="@id"/>
                        </xsl:apply-templates>

                    </DIV>
                </DIV>

                <DIV class="index-header">Numeric Opcode Index</DIV>

                <DIV class="index-body">
                    <DIV class="numeric-index">
                        <DIV class="index-table-header">
                            <SPAN class="numeric">
                                Numeric
                            </SPAN>
                            <SPAN class="link">
                                Opcode
                            </SPAN>
                        </DIV>

                        <xsl:apply-templates select="devinit_spec/opcode" mode="numeric_index">
                            <xsl:sort data-type="text" select="@value"/>
                        </xsl:apply-templates>

                    </DIV>
                </DIV>

                <xsl:apply-templates select="devinit_spec/opcode">
                </xsl:apply-templates>

            </BODY>
        </HTML>
    </xsl:template>


    <xsl:template match="topic">
        <DIV class="topic-header"><xsl:value-of select="@name"/></DIV>
        <DIV class="topic-body">
            <xsl:apply-templates/>
        </DIV>
    </xsl:template>


    <xsl:template match="paragraph[parent::topic]">
        <DIV class="topic-paragraph">
            <xsl:apply-templates/>
        </DIV>
    </xsl:template>


    <xsl:template match="code">
        <DIV class="topic-code">
            <xsl:apply-templates/>
        </DIV>
    </xsl:template>


    <xsl:template match="opcode" mode="opcode_index">

        
        <DIV class="index-row">
            <SPAN class="link">
                <A>
                    <xsl:attribute name="href">#<xsl:value-of select="@id"/></xsl:attribute>
                    <xsl:value-of select="@id"/>
                </A>
            </SPAN>
            <SPAN class="element">
                <xsl:value-of select="summary"/>
            </SPAN>
        </DIV>

    </xsl:template>


    <xsl:template match="opcode" mode="numeric_index">

        
        <DIV class="index-row">
            <SPAN class="numeric">
                <xsl:value-of select="@value"/>
            </SPAN>
            <SPAN class="link">
                <A>
                    <xsl:attribute name="href">#<xsl:value-of select="@id"/></xsl:attribute>
                    <xsl:value-of select="@id"/>
                </A>
            </SPAN>
        </DIV>

    </xsl:template>


    <xsl:template match="opcode">

        <DIV class="opcode-page">
            <DIV class="opcode">

                <xsl:choose>
                    <xsl:when test="( @deprecated = 'true' )">
                        <xsl:attribute name="class">opcode-deprecated</xsl:attribute>
                    </xsl:when>
                    <xsl:when test="( ( history/core[@name='core6']/@supported = 'true' ) or ( history/core[@name='core6r2']/@supported = 'true' ) ) and ( history/core[@name='core5']/@supported = 'false' )">
                        <xsl:attribute name="class">opcode-new</xsl:attribute>
                    </xsl:when>
                    <xsl:otherwise>
                        <xsl:attribute name="class">opcode</xsl:attribute>
                    </xsl:otherwise>
                </xsl:choose>

                <DIV class="opcode-header">
                    <xsl:attribute name="id"><xsl:value-of select="@id"/></xsl:attribute>
                    <xsl:value-of select="@id"/> (<xsl:value-of select="@value"/>)
                </DIV>

                <xsl:apply-templates select="summary"/>

                <xsl:apply-templates select="layout"/>

                <xsl:apply-templates select="details"/>

                <xsl:apply-templates select="example"/>

                <xsl:apply-templates select="history"/>

                <xsl:if test="./audit">
                    <DIV class="audits-header">Audits</DIV>
                    <DIV class="audits-body">
                        <xsl:apply-templates select="audit"/>
                    </DIV>
                </xsl:if>

            </DIV>
        </DIV>

    </xsl:template>


    <xsl:template match="summary">
        <DIV class="summary">
            <xsl:apply-templates/>
        </DIV>
    </xsl:template>


    <xsl:template match="layout">
        <DIV class="layout-header">Format</DIV>
        <DIV class="layout-body">
            <DIV class="layout-table">
                <xsl:choose>
                    <xsl:when test="( ../@deprecated = 'true' )">
                        <DIV class="watermark">
                            Deprecated
                        </DIV>
                    </xsl:when>
                    <xsl:when test="( ( ../history/core[@name='core6']/@supported = 'true' ) or ( ../history/core[@name='core6r2']/@supported = 'true' ) ) and ( ../history/core[@name='core5']/@supported = 'false' )">
                        <DIV class="watermark">
                            New
                        </DIV>
                    </xsl:when>
                </xsl:choose>
                <DIV class="parameter">
                    <SPAN class="parameter-size">BYTE </SPAN>
                    <SPAN class="parameter-name"><xsl:value-of select="../@id"/></SPAN>
                    <SPAN class="parameter-desc">; Opcode</SPAN>
                </DIV>
                <xsl:apply-templates/>
            </DIV>
        </DIV>
    </xsl:template>


    <xsl:template match="array">
        <xsl:apply-templates>
            <xsl:with-param name="n">0</xsl:with-param>
        </xsl:apply-templates>
        <xsl:apply-templates>
            <xsl:with-param name="n">1</xsl:with-param>
        </xsl:apply-templates>
        <xsl:apply-templates>
            <xsl:with-param name="n">&lt;N-1&gt;</xsl:with-param>
        </xsl:apply-templates>
    </xsl:template>


    <xsl:template match="parameter">
        <xsl:param name="n"></xsl:param>
        <DIV class="parameter">
            <SPAN class="parameter-size">
                <xsl:choose>
                    <xsl:when test="( @size = '8' )">BYTE </xsl:when>
                    <xsl:when test="( @size = '-8' )">BYTE </xsl:when>
                    <xsl:when test="( @size = '16' )">WORD </xsl:when>
                    <xsl:when test="( @size = '-16' )">WORD </xsl:when>
                    <xsl:when test="( @size = '32' )">DWORD </xsl:when>
                    <xsl:otherwise><SPAN class="error">&lt;Unknown size&gt;</SPAN></xsl:otherwise>
                </xsl:choose>
            </SPAN>
            <SPAN class="parameter-name">
                <xsl:choose>
                    <xsl:when test="( @size = '8' )">u008</xsl:when>
                    <xsl:when test="( @size = '-8' )">s008</xsl:when>
                    <xsl:when test="( @size = '16' )">u016</xsl:when>
                    <xsl:when test="( @size = '-16' )">s016</xsl:when>
                    <xsl:when test="( @size = '32' )">u032</xsl:when>
                    <xsl:otherwise><SPAN class="error">unknown</SPAN></xsl:otherwise>
                </xsl:choose>
                <xsl:value-of select="@name"/>
                <xsl:value-of select="$n"/>
            </SPAN>
            <SPAN class="parameter-desc">
                ; <xsl:value-of select="@description"/><xsl:text> </xsl:text><xsl:value-of select="$n"/>
            </SPAN>
        </DIV>
    </xsl:template>


    <xsl:template match="details">
        <DIV class="details-header">Details</DIV>
        
        <DIV class="details-body">
       
            <DIV class="attrib-table">
                <DIV class="attrib-row">
                    <SPAN class="attrib-name">Deprecated</SPAN>
                    <SPAN class="attrib-value">
                        <xsl:choose>
                            <xsl:when test="( ../@deprecated = 'true' )">Yes</xsl:when>
                            <xsl:otherwise>No</xsl:otherwise>
                        </xsl:choose>
                    </SPAN>
                </DIV>
                <DIV class="attrib-row">
                    <SPAN class="attrib-name">Condition Flag</SPAN>
                    <SPAN class="attrib-value">
                        <xsl:choose>
                            <xsl:when test="( ../@conditionflag = 'failsets' )">Set to SKIP operations on failure</xsl:when>
                            <xsl:when test="( ../@conditionflag = 'clears' )">Reset to allow operations to be performed</xsl:when>
                            <xsl:when test="( ../@conditionflag = 'honors' )">Operations shall not performed when set to SKIP</xsl:when>
                            <xsl:when test="( ../@conditionflag = 'skipswrite' )">Only final register write not performed when set to SKIP</xsl:when>
                            <xsl:when test="( ../@conditionflag = 'skipsarraywrite' )">Only final writes to each register are not performed when set to SKIP</xsl:when>
                            <xsl:when test="( ../@conditionflag = 'skipsrw' )">Only final read/modify/write not performed when set to SKIP</xsl:when>
                            <xsl:when test="( ../@conditionflag = 'inverts' )">Inverted</xsl:when>
                            <xsl:when test="( ../@conditionflag = 'ignores' )">Ignored</xsl:when>
                            <xsl:otherwise><SPAN class="error">&lt;UNKNOWN&gt;</SPAN></xsl:otherwise>
                        </xsl:choose>
                    </SPAN>
                </DIV>
            </DIV>

            <xsl:apply-templates/>

            <xsl:apply-templates select="../restriction"/>

            <xsl:apply-templates select="../todo"/>

        </DIV>

    </xsl:template>


    <xsl:template match="paragraph[parent::details]">
        <DIV class="paragraph">
            <xsl:apply-templates/>
        </DIV>
    </xsl:template>


    <xsl:template match="note">
        <DIV class="note">
            <xsl:apply-templates/>
        </DIV>
    </xsl:template>

    <xsl:template match="list">
        <DIV class="list">
            <xsl:apply-templates/>
        </DIV>
    </xsl:template>


    <xsl:template match="item">
        <DIV class="item">
            <xsl:apply-templates/>
        </DIV>
    </xsl:template>


    <xsl:template match="todo">
        <DIV class="todo">
            TODO: <xsl:apply-templates/>
        </DIV>
    </xsl:template>

    <xsl:template match="restriction">
        <DIV class="restriction">
            <xsl:apply-templates/>
        </DIV>
    </xsl:template>

    <xsl:template match="example">
        <DIV class="example-header">Example</DIV>
        <DIV class="example-body">
            <xsl:apply-templates select="description"/>
            <DIV class="example">
                <xsl:apply-templates select="line"/>
            </DIV>
        </DIV>
    </xsl:template>

    <xsl:template match="description">
        <DIV class="example-description">
            <xsl:apply-templates/>
        </DIV>
    </xsl:template>

    <xsl:template match="line">
        <DIV class="example-line">
            <xsl:apply-templates/>
        </DIV>
    </xsl:template>


    <xsl:template match="history">
        <DIV class="history-header">History</DIV>
        <DIV class="history-body">
            <DIV class="history-table">
                <xsl:apply-templates/>
            </DIV>
        </DIV>
    </xsl:template>

    <xsl:template match="core">
        <DIV class="history-row">
            <SPAN class="history-core"><xsl:value-of select="@name"/></SPAN>
            <SPAN class="history-value">
                <xsl:choose>
                    <xsl:when test="( @supported = 'true' )">Yes</xsl:when>
                    <xsl:when test="( @supported = 'false' )">No</xsl:when>
                    <xsl:when test="( @supported = 'modified' )">Modified</xsl:when>
                    <xsl:when test="( @supported = 'resman' )">Resman only</xsl:when>
                    <xsl:when test="( @supported = 'vbios' )">VBIOS only</xsl:when>
                    <xsl:otherwise><SPAN class="error">&lt;UNKNOWN&gt;</SPAN></xsl:otherwise>
                </xsl:choose>
            </SPAN>
        </DIV>
    </xsl:template>

    <xsl:template match="audit">
        <DIV class="audit">
            <xsl:value-of select="@date"/> - 
            <xsl:value-of select="@engine"/>:
            <xsl:apply-templates/>
        </DIV>
    </xsl:template>

</xsl:stylesheet>
