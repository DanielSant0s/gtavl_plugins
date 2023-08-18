<?xml version="1.0" encoding="ISO-8859-1"?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
                version="1.0">

<xsl:output method="html" indent="no"/>
 
<!-- **************
     Set parameters
     ************** -->

  <xsl:param name="toc.section.depth" select="'4'"/>

  <xsl:param name="html.stylesheet" select="'default.css'"/>
  <xsl:param name="html.stylesheet.type" select="'text/css'"/>

  <xsl:param name="html.cleanup" select="'1'"/>
  <xsl:param name="make.valid.html" select="'1'"/>
  <xsl:param name="make.single.year.ranges" select="'1'"/>
  <xsl:param name="make.year.ranges" select="'1'"/>

  <!-- Use ID value for generated filenames -->
  <xsl:param name="use.id.as.filename" select="'1'"/>

  <!-- Depth to which sections are chunked -->
  <xsl:param name="chunk.section.depth" select="'1'"/>

  <!-- Create a chunk for the 1st top-level section too  -->
  <xsl:param name="chunk.first.sections" select="'1'"/>

  <xsl:param name="admon.graphics" select="'0'"/>
  <xsl:param name="navig.graphics" select="'0'"/>
  <xsl:param name="navig.showtitles" select="'1'"/>

  <!-- Generate more links for Site Navigation Bar -->
  <xsl:param name="html.extra.head.links" select="1"/>
 
  <!-- Label sections too (eg. 2.1, 2.1.1) -->
  <xsl:param name="section.autolabel" select="'1'"/>
  <xsl:param name="section.label.includes.component.label" select="'1'"/>

  <!-- Use informal procedures; no need to number them -->
  <xsl:param name="formal.procedures" select="'0'"/>
  <xsl:param name="formal.title.placement">
    figure after
		example before
		equation after
		table after
		procedure before
  </xsl:param>

  <xsl:param name="generate.toc">
    appendix  toc
    article   toc
    book      toc
    chapter   toc
    part      toc
    preface   toc
    qandadiv  toc
    qandaset  toc
    reference toc
    section   toc
    set       toc
  </xsl:param>

<!-- *********
     Templates
     ********* -->

  <xsl:template match="application">
    <span class="application"><xsl:apply-templates/></span>
  </xsl:template>

  <xsl:template match="option">
    <tt class="option"><xsl:apply-templates/></tt>
  </xsl:template>

  <xsl:template match="filename">
    <tt class="filename"><xsl:apply-templates/></tt>
  </xsl:template>

  <xsl:template match="keycap">
    <span class="keycap"><b><xsl:apply-templates/></b></span>
  </xsl:template>

  <xsl:template match="guimenu">
    <span class="guimenu"><xsl:apply-templates/></span>
  </xsl:template>

  <xsl:template match="guisubmenu">
    <span class="guisubmenu"><xsl:apply-templates/></span>
  </xsl:template>

  <xsl:template match="guimenuitem">
    <span class="guimenuitem"><xsl:apply-templates/></span>
  </xsl:template>

  <xsl:template name="user.footer.content">
<!-- Start of StatCounter Code -->
<script type="text/javascript" language="javascript">
<xsl:comment>
var sc_project=1113098;
var sc_invisible=1;
var sc_partition=10;
var sc_security="357050be";
</xsl:comment>
</script>
<script type="text/javascript" language="javascript" src=
"http://www.statcounter.com/counter/counter.js">
</script>
<noscript><a href="http://www.statcounter.com/" target=
"_blank"><img src=
"http://c11.statcounter.com/counter.php?sc_project=1113098&amp;java=0&amp;security=357050be&amp;invisible=1"
alt="counter create hit" border="0"/></a></noscript>
<!-- End of StatCounter Code -->
   </xsl:template>

</xsl:stylesheet>
