function stl2json(stlString)
{
		//console.log(stlString);
		var colors = [0x808080, 0x808000, 0xFFFF00, 0x00FF00, 0x00FFFF, 0xFF00FF, 0x008080, 0x800080, 0x0000FF, 0xFF0000, 0x000080, 0x800000] //0x00FFFF Aqua

		
		var solids = [];
		var solidsString = stlString.replace(/endsolid[^\n]*/, "");
		solidsString = solidsString.split("solid"); 
		console.debug( solidsString.length);
		//parsedSolid = ParseSTLSolid(solidsString[1], colors[3]);
		//console.debug("+++++++++++++++++++++++++");
		
		//console.debug( solidsString[0]);
		//console.debug("--------------------------");
		//return;
		//solids.push(parsedSolid);
		var factor = 5;
		var denominator = null;
		var colorIndex = 0;
		for ( var i = 0; i < solidsString.length; i++ )
		{
			//console.debug("before: " + solidsString[i].length);
			solidsString[i] = solidsString[i].replace(/^\s+/, ""); //remove emply lines
			//console.debug("after: " + solidsString[i].length);
			if ( solidsString[i].length > 1) {
				parsedSolid = ParseSTLSolid("solid " + solidsString[i], colors[colorIndex++], factor, denominator);
				console.debug("+++++++++" + i + "++++++++++++++++");
				console.debug( "colorIndex: " + colors[colorIndex - 1]);
				console.debug("solid " + solidsString[i].substring( 0, solidsString[i].indexOf("\n")));
				console.debug("--------------------------");
		
				
				solids.push(parsedSolid);
				//console.debug( "denominator: " + denominator);
			}
		}
		
		return solids;
		
	
		
  // build stl's vertex and face arrays
	function ParseSTLSolid (STLString, color) {
    var vertexes  = []; //array of array
    var faces     = [];
	var normals = []; //array of array
	var normalIndices = [];
	
	var vertexCord = []; //vertex in single array
	var normalCord =[]; //normals in single array
  
    var face_vertexes = [];
    var vert_hash = {}
	var normal_hash = {};

	//STLString = STLString.replace(/^\s+/, ""); //remove emply lines
	//var id = STLString.substring( STLString.indexOf("solid ") + 6, STLString.indexOf("\n"));
	var id = STLString.substring( 0, STLString.indexOf("\n"));
	console.debug( "id: " + id);	
	
	
    // strip out extraneous stuff
    STLString = STLString.replace(/\r/, "\n");
    STLString = STLString.replace(/^solid[^\n]*/, ""); //removing solid line
    STLString = STLString.replace(/\n/g, " ");
    STLString = STLString.replace(/facet normal /g,"");
    STLString = STLString.replace(/outer loop/g,"");  
    STLString = STLString.replace(/vertex /g,"");
    STLString = STLString.replace(/endloop/g,"");
    STLString = STLString.replace(/endfacet/g,"");
    STLString = STLString.replace(/endsolid[^\n]*/, "");
    STLString = STLString.replace(/\s+/g, " ");
    STLString = STLString.replace(/^\s+/, "");

 
    var facet_count = 0;
    var block_start = 0;

    var points = STLString.split(" "); 
	var face_indices = [];
	var normal_indices = [];
	var xmin, xmax, ymin, ymax, zmin, zmax;
    //console.log({'status':'message', 'content':'Parsing vertices...'});
    for (var i=0; i<points.length/12-1; i++) {
      //if ((i % 100) == 0) {
      //  console.log({'status':'progress', 'content':parseInt(i / (points.length/12-1) * 100) + '%'});
      //}
	  //console.debug( i + " " + points[i]);
      
      for (var x=0; x<3; x++) {
		if ( (x %3) == 0 ){
			var normal = [parseFloat(points[block_start+x*3]), parseFloat(points[block_start+x*3+1]), parseFloat(points[block_start+x*3+2])];
		}
        var vertex = [parseFloat(points[block_start+x*3+3]), parseFloat(points[block_start+x*3+4]), parseFloat(points[block_start+x*3+5])];
        var vertexIndex = vert_hash[vertex];
        if (vertexIndex == null) {
          vertexIndex = vertexes.length;
          vertexes.push(vertex);
          vert_hash[vertex] = vertexIndex;
		  vertexCord.push( vertex[0], vertex[1], vertex[2]);
		  
			if ( xmin == null ){
				xmin = vertex[0];
				xmax = vertex[0];
				ymin = vertex[1];
				ymax = vertex[1];
				zmin = vertex[2];
				zmax = vertex[2];
				}
			else{
				if (vertex[0] < xmin)
					xmin = vertex[0];
				else if ( vertex[0] > xmax)
					xmax = vertex[0];
				if (vertex[1] < ymin)
					ymin = vertex[1];
				else if ( vertex[1] > ymax)
					ymax = vertex[1];
				if (vertex[2] < zmin)
					zmin = vertex[2];
				else if ( vertex[2] > zmax)
					zmax = vertex[2];					
			}
			
        }
		var normalIndex = normal_hash[normal];
		if (normalIndex == null){
			normalIndex = normals.length;
			normals.push(normal);
			normal_hash[normal] = normalIndex;
			normalCord.push( normal[0], normal[1], normal[2]);
		}
		normal_indices.push(normalIndex);
        face_indices.push(vertexIndex);
      }
      faces.push(face_indices);
	  normalIndices.push(normal_indices);
    
      block_start = block_start + 12;
    }
	var xmid = (xmax + xmin)/2;
	var ymid=  (ymax + ymin)/2;
	var zmid = (zmax + zmin)/2;
	
	//var factor = 0.5; //1 for coord value to be 0 to 1.  Larger the number 
	if ( factor != null){
		//var denominator = Math.sqrt(Math.pow(vertexCord[0],2) + Math.pow(vertexCord[1],2) + Math.pow(vertexCord[2],2));
		if ( denominator == null)
			denominator = (Math.sqrt(Math.pow(xmax,2) + Math.pow(ymax,2) + Math.pow(zmax,2)) + Math.sqrt(Math.pow(xmin,2) + Math.pow(ymin,2) + Math.pow(zmin,2)))/2;
		for ( var i = 0; i < vertexCord.length; i++){
			vertexCord[i] = vertexCord[i]/denominator * factor;
		}
		for ( var i = 0; i < normalCord.legnth; i++ ){
			normalCord[i] = normalCord[i]/denominator * factor
		}
		xmid = xmid/denominator * factor;
		ymid=  ymid/denominator * factor;
		zmid = zmid/denominator * factor;
	}
	console.debug("vertexCount: " + vertexes.length);
	console.debug("FacetCount: " + faces.length);
	console.debug("xmin: " + xmin + " xmax: " + xmax);
	console.debug("xmin: " + xmin*factor/denominator + " xmax: " + xmax*factor/denominator);
	//console.debug("ymin: " + ymin + " ymax: " + ymax);
	//console.debug("zmin: " + zmin + " zmax: " + zmax);
	console.debug( xmid + " " + ymid + " " + zmid);
	return {'VertexCount':vertexes.length,
	'VertexIndices': faces[0],
	'FacetCount':faces.length,
	 'Normals':normalCord,
	 'Id':id, 
     'Center':[xmid, ymid, zmid],
	 'NormalIndices': normalIndices[0], 
	 'VertexCoords':vertexCord,  
	 'Color': color};
  };
		
}