<?xml version="1.0" standalone="no"?>
<!DOCTYPE svg PUBLIC "-//W3C//DTD SVG 1.1//EN" "http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd">

<svg width="20cm" height="50cm" xmlns="http://www.w3.org/2000/svg" version="1.1">
<title>Schedule for {{TRACEVIZ_NAME}}</title>

<defs>
<marker id="Triangle" viewBox="0 0 15 15" refX="7" refY="7" 
	markerUnits="strokeWidth" markerWidth="15" markerHeight="15" orient="auto">
	<path d="M 0 0 L 0 14 L 7 7 z" fill="white" stroke="white" />
</marker>
</defs>

<!-- ECMAScript to change the radius with each click -->
<script type="text/ecmascript">
    <![CDATA[
  
  function Highlight(evt) {
    evt.target.setAttribute( "opacity", "1.0" );
  }

  function Unhighlight(evt) {
    evt.target.setAttribute( "opacity", "0.8" );
  }

]]></script>


  <!-- General Formatting --> 
<text x="355" y="45" font-family="{{TRACEVIZ_FONT}}" font-size="20" fill="black" text-anchor="middle">Schedule for {{TRACEVIZ_NAME}}</text>
<path d="M 1 55 h {{TRACEVIZ_WIDTH}}" fill="green" stroke="black" stroke-width="3" />

<g id="MasterBlock" >
<!-- Superblocks -->
{{#SUPERBLOCK}}
    <g id="{{SUPERBLOCK_NAME}}_g" >
        <rect x="10" y="{{SUPERBLOCK_Y}}" rx="10" ry="10" width="{{SUPERBLOCK_WIDTH}}" 
            height="{{SUPERBLOCK_HEIGHT}}" fill="{{SUPERBLOCK_COLOR}}" stroke="black" stroke-width="1" opacity="0.6" />
        <text x="15" y="{{SUPERBLOCK_TITLE_Y}}" font-family="{{TRACEVIZ_FONT}}" font-size="20" fill="white">{{SUPERBLOCK_NAME}}</text>
{{#HYPERPERIODBLOCK}}
        <text x="15" y="{{HYPERPERIOD_Y}}" font-family="{{TRACEVIZ_FONT}}" font-size="10" fill="white">({{HYPERPERIOD}})</text>
{{/HYPERPERIODBLOCK}}
{{#TRACEBLOCK}}
        <!-- {{TRACE_NAME}} Trace Block -->
        <g id="{{TRACE_NAME}}_g">
            <text x="145" y="{{TRACE_LABEL_Y}}" font-family="{{TRACEVIZ_FONT}}" font-size="18" fill="black" text-anchor="end">{{TRACE_NAME}}</text>
            <path d="M 150 {{TRACE_HSTROKE_Y}} h {{TRACE_WIDTH}}" stroke="black" stroke-width="1" />
{{#PERIODBLOCK}}
            <path d="M {{PERIOD_VSTROKE_X}} {{PERIOD_VSTROKE_Y}} v 75" stroke="black" stroke-width="1" />
{{/PERIODBLOCK}}
            <!-- Trace Instances -->
{{#INSTANCEBLOCK}}
            <g onmouseover="Highlight(evt);" onmouseout="Unhighlight(evt);" >
                <rect x="{{INSTANCE_X}}" y="{{INSTANCE_Y}}" rx="3" ry="3" width="{{INSTANCE_WIDTH}}"
                    height="25" fill="{{INSTANCE_COLOR}}" stroke="black" stroke-width="1" opacity="0.8" />
                <text x="{{INSTANCE_LABELX}}" y="{{INSTANCE_LABELY}}" font-family="{{TRACEVIZ_FONT}}" font-size="10" fill="black">{{INSTANCE_TIME}}</text>
            </g>
{{/INSTANCEBLOCK}}
        </g>
{{/TRACEBLOCK}}
    </g>
{{/SUPERBLOCK}}
</g>

  <!-- Message Send/Receive Arrows -->
{{#SNDRCVBLOCK}}
<path d="M {{SNDRCV_X}} {{SNDRCV_Y}} v {{SNDRCV_HEIGHT}}" fill="none" stroke="white" stroke-dasharray="5,2" marker-end="url(#Triangle)"/>
{{/SNDRCVBLOCK}}

{{#KEYBLOCK}}
<!-- Build the message key -->
<text x="25" y="{{KEY_Y}}" text-decoration="line-through" font-family="{{TRACEVIZ_FONT}}" font-size="16" fill="black">Visualization Key</text>
{{#KEY_INST}}
<rect x="25" y="{{KEY_INSTY}}" width="25" height="25" fill="{{KEY_COLOR}}" 
      stroke="black" stroke-width="1" opacity="0.8" />
<text x="55" y="{{KEY_LABELY}}" font-family="{{TRACEVIZ_FONT}}" font-size="12" fill="black">{{KEY_NAME}}</text>
{{/KEY_INST}}
{{/KEYBLOCK}}

</svg>