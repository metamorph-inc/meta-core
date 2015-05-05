/*///////////// vf-mvplot /////////////*/
function vf_mvplot(element){
	
	var self = this;
	this.data = eval($(element).attr('data'));
	this.element = element;
	
	
	
	/* Render Method */
	this.render = function () {
		try{
		/* 
		Check if element has already been rendered 
		if it has then skip it
		if it has not render it
		*/
		
		if (!($(element).attr('rendered') == undefined || $(element).attr('rendered') == "false")) {
			return 'vf-mvplot: already rendered';
		}
		
			var outputs = eval($(this.element).attr('data'));
			
			//console.log("!!!!!!!!!!!!!!!output!!!!!!!!!!!");
			//console.log(outputs);
			
			// Size parameters.
			var n = outputs.variables.length;
			var padding = 35;
			var size = (($(element).parent().width() - (padding)) / n);
			var margin = {"right" : 20,
						  "left" : 0,
						  "top" : 0,
						  "bottom" : 20};
			
			var minMax = {};

			outputs.variables.forEach(function(d, i){
				minMax[d] = {};
				
				//initialize minTest and maxTest variables
				var minTest = outputs.values[0][d];
				var maxTest = outputs.values[0][d];
				
				for (var j = 0; j < outputs.values.length; j++)
				{
					//test to see if minTest is still the minimum
					if (minTest > outputs.values[j][d])
					{
						minTest = outputs.values[j][d]
					}
					
					//tests to see if maxTest is still the maximum
					if (maxTest < outputs.values[j][d])
					{
						maxTest = outputs.values[j][d]
					}
					
					//test to see if both thresholds and objectives are defined
					if (!(outputs.thresholds[d] == undefined) && !(outputs.objectives[d] == undefined))
					{
						minMax[d].min = Math.min(minTest, outputs.thresholds[d], outputs.objectives[d]);
						minMax[d].max = Math.max(maxTest, outputs.thresholds[d], outputs.objectives[d]);						
					}
					//test to see if only objectives are defined
					else if (outputs.thresholds[d] == undefined  && !(outputs.objectives[d] == undefined))
					{
						minMax[d].min = Math.min(minTest, outputs.objectives[d]);
						minMax[d].max = Math.max(maxTest, outputs.objectives[d]);	
					}
					//test to see if only thresholds are defined
					else if (!(outputs.thresholds[d] == undefined) && outputs.objectives[d] == undefined)
					{
						minMax[d].min = Math.min(minTest, outputs.thresholds[d]);
						minMax[d].max = Math.max(maxTest, outputs.thresholds[d]);	
					}
					//test to see if 
					else if (outputs.thresholds[d] == undefined && outputs.objectives[d] == undefined)
					{
						minMax[d].min = minTest;
						minMax[d].max = maxTest;	
					}
				}
			});
				
			var range = {};
			var minimum = {};
			var maximum = {};
			
			var percentScaling = 0.1;
			
			//make each minMax a bit bigger or smaller to better plot the data set
			outputs.variables.forEach(function(d, i){
				range[d] = Math.abs(minMax[d].max - minMax[d].min);
			
				minimum[d] = minMax[d].min - range[d] * percentScaling;
				maximum[d] = minMax[d].max + range[d] * percentScaling;
				
				//what if both the min and the max were equal?  Give it some "range" about the point
				if (minMax[d].max == minMax[d].min)
				{
					maximum[d] = minMax[d].max * (percentScaling + 1);
					minimum[d] = minMax[d].max * (1 - percentScaling)
				}
				
				//what if the only value for a variable is zero? Give it some "range" about zero
				if (minMax[d].max == 0 && minMax[d].min == 0)
				{
					maximum[d] = percentScaling;
					minimum[d] = -percentScaling;
				}
			});
			
			
			/* if n is zero do nothing */
					if(n != 0){
						// Position scales.
						var x = {}, y = {};
						
						outputs.variables.forEach(function(variable, index) 
						{
							//if (index == 0)
							//{
								var value = function(d) { 
									return +d[variable]; 
								};
								var domain = [minimum[variable], maximum[variable]],
									range = [padding / 2, size - padding / 2];
							//}
							//else if (index == outputs.variables.length)
							//{
							
							//}
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
						var brush = d3.svg.brush()
						  .on("brushstart", brushstart)
						  .on("brush", brush)
						  .on("brushend", brushend);

						// Root panel.
						var svg = d3.select('#'+$(element).attr('id')).append("svg")
						  .attr("width", size * n + padding)
						  .attr("height", size * n + padding);

						// X-axis.
						svg.selectAll("g.x.axis")
							.data(outputs.variables)
							.enter().append("g")
								.attr("class", "x axis")
								.attr("transform", function(d, i) {
									return "translate(" + i * size + ",0)";
								})
								.each(function(d) {
									d3.select(this).call(axis.scale(x[d])
										.orient("bottom")); 
								});

						// Y-axis.
						svg.selectAll("g.y.axis")
						  .data(outputs.variables)
						.enter().append("g")
						  .attr("class", "y axis")
						  .attr("transform", function(d, i) { return "translate(0," + i * size + ")"; })
						  .each(function(d) { d3.select(this).call(axis.scale(y[d]).orient("right")); });

						// Cell and plot.
						var cell = svg.selectAll("g.cell")
						  .data(cross(outputs.variables, outputs.variables))
						.enter().append("g")
						  .attr("class", "cell")
						  .attr("transform", function(d) { 
							return "translate(" + d.i * size + "," + d.j * size + ")"; 
						  })
						  .each(plot);

						// Titles for the diagonal. Add the label for the variable names
						cell.filter(function(d) { 
							return d.i == d.j; 
						}).append("text")
						  .attr("x", padding)
						  .attr("y", padding)
						  .attr("dy", ".71em")
						  .text(function(d, i) {
								var unit = "";
								if(outputs.Unit[$.inArray(d.x, outputs.variables)] != ""){
									unit = '  [' + outputs.Unit[$.inArray(d.x, outputs.variables)] + ']'
								}
								return outputs.labels[$.inArray(d.x, outputs.variables)] + unit;
						  });
						
						//add the label for the testbench names
						cell.filter(function(d) { 
							return d.i == d.j; 
						}).append("text")
						  .attr("x", padding)
						  .attr("y", padding + 10)
						  .attr("dy", ".71em")
						  .text(function(d, i) {
								return outputs.testBenchs[i];
						  });
						 
						  // fix negative signs in axis
						$('text').each(function(index,item){item.textContent = item.textContent.replace("−","-");});
				
			}
						function plot(p) {
							var cell = d3.select(this);

							// Plot frame.
							cell.append("rect")
								.attr("class", "frame")
								.attr("x", padding / 2)
								.attr("y", padding / 2)
								.attr("width", size - padding)
								.attr("height", size - padding);
							
							// Plot brush.
							cell.call(
								brush.x(x[p.x]).y(y[p.y])
							);
							
							cell.on("mousedown", function(d,i) {
								if ($("#contextmenu .dropdown-menu").css("display") != "none" && $(event.target).attr("config") == undefined)
								{
									$("#contextmenu .dropdown-toggle").dropdown("toggle");
								}
							});
							
							// Plot dots.
							cell.selectAll("circle")
								.data(outputs.values)
								.enter().append("circle")
								.on("mouseover", function(d) {
									cell.append("text")
										.attr("class", "designID")
										.attr("x", function(){
															return x[p.x](d[p.x]) + 5;
														}) //mouse location (x) minus margin
										.attr("y", function(){
															return y[p.y](d[p.y]) - 5;
														}) //mouse location (y) minus margin and some padding for better text display
										.attr("fill", "white")
										.text(d.design);
								}) 
								.on("mouseout", function(d) {							
									//remove the text element added on the mouseover event when the mouseout event is triggered
									$("text.designID").remove(); 
								}) 
								.on("mousedown", function(d, i) {
									event.stopPropagation(); //stop brushing events
									if (event.ctrlKey)
									{
										event.stopPropagation(); //stop brushing events
										
										$("#contextmenu").css({
											position:'absolute',
											top: event.pageY - 1 ,
											left: event.pageX - 1,
											zIndex:5000
										});

										$("#contextmenu").attr("config", d.design);

										$("#contextmenu .dropdown-toggle").dropdown("toggle");
									}
									else
									{
										if ($("#contextmenu .dropdown-menu").css("display") != "none")
										{
											$("#contextmenu .dropdown-toggle").dropdown("toggle");
										}
									}
									
								})
								.on("click", function(d){
									$(document).trigger($(element).attr('id') + '_clicked', [event, d.design]);	
								})
								//.attr("class", "selected")
								.attr("cx", function(d) {
									return x[p.x](d[p.x]); 
								})
								.attr("cy", function(d) {
									return y[p.y](d[p.y]); 
								})
								.attr("class", function(d,i) {
									var string = "design " + d.cid + " selected";
									
									// if(outputs.violations[i])
									// {
										// return string + " violation";
									// }
									// else
									// {
										return string;
									//}
								})
								.attr("cid", function(d,i){
									return d.cid;
								})
								.attr("config", function(d,i){
									return d.design;
								})
								.style("z-index", 4999)
								.attr("r", 3)
								.attr("fill", function(d,i){
									var color = outputs.color[i];
									
									if (!color || 0 === color.length)
									{
										return "#1f77b4";
									}
									else
									{
										return color;
									}
								});
							
							//Veritcal and Horizontal line thresholds... MJD						
							cell.append("g")
								.attr("class", "threshold vertical") //This is the class for the vertical line used by the 2Dscatter plot
								.append("line")
									.attr("x1", function(d) { 
										if (outputs.thresholds[p.x] == undefined)
										{
											return 0;
										}
										else
										{
											return x[p.x](outputs.thresholds[p.x]); 
										}
									})
									.attr("y1", padding / 2)
									.attr("x2", function(d){ 
										if (outputs.thresholds[p.x] == undefined)
										{
											return 0;
										}
										else
										{
											return x[p.x](outputs.thresholds[p.x]); 
										}
									})
									.attr("y2", size - padding / 2)
									.style("visibility", function(d){ 
										if (outputs.thresholds[p.x] == undefined)
										{
											return "hidden";
										}
										else
										{
											return "";
										}
									})
									.on("mouseover", function(d) {
										svg.append("text")
											.text(outputs.thresholds[p.x])
											.attr("class", "objectiveLabel")
											.attr("x", function(){
													return event.pageX - padding / 2 - 3;
												}) //mouse location (x) minus margin
											.attr("y", function(){
													return event.pageY - padding / 2 + 3 - this.offsetTop;
												}) //mouse location (y) minus margin and some padding for better text display
											.attr("text-anchor", "start");
											//.attr("fill", "green");
									})
									.on("mouseout", function() {
										$('text.objectiveLabel').remove();
									});
								
							cell.append("g")
								.attr("class", "threshold horizontal") //This is the class for the horizontal line used by the 2Dscatter plot
								.append("line")
									.attr("x1", padding / 2)
									.attr("y1", function(p) { 
										if (outputs.thresholds[p.y] == undefined)
										{
											return 0;
										}
										else
										{
											return y[p.y](outputs.thresholds[p.y]); 
										}
									})
									.attr("x2", size - padding / 2)
									.attr("y2", function(p) { 
										if (outputs.thresholds[p.y] == undefined)
										{
											return 0;
										}
										else
										{
											return y[p.y](outputs.thresholds[p.y]); 
										}
									})
									.style("visibility", function(p){ 
										if (outputs.thresholds[p.y] == undefined)
										{
											return "hidden";
										}
										else
										{
											return "";
										}
									})
									.on("mouseover", function(d) {
										svg.append("text")
											.text(outputs.thresholds[p.y])
											.attr("class", "objectiveLabel")
											.attr("x", function(){
													return event.pageX - padding;
												}) //mouse location (x) minus margin
											.attr("y", function(){
													return event.pageY - padding + 5 - this.offsetTop;
												}) //mouse location (y) minus margin and some padding for better text display
											.attr("text-anchor", "middle")
											.attr("fill", "green");
									})
									.on("mouseout", function(d) {
										$('text.objectiveLabel').remove();
									});								
															
							//Veritcal and Horizontal line objective... MJD						
							cell.append("g")
								.attr("class", "objective vertical") //This is the class for the vertical line used by the 2Dscatter plot
								.append("line")
									.attr("x1", function(p) { 
										if (outputs.objectives[p.x] == undefined)
										{
											return 0;
										}
										else
										{
											return x[p.x](outputs.objectives[p.x]); 
										}
									})
									.attr("y1", padding / 2)
									.attr("x2", function(p){ 
										if (outputs.objectives[p.x] == undefined)
										{
											return 0;
										}
										else
										{
											return x[p.x](outputs.objectives[p.x]); 
										}
									})
									.attr("y2", size - padding / 2)
									.style("visibility", function(p){ 
										if (outputs.objectives[p.x] == undefined)
										{
											return "hidden";
										}
										else
										{
											return "";
										}
									})
									.on("mouseover", function(d) {
										svg.append("text")
											.text(outputs.objectives[p.x])
											.attr("class", "objectiveLabel")
											.attr("x", function(){
													return event.pageX - padding / 2 - 3;
												}) //mouse location (x) minus margin
											.attr("y", function(){
													return event.pageY - padding / 2 + 3 - this.offsetTop;
												}) //mouse location (y) minus margin and some padding for better text display
											.attr("text-anchor", "start")
											.attr("fill", "green");
									})
									.on("mouseout", function(d) {
										$('text.objectiveLabel').remove();
									});
								
							cell.append("g")
								.attr("class", "objective horizontal") //This is the class for the horizontal line used by the 2Dscatter plot
								.append("line")
									.attr("x1", padding / 2)
									.attr("y1", function(p) { 
										if (outputs.objectives[p.y] == undefined)
										{
											return 0;
										}
										else
										{
											return y[p.y](outputs.objectives[p.y]); 
										}
									})
									.attr("x2", size - padding / 2)
									.attr("y2", function(p){ 
										if (outputs.objectives[p.y] == undefined)
										{
											return 0;
										}
										else
										{
											return y[p.y](outputs.objectives[p.y]); 
										}
									})
									.style("visibility", function(p){ 
										if (outputs.objectives[p.y] == undefined)
										{
											return "hidden";
										}
										else
										{
											return "";
										}
									})
									.on("mouseover", function(d) {
										svg.append("text")
											.text(outputs.objectives[p.y])
											.attr("class", "objectiveLabel")
											.attr("x", function(){
													return event.pageX - padding;
												}) //mouse location (x) minus margin
											.attr("y", function(){
													return event.pageY - padding + 5 - this.offsetTop;
												}) //mouse location (y) minus margin and some padding for better text display
											.attr("text-anchor", "middle")
											.attr("fill", "green");
									})
									.on("mouseout", function(d) {
										$('text.objectiveLabel').remove();
									});
							
							//rotate all the X axis values -45 deg
							d3.selectAll("#" + $(element).attr("id") + " .x.axis text").attr("transform", function(){
								return "rotate(-45,0," + $(this).attr("y") + ")";
							});
						}

						// Clear the previously-active brush, if any.
						function brushstart(p) 
						{
							if(!event.ctrlKey)
							{
								if (brush.outputs !== p) 
								{
									cell.call(brush.clear());
									brush.x(x[p.x]).y(y[p.y]).outputs = p;

									$(".design").addClass("selected"); //reselect all points if brush is cleared
								}
							}
						}

						// Highlight only the brushed circles (conversely, deselect all non-brushed circles)
						function brush(p) 
						{
							var e = brush.extent();
							svg.selectAll("circle").classed("selected", function(d) {							
							  return e[0][0] <= d[p.x] && d[p.x] <= e[1][0]
								  && e[0][1] <= d[p.y] && d[p.y] <= e[1][1];
								  //? "selected" : null
							}); 
							
							//globals
							//list of cids selected and not selected
							cidSelected = [];
							cidNotSelected = [];
								
							svg.selectAll("circle").classed("selected", function(d) {
								if (e[0][0] <= d[p.x] && d[p.x] <= e[1][0]    && e[0][1] <= d[p.y] && d[p.y] <= e[1][1])
								{
									//link pplot to mvplot brushing
									if (jQuery.inArray($(this).attr("class").split(" ")[1], cidSelected) == -1)
									{
										cidSelected.push($(this).attr("class").split(" ")[1]);
									}

									//add the selected class from the mvplot element
									return true;
								}
								else
								{
									//link pplot to mvplot brushing
									if (jQuery.inArray($(this).attr("class").split(" ")[1], cidNotSelected) == -1)
									{
										cidNotSelected.push($(this).attr("class").split(" ")[1]);
									}
									//$("#pplot ." + cidSelected).removeClass("selected");

									//remove the selected class from the mvplot element
									return false;
								}
							});

							d3.selectAll("#pplot path.design").classed("selected", false);

							if (!(cidSelected.length == 0))
							{
								for (var i = 0; i < cidSelected.length; i++)
								{
									d3.selectAll("#pplot path." + cidSelected[i]).classed("selected", true);
								}
							}
							
							//IF THE BRUSH IS CLEARED (ALL MVPLOT SELECTED), RESET PPLOT, TOO
							if( (e[0][0] - e[1][0] == 0) && (e[0][1] - e[1][1] == 0))
							{
								d3.selectAll("#pplot path.design").classed("selected", true);
								
								$("#pplot path.design").each(function(i){
									cidSelected.push($(this).attr("class").split(" ")[1]);
								});
								
								cidNotSelected = [];
								
								SelectAll(outputs);
								return;
							}
							
							SelectedList($(element).attr("id"), cidSelected, cidNotSelected);
						}
						//SelectedList($(element).attr("id"));
						
						function SelectedList(id, cidSelected, cidNotSelected){							
							//trigger the selectionChange event defined in vf-events.js
							$(document).trigger('selectionChange', {
								"widget" : id,
								"eventForWidget" : ["pplot", "dsatable"],
								"data" : outputs,
								"selected" : cidSelected,
								"notSelected" : cidNotSelected
							});
						}
						
						//select all configurations
						function SelectAll(outputs){
							$(document).trigger('selectAll', outputs);
						}

						// If the brush is empty, select all circles.
						function brushend() 
						{
							if (brush.empty()) 
							{
								//if already selected, leave it selected. if not selected, select it.
								svg.selectAll("circle").classed("selected", true);
							}
						}
						
						function cross(a, b) 
						{
							var c = [], n = a.length, m = b.length, i, j;
							for (i = -1; ++i < n;) 
							{
								for (j = -1; ++j < m;) 
								{
									c.push({x: a[i], i: i, y: b[j], j: j});
								}
							}
							return c;
						}	
			
			/* Mark <vf-table> as rendered */
			$(element).attr('rendered', 'true');
			
			}
			catch(err){
				$(element).attr('rendered', 'true');
			}
		
	}
	
	/* Rerender Method */
	this.rerender = function () {
		updateTabSize();
		$(this.element).empty();
		$(element).attr('rendered', 'false');
		this.render();
	}
	
	/* Update Method */
	this.update = function () {
		this.rerender();
	}
}
