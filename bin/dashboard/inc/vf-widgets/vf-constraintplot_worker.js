/*self.addEventListener('message', function(e) {
	//self.postMessage(getPerCell(vconfigs, allplots_f, num_of_grids));
	self.postMessage("xx is called");
	//self.postMessage({'size': size, 'plot_data': plot_data});
}, false);
*/
self.addEventListener('message', function(e) {
	//data coming in
	var data = e.data;
	var type = data.type;
	
	

	getPerCell = function (vconfigs, allplots_f, num_of_grids)
	{
		//get responces per cell
		per_cell = getrs(vconfigs, allplots_f, num_of_grids);
		return {per_cell: per_cell, vconfigs: vconfigs} ;
	}
	getConstraintPlots=	function (i, j, json_rs, vconfigs, num_of_grids, existing_cps)
	{
		function boundary(x, y) {
				return { xmin: x.min, xmax: x.max, ymin: y.min, ymax: y.max }
		}
		cps = new Array();
		var counter = 0;
		for (var k = 0; k < json_rs.length; k++) 
		{
			//reverse = false;
			if (json_rs[k].xlabel == vconfigs[i].label && json_rs[k].ylabel == vconfigs[j].label) 
			{
				cps[counter] =  new ConstraintPlot(json_rs[k], num_of_grids, boundary(vconfigs[i], vconfigs[j]), vconfigs[i].label, vconfigs[j].label, false);
				counter++;
			}
			//reverse = true
			else if (json_rs[k].xlabel == vconfigs[j].label && json_rs[k].ylabel == vconfigs[i].label) 
			{
				cps[counter] = new CConstraintPlot(existing_cps[counter]);
				counter++;
			}
		}
		if (cps.length == 0)
			return null;
		else 
			return cps;
	}
	getrs =	function (vconfigs, allplots_f, num_of_grids) {
		var r = {};
		var cps;
		for (var i = 0; i < vconfigs.length; i++) 
		{
			for (var j = 0; j < vconfigs.length; j++) 
			{
				if (vconfigs[i].min == null || vconfigs[i].max == null) 
					r[i + "" + j] = null;
				else if (i == j)
					r[i + "" + j] = null;
				else {
					//new algorithm - only calculate half of plots
					r[i + "" + j] = new getConstraintPlots(i, j, allplots_f, vconfigs, num_of_grids ,r[j + "" + i]);
				}
			} //end of for
		} //end of for
		return r;
	}
			// copy existing ConstraintPlot but make reverse opposite
		CConstraintPlot = function ( cp )
			{
				this.xlabel = cp.ylabel;
                this.ylabel = cp.xlabel;
				
				this.rid = cp.rid;
				this.inequality = cp.inequality;
				this.color = cp.color;
				this.z_target = cp.z_target;
				this.xmin = cp.ymin;
                this.xmax = cp.ymax;
                this.ymin = cp.xmin;
                this.ymax = cp.xmax;
                this.a = cp.a;
                this.b = cp.b;
                this.c = cp.c;
                this.d = cp.d;
                this.e = cp.e;
                this.f = cp.f;
				
				this.name = cp.name; 
                this.zmin = cp.zmin;
                this.zmax = cp.zmax;
				this.copied = !cp.copied;
                
			}
			//per line
		ConstraintPlot = function  (fn, num_of_grids, v, xlabel, ylabel, copied)
			{
				//this.intercecting_y_axis = [];
			    //this.intercecting_x_axis = [];
				this.copied = copied;
				this.rid = fn.rid;
				this.color = fn.color;
				this.name = fn.r.metric; //MJD

                 //added for 3Dplot
                this.xlabel = xlabel;
                this.ylabel = ylabel;
				this.a = Number(fn.a);
                this.b = Number(fn.b);
                this.c = Number(fn.c);
                this.d = Number(fn.d);
                this.e = Number(fn.e);
                this.f = Number(fn.f);

                this.z_target = Number(fn.z); ;
                this.inequality = fn.inequality;
				
				this.xmin = Number(v.xmin);
				this.xmax = Number(v.xmax);
				this.ymin = Number(v.ymin);
				this.ymax = Number(v.ymax);
			    
				//console.debug("========================= rname: " + this.name);
				//console.debug("xmin: " + this.xmin + " xmax: " + this.xmax);
				//console.debug("ymin: " + this.ymin + " ymax: " + this.ymax);
				//console.debug("ztarget: " + inequality + " " + z_target);
				//console.debug("a: " + a );
				//console.debug("b: " + b );
				//console.debug("c: " + c );
				//console.debug("d: " + d );
				//console.debug("e: " + e );
				//console.debug("f: " + f );
                
				this.useClipper = true;
				var isAnyZero = false;
				
				var x_grid_data = (new XGrids(this.xmin, this.xmax, num_of_grids)).x_data;
				var y_grid_data = (new XGrids(this.ymin, this.ymax, num_of_grids)).x_data;


				this.xdelta = Math.abs(x_grid_data[1] - x_grid_data[0]);
				this.ydelta = Math.abs(y_grid_data[1] - y_grid_data[0]);
				
				this.contour_index = [];
				this.xVsy = [];
				var num_of_contours = 0;
                this.zmin = null;
                this.zmax = null;
				for (var i = 0; i < x_grid_data.length; i++) {
                    //console.debug("x_grid_data.length: " + x_grid_data.length);
                    //console.debug("y_grid_data.length: " + y_grid_data.length);
					perY = [];
					for (j = 0; j < y_grid_data.length; j++) {
						vs = getZValue(this.a, this.b, this.c, this.d, this.e, this.f, x_grid_data[i], y_grid_data[j]);

                        //added zmin, zmax for 3D plot
                        if ( this.zmin == null){
                            this.zmin = vs;
                            this.zmax = vs;
                        }
                        else if ( this.zmin > vs)
                            this.zmin = vs;
                        else if ( this.zmax < vs)
                            this.zmax = vs;

                        if (this.inequality == ">=" && vs >= this.z_target){
							perY.push(0);
							isAnyZero = true;
							if ( this.useClipper && (i == 0 || i == x_grid_data.length - 1 || j == 0 || j == y_grid_data.length - 1))
								this.useClipper = false;
								
						}
						else if ( this.inequality == "<=" && vs <= this.z_target)	{
							perY.push(0);
							isAnyZero = true;
							if ( this.useClipper && (i == 0 || i == x_grid_data.length - 1 || j == 0 || j == y_grid_data.length - 1))
								this.useClipper = false;
						}
						else {
							if ( j != 0 ){
								if (perY[j-1] != 0){
									if ( i != 0 )
									{
										if (this.xVsy[i-1][j] != 0 ) {
											m = resetIfSelfAndPrevious(j, perY, this.xVsy[i-1][j], this.contour_index, this.xVsy);
											perY = m.perY;
											this.contour_index = m.contour_index;
										}
										else if ( this.xVsy[i-1][j-1] != 0 ){
											 m = resetIfSelfAndPrevious(j, perY, this.xVsy[i-1][j-1], this.contour_index, this.xVsy);
											 perY = m.perY;
											this.contour_index = m.contour_index;
										}
										else if ( j < y_grid_data.length -1 && this.xVsy[i-1][j+1] != 0 ){
											m = resetIfSelfAndPrevious(j, perY, this.xVsy[i-1][j+1], this.contour_index, this.xVsy);
											perY = m.perY;
											this.contour_index = m.contour_index;
											this.xVsy = m.xVsy;
										}
										else {
											perY.push(perY[j-1]);
										}
									}
									else 
										perY.push(perY[j-1]);
								}
								else if ( i != 0 )
								{
									if (this.xVsy[i-1][j] != 0 ){
										m =  resetIfSelfAndPrevious(j, perY, this.xVsy[i-1][j], this.contour_index,this.xVsy);
										perY = m.perY;
										this.contour_index = m.contour_index;
									}
									else if ( this.xVsy[i-1][j-1] != 0 ){
										m = resetIfSelfAndPrevious(j, perY, this.xVsy[i-1][j-1], this.contour_index,this.xVsy);
										perY = m.perY;
										this.contour_index = m.contour_index;
									}
									else if ( j < y_grid_data.length -1 && this.xVsy[i-1][j+1] != 0 ){
										m = resetIfSelfAndPrevious(j, perY, this.xVsy[i-1][j+1], this.contour_index,this.xVsy);
										perY = m.perY;
										this.contour_index = m.contour_index;
									}
									else {
										perY.push(++num_of_contours);
										this.contour_index.push(num_of_contours);
										}
								}
								else {
									perY.push(++num_of_contours);
									this.contour_index.push(num_of_contours);
								}
							}
							else if (i != 0) //j = 0
							{
								if (this.xVsy[i-1][j] != 0 ){
									m = resetIfSelfAndPrevious(j, perY, this.xVsy[i-1][j], this.contour_index, this.xVsy);
									perY = m.perY;
									this.contour_index = m.contour_index;
								}
								else if ( this.xVsy[i-1][j+1] != 0 ){
									m = resetIfSelfAndPrevious(j, perY, this.xVsy[i-1][j+1], this.contour_index, this.xVsy);
									perY = m.perY;
									this.contour_index = m.contour_index;
								}
								else {
									perY.push(++num_of_contours);
									this.contour_index.push(num_of_contours);
								}
							}
							else {
								perY.push(++num_of_contours);
								this.contour_index.push(num_of_contours);
							}
						}	
					}
					
					this.xVsy.push(perY);
				} //end of i = 0;
				if ( this.useClipper && isAnyZero)
					this.contour_index.push(0);
				else 
					this.useClipper = false; //reset not use clipper because there is no zero area.
			} //end of ConstraintPlot
			function resetIfSelfAndPrevious (j, perY, value, contour_index, xVsy){
				//console.debug("value: " + value);
				perY.push(value);
				var k = j-1;
				var beforeValue = new Number(perY[k]);
				var firstRepacedKIndex = null, lastRepacedKIndex = null;
				var replaced = false;
				while( k != -1 && perY[k] != 0 ){
					if (perY[k] != value){
						//console.debug( k + "- replaced: old: " + perY[k] + " new: " + value + " beforeValue: " + beforeValue);
						replaced = true;
						lastRepacedKIndex = k;
						if (firstRepacedKIndex == null )
							firstRepacedKIndex = k;
						perY[k--] = value;	
					}
					else
						k--;
					//perY[k--] = value;
				}
				var imaxIndex = xVsy.length -1;
				
				if ( firstRepacedKIndex != null ){
					for ( var z = firstRepacedKIndex; z >= lastRepacedKIndex; z-- )
					{
						if ( xVsy[imaxIndex][z] != 0 && xVsy[imaxIndex][z] != value ){
							xVsy = floodFillStack( imaxIndex, z, xVsy, value); 	
							break;
						}
					}
				}
				if ( replaced ){
					var removingIndex = contour_index.indexOf(beforeValue);
					var beforeTheIndex = contour_index.splice(0, removingIndex);
					var afterTheIndex =  contour_index.splice(removingIndex + 1 );
					var reconstructed = beforeTheIndex.concat(afterTheIndex);
					contour_index = reconstructed;
				}
				
				return {perY: perY, contour_index: contour_index, xVsy: xVsy} ;
			}
			//stack version of floodFill - this does not crush? 
			function floodFillStack (i, j, xVsy, newValue) {
				var theStack = [];
				theStack.push([i, j]);
				var popped;
				while (theStack.length > 0){
				
					popped = theStack.pop();
					if(alreadyFilled(popped[0], popped[1], xVsy, newValue)) 
						continue;
					xVsy[popped[0]][popped[1]]=newValue;
				
					theStack.push([popped[0],   popped[1]-1]); //down
					theStack.push([popped[0]+1,  popped[1]]); //right
					theStack.push([popped[0],    popped[1]+1]); //up
					theStack.push([popped[0]-1,  popped[1]]); //left
				}
				return xVsy;
			}
			function alreadyFilled(i, j, xVsy, newValue) {
				if ( !xVsy[i]){//i < 0 ){
					return true;
				}
				else if ( !xVsy[i][j]) //j < 0 )
					return true;
				else
					return  xVsy[i][j] == 0 || xVsy[i][j] == newValue;
			}
		//finding z value
		getZValue= 	function (a, b, c, d, e, f, x, y) {
				return a + b * x + c * y + d * x * x + e * y * y + f * x * y;
			}

		Point=function (px, py) {
				this.x = px;
				this.y = py;
			}
		XGrids=	function (xmin, xmax, num) {
				this.x_data = [];
				var dx = (xmax - xmin) / num;
				for (var x0 = xmin; x0 <= xmax - dx; x0 = x0 + dx) {
					this.x_data.push(x0);
					//console.debug(x0);
				}
				this.x_data.push(xmax); //last point is added as xmax because significant error make the last points not be xmax
				this.delta = dx;
		}
					// helper function to scale up polygon coordinates
				
				//0 and 1 -> r01 
				//r01 & 2 -> r012
				//r012 & 43 -> r0123
				//Each results can contain 1, or more
				//then the next union will do multiple types
				//return [[]];
								
				function PolyUnions(original, scale)
				{
					//console.debug("before UNION" );
					//console.debug( original);
					var modified2 = [];
					if ( original.length >= 2 ){
						modified0 = polyUnion(original[0], original[1], scale);
						if ( original.length == 2)
							modified2.push(modified0);
						else {
							for ( var i = 0; i < modified0.length; i++ )
							{
								for ( k = 2; k < original.length; k++ ){
									modified1 =  polyUnion(modified0[i], original[k], scale);
									modified2.push( modified1);
								}
							}
						}
					}
					else {
						modified2.push(original);
					}
					return modified2;
				}			
	polyUnion =	function (first, second, scale)
				{
					var one = [];
					var two = [];
					one.push(first);
					two.push(second);
					subj_polygons = scaleup(one, scale);
					clip_polygons = scaleup(two, scale);
					var cpr = new ClipperLib.Clipper();
					cpr.AddPolygons(subj_polygons, ClipperLib.PolyType.ptSubject);
					cpr.AddPolygons(clip_polygons, ClipperLib.PolyType.ptClip);
					var subject_fillType = ClipperLib.PolyFillType.pftNonZero;
					var clip_fillType = ClipperLib.PolyFillType.pftNonZero;
					var solution_polygons = new ClipperLib.Polygons();
					cpr.Execute( ClipperLib.ClipType.ctUnion, solution_polygons, subject_fillType, clip_fillType);
					return ploy2Array(solution_polygons, scale);
				}		
		polyDifference = function (subj_polygons, clip_polygons, scale)
				{
					subj_polygons = scaleup(subj_polygons, scale);
					clip_polygons = scaleup(clip_polygons, scale);
					var cpr = new ClipperLib.Clipper();
					cpr.AddPolygons(subj_polygons, ClipperLib.PolyType.ptSubject);
					cpr.AddPolygons(clip_polygons, ClipperLib.PolyType.ptClip);
					var subject_fillType = ClipperLib.PolyFillType.pftNonZero;
					var clip_fillType = ClipperLib.PolyFillType.pftNonZero;
					var solution_polygons = new ClipperLib.Polygons();
					cpr.Execute( ClipperLib.ClipType.ctDifference, solution_polygons, subject_fillType, clip_fillType);
					//console.debug(JSON.stringify(solution_polygons));
					return solution_polygons;
				}		
		scaleup = function (poly, scale) {
				  var i, j;
				  if (!scale) scale = 1;
				  for(i = 0; i < poly.length; i++) {
					for(j = 0; j < poly[i].length; j++) {
					  poly[i][j].X *= scale;
					  poly[i][j].Y *= scale;
					}
				  }
				  return poly;
				}
				//[{X:10,Y:10},{X:110,Y:10},{X:110,Y:110},{X:10,Y:110}];
		ploy2Array = function (poly, scale)
				{
					var	arrayB = [];
					if (!scale) scale = 1;
					for(i = 0; i < poly.length; i++) {
						var arrayA = [];
						for(j = 0; j < poly[i].length; j++) {
							arrayA.push( {X: poly[i][j].X, Y: poly[i][j].Y} );
						}
						arrayB.push(arrayA);
					}
					return arrayB;
				}

				// converts polygons to SVG path string
		polys2path = function  (poly, scale, xmin, ymin, xdelta, ydelta, x, y, xlabel, ylabel) {
				  var path = "", i, j;
				  if (!scale) scale = 1;
				  for(i = 0; i < poly.length; i++) {
					for(j = 0; j < poly[i].length; j++) {
					  if (!j) path += "M";
					  else path += "L";
					  //path += (poly[i][j].X / scale) + ", " + (poly[i][j].Y / scale);
					  
					  path += x[xlabel](xmin + (poly[i][j].Y / scale)* xdelta) + ", " +  y[ylabel](ymin + (poly[i][j].X / scale)* ydelta);
					  //return [ x[p.x.label](xmin + point[1]* xdelta),  y[p.y.label](ymin + point[0]* ydelta) ];}
					}
					path += "Z";
				  }
				  return path;
				}
	////////////////////////method used for calculate type = 1			
	getResponse = function (rid, json_fns) {
		for (var i = 0; i < json_fns.length; i++) {
		   if (json_fns[i].rid == rid) {
			   return json_fns[i];
		   }
		}
		return null;
	}
	getPlotsForRid = function( jsondata, rid )
	{
		var allplots_f = [];
		var p;
		var json_a_response = getResponse(rid, jsondata.responses);
		if (json_a_response == null)
				return;
		var json_a_user_selected_response = getResponse( rid, jsondata.userInputs.selectedResponses);
		var numinputs = json_a_response.inputs.length;
		for (var ii = 1; ii < numinputs; ii++) {
			for (var jj = 2; jj < numinputs + 1; jj++) {
				if (ii < jj) {
					p = new Plot_p(ii, jj, json_a_response, json_a_user_selected_response, jsondata.userInputs.defaultVariableValues, jsondata.userInputs.selectedVariables, null);
					if (p.r != undefined)  //p.r is undefined if x or y is not selected variable
						allplots_f.push(p);
				}
			}
		}
		return allplots_f;
	} 
	//calculate coefficient a,b,c,d,e,f of a+b*x+c*y+d*x*x+e*y*y+f*x*y by setting 2 variables as x and y and others as constant
	Plot_p =function (ii, jj, json_a_response, json_a_user_selected_response, json_userInputs_defaultvariablevalues, selected_variables, old_c) {
	
		if ( old_c != null)
		{
			this.xlabel = old_c.xlabel;
			this.ylabel = old_c.ylabel;
			this.r = old_c.r;
			this.rid  = old_c.rid;
			this.inequality  = old_c.inequality;
			this.color = old_c.color;
			this.z  = old_c.z_target;
			this.xmin  = old_c.xmin;
			this.xmax = old_c.xmax;
			this.ymin = old_c.ymin;
			this.ymax = old_c.ymax;
			this.a = old_c.a;
			this.b = old_c.b;
			this.c = old_c.c;
			this.d = old_c.d;
			this.e = old_c.e;
			this.f = old_c.f;
			return;

		}
			this.xlabel = json_a_response.inputs[ii - 1].label;
			this.ylabel = json_a_response.inputs[jj - 1].label;
	
			var x_is_selected_variable = false;
			var y_is_selected_variable = false;
			for ( var i = 0; i < selected_variables.length; i++ )
			{
				if ( selected_variables[i] == this.xlabel)
					x_is_selected_variable = true;
				else if (selected_variables[i] == this.ylabel)
					y_is_selected_variable = true;
			}
			if ( !x_is_selected_variable  ||  ! y_is_selected_variable )
                return null;	
	
	
			//console.debug("==========calculating ===========plot_p=====" + json_a_response.metric + " " + json_a_response.rid + "=========================="+
			//											json_a_response.inputs[ii-1].label + " vs. " + json_a_response.inputs[jj-1].label);
			this.r = json_a_response;
			this.rid = json_a_response.rid;
			this.inequality = json_a_user_selected_response.inequality;
			this.color = json_a_user_selected_response.color;
			this.z = json_a_user_selected_response.value;
			//this.x_index = ii;
			this.xmin = json_a_response.inputs[ii - 1].min;
			this.xmax = json_a_response.inputs[ii - 1].max;
			//this.y_index = jj;
			this.ymin = json_a_response.inputs[jj - 1].min;
			this.ymax = json_a_response.inputs[jj - 1].max;
			
			var counter = 0;
			var constvs = new Array();
			for (var k = 1; k < json_a_response.inputs.length + 1; k++) {
				if (parseInt(k) != parseInt(ii) && parseInt(k) != parseInt(jj)) {
					constvs[counter++] = k;
				}
			}
			this.a = json_a_response.coefficients["b0"];
			this.b = json_a_response.coefficients["b" + ii];
			this.c = json_a_response.coefficients["b" + jj];
			this.d = json_a_response.coefficients["b" + ii + ii];
			this.e = json_a_response.coefficients["b" + jj + jj];
			this.f = json_a_response.coefficients["b" + ii + jj];

			/*console.debug("=========Before===============");
			console.debug("plot.a: " + this.a);
			console.debug("plot.b: " + this.b);
			console.debug("plot.c: " + this.c);
			console.debug("plot.d: " + this.d);
			console.debug("plot.e: " + this.e);
			console.debug("plot.f: " + this.f);
			console.debug("==============================");
            */

			//based on constant coefficient a, b, and c are modified.
			for (var i = 0; i < constvs.length; i++) {
				var cindex = "b" + constvs[i];
				var cindexindex = cindex + constvs[i];
				var index = constvs[i] - 1;
				var defaultValue = getDefault(json_a_response.inputs[index], json_userInputs_defaultvariablevalues);
				this.a += json_a_response.coefficients[cindexindex] * defaultValue * defaultValue;
				this.a += json_a_response.coefficients[cindex] * defaultValue;
				var bindex = "b" + smtolg(ii, constvs[i]);
				var cindex = "b" + smtolg(jj, constvs[i]);
				this.b += json_a_response.coefficients[bindex] * defaultValue;
				this.c += json_a_response.coefficients[cindex] * defaultValue;
			}
			
			for (var i = 0; i < constvs.length; i++ ){
				for ( var j = i + 1; j < constvs.length; j++ ){
					if ( j > i ) {
						var cindex = "b"+ constvs[i] + constvs[j];
						this.a += json_a_response.coefficients[cindex] * getDefault(json_a_response.inputs[constvs[i]-1], json_userInputs_defaultvariablevalues) *getDefault(json_a_response.inputs[constvs[j]-1], json_userInputs_defaultvariablevalues);
					}
				}
			}
			/*
			console.debug("===========After==============");	
			console.debug("plot.a: " + this.a);
			console.debug("plot.b: " + this.b);
			console.debug("plot.c: " + this.c);
			console.debug("plot.d: " + this.d);
			console.debug("plot.e: " + this.e);
			console.debug("plot.f: " + this.f);
			console.debug("==============================");
			console.debug( this);
			*/
 		} //end of plot function
	//small to large - used to change 31 to 13 because coefficient exists as b13 instead of b31
	smtolg = function (a,b) {
		if(parseInt(a)<parseInt(b))
			return a +""+ b;
		else 
			return b +""+ a;
	}
	
	//default value for variable
	getDefault = function (input, json_userInputs_defaultvariablevalues) {
		for (var i = 0; i < json_userInputs_defaultvariablevalues.length; i++) {
			if(json_userInputs_defaultvariablevalues[i].label==input.label)
				return json_userInputs_defaultvariablevalues[i].defaultValue;
		}
	}	
	//////////////////////////////////////////			
				
	if ( type == 0){
		var vconfigs = data.vconfigs;
		var allplots_f = data.new_plots;
		var num_of_grids = data.num_of_grids;
		self.postMessage({'type':type, 'rid': data.rid, 'result': getPerCell(vconfigs, allplots_f, num_of_grids)});
		
	}
	else //data is gone so restart function calculation-> grid data
		self.postMessage({'type':type, 'rid': data.rid, 'result': getPlotsForRid(data.jsondata, data.rid)});
	self.close();
}, false);
