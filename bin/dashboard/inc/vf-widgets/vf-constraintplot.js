/**
Javascript Libraies used
- Javascript Clipper - http://sourceforge.net/p/jsclipper/wiki/Home/
  To create a hole in sharded area 
- MathBox   - http://acko.net/blog/making-mathbox/ https://github.com/unconed/MathBox.js
  To display plots in 3D
- Contour Plot - http://bl.ocks.org/mbostock/4241134
  To create 2D plot from index of grid data
*/
function vf_constraintplot(element) {
	var self = this;
	this.element = element;

	//small to large - used to change 31 to 13 because coefficient exists as b13 instead of b31
	smtolg = function (a,b) {
		if(parseInt(a)<parseInt(b))
			return a +""+ b;
		else 
			return b +""+ a;
	}
	//default value for variable
	getDefault = function (defaultlabel, json_userInputs_defaultvariablevalues) {
		for (var i = 0; i < json_userInputs_defaultvariablevalues.length; i++) {
			if(json_userInputs_defaultvariablevalues[i].label==defaultlabel)
				return json_userInputs_defaultvariablevalues[i].defaultValue;
		}
	}	
	 // from a response from the json.responses based on a json.selectedResponse.rid	
	getResponse = function (rid, json_fns) {
		for (var i = 0; i < json_fns.length; i++) {
		   if (json_fns[i].rid == rid) {
			   return json_fns[i];
		   }
		}
		return null;
	}
	getPerCell = function (vconfigs, allplots_f, num_of_grids)
	{
		//get responces per cell
		per_cell = getrs(vconfigs, allplots_f, num_of_grids);
		return {per_cell: per_cell, vconfigs: vconfigs} ;
	}

	getNumOfGrids = function( fidelity, n ){
		var base_width = $(element).parent().width() - padding;
		//setting global variable "size"
		size  = (base_width / n) - 1.5;
		var num_of_grids = (base_width / n /2) - 1.5; //default = "low"
		if ( fidelity == "high") 
			num_of_grids = (base_width / n)* 2 - 1.5;
		else if ( fidelity == "med")
			num_of_grids = (base_width / n) - 1.5;
		console.debug("num_of_grids: " + num_of_grids);
		return num_of_grids;
	}
	getVConfigs  = function (selected_variables, allplots_f){
		var vconfigs = []; //ie., {label: "Crr", min: 0.005, max: 0.015}
		for (var i = 0; i < selected_variables.length; i++) {
			vconfigs.push(vConfig(selected_variables[i], allplots_f));
		}
		return vconfigs;
	}
	//find absolute min, max of a selected_variable from used functions
	vConfig = function (selected_variable, json_fns_o) {
		//var json_fns_index = [];
		var n_min; //possible to be undefined if no response uses this variable
		var n_max; //possible to be undefined if no response uses this variable
		var rfn;
		for (var i = 0; i < json_fns_o.length; i++) {
			rfn = json_fns_o[i];
			if (rfn.xlabel == selected_variable) {
				if (n_min == null || n_min < Number(rfn.xmin))
					n_min = Number(rfn.xmin);
				if (n_max == null || n_max > Number(rfn.xmax))
					n_max = Number(rfn.xmax);
			}
			else if (rfn.ylabel == selected_variable) {
				if (n_min == null || n_min < Number(rfn.ymin))
					n_min = Number(rfn.ymin);
				if (n_max == null || n_max > Number(rfn.ymax))
					n_max = Number(rfn.ymax);
			}
		}
		if (n_min >= n_max) {
			n_min = null;
			n_max = null;
		}
		return { label: selected_variable, min: n_min, max: n_max};
	}
	//calculate coefficient a,b,c,d,e,f of a+b*x+c*y+d*x*x+e*y*y+f*x*y by setting 2 variables as x and y and others as constant
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
	
	
			console.debug("==========calculating ===========" + json_a_response.metric + " " + json_a_response.rid + "=========================="+
														json_a_response.inputs[ii-1].label + " vs. " + json_a_response.inputs[jj-1].label);
  		
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
				var defaultValue = getDefault(json_a_response.inputs[index].label, json_userInputs_defaultvariablevalues);
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
						this.a += json_a_response.coefficients[cindex] * getDefault(json_a_response.inputs[constvs[i]-1].label, json_userInputs_defaultvariablevalues) *getDefault(json_a_response.inputs[constvs[j]-1].label, json_userInputs_defaultvariablevalues);
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
						cps[counter] =  new ConstraintPlot(json_rs[k], num_of_grids, boundary(vconfigs[i], vconfigs[j]), /*false, */ vconfigs[i].label, vconfigs[j].label, false);
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
		ConstraintPlot = function  (fn, num_of_grids, v, /*reverse, */xlabel, ylabel, copied)
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
			    /*
				console.debug("========================= rname: " + this.name);
				console.debug("xmin: " + this.xmin + " xmax: " + this.xmax);
				console.debug("ymin: " + this.ymin + " ymax: " + this.ymax);
				console.debug("ztarget: " + inequality + " " + z_target);
				console.debug("a: " + a );
				console.debug("b: " + b );
				console.debug("c: " + c );
				console.debug("d: " + d );
				console.debug("e: " + e );
				console.debug("f: " + f );
                */
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
			/** stack version of floodFill - this does not crush? **/
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
				/**
				0 and 1 -> r01 
				r01 & 2 -> r012
				r012 & 43 -> r0123
				Each results can contain 1, or more
				then the next union will do multiple types
				return [[]];
				*/				
				function PolyUnions(original, scale)
				{
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
		plotframe = function(p)
		{
			var cell = d3.select(this);
			cell.append("rect")
					.attr("class", "frame")
					.attr("x", padding / 2)
					.attr("y", padding / 2)
					.attr("width", size - padding)
					.attr("height", size - padding)
					.attr("height", size - padding);
		}
		plot2 = function(p)
		{
			//p_cell is null if i=j(diagonal) or variable's min > max
			if ( p.i == p.j )
				return;
			else {
				var cell = d3.select(this); //g.[object SVGAnimatedString]
				// Plot brush?
				//cell.call(brush.x(x[p.x]).y(y[p.y]));
				var p_cell = per_cell[p.i + "" + p.j]; //ConstraintPlot or CConstraintPlot
				if ( p_cell != null ){ //if no selected responses, p_cell is null
					
					for (var i = 0; i < p_cell.length; i++) {
						if ( p_cell[i].xVsy == undefined) {
							//console.debug("plotting..." + p.i + p.j);
							//clone path from original one based on class (i.e., isoline01)
							var paths =$(".isoline" +p.j + "" + p.i); //path, text or line
							for ( var k = 0; k < paths.length; k++ ){
								var clonedpath = paths[k].cloneNode(); //if cloneNode(true) will copy deep but consume too much memory
								if ( clonedpath.getAttribute("copied") == null){
									paths[k].setAttribute("copied", "true"); //set attribute to original ones so jquery($(".isoline" +p.j + "" + p.i) will not pick up this path anymore
									clonedpath.setAttribute("copied", "true"); //set attribute to cloned ones so jquery($(".isoline" +p.j + "" + p.i) will not pick up this path anymore
									
									//calculate new X and Y value "rotate(90 0 0) scale( -1, 1) translate(tx, ty)"
									if ( clonedpath.nodeName == "text"){ 
										var newX = (new Number(clonedpath.getAttribute("y")))*(-1)+size; //newX = -Y-ty
										var newY = (new Number(clonedpath.getAttribute("x")))*(-1)+size; //newX = -X-tx
										clonedpath.setAttribute("x", newX);
										clonedpath.setAttribute("y", newY);
									}
									else
										clonedpath.setAttribute("transform", "rotate(90 0 0) scale( -1, 1) translate(-" + size + ",-" + size + ")");
									clonedpath.textContent =  paths[k].textContent;
									cell.select(function() { return this.appendChild(clonedpath); });
									if ( clonedpath.nodeName == "line"){ //add mouseover and mouseout event to svg.line
										crosstick = d3.select(clonedpath);
										crosstick.on("mouseover", mouseinCopied).on("mouseout", mouseoutCopied);
										crosstick.datum(clonedpath.getAttribute("rid"));
									}
								}
							}
						}
					}
					//loop to add text on top of graphs
					for (var i = 0; i < p_cell.length; i++) {
						if ( p_cell[i] != null) {
							var copied; //copied or not to shown as debugging purpose
							var paths; //if path is > 0 then there is this path on the graph, so show the rectbox with its name
							if ( p_cell[i].xVsy == undefined) {
								paths = $(".isoline" + p.j + "" + p.i + "[rid='" +  p_cell[i].rid + "']" );
								copied = true;
								}
							else {
								paths = $(".isoline" + p.i+ "" + p.j + "[rid='" +  p_cell[i].rid + "']" );
								copied = false;
						}
							//updating data to both rect and text
						svg.selectAll("g.cell[id='"+ p.i + p.j+"'] .labeltext[rid='"+ p_cell[i].rid+ "']").datum(p_cell[i]);
						
						if (paths.length > 0 ){ //only show label when path exists
							//adding response label on left of cell
							var num_of_exisiting_label_rid = svg.selectAll("g.cell[id='"+ p.i + p.j+"'] .labeltext[rid='"+ p_cell[i].rid+ "']").size();
							//if already exist just to show it- data is updated in above
							if ( num_of_exisiting_label_rid == 2 ){ //rect and label both has .labeltext
								//label Existing for : [" + p.i + "" + p.j+ "] " + i + " : " + p_cell[i].rid
								svg.selectAll("g.cell[id='"+ p.i + p.j+"'] .labeltext[rid='"+ p_cell[i].rid+ "']").style("display","block");
							}
							else { //not existing
								//label NOT Existing for : [" + p.i + "" + p.j+ "] " + i + " : " + p_cell[i].rid
								//used to put (y) where put label text
								var num_of_exisiting_labels = svg.selectAll("g.cell[id='"+ p.i + p.j+"'] .labeltext").size()/2; //2 labeltext(rect and label)
								rrect = cell.append("rect")
									.attr("class", "rrect")
									.attr("class", "labeltext")
									.attr("opacity", 0.5) //original opacity
									//.attr("d", line)
									.attr("x", padding - padding * 1 + 25)
									.attr("y", padding + (padding - 18) * num_of_exisiting_labels - 10)
									.attr("width", 10)
									.attr("height", 10)
									.attr("fill", p_cell[i].color)
									.attr("rid", p_cell[i].rid)
									.on("dblclick", plot3d)
									.on("mouseover", mousein)
									.on("mouseout", mouseout);
								rrect.datum(p_cell[i]);
								rtext = cell.append("text")
									.attr("class", "labeltext")
									.attr("opacity", 0.5) //original opacity
									.attr("x", padding - padding * 1 + 40)
									.attr("y", padding + (padding - 18) * num_of_exisiting_labels - 2)
									.attr("fill", "white")
									.attr("rid", p_cell[i].rid)
									.on("mouseover", mousein)
									.on("mouseout", mouseout)
									.on("dblclick", plot3d)
									.text(function(){
										return p_cell[i].name;
									 });
								rtext.datum(p_cell[i]);
								}
							}
						}
					}
				}
				//remove existing frame
				svg.selectAll("g.cell[id='"+ p.i + p.j+"'] .frame").remove();
				
				//Make the cell box on top of the contours (paths) drawn
				//This will make hovering over contours look a lot better... MJD
				//console.debug( padding + " " + size );
				cell.append("rect")
					.attr("class", "frame")
					.attr("id", p.i + p.j)
					.attr("x", padding / 2)
					.attr("y", padding / 2)
					.attr("width", size - padding)
					.attr("height", size - padding)
					.attr("height", size - padding)
					.attr("fill", "#444")
					.attr("fill-opacity", 0 )
					.on("click", function(d){
						var rect = this.getBoundingClientRect();
						//console.debug(rect.top, rect.right, rect.bottom, rect.left);
						var tempx = event.clientX - rect.left + padding/2;
						var tempy = event.clientY - rect.top + padding/2;
						var xdomain =  x[d.x.label].invert(tempx);
						var ydomain =  y[d.y.label].invert(tempy);
						$(document).trigger($(element).attr('id') + '_clicked', [event, d, xdomain, ydomain]);		
						
					});
				}
				
				//moving labeltext just after the frame so it can be double click to show 3D
				$("g.cell[id='"+ p.i + p.j+"'] .labeltext").insertAfter("g.cell[id='"+ p.i + p.j+"'] .frame");
			$(".crossticktext").hide();
		}
		/* plot in 3D 
			requires following variables
			- xmin, xmax
			- ymin, ymax
			- zmin, zmax 
			- z_target, ineuqlity
			- xlabel, ylabel
			- name (response name)
		*/
        function plot3d(d)
        {
            //remove existing iframe
            $("#plot3dif").empty();

           if ( d.copied == true)
				reverse = 1;
			else
				reverse = 0;

			if (d.inequality == ">=" )
                inequality = "L";
            else
                inequality = "S";

                  var attvalues = "vf-plot3D-url.html?" +
                 "xmin=" + d.xmin +
                 "&xmax=" + d.xmax +
                 "&ymin=" +d.ymin +
                 "&ymax=" + d.ymax +
                 "&zmin=" + d.zmin +
                 "&zmax=" + d.zmax +
                 "&ztarget=" + d.z_target +
                 "&scolor=" + d.color[0].replace("#", "0x") + //var scolor = "#20C050".replace("#", "0x"); //#20C050
                 "&a=" + d.a +
                 "&b=" + d.b +
                 "&c=" + d.c +
                 "&d=" + d.d +
                 "&e=" + d.e +
                 "&f=" + d.f +
                 "&name=" + d.name +
                 "&xlabel=" + d.xlabel +
                 "&ylabel=" + d.ylabel +
                 "&reverse=" + reverse +
                 "&inequality=" + inequality + "";

            myWidth = Math.floor($(element).parent().width()/2 - (padding*2));
            myHeight = Math.floor($(element).parent().height()/2-(padding*3));
            $("#plot3dif").append( $("<iframe></iframe>")
                     .attr("id", d.name + "_" + d.xlabel + "vs." + d.ylabel)
                     .attr("src",attvalues)
                     .attr("width", "100%") //default - but replace with show
                     .attr("height", "100%")
                     .css("max-height", myHeight )
                     .css("min-height", myHeight-50 )
                     .css('border', 'none')

            );


           $("#plot3dh h3").remove();
            $("#plot3dh").append("<h3>" +  d.name + " (" + d.xlabel + " vs. " + d.ylabel+ ")</h3>");
            $("#plot3d .modal-body")
                .css("maxHeight",myHeight)
                .css("padding", "0px");
            $("#plot3d html").css("height", "100%");
            $("#plot3d body").css("height", "100%")
                        .css("margin", "0")
                        .css("padding", "0");
            $('#plot3d').modal('show');

        }
		function createDataTablePoint( data, did)
		{
				function createDataTable(headers, tableData)
				{
					var aoCol = [];
					for (var i=0; i < headers.length; i++) {
						aoCol.push({'sTitle' : headers[i], 'sClass': 'center'});	
					}
				
					//default
					var aColumConfig=
						 {
							"bJQueryUI": true,
							"aaData": tableData,
							"aoColumns": aoCol,
							"bPaginate": false,
							"bAutoWidth": true,
							"bFilter": false,
							"bInfo": false

						};
					return aColumConfig;
				}
				
				var rows="";
				var keys = Object.keys(data);
				var htmlText = "";
				var tables =[];
				for ( var i = 0; i < keys.length; i++ )
				{
					if (!$.isArray( data[keys[i]])){
						rows+= "<tr><td><div> "+ keys[i] + "" + "</div></td><td><div>" + data[keys[i]] + "</div></td><tr>";
					}
					else {
						if ( rows.length != 0 ){
							htmlText += '<table class="ccpTable">" + rows + "</table>';
							rows="";
						}
						for ( var j = 0; j < data[keys[i]].length; j++ )
						{
							htmlText += '<p><table class="ccpTable" width ="100%" cellpadding="0" cellspacing="0" border="0" class="display" id="' + did + "table" + tables.length + '"></table>'
							tables.push(createDataTable(data[keys[i]][j].Header, data[keys[i]][j].Body));
						}
					}
				}
				if ( rows != "")
					htmlText += '<table class="ccpTable">' + rows + "</table>";
				
				$("#" + did).html( htmlText );
				for ( var i = 0; i < tables.length; i++ )
				{
					$("#" + did+"table" + i).dataTable(tables[i]);
				}	
		}
		function createDataTable3( data, did)
		{
				function createDataTable(headers, tableData)
				{
					var aoCol = [];
					for (var i=0; i < headers.length; i++) {
						aoCol.push({'sTitle' : headers[i], 'sClass': 'center'});	
					}
				
					//default
					var aColumConfig=
						 {
							"bJQueryUI": true,
							"aaData": tableData,
							"aoColumns": aoCol,
							"bPaginate": false,
							"bAutoWidth": true,
							"bFilter": false,
							"bInfo": false

						};
					return aColumConfig;
				}
				
				var rows="";
				var keys = Object.keys(data);
				var htmlText = "";
				var tables =[];
				for ( var i = 0; i < keys.length; i++ )
				{
					if (!$.isArray( data[keys[i]])){
						rows+= "<tr><td><div> "+ keys[i] + "" + "</div></td><td><div>" + data[keys[i]] + "</div></td><tr>";
					}
					else {
						if ( rows.length != 0 ){
							htmlText += '<table class="ccpTable">" + rows + "</table>';
							rows="";
						}
						for ( var j = 0; j < data[keys[i]].length; j++ )
						{
							htmlText += '<p><table class="ccpTable" width ="100%" cellpadding="0" cellspacing="0" border="0" class="display" id="' + did + "table" + tables.length + '"></table>'
							tables.push(createDataTable(data[keys[i]][j].Header, data[keys[i]][j].Body));
						}
					}
				}
				if ( rows != "")
					htmlText += '<table class="ccpTable">' + rows + "</table>";
				
				$("#" + did).html( htmlText );
				for ( var i = 0; i < tables.length; i++ )
				{
					$("#" + did+"table" + i).dataTable(tables[i]);
				}	
		}
		function createATable(d)
		{
			var tableDataVariables = {};
			var variables = {};
			var v_body = [];
			d3.selectAll( ".cpxvalue").each( function (){
				v_body.push(new Array( this.getAttribute("id"), this.textContent));
				variables[this.getAttribute("id")] = this.textContent;
			});
			tableDataVariables.Tables0 = [];
			tableDataVariables.Tables0.push({"Header": ["Name", "Value"], "Body": v_body});
			
			var tableDataResponses ={};
			v_body = [];
			
			/*console.debug("value cacluated from Constraint Plot object...");
			d3.selectAll("g.cell[id='01'] .rrect").each( function(d){ 
				//console.debug( d); //Constraint Plot
				console.debug( d.name  + "=" + getZValue(d.a, d.b, d.c, d.d, d.e, d.f, variables[d.xlabel], variables[d.ylabel]));
			});*/
			//console.debug("Value calculated using function...");
			for ( var i = 0; i < jsondata.userInputs.selectedResponses.length; i++ )
			{
				//var fString = jsondata.responses[i].function;
				var json_response =getResponse(jsondata.userInputs.selectedResponses[i].rid, jsondata.responses);
				var fString = json_response.function; //function defined in jsondata
				console.debug(fString);
				//substitute variable to variable values
				//i.e., 5*crr+6 -> 5*1+6 if crr = 1
				for (var vName  in variables){
					fString = fString.replace(eval("/"+vName+"/g"),  variables[vName]);
				}
				var z_calculated = eval(fString); 				//"Value calculated using function...");
				//console.debug( fString);
				//console.debug( json_response.metric  + "=" + z_calculated);
				
				var z_target_description = "";
				//inequality defines good area (passing area)
				var inequality =jsondata.userInputs.selectedResponses[i].inequality;
				if (inequality == ">=" ){ //larger than
						//console.debug ("larger than: " + inequality);
						z_target_description = inequality + jsondata.userInputs.selectedResponses[i].value;
						if (z_calculated >= jsondata.userInputs.selectedResponses[i].value) //good area
							pass_or_fail = "T"; 
						else 
							pass_or_fail = "F";
				}
				else { //less than
					//console.debug ("less than: " + inequality);
					z_target_description = inequality + jsondata.userInputs.selectedResponses[i].value;
					if (z_calculated  <= jsondata.userInputs.selectedResponses[i].value) //good area
						pass_or_fail = "T"; 
					else 
						pass_or_fail = "F";
				}
				v_body.push( new Array(jsondata.userInputs.selectedResponses[i].name,z_calculated, z_target_description, pass_or_fail)); 
			}
			tableDataResponses.Tables1 = [];
			tableDataResponses.Tables1.push({"Header":  ["Name", "Value", "Target", "Pass"], "Body":  v_body});
		
			//remove existing iframe
            $("#ccpdetailb1").empty();
			$("#ccpdetailb2").empty();
			createDataTablePoint( tableDataVariables, "ccpdetailb1");
			createDataTable3( tableDataResponses, "ccpdetailb2");
            $('#ccpdetail').modal('show');
			
			$(document).trigger($(element).attr('id') + '_click_on_xx', [event ,d]);
			
		}
		function createXLines(p){
		
			svg.selectAll("g.cell[id='"+ p.i + p.j+"'] .cpxline").remove();
		
			var cell = d3.select(this);
			var id = p.i + "" + p.j;
			var p_cell = per_cell[p.i + "" + p.j];
			
			//var ymiddle = (p.y.min + p.y.max)/2;
			//var xmiddle = (x[p.x.label](p.x.min) + x[p.x.label](p.x.max))/2;
			var ydefault = getDefault( p.y.label, jsondata.userInputs.defaultVariableValues);
			//	getDefault(
			
			if ( p.x.min != undefined)
			{
				cell.append("line")
					.attr("class", "cpxline")
					.attr("x1", x[p.x.label](p.x.min))
					.attr("y1", y[p.y.label](ydefault))
					.attr("x2", x[p.x.label](p.x.max))
					.attr("y2", y[p.y.label](ydefault))
					.attr("id", p.y.label);
					
				if ( p.i == (vconfigs.length -1) ){	
					svg.selectAll("g.cell[id='"+ p.i + p.j+"'] .cpyvalue").remove();
					cell.append("text")
						.attr("class","cpyvalue")
						.attr("text-anchor","middle")
						.attr("x", x[p.x.label](p.x.max) + padding/3)
						.attr("y", y[p.y.label](ydefault))
						.style("fill","red")
						.attr("id", p.y.label)
						.text(function(d) {
							return ydefault;
						})
						.on("click", createATable);
				}
			}
		}
		function createYLines(p){
		
			svg.selectAll("g.cell[id='"+ p.i + p.j+"'] .cpyline").remove();
		
			function linedrag(xlinear, yy){
				var drag = d3.behavior.drag();
				drag.on("dragend", function(d){
					//console.debug( "dragend.... "  + d.x.label  + " xdomain: " + d.xdomain + " xrange: " + d.xrange);
					$(document).trigger($(element).attr('id') + '_dragend', [event, d]);
					//moving ylines' xvalue
					d3.selectAll( ".cpxvalue[id='" + d.x.label+ "']")
					.each( function (dd){
						d3.select(this)
						.attr("x", d.xrange)
						.text(function(ddd) {
							return d.xdomain;
						});
						//dd.xdomain = d.xdomain;
					});
					//moving xlines yvalue
					d3.selectAll( ".cpyvalue[id='" + d.x.label+ "']")
					.each( function (dd){
						d3.select(this)
						.attr("y", d.yrange)
						.text(function(ddd) {
							return d.xdomain;
						});
						//dd.xdomain = d.xdomain;
					});
					
					
				});
				drag.on("drag", function(d){
						var xrange = d3.event.x;
						var xdomain =  xlinear.invert(xrange);
						//moving y lines
						//adjust when mouse is moved outside of boundary
						d3.selectAll( ".cpyline[id='" + d.x.label+ "']")
						.each(function (dd){
							if ( xdomain > dd.x.max )
								xrange = xlinear(dd.x.max);
							else if ( xdomain < dd.x.min)
								xrange = xlinear(dd.x.min);
							d3.select(this)
								.attr("x1",xrange)
								.attr("x2",xrange)
							}
							
						);
						d.xdomain = xdomain;
						d.xrange = xrange;
						d.yrange = yy[d.x.label](xdomain); //where this label should moved in y direction for other plots
						//moving x lines
						d3.selectAll( ".cpxline[id='" + d.x.label+ "']")
						.each(function (dd){
							d3.select(this)
								.attr("y1",d.yrange)
								.attr("y2",d.yrange)
							}
							
						);
				});
				return drag;
			}
		
			var cell = d3.select(this);
			var xdefault = getDefault(p.x.label, jsondata.userInputs.defaultVariableValues);
			
			if ( p.y.min != undefined)
			{
				var yline = cell.append("line")
					.attr("class", "cpyline")
					.attr("x1",  x[p.x.label](xdefault))
					.attr("y1", y[p.y.label](p.y.min))
					.attr("x2",  x[p.x.label](xdefault))
					.attr("y2", y[p.y.label](p.y.max))
					.attr("id", p.x.label)
					.call(linedrag( x[p.x.label], y));
					
				
				if ( p.j == (vconfigs.length -1) ){	
					svg.selectAll("g.cell[id='"+ p.i + p.j+"'] .cpxvalue").remove();
					cell.append("text")
						.attr("class","cpxvalue")
						.attr("text-anchor","middle")
						.attr("x", x[p.x.label](xdefault))
						.attr("y", y[p.y.label](p.y.min) + padding/3)
						.style("fill","red")
						.attr("id", p.x.label)
						.text(function(d) {
							return xdefault;
						})
						.on("click", createATable);
				}
			}
		
		} //end of createYLines


		plot = function (p)
		{
			var cell = d3.select(this); //g.[object SVGAnimatedString]
			// Plot brush?
			//cell.call(brush.x(x[p.x]).y(y[p.y]));
            //var vf_constraintplot_sibling_div = document.getElementsByTagName("vf-constraintplot")[0].parentNode;
			//p_cell is null if i=j(diagonal) or variable's min > max
			if ( p.i == p.j )
			{
				if (($("g.cell[id='"+ p.i + p.j+"'] .frame")).length == 0 ){
					//Even if its the diagonal, plot a box... MJD
					cell.append("rect")
					.attr("class", "frame")
					.attr("x", padding / 2)
					.attr("y", padding / 2)
					.attr("width", size - padding)
					.attr("height", size - padding);
				}
				return;
				
			}
			else { //start Demo grid-contour
				var p_cell = per_cell[p.i + "" + p.j]; //Array of ConstraintPlots or CConstraintPlots
//				console.debug("plotting..." + p.i + p.j);
//				console.debug(p_cell);
				if ( p_cell != null )
				{
					for (var i = 0; i < p_cell.length; i++) 
					{
					if ( p_cell[i].xVsy !== undefined)
					{
                        //console.debug(p_cell[i]);
						xmin = p_cell[i].xmin;
						ymin = p_cell[i].ymin;
						xdelta = p_cell[i].xdelta;
						ydelta = p_cell[i].ydelta;
						
						var griddata = p_cell[i].xVsy;
/////////////////////  dummy data for 3 holes //////////////////////////////						
						/* 
						// creating dummy data based on original data
						var original = p_cell[i].xVsy;
						griddata = [];
						for ( var k = 0; k < original.length; k++ ){
							griddata_y = [];
							for ( var kk = 0; kk < original[k].length; kk++ ){
								if ( k > 220 && k < 250 &&  kk > 150 && kk < 250){
									griddata_y[kk] = 0;
								}
								else if ( k > 120 && k < 150 &&  kk > 150 && kk < 250){
									griddata_y[kk] = -1;
								}
								else if ( k > 50 && k < 130 &&  kk > 50 && kk < 160){
									griddata_y[kk] = -2;
								}
								else if ( k > 150 && k < 200 &&  kk > 30 && kk < 100){
									griddata_y[kk] = -3;
								}
								else
									griddata_y[kk] = 1;
							}
							griddata[k] = griddata_y;
						}
						console.debug("!!!!!!!!!!!!!!!!!!!!!griddata!!!!!!!!!!!!!!!!!!!!");
						console.debug(griddata);
						console.debug(original);
						//must be true to use clipper
						p_cell[i].useClipper = true;
						//adding hole indexes
						p_cell[i].contour_index.push(0);
						p_cell[i].contour_index.push(-1);
						p_cell[i].contour_index.push(-2);
						p_cell[i].contour_index.push(-3);
						
						*/
//////////////// end of dummt data ///////////////////////						
						
						var dx = griddata[0].length -1 , dy = griddata.length -1;
						var intercecting_y_axis = [];
						var intercecting_x_axis = [];
					   
						if ( !p_cell[i].useClipper )
						{
							//console.debug("NOT USING CLIPPER");
							for ( var k = 0; k < p_cell[i].contour_index.length; k++ ){
								cell.data([ function (x, y) {  return (x >= 0 && y >= 0 && x < dx && y < dy && griddata[y][x] == p_cell[i].contour_index[k]);	}]);
								cell.append("path")
									.datum(function(d) { 
											var c = d3.geom.contourOriginal(d, griddata);
											if ( c == undefined )
												return []; //throw exception because can't plot oneline like
															//00001
															//00001
															//00000
											return c.map(function(point){
												//console.debug(point[1] + " , " + point[0]);
												//x = point[1]
												//y = point[0]
												// intercecting  x or y axis
												if ( point[0] != 0 && point[0] != dx ){
													if ( point[1] == 0 ){
														intercecting_y_axis.push( {x: xmin + point[1]* xdelta, y: ymin + point[0]* ydelta, delta: -padding/2});
														//console.debug(p.i + "" + p.j + " intercectiing_y_axis-----x: " + point[1] + " y: " + point[0]);
													}
                                                    else if ( point[1] == dy ){
                                                        intercecting_y_axis.push( {x: xmin + point[1]* xdelta, y: ymin + point[0]* ydelta, delta: -padding/4});
                                                        //console.debug(p.i + "" + p.j + " intercectiing_y_axis-----x: " + point[1] + " y: " + point[0]);
                                                    }
												}
												else if (point[1] != 0 && point[1] != dy ){
													if ( point[0] == 0) {
														intercecting_x_axis.push( {x: xmin + point[1]* xdelta, y: ymin + point[0]* ydelta, delta: -padding/2});
														//console.debug(p.i + "" + p.j + "intercectiing_x_axis-----x: " + point[1] + " y: " + point[0]);
													}
													else if ( point[0] == dx ){
														intercecting_x_axis.push( {x: xmin + point[1]* xdelta, y: ymin + point[0]* ydelta, delta: -padding/4});
														//console.debug(p.i + "" + p.j + " intercectiing_x_axis-----x: " + point[1] + " y: " + point[0]);
													}
												}
												return [ x[p.x.label](xmin + point[1]* xdelta),  y[p.y.label](ymin + point[0]* ydelta) ];}
											)
								})
								.attr("class","isoline" + p.i + "" + p.j)
								.attr("d", function (d) { if (d.length != 0) return "M" + d.join("L") + "Z"; else return null; })
								.attr("opacity", 0.2)
								.attr("pointer-events", "stroke")
								.attr("rid", p_cell[i].rid)
								.style("fill", p_cell[i].color) //.style("fill", "red");
								.on("mouseover", mousein)
								.on("mouseout", mouseout);
							}
						}
						else {
							///////////////// clipper ////////////////////
							console.debug("USING CLIPPER");
							////////////////////////// start clipper //////////////////////////
							var subj_polygons = [];
							var clip_polygons = [];
							var a_clip_polygons = [];
							var a_subj_polygons = [];
							
							for ( var k = 0; k < p_cell[i].contour_index.length; k++ ){
								fn = function (x, y) {  return (x >= 0 && y >= 0 && x < dx -1 && y < dy - 1&& griddata[y][x] == p_cell[i].contour_index[k])};
								var c = d3.geom.contourOriginal(fn, griddata);
								//console.debug(c);
								if ( c != undefined){
									if (p_cell[i].contour_index[k] <= 0){ //less the 0 is difference
										var a_clip_polygons = [];
										for ( var j = 0; j < c.length; j++ )
											a_clip_polygons.push({X: c[j][0], Y:c[j][1]});
										clip_polygons.push(a_clip_polygons);	
									}
									else { //larger1 is unioned first
										var a_subj_polygons = []
										for ( var j = 0; j < c.length; j++ )
											a_subj_polygons.push({X: c[j][0], Y:c[j][1]});
										subj_polygons.push(	a_subj_polygons);
									}
								}
							}
							var scale = 1;								
							var unions = PolyUnions( subj_polygons, scale); //ArrayOfArray
							for ( k = 0; k < unions.length; k++ )
							{
								subj_polygons = unions[k];
								if ( subj_polygons.length == 0 ) //nothing to shade so no DIFFERENCE operation is necessary
									continue;
								var solution_polygons = polyDifference(subj_polygons, clip_polygons, scale);
								line = polys2path(solution_polygons, scale,  xmin, ymin, xdelta, ydelta, x, y, p.x.label, p.y.label);
							
								path = cell.append("path")
										.attr("class", "isoline" + p.i + "" + p.j)
										.attr("fill",  p_cell[i].color)
										.style("stroke-width",2)
										.attr("opacity", 0.2) //original opacity
										.attr("rid", p_cell[i].rid)
										.attr("d", line)
										.attr("pointer-events", "stroke")
										//.hover(mousein, mouseout); this method does not work when clipper is used to create the plots.
										.on("mouseover", mousein)
										.on("mouseout", mouseout);
							}
							/////////////////// end of clipper///////////////////////
						}
						
						//intercecting y axis
						for (var j = 0; j < intercecting_y_axis.length; j++) {
							crosstick = cell.append("line")
							.attr("class", "crosstick isoline" + p.i + "" + p.j)
							.attr("x1", x[p.x.label](intercecting_y_axis[j].x) - padding/10)
							.attr("y1", y[p.y.label](intercecting_y_axis[j].y))
							.attr("x2", x[p.x.label](intercecting_y_axis[j].x) + padding/10)
							.attr("y2", y[p.y.label](intercecting_y_axis[j].y))
							.attr("rid", p_cell[i].rid)
							.on("mouseover", mousein)
							.on("mouseout", mouseout);
							crosstick.datum(p_cell[i]);
							
							crossticktext =	cell.append("text")
							.attr("class", "crossticktext isoline" + p.i + "" + p.j)
							.attr("x", x[p.x.label](intercecting_y_axis[j].x) + intercecting_y_axis[j].delta)
							.attr("y", y[p.y.label](intercecting_y_axis[j].y) + intercecting_y_axis[j].delta/4)
							.attr("text-anchor", "start")
							.attr("rid", p_cell[i].rid)
							.text(intercecting_y_axis[j].y.toPrecision(4));
							crossticktext.datum(p_cell[i]);
						} //end of loop j
						
						//intercecting x axis
						for (var j = 0; j < intercecting_x_axis.length; j++) {
							crosstick = cell.append("line")
							.attr("class", "crosstick isoline" + p.i + "" + p.j)
							.attr("x1", x[p.x.label](intercecting_x_axis[j].x))
							.attr("y1", y[p.y.label](intercecting_x_axis[j].y) - padding/10)
							.attr("x2", x[p.x.label](intercecting_x_axis[j].x))
							.attr("y2", y[p.y.label](intercecting_x_axis[j].y) + padding/10)
							.attr("rid", p_cell[i].rid)
							.on("mouseover", mousein)
							.on("mouseout", mouseout);
							crosstick.datum(p_cell[i]);
							
							crossticktext =	cell.append("text")
							.attr("class", "crossticktext isoline" + p.i + "" + p.j)
							.attr("x", x[p.x.label](intercecting_x_axis[j].x))
							.attr("y", y[p.y.label](intercecting_x_axis[j].y) + intercecting_x_axis[j].delta)
							.attr("text-anchor", "start")
							.attr("rid", p_cell[i].rid)
							.text(intercecting_x_axis[j].x.toPrecision(4));
							crossticktext.datum(p_cell[i]);
							
						} //end of loop j
						
					}
				}
				}
			}
			cell.datum(p);
		} //end of function plotfunction plot(p)
				
		mousein = function (d) {
			mouseinCopied(d.rid);
		}
		mouseout = function (d) {
			mouseoutCopied(d.rid);
		}
		mouseinCopied = function (rid) {
			$("text[rid='" + rid + "']").attr("mover", true);
			$("rect[rid='" + rid + "']").attr("mover", true);
			$("path[rid='" + rid + "']").attr("mover", true);
			$(".crossticktext[rid='" + rid + "']").show();
		}
		mouseoutCopied = function (rid) {
			$("text[rid='" + rid + "']").attr("mover", false);
			$("rect[rid='" + rid + "']").attr("mover", false);
			$("path[rid='" + rid + "']").attr("mover", false);
			$(".crossticktext[rid='" + rid + "']").hide();
		}
		cross = function (a, b) {
			var c = [], n = a.length, m = b.length, i, j;
			for (i = -1; ++i < n; ) {
				for (j = -1; ++j < m; ) {
					c.push({ x: a[i], i: i, y: b[j], j: j });
				}
			}
			return c;
		}
	var svg;
	var x, y;
	
	render2 = function (plot_data) {
		per_cell = plot_data.per_cell;  //ConstraintPlot
		vconfigs = plot_data.vconfigs;
		var n = vconfigs.length;
		// Position scales.
		x = {}, y = {};
		vconfigs.forEach(function (vconfigs, index) {
			variable = vconfigs.label;
			var domain = [vconfigs.min, vconfigs.max],
				range = [padding / 2, size - padding / 2];

			x[variable] = d3.scale.linear()
							.domain(domain)
							.range(range);

			y[variable] = d3.scale.linear()
							.domain(domain)
							.range(range.slice().reverse());
		});

		// Axes. cell
		var axis = d3.svg.axis()
					  .ticks(5)
					  .tickSize(size * n);

		// Brush.
		//            var brush = d3.svg.brush()
		//						  .on("brushstart", brushstart)
		//						  .on("brush", brush)
		//						  .on("brushend", brushend);

		// Root panel.
		svg = d3.select('#'+$(element).attr('id')).append("svg")
					  .attr("width", size * n + padding)
					  .attr("height", size * n + padding);

		// X-axis.
		svg.selectAll("g.x.axis")
					   .data(vconfigs)
						.enter().append("g")
							.attr("class", "x axis")
							.attr("transform", function (d, i) {
								return "translate(" + i * size + ",0)";
							})
							.each(function (d) {
								d3.select(this)
									.call(axis.scale(x[d.label])
									.orient("bottom"));
							});

		// Y-axis.
		svg.selectAll("g.y.axis")
					  .data(vconfigs)
					.enter().append("g")
					  .attr("class", "y axis")
					  .attr("transform", function (d, i) { return "translate(0," + i * size + ")"; })
					  .each(function (d) { d3.select(this).call(axis.scale(y[d.label]).orient("right")); });

		// Cell and plot.
		var cell = svg.selectAll("g.cell")
					.data(cross(vconfigs, vconfigs))
					.enter().append("g")
					 .attr("class", "cell")
					 .attr("id", function (d) {
						  return  d.i + "" + d.j;
					  })
					  .attr("transform", function (d) {
						  return "translate(" + d.i * size + "," + d.j * size + ")";
					  })
					  //.each(plotframe)
					  .each(plot)
					  .each(plot2)
					  .each(createYLines)
					  .each(createXLines);

	    // Titles for the diagonal.
		cell.filter(function (d) { return d.i == d.j; })
			.append("text")
					  .attr("x", padding)
					  .attr("y", padding)
					  .attr("dy", ".71em")
					  .text(function (d) {
						  //								var unit = "";
						  //								if(outputs.Unit[$.inArray(d.x, outputs.variables)] != ""){
						  //									unit = '  [' + outputs.Unit[$.inArray(d.x, outputs.variables)] + ']'
						  //								}
						  //								return outputs.labels[$.inArray(d.x, outputs.variables)] + unit;
						  if (!(d instanceof Array)) {
							 return d.x.label;
						  }

					  });
					  
		// fix negative signs in axis
		$('text').each(function(index,item){item.textContent = item.textContent.replace("−","-");});
	
		/* Mark <vf-table> as rendered */
		$(element).attr('rendered', 'true');
		
	} //end of render2
	
	
	 
	
	padding = 35;
	//--- exposed below to be used in plot and plot2 function 
	//size
	//padding - constant
	//per_cell
	//x
	//y
	/* Render Method */
	var previous_jsondata;
	
	function isJSONChanged(o1, o2){
		return JSON.stringify(o1) !== JSON.stringify(o2);
	}
	//return true --- require all render
	//return false --- not changed
	function isSelectedVariableChanged(old, current) {
		if ( old.userInputs.selectedVariables.length != current.userInputs.selectedVariables.length ){
			return true; //changed
		}
		else { //length is the same
			for ( var i = 0; i < old.userInputs.selectedVariables.length; i++ ) {
				if ( old.userInputs.selectedVariables != current.userInputs.selectedVariables ){
					return true; //changed
				}
			}
		}
		return false;
	}
	function addBackPlots( plot_data ){

		per_cell = plot_data.per_cell;  //ConstraintPlot
		vconfigs = plot_data.vconfigs;
		// Cell and plot.
		var cell = svg.selectAll("g.cell")
					  //.data(cross(vconfigs, vconfigs))
					  .each(plot)
					  .each(plot2)
					  .each(createYLines)
					  .each(createXLines);
	}
	function getOldConstraintPlots( rid )
	{
		var olddata = [];
		svg.selectAll("g.cell rect[rid='"+ rid+ "']").each( 
			function(d){ 
				
				if ( d.copied == false ){ //copy only ConstraintPlot not CConstraintPlot "if (d instanceof ConstraintPlot)" does not work because worker calculated will be just object
					olddata.push(d);
				}
			}
		);
		return	olddata;
	}
	
	function addBackAPlotWithNewData(rid, new_plots, new_jsondata) {
	
		var vconfigs = getVConfigs( new_jsondata.userInputs.selectedVariables, new_plots );
		//debugger;
		//calculate vconfigs so set x and y based on new vconfigs
		x = {}, y = {};
		vconfigs.forEach(function (vconfigs, index) {
			variable = vconfigs.label;
			var domain = [vconfigs.min, vconfigs.max],
				range = [padding / 2, size - padding / 2];

				
			x[variable] = d3.scale.linear()
							.domain(domain)
							.range(range);

			y[variable] = d3.scale.linear()
							.domain(domain)
							.range(range.slice().reverse());
		});
		svg.selectAll("g.cell").data(cross(vconfigs, vconfigs));
		
		
		//var num_of_grids = getNumOfGrids( new_jsondata.fidelity, vconfigs.length);
		//I think vconfigs.length = new_jsondata.userInputs.selectedVariables.length
		var num_of_grids = getNumOfGrids( new_jsondata.fidelity, new_jsondata.userInputs.selectedVariables.length);
		
		//00: null
		//01: Array[1] - ConstraintPlot
		//10: Array[1] - CConstraintPlot
		//11: null
		//var plot_data = getPerCell(vconfigs, new_plots, num_of_grids);
		
		//start worker - "sending to grid calculation worker..."
		var worker = initializeWorker();
		worker.postMessage({'type': 0, 'vconfigs': vconfigs, 'new_plots': new_plots, 'num_of_grids': num_of_grids, 'rid': rid});
	}
	
	//only fidelity is changed- function does not change, only grid data change
	function handleFidelityChange( new_jsondata){
		console.debug("fidelity change");
		
		for ( var j = 0; j < new_jsondata.userInputs.selectedResponses.length; j++ ){
			var new_a_selected_response = new_jsondata.userInputs.selectedResponses[j];
			var new_plots = [];
			var old_ConstraintPlots = getOldConstraintPlots( new_a_selected_response.rid);
			console.debug( "new_a_selected_response.rid: " + new_a_selected_response.rid );
			var new_plot_from_old;
			if ( old_ConstraintPlots.length != 0 )
			{
				//old data exists
				for ( var i = 0; i < old_ConstraintPlots.length; i++ ){
					new_plot_from_old = new Plot_p(null, null, null, null, null, null, old_ConstraintPlots[i]);
					new_plot_from_old.r = getResponse( new_a_selected_response.rid, new_jsondata.responses);
					new_plots.push(new_plot_from_old);
				}
				addBackAPlotWithNewData( new_a_selected_response.rid, new_plots, new_jsondata); 
			}
			else {
				//this is a case when a data is gone (nothing to plot)
				//a,b,c,d, etc... coefficient when userInputs.defaultVariableValues changed.
				//sending worker to 1) function calculate and 2) grid caculate 
				var worker = initializeWorker();
				worker.postMessage({'type': 1, 'jsondata': new_jsondata, 'rid': new_a_selected_response.rid });
			}
		}
	}
	
	//same as handleAResponseEqualityChange except one line
	function handleAResponseValueChange( old_selected_responses_i, new_a_selected_response, new_jsondata )
	{
		var new_plots = [];
		var old_ConstraintPlots = getOldConstraintPlots( new_a_selected_response.rid);
	
		var new_plot_from_old;
		if ( old_ConstraintPlots.length != 0 )
		{
			for ( var i = 0; i < old_ConstraintPlots.length; i++ )
			{
				new_plot_from_old = new Plot_p(null, null, null, null, null, null, old_ConstraintPlots[i]);
				new_plot_from_old.r = getResponse( new_a_selected_response.rid, new_jsondata.responses);
				//change value --- different
				new_plot_from_old.z = new_a_selected_response.value; //////////////////////////////////// only this write is different
				new_plots.push(new_plot_from_old);
			}
			addBackAPlotWithNewData( new_a_selected_response.rid, new_plots, new_jsondata);
		}
		else {
			//this is a case when a data is gone (nothing to plot)
			//a,b,c,d, etc... coefficient when userInputs.defaultVariableValues changed.
			
			//sending worker to 1) function calculate and 2) grid caculate 
			var worker = initializeWorker();
			worker.postMessage({'type': 1, 'jsondata': new_jsondata, 'rid': new_a_selected_response.rid });
		}
	}
	
	//same as handleAResponseValueChange except one line
	function handleAResponseEqualityChange( old_selected_responses_i, new_a_selected_response, new_jsondata ){
		
		var new_plots = [];
		var old_ConstraintPlots = getOldConstraintPlots( new_a_selected_response.rid);
		
		var new_plot_from_old;
		if ( old_ConstraintPlots.length != 0 )
		{
			for ( var i = 0; i < old_ConstraintPlots.length; i++ )
			{
				new_plot_from_old = new Plot_p(null, null, null, null, null, null, old_ConstraintPlots[i]);
				new_plot_from_old.r = getResponse( new_a_selected_response.rid, new_jsondata.responses);
				//change inequality --- different
				new_plot_from_old.inequality = new_a_selected_response.inequality; //////////////////////////////////// only this write is different
				new_plots.push(new_plot_from_old);
			}
			addBackAPlotWithNewData( new_a_selected_response.rid, new_plots, new_jsondata);
		}
		else {
			//this is a case when a data is gone (nothing to plot)
			//a,b,c,d, etc... coefficient when userInputs.defaultVariableValues changed.
			//sending worker to 1) function calculate and 2) grid caculate 
			var worker = initializeWorker();
			worker.postMessage({'type': 1, 'jsondata': new_jsondata, 'rid': new_a_selected_response.rid });
		}
	}
	
	function handleAResponseColorChange( rid, newColor)
	{
		svg.selectAll("g.cell path[rid='"+ rid+ "']")
			.each( 	function(){ this.setAttribute("style", "fill: " + newColor);	});
		svg.selectAll("g.cell rect[rid='"+ rid+ "']") //no rid in frame
			.each( 	function(){ this.setAttribute("style", "fill: " + newColor);	});	
			
	}

	function removeAResponse(rid)
	{
		//var olddata;
		//console.debug("removing old rid = " + rid);
		svg.selectAll("g.cell path[rid='"+ rid+ "']").each( 
			function(d){ this.remove();}
			);
		
		svg.selectAll("g.cell line[rid='"+ rid+ "']").each( 
			function(d){ this.remove();}
			);
		
		//rect and text label - not removed but style.display is set to none, the it will not show up.
		svg.selectAll("g.cell .labeltext[rid='"+ rid+ "']").each( 
			function(d){ 
				 this.style.display="none";
				}
			)
		svg.selectAll("g.cell .crossticktext[rid='"+ rid+ "']").each( 
			function(d){
				this.remove(); //removing data
				}
			);
	}
	
	function getAddedSelectedResponsed( new_jsondata ){
		
		var added = [];
		var isAdded;
		for ( var i = 0; i < new_jsondata.userInputs.selectedResponses.length; i++ ){
			isAdded = true;
			for ( var j = 0; j < previous_jsondata.userInputs.selectedResponses.length; j++ ){
				if ( new_jsondata.userInputs.selectedResponses[i].rid == previous_jsondata.userInputs.selectedResponses[j].rid ){
					isAdded = false;
					break; //find the response in in previous_jsondata
				}
			}
			if ( isAdded )
				added.push(new_jsondata.userInputs.selectedResponses[i])
		}
		return added;
	}
	function getRemovedSelectedResponsed( new_jsondata ){
		
		var removed = [];
		var isRemoved;
		for ( var i = 0; i < previous_jsondata.userInputs.selectedResponses.length; i++ ){
			isRemoved = true;
			for ( var j = 0; j < new_jsondata.userInputs.selectedResponses.length; j++ ){
				if ( new_jsondata.userInputs.selectedResponses[j].rid == previous_jsondata.userInputs.selectedResponses[i].rid ){
					isRemoved = false;
					break; //find the response in in previous_jsondata
				}
			}
			if ( isRemoved )
				removed.push(previous_jsondata.userInputs.selectedResponses[i])
		}
		return removed;
	}
	function isAnyDefaultVariableValuesChanged( new_jsondata){
		
		for (var i = 0; i < new_jsondata.userInputs.selectedVariables.length; i++ ){
			if (getDefault(new_jsondata.userInputs.selectedVariables[i], previous_jsondata.userInputs.defaultVariableValues) != 
				getDefault(new_jsondata.userInputs.selectedVariables[i], new_jsondata.userInputs.defaultVariableValues)) //default unchanged
				return true;
		}
		return false;
	}
	//
	function initializeWorker(){
		
		var worker = new Worker('./inc/vf-widgets/vf-constraintplot_worker.js');
		worker.addEventListener('message', function(e) {
				 //define how to handle what coming back form workier
				if (e.data.type == 0 ){ //type = 0 ----- plot
					//console.debug("reply0: " + e.data.type);
					var plot_data = e.data.result;
					 if ( plot_data != null ){
						if ( svg == null)
							render2(plot_data);
						else { 
							if ( e.data.rid != null)
								removeAResponse(e.data.rid);
							addBackPlots( plot_data);
						}
					 }
				 }
				 else { //type == 1  ----- calculate -> plot
					//console.debug("reply1: " + e.data.type);
					var new_plots = e.data.result;
					//console.debug( new_plots);
					if ( new_plots != null ){
						addBackAPlotWithNewData(e.data.rid, new_plots, jsondata); //calling worker 0
					}
				}
			  }, false);
	    //adding below start making work when leaving tub and coming back to this tub
		$(element).attr('rendered', 'true');	  
		return worker;	  
	}
	
	//this method is called 1st time to plot
	createPlot = function (jsondata, selected_responses, selected_variables)
	{
		//var responses = jsondata.responses;
		//find allplots_f = functions to plot (a response function with 3 variables create 3 graphs - each variable being constant)
		var allplots_f = [];
		for (var kk = 0; kk < selected_responses.length; kk++) {
			var json_a_response = getResponse(selected_responses[kk].rid, jsondata.responses);
			if (json_a_response == null)
				continue;
			var numinputs = json_a_response.inputs.length;
			for (var ii = 1; ii < numinputs; ii++) {
				for (var jj = 2; jj < numinputs + 1; jj++) {
					if (ii < jj) {
						p = new Plot_p(ii, jj, json_a_response, selected_responses[kk], jsondata.userInputs.defaultVariableValues, selected_variables, null);
						if (p.r != undefined)  //p.r is undefined if x or y is not selected variable
							allplots_f.push(p);
					}
				}
			}
		}
		var vconfigs = getVConfigs( selected_variables, allplots_f );
		//num of grids to calculate target value
		var num_of_grids = getNumOfGrids( jsondata.fidelity, vconfigs.length);

		//sending grid calculation worker
		var worker = initializeWorker(); 
		worker.postMessage({'type': 0, 'vconfigs': vconfigs, 'new_plots': allplots_f, 'num_of_grids': num_of_grids, 'rid': null});
	}
	this.render = function(){
		if (($(element).attr('rendered') == undefined || $(element).attr('rendered') == "false")) {
			var jsondata = eval($(element).attr('data'));
			//console.debug( "render is called");
			console.debug(jsondata);
			var selected_variables = jsondata.userInputs.selectedVariables;
			if ( selected_variables.length < 2 ){
				$(element).attr('rendered', 'true');
				return;
			}
			var base_width = $(element).parent().width() - padding;
			size  = (base_width / selected_variables.length) - 1.5;
			
			//
			
			var selected_responses = jsondata.userInputs.selectedResponses;
			createPlot(jsondata, selected_responses, selected_variables);
			previous_jsondata = jsondata;
		}
	}
	
	
	


	
	/* Rerender Method */
	this.rerender = function () {
		//console.debug("rerender is called...");
		//console.debug("update Tab size is called...");
		updateTabSize();
		
		$(this.element).empty();
		$(element).attr('rendered', 'false');
		svg = null;
		this.render();
	}
	/* Update Method */
	this.update = function () {
	
		jsondata = eval($(element).attr('data'));
		console.debug("update is called");
		/*console.debug("=============new===============");
		console.debug(jsondata);
		console.debug("=============old===============");
		console.debug(previous_jsondata);
		*/

		if (!isJSONChanged(jsondata, previous_jsondata)){ 
			$(element).attr('rendered', 'true'); 
			return; //not changed so return
		}

		var selected_variables = jsondata.userInputs.selectedVariables;
		if ( selected_variables.length < 2 ){
			$(element).attr('rendered', 'true');
			//console.debug( "# of selected variables 0");
			return; //can't plot anything
		}
			
		var selected_responses = jsondata.userInputs.selectedResponses;
		//if ( selected_responses.length == 0 ){
		//	console.debug( "# of selected responses 0"); //no response but frame can be printed
		//}
		
		var base_width = $(element).parent().width() - padding;
		size  = (base_width / selected_variables.length) - 1.5;
		//console.debug("base_width: " + base_width + " size: " + size );
		
		
		if ( previous_jsondata != undefined && svg != undefined) {
			
			//case 1) fidelity changed
			if ( previous_jsondata.fidelity !== jsondata.fidelity)
				handleFidelityChange(jsondata);
			else {
				if ( previous_jsondata.userInputs.selectedVariables.length != jsondata.userInputs.selectedVariables.length ){
					//TODO: improve instead of replotting?
					//check if the information is the same
					//1. what is added or removed?
					//1.a if all remaining default values are unchanged -> just remove the plots
					//1.b if all renmaining default values are changed -> recalculucation required.
					this.rerender();
				}
				else { //# of variables are the same as before
					//if any default variable value is changed replot back
					if (isAnyDefaultVariableValuesChanged(jsondata)){
						console.debug( "default value of one of variable is changed so recalculate and replot...");
						for ( var i = 0; i < jsondata.userInputs.selectedResponses.length; i++ ){
							//console.debug("replotting for " + jsondata.userInputs.selectedResponses[i].rid );
							var worker =initializeWorker();
							worker.postMessage({'type': 1, 'jsondata': jsondata, 'rid': jsondata.userInputs.selectedResponses[i].rid });
						}
					}
					//number of selectedResponses 
					else if (previous_jsondata.userInputs.selectedResponses.length == jsondata.userInputs.selectedResponses.length ){

						var allResponsesOrdersAreSame = true;
						for ( var i = 0; i <  jsondata.userInputs.selectedResponses.length; i++ ){
							if (jsondata.userInputs.selectedResponses[i].rid !== previous_jsondata.userInputs.selectedResponses[i].rid){
								allResponsesOrdersAreSame = false;
								break;
							}
						}
						if ( allResponsesOrdersAreSame ){
							//case 2) a response's value, color, equality changed
							for ( var i = 0; i <  jsondata.userInputs.selectedResponses.length; i++ )
							{
								if (jsondata.userInputs.selectedResponses[i].color !== previous_jsondata.userInputs.selectedResponses[i].color){
									//console.debug("color changing " + jsondata.userInputs.selectedResponses[i].rid + " " + jsondata.userInputs.selectedResponses[i].color);
									handleAResponseColorChange (jsondata.userInputs.selectedResponses[i].rid, jsondata.userInputs.selectedResponses[i].color);
									break;
								}
								else if (jsondata.userInputs.selectedResponses[i].value !== previous_jsondata.userInputs.selectedResponses[i].value){
									handleAResponseValueChange(previous_jsondata.userInputs.selectedResponses[i], jsondata.userInputs.selectedResponses[i], jsondata);
									break;
								}
								if (jsondata.userInputs.selectedResponses[i].inequality !== previous_jsondata.userInputs.selectedResponses[i].inequality){
									handleAResponseEqualityChange(previous_jsondata.userInputs.selectedResponses[i], jsondata.userInputs.selectedResponses[i], jsondata);
									break;
								}
							}
						}
						else { //Order of selectedResponses are not the same (i.e, old = 1, 2, 3, new = 1, 3, 4) 
							 //This may not possible by a user action 
							 console.error("Not supported when responses in different order.");
						}
					}
					//case 6) "Show None" is pressed (# of selected responses decresed) //above function for checking the number of responses handle and never reach to this point
					else if ( jsondata.userInputs.selectedResponses.length == 0 ){
						//console.debug("Show None in Responses are selected");
						for ( var i = 0; i <  previous_jsondata.userInputs.selectedResponses.length; i++ ){
							removeAResponse(  previous_jsondata.userInputs.selectedResponses[i].rid);
						}
					}
					//case 3) a response is unselected 
					else if (previous_jsondata.userInputs.selectedResponses.length > jsondata.userInputs.selectedResponses.length ){
						//console.debug( "case 3) a response is unselected" );
						var removed = getRemovedSelectedResponsed(jsondata);
						//console.debug( removed);
						for ( var i = 0; i < removed.length; i++ ){
							//console.debug("removed a plot for " + removed[i].rid );
							removeAResponse(  removed[i].rid);
						}
					}
					//case 4) a response is added or "Show All" is selected
					else if (previous_jsondata.userInputs.selectedResponses.length < jsondata.userInputs.selectedResponses.length ){
						/*if ( jsondata.userInputs.selectedResponses.length == previous_jsondata.userInputs.selectedResponses.length + 1 )
							console.debug( "case 4) a response is selected");
						else
							console.debug("case 5) Show All is selected");
						*/
						
											
						
						var added = getAddedSelectedResponsed(jsondata);
						console.debug( added);
					
						for ( var i = 0; i < added.length; i++ ){
							//console.debug("adding a plot for " + added[i].rid );
							var worker = initializeWorker();
							worker.postMessage({'type': 1, 'jsondata': jsondata, 'rid': added[i].rid });
						}
					}
				}
				//only one response color, value, equality is changed handled by below
				previous_jsondata = jsondata;
			}
		}
		else //previous_joson is null
			this.rerender();
		}
	
/*
//Grade 0-9 makes one hole
//Grade 0-7 makes a hole touching outer bound.		
	var jsondatax	=
{   
"fidelity": "low",	
"userInputs":
		{
		   "selectedResponses":
		   [
			   {
				   "rid": "r1",
				   "value" : "2318.96967713246",
				   "inequality": ">=",
				   "color" : "#1f77b4"
			   }
			 
		   ],
		   "selectedVariables":[ "Crr", "Grade"],
		  
		   "defaultVariableValues":
		   [
			   {
					"label": "Crr",
					"defaultValue": 0.01
			   },
			   {
				   "label": "Grade",
				   "defaultValue": 4
			   }
			  
		   ]//end

		}//end user input
		 ,

		"responses": [
				{
					"function": "17.13030865-18289.1764903*Crr+91.0721511706*Grade+0.0101519661279*VehicleMass+-484464.411886*Crr*Crr+-7.17693021898*Grade*Grade+-2.75649654213e-07*VehicleMass*VehicleMass+290.31427856*Crr*Grade+0.904684617262*Crr*VehicleMass+-0.000376526814627*Grade*VehicleMass",
					"inputs": [
						{
							"max": 0.015, 
							"label": "Crr", 
							"min": -0.005
						}, 
						{
							"max": 9, 
							"label": "Grade", 
							"min": 0
						}
					], 
					"surrogateType": "Response Surface", 
					"min": 2054.5630194034056, 
					"max": 2318.9696771324598, 
					"metric": "TorqueThruDriveShaft", 
					"rid": "r1", 
					"coefficients": {
						"b0":  2098.648427959366,
						"b1": 4327.938941225122,
						"b2": 81.65898080496588,
						"b11": -484464.4118859415,
						"b22": -7.1769302189811714,
						"b12": 290.31427855961044
					}
				}
			]
		};
var jsondatay =
{   
"fidelity": "low",	
"userInputs":
		{
		   "selectedResponses":
		   [
			   {
				   "rid": "r1",
				   "value" : "2313.68154397788",
				   "inequality": ">=",
				   "color" : "#1f77b4"
			   }
			 
		   ],
		   "selectedVariables":[ "Crr", "Grade"],
		  
		   "defaultVariableValues":
		   [
			   {
					"label": "Crr",
					"defaultValue": 0.01
			   },
			   {
				   "label": "Grade",
				   "defaultValue": 4
			   }
			  
		   ]//end

		}//end user input
		 ,

		"responses": [
				{
					"function": "",
					"inputs": [
						{
							"max": 0.016, 
							"label": "Crr", 
							"min": -0.0001
						}, 
						{
							"max": 7.5, 
							"label": "Grade", 
							"min": 3.8
						}
					], 
					"surrogateType": "Response Surface", 
					"min": 2054.5630194034056, 
					"max": 2318.9696771324598, 
					"metric": "TorqueThruDriveShaft", 
					"rid": "r1", 
					"coefficients": {
						"b0":  2087.849326163504,
						"b1": 6589.650484379083,
						"b2": 80.71766376839791,
						"b11": -484464.4118859415,
						"b22": -7.1769302189811714,
						"b12": 290.31427855961044 
					}
				}
			]
		};
	*/	
} //end of vf-constraint_plot