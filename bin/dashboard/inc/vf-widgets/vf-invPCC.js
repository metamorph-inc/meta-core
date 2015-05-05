/*///////////// vf-invPCC /////////////*/
function vf_invPCC(element) {
	var self = this;
	this.data = eval($(element).attr('data'));
	this.element = element;
	
	/* Render Method */
	this.render = function () {
			
		/* 
		Check if element has already been rendered 
		if it has then skip it
		if it has not render it
		*/
		try
		{
			if (($(element).attr('rendered') == undefined || $(element).attr('rendered') == "false")) {
				this.data = eval($(this.element).attr('data'));
				
				/* ============================== PROFILER CODE ================================*/
				// /*
				
				var distributionLines = [];
				var w = $(element).parent().width(),
					plot_x = 60,
					plot_x_margin = 15,
					plot_y = 60;

					root = this.data.profilerData;
					console.log("inverse");
					console.log(this.data);
					
					var lPad = 50,
						rPad = 100,
						tPad = 50,
						bPad = 50,
						plotPad = 0;

					var outputs = root.Outputs,
						inputs = root.Inputs,
						num_out = outputs.length;
					
					var num_in = 0;
					
					for (i = 0; i < inputs.length; i++)	{
						if(inputs[i].display) {
							num_in++;
						}
					}
					// reduce the size of the boxes to include the distributions/pareto without scrolling
					num_in = num_in+this.data.distData.length;
					if(this.data.paretoData.data.length>0){
						num_in++;
					}
					
					var main_w = w - rPad - lPad,
						cell_w = main_w / num_in,
						cell_h = cell_w,
						main_h = cell_h * num_out
						h = main_h + tPad + bPad,
						x = d3.scale.linear().range([0, w]),
						y = d3.scale.linear().range([0, h]);
					
					var vis = d3.select('#' + $(element).attr('id')).append("div")
						.attr("class", "chart")
						.style("width", (w+2*cell_w) + "px")
						.style("height", (h+2*cell_h) + "px")
						.append("svg:svg")
						.attr("width", (h+2*cell_w+20))
						.attr("height", (h+2*cell_h+20))
						.append("svg:g")
						.attr("transform","translate(" + rPad + "," + (tPad+cell_h) + ")");
						
					xAxes = [];
						
					for (var i = 0; i < inputs.length; i++)	{
						//Create output variables, set to mean
						var Input = inputs[i];
						eval("var " + Input.name + "=" + Input.value);
					}
					
					var dataset = [];
					//var count = 0;
					var x_ini = 0;
					

					for (var j = 0; j < num_out; j++) {//every Y
						for (i = 0; i < inputs.length; i++) { //every X
							if(inputs[i].display) {
								dataset.push({'Input':inputs[i], 'Output':outputs[j], 'x': x_ini, 'y':(cell_h + plotPad) * j});
								x_ini += cell_w;
							}
						}
						x_ini = 0;
					}
					
					var g = vis.selectAll("g")
						.data(dataset)
						.enter().append("svg:g");
					
					g.attr("transform", function(d) { 
						return "translate(" + d.x + "," + d.y + ")"; 
					})
						.attr("class", function(d) { 
							return (d.y == ((num_out - 1) * (cell_h + plotPad)) ? (d.x == 0 ? "CornerPlot" : "HozPlot") : (d.x == 0 ? "VertPlot" : "CenterPlot"));
						});
					
					g.each(createScales)
						.each(createBorder)
						.each(createYLines)
						.each(createXLines)
						.each(createPlot);
					
					//Add Axes
					vis.selectAll(".CornerPlot")
						.each(createXAxis)
						.each(createYAxis);
						
					vis.selectAll(".HozPlot")
						.each(createXAxis);
						
					vis.selectAll(".VertPlot")
						.each(createYAxis);
					
					function createScales(d) {
						var PlotObj = this;
						var Plot = d3.select(PlotObj);
						
						d.xScale = d3.scale.linear()
							.range([0, cell_w])
							.domain([d.Input.min, d.Input.max]);
						
						d.yScale = d3.scale.linear()
							.range([0, cell_h])
							.domain([d.Output.max, d.Output.min]);
							
						return Plot;
					}
						
					function createXAxis(d)	{
						var PlotObj = this,
							Plot = d3.select(PlotObj);
							
						var xAxis = d3.svg.axis()
							.scale(d.xScale)
							.orient("bottom")
							.ticks(5);
							
						newAxis = Plot.append("g")
							.attr("class", "axis")
							.attr("id", "xAxis")
							.attr("transform", "translate(0,"  + (0 + cell_h) + ")")
							.call(xAxis);
							
						Plot.append("text")
							.attr("transform", "translate(" + (cell_w/2) + "," + (cell_h + 50) + ")")
							.attr("text-anchor","middle")
							.text(function(d) { 
								return d.Input.name
							});
							
						Plot.append("text")
							.attr("transform", "translate(" + (cell_w/2) + "," + (cell_h + 35) + ")")
							.attr("class","inputVal")
							.attr("text-anchor","middle")
							.style("fill","red")
							.text(function(d) {
								return (eval(d.Input.name))
							});

						d3.select(newAxis.selectAll("g")[0][0]).select("text").style("opacity",0)			
						d3.select(newAxis.selectAll("g")[0][(newAxis.selectAll("g")[0].length - 1)])
							.select("text")
							.style("opacity",0)
						
						return Plot
					}
					
					function createYAxis(d){
						var PlotObj = this,
							Plot = d3.select(PlotObj)

						var yAxis = d3.svg.axis()
							.scale(d.yScale)
							.orient("left")
							.ticks(5);
							
						newAxis = Plot.append("g")
							.attr("class", "axis")
							.attr("id", "yAxis")
							.attr("transform", "translate(0,0)")
							.call(yAxis)
						
						d3.select(newAxis.selectAll("g")[0][0]).select("text").style("opacity",0)		
						d3.select(newAxis.selectAll("g")[0][(newAxis.selectAll("g")[0].length - 1)]).select("text").style("opacity",0)
						
						Plot.append("text")
							.attr("transform", "rotate(-90,0,0) translate(" + (-cell_h / 2) + "," + (-50) + ")")
							.attr("text-anchor","middle")
							.text(function(d) { 
							return d.Output.name
							})
							
						Plot.append("text")
							.attr("class","outputVal")
							.attr("transform", "rotate(-90,0,0) translate(" + (-cell_h / 2) + "," + (-35) + ")")
							.attr("text-anchor","middle")
							.style("fill","red")
							.text(function(d) { 
							return eval(d.Output.function)
							})
							
						return Plot
					}
					
					function createBorder(d){
						var Plot = d3.select(this)
						
						Plot.append("svg:rect")
							.attr("x", 0)
							.attr("y", 0)
							.attr("width", cell_w)
							.attr("height", cell_h)
							//fill must be set to able to click
							.attr('fill', '#444')
							.attr('fill-opacity', 0.0)
							.on('click', function(d){
								var rect = this.getBoundingClientRect();
								var tempx = d3.event.x - rect.left;
								//below code is almost the same as Ydrag except x->xdomain d.evnet.x ->tempx
								var xdmain = d.xScale;
								var xval = xdmain.invert(tempx)
								
								if (xval > d.Input.max) {
									xval = d.Input.max
									}
								else if (xval < d.Input.min) {
									xval = d.Input.min
									}
								
								eval(d.Input.name + "=" + xval)
								
								g.selectAll(".yline")
									.each(updateYLine)
								g.selectAll(".plotline")
									.each(updatePlot)
								g.selectAll(".xline")
									.each(updateXLine)
								g.selectAll(".outputVal")
									.each(updateOutputVal)
								g.selectAll(".inputVal")
									.each(updateInputVal);
								});
					}
					
					function createYLines(d){
						// Vertical Line Generation
						var PlotObj = this,
							Plot = d3.select(PlotObj)
							
						var YDataSet = [[eval(d.Input.name), d.Output.min], [eval(d.Input.name), d.Output.max]] 
						
						var x = d.xScale,
							y = d.yScale
							
						var line = d3.svg.line()
							.x(function(d) {return x(d[0])})
							.y(function(d) {return y(d[1])})
							.interpolate("linear")
							
						var drag = d3.behavior.drag()
							.origin(function(d) { 
								return {x: x(eval(d.Input.name))}; 
							})
							.on("drag", Ydrag);
							
						var PlotLine = Plot.append("svg:path")
							.attr("d", line(YDataSet))
							.attr("class","line yline")
							.call(drag);
								
						return Plot
					}
					
					function dragstart(d) {
						d.call(d3.behavior.drag()
							.origin(function(d) { 
							return {x: x(eval(d.Input.name))}})); 
						}

					function Ydrag(d)  {
						var x = d.xScale,
							y = d.yScale
							
						var xval = x.invert(d3.event.x)
						
						if (xval > d.Input.max) {
							xval = d.Input.max
							}
						else if (xval < d.Input.min) {
							xval = d.Input.min
							}
						
						console.debug( "eventx: " + d3.event.x );
						console.debug( "x: " + x );
						console.debug(" y: " + y );
						console.debug( "xval: " + xval);
						console.debug( d.Input.name );
						eval(d.Input.name + "=" + xval)
						
						g.selectAll(".yline")
							.each(updateYLine)
						g.selectAll(".plotline")
							.each(updatePlot)
						g.selectAll(".xline")
							.each(updateXLine)
						g.selectAll(".outputVal")
							.each(updateOutputVal)
						g.selectAll(".inputVal")
							.each(updateInputVal)
					}
						
					function updateYLine(d) {
						YLine = d3.select(this)
						
						var x = d.xScale,
							y = d.yScale
						
						var YDataSet = [[eval(d.Input.name), d.Output.min], [eval(d.Input.name), d.Output.max]] 
						
						var line = d3.svg.line()
							.x(function(d) {return x(d[0])})
							.y(function(d) {return y(d[1])})
							.interpolate("linear")
							
						YLine.attr("d", line(YDataSet))
						
						vis.selectAll("."+d.Input.name+"-dynamicInputLine")
								.attr("x1", (YDataSet[0][0]-d.Input.min)/(d.Input.max - d.Input.min)*cell_w-1)
								.attr("x2", (YDataSet[0][0]-d.Input.min)/(d.Input.max - d.Input.min)*cell_w-1)
					}
					
					function updateXLine(d) {
						XLine = d3.select(this)
						
						var XDataSet = [[d.Input.min, eval(d.Output.function)], [d.Input.max, eval(d.Output.function)]] 
					
						var x = d.xScale,
							y = d.yScale
							
						var line = d3.svg.line()
							.x(function(d) {return x(d[0])})
							.y(function(d) {return y(d[1])})
							.interpolate("linear")
							
						XLine.attr("d", line(XDataSet))
						g.selectAll(".distGraph").append("path")
							.attr("d,", line(XDataSet))
							.attr("transform", "translate("+20+","+0+")");
						
						vis.selectAll("."+d.Output.name+"-dynamicInputLine")
							.attr("y1", (XDataSet[0][1]-d.Output.max)/(d.Output.min - d.Output.max)*cell_w)
							.attr("y2", (XDataSet[0][1]-d.Output.max)/(d.Output.min - d.Output.max)*cell_w)
					}
					
					function updatePlot(d) {
						Plot = d3.select(this),
						x = d.xScale
						y = d.yScale,
						fString = d.Output.function,
						i = 0,
						DataSet = [];
						
						fString = fString.replace(eval("/"+d.Input.name+"/g"), "i")
					
						for (var j = 0; j<=100; j++) {
							i = (j / 100) * (d.Input.max - d.Input.min) + d.Input.min
							DataSet.push([i, eval(fString)])
						}
							
						var line = d3.svg.line()
							.x(function(d) {return x(d[0])})
							.y(function(d) {return y(d[1])})
							.interpolate("linear")
							
						Plot.attr("d", line(DataSet))			
					}
						
					function updateOutputVal(d) {
						Text = d3.select(this);
						
						newVal = roundNumber(eval(d.Output.function), 2)
						Text.text(newVal);
					}
						
					function updateInputVal(d) {
						Text = d3.select(this);
						
						newVal = roundNumber(eval(d.Input.name), 2)
						Text.text(newVal)
					}
					
					function createXLines(d) {
						var PlotObj = this,
							Plot = d3.select(PlotObj)
							
						var XDataSet = [[d.Input.min, eval(d.Output.function)], [d.Input.max, eval(d.Output.function)]] 
						
						var x = d.xScale,
							y = d.yScale
							
						var temp = [];
						temp.pixelPosition = y(XDataSet[0][1]);
						temp.scalePosition = XDataSet[0][1];
						distributionLines.push(temp);
		
						var line = d3.svg.line()
							.x(function(d) {return x(d[0])})
							.y(function(d) {return y(d[1])})
							.interpolate("linear")
							
						Plot.append("svg:path")
							.attr("d", line(XDataSet))
							.attr("class","line xline")
						
						return Plot
					}
						
					function createPlot(d) {
						var PlotObj = this,
							Plot = d3.select(PlotObj),
							x = d.xScale
							y = d.yScale,
							fString = d.Output.function,
							i = 0,
							DataSet = [];
							
						fString = fString.replace(eval("/"+d.Input.name+"/g"), "i")
						
						for (var j = 0; j<=100; j++) {
							i = (j / 100) * (d.Input.max - d.Input.min) + d.Input.min
							DataSet.push([i, eval(fString)])
						}
							
						var line = d3.svg.line()
							.x(function(d) {return x(d[0])})
							.y(function(d) {return y(d[1])})
							.interpolate("linear")
							
						Plot.append("svg:path")
							.attr("d", line(DataSet))
							.attr("class","line plotline")	
					}
					
					function roundNumber(x,places){
						var dec = (Math.pow(10,places))
						var y = Math.round(x*dec)/dec
						
						return y
					}

					
					
				//vis.selectAll("g").append("circle")
				//	.style("fill", "yellow")
				//	.attr("cy", 90)
				//	.attr("r", 30)
				//	.attr("x", 50)
				//	.attr("y", 50)
								
				//});

				
				//*/
				/* ============================== END PROFILER CODE ===========================*/
				/* ============================ DIST CODE ==================================*/
				if(this.data.distData.length != 0) {
					var distData = this.data.distData[0];
					var tempNames = [];
					for(var q=0; q<vm.listOfPETResponses().length; q++) {
						tempNames.push(vm.listOfPETResponses()[q].name());
					}				
					for(var q=0; q<distData.length; q++) {
						var outputs = distData[q];
						var index = tempNames.indexOf(outputs.TestBenchMetricName);
						
						// matches the scale for the y axis
						var inputRange = [];
						for(var j=0; j<this.data.profilerData.Outputs.length; j++) {
							if(outputs.TestBenchMetricName == this.data.profilerData.Outputs[j].name) {
								inputRange.min = this.data.profilerData.Outputs[j].min;
								inputRange.max = this.data.profilerData.Outputs[j].max;
							}
							
						}
						
						
						if(outputs != "na") {
							var fixedWidth = 300;
						
							var min; 
							var max;
							if(outputs == "na") {
								outputs = {alpha: 9.6, beta: 9.6, hi: 1, lo: 0};
								min = 0;
								max = 1;
							} else {
								if(outputs.Limits != undefined) {
									min = outputs.Limits.Min;
									max = outputs.Limits.Max;
									data = outputs.Plotting;
								}
							}
							
							var dataset = []; //placeholder for computed dataset points
							var alph = data.alpha;
							var beta = data.beta;
							var hi = data.hi;
							var lo = data.lo;
							
							var hiTemp = data.hi;
							var loTemp = data.lo;
							hiTemp = inputRange.max;
							loTemp = inputRange.min;
							
							hi = 1;
							lo = 0;
							var points = 250;
							//var data = [];
													
							with(Math)
							{	
								var betaFUN = (sqrt(2*PI)*pow(alph,alph-.5)*pow(beta,beta-.5))/(pow(alph+beta,alph+beta-.5));
							}
							var numOfPoints = points;
							for (i = lo; i < hi; i=i+((hi-lo)/points)) {
								with(Math) {
									var y = (1/(hi-lo))*(1/betaFUN)*(pow(i,alph-1))*(pow(1-i,beta-1));
								}
								dataset.push({'x' : loTemp+i*(hiTemp-loTemp), 'y' : y});
							}
							
							var margin = {top: 15, right: 15, bottom: 40, left: 20};
							var margin = {top: 0, right: 0, bottom: 0, left: 0};

							var	width = 200 - margin.left - margin.right;
							var	height = 200 - margin.top - margin.bottom ;
							width = cell_w;
							height = width;
							
							var x = d3.scale.linear()
								//.domain([dataset[0].x, dataset[dataset.length - 1].x])
								.domain([dataset[dataset.length - 1].x, dataset[0].x])
								.range([0, width]);
								
							var maxY = dataset[0].y;
							var minY = dataset[0].y;
							for (j= 1; j < dataset.length; j++) {
								if (dataset[j].y > maxY) {
									maxY = dataset[j].y;
								}
								if (dataset[j].y < minY) {
									minY = dataset[j].y;
								}
							}
							
							var y = d3.scale.linear()
								.domain([minY, maxY])
								.range([0, height]);

							var line = d3.svg.line()
								.x(function(d) { 
									return x(d.x); 
								})
								.y(function(d) { 
									return y(d.y); 
								});

							var area = d3.svg.area()
								.y1(line.x())
								.x(line.y())
								.y0(y(0));
							
							
							var xposition = 0;
							for(var k=0; k<root.Inputs.length; k++) {
								if(root.Inputs[k].display == true) {
									xposition +=1;
								}
							}	
							//this.data.profilerData.Inputs.length
							var distGraph = vis.append("g")
								.attr("class", "distGraph")
								//.attr("transform", "translate("+(xposition*cell_w+(3-xposition))+","+(q*cell_h)+")")
								.attr("transform", "translate("+(xposition*cell_w+(3-xposition))+","+(index*cell_h)+")")
								.attr("width", width)
								.attr("height", height)
							distGraph.append("path")
								.attr("class", "distLine")
								.datum(dataset)
								.attr("d", area)
							distGraph.append("rect")
								.attr("width", cell_w)
								.attr("height", cell_h)
								.attr('fill-opacity', 0.0)
								.attr("x", -1)
								
							// CODE TO CREATE ORANGE DIST CURVE
							function approxEqual(a, b, c) {
								var A = Math.abs(a);
								var B = Math.abs(b);
								return Math.abs(A - B) <= (A < B ? B : A) * c;
							}
			 
							//if(!approxEqual(max,hiTemp,.01) && !approxEqual(min,loTemp,.01)) {
								var subsetBegin = null;
								var subsetEnds = null;
								var plotMax = inputRange.max;
								var plotMin = inputRange.min;
								
								if(outputs.Limits.Min > inputRange.min) {
									plotMin = outputs.Limits.Min;
								}
								if(outputs.Limits.Max < inputRange.max) {
									plotMax = outputs.Limits.Max;
								}
								for (var i = 0; i < dataset.length; i++)
								{
									if ((dataset[i].x < plotMax && dataset[i].x > plotMin) )
									{
										if (subsetBegin == null)
										{
											subsetBegin = i;
										}
										subsetEnds = i;
										
										dataset[i]["shaded"] = 1;
									}
									else
									{
										dataset[i]["shaded"] = 0;
									}
								}
								
							var counter = 0;				
								line = d3.svg.line()
									.x(function(d, i) { 
										if (d.shaded == 1 && i != subsetBegin && i != subsetEnds)
										{
											return x(d.x);
										}
										else
										{
											if (i == subsetBegin)
											{
												if (plotMin < plotMax)
												{
													return x(plotMin);
												}
												else
												{
													return x(plotMax);
												}
											}
											else //if (i == subsetEnd)
											{
												if (plotMin < plotMax)
												{
													return x(plotMax);
												}
												else
												{
													return x(plotMin);
												}
											}
										}
									})
									.y(function(d, i) { 
										
										if (d.shaded == 1 && i != subsetBegin && i != subsetEnds)
										{
											return y(d.y);
										}
										else
										{
											return y(0);
										}
									});

								area = d3.svg.area()
									.y(line.x())
									.x(line.y())
									.y0(y(0))
							//}
							distGraph.append("path")
								.attr("class", "distLineOverlay")
								.datum(dataset)
								.attr("d", area)
								
							var xAxis = d3.svg.axis()
								.scale(x)
								.orient("right")
								.ticks(width/55); 
								
							distGraph.append("g")
								.attr("class", "x axis")
								.attr("transform", "translate(0," + (0) + ")")
								.call(xAxis);
							
							// CODE TO CREATE MIN/MAX/MEAN LINES
							var minline;
							var maxline;
							
							var meanValue = (alph/(alph+beta)).toFixed(3);
							var vertLineY1 = 0;
							var vertLineY2 = height;
							var vertLineX = (meanValue/(hi-lo))*width;
							
							if(min > loTemp && min < hiTemp) {
								minline = ((min-loTemp)/(hiTemp-loTemp))*width;
							} else {
								minline = 0;
							}
							if(max > loTemp && max < hiTemp) {
								maxline = ((max-loTemp)/(hiTemp-loTemp))*width;	
							} else {
								maxline = width;
							}			
							
							distGraph.append("line")
								.attr("class", "pareto threshold")
								.attr("y1", cell_h-vertLineX)
								.attr("x1", vertLineY1)
								.attr("y2", cell_h-vertLineX)
								.attr("x2", vertLineY2)
								.style("stroke","white")
								.style("stroke-dasharray",5)
								.style("stroke-width", 2)
							
							// min limit line
							distGraph.append("line")
								.attr("class","paretomin")
								.attr("y1",cell_h-minline)
								.attr("x1",vertLineY1) 
								.attr("y2",cell_h-minline)
								.attr("x2",vertLineY2)
								.style("stroke","red")
								.style("stroke-dasharray",5)
								.style("stroke-width", 2)
								
							//max limit line
							distGraph.append("line")
								.attr("class","paretomax")
								.attr("y1",cell_h-maxline)
								.attr("x1",vertLineY1) 
								.attr("y2",cell_h-maxline)
								.attr("x2",vertLineY2)
								.style("stroke","green")
								.style("stroke-dasharray",5)
								.style("stroke-width", 2)
							
							// creates the dynamic horizontal lines
							for(var i=0; i<distributionLines.length; i++) {
								if(distributionLines[i].scalePosition > loTemp && distributionLines[i].scalePosition < hiTemp) {
								//	value = distributionLines[i].scalePosition;
								//	value = (value-loTemp)/(value-hiTemp)*cell_h;
									value = distributionLines[i].pixelPosition;
								}
							}
							distGraph.append("line")
								.attr("class", outputs.TestBenchMetricName+"-dynamicInputLine")
								.attr("y1", value)
								.attr("x1", 2)   
								.attr("y2", value)
								.attr("x2", cell_w)
								.style("stroke","red")
								.style("stroke-dasharray",1)
								.style("stroke-width", 2)
								.style("opacity", .5)
							
						}
					}
				}
				/* ================================= END DIST CODE ====================================*/
				/* ================================== PARETO CODE ================================= */
				if(this.data.paretoData.data.length != 0) {
					for(var q = 0; q<this.data.paretoData.data.length; q++) {
						var outputs = this.data.paretoData.data[q];
						var tempData = outputs;
						var tempNames = [];
						for(var qq=0; qq<vm.listOfPETResponses().length; qq++) {
							tempNames.push(vm.listOfPETResponses()[qq].name());
						}	
						var index = tempNames.indexOf(this.data.paretoData.names[q])
						
						if(outputs.length != 0)
						{
							// resizes the Pareto height based on the number of parameters
							//height = tempData.length*15+100;
							
							var format = d3.format(",.2f");  // formats to 2 decimal places
							
							var x = d3.scale.linear()
								.range([0, width]);
							
							var y = d3.scale.ordinal()
								.rangeRoundBands([0, height-60], .1);
										
							//bar graph axes formatting
							var xAxis = d3.svg.axis()
								.scale(x)
								.orient("top")
								.tickSize(5)
								.ticks(width/35); //dynamical changes number of ticks based on width of svg
							
							var yAxis = d3.svg.axis()
								.scale(y)
								.orient("left")
								.tickSize(0);

							//Pareto plot axis formatting
							var xPareto = d3.scale.linear()
								.domain([0, 100]) //from 0 to 100%
								.range([0, width]);
							
							var xPercentAxis = d3.svg.axis()
								.scale(xPareto)
								.orient("bottom")
								.tickSize(5)
								.ticks(width/35); //dynamical changes number of ticks based on width of svg
							
							var xposition = 0;
							for(var k=0; k<root.Inputs.length; k++) {
								if(root.Inputs[k].display == true) {
									xposition +=1;
								}
							}				
							
							var paretoPlot = vis.append("g")
								.attr("class", "paretoPlot")
								.attr("transform", "translate("+(3+(xposition+1)*cell_w+((xposition+1)))+","+(index*(cell_h))+")")
								.attr("width", width)
								.attr("height", height)
							paretoPlot.append("rect")
								.attr("width", cell_w+20)
								.attr("height", cell_h)
								.attr('fill-opacity', 0.0)
								.attr("x", -8)
									
							var sum = 0;
							var cumulativePercent = [];
							var indexCounter = [];
							
							// Parse numbers, sort by value, sum all data, and find the relative cumulative percent
							tempData.forEach(function (d) {
								d.value = +d.value;
							});
							tempData.sort(function (a, b) {
								return b.value - a.value;
							});
							tempData.forEach(function (d) {
								return sum += +d.value;
							});
							cumulativePercent.push({
								"x" : tempData[0].value / sum * 100,
								"y" : tempData[0].name
							});
							//indexCounter.push({'index' : 0});
							for (var i = 1; i < tempData.length; i++) {
								cumulativePercent.push({
									"x" : cumulativePercent[cumulativePercent.length - 1].x + (tempData[i].value / sum) * 100,
									"y" : tempData[i].name
								});
								//indexCounter.push({'index' : i});
							}

							// Set the scale domain.
							x.domain([0, sum]);
							y.domain(tempData.map(function (d) {
									return d.name;
								}));
							
							//Make the bars
							var bar = paretoPlot.selectAll("g.bar")
								.data(tempData)
								.enter().append("g")
								.attr("class", "bar")
								.attr("transform", function (d) {
									return "translate(0," + (y(d.name)+30) + ")";
								});
							
							bar.append("rect")
							.attr("width", function (d) {
								return x(d.value);
							})
							.attr("height", y.rangeBand());
							
							//Append the text on the bar
							bar.append("text")
							.attr("class", "value")
							.attr("x", function (d) {
								return x(d.value) + 5;
							})
							.attr("y", y.rangeBand() / 2)
							.attr("dx", 0)
							.attr("dy", ".35em")
							.attr("text-anchor", "start")
							.text(function (d) {
								return d.name+ " ("+format(d.value)+")";
							});
							
							//Render the bar x axis
							paretoPlot.append("g")
							.attr("class", "x axis")
							.call(xAxis)
							.attr("transform", "translate(0," + (30) + ")")

							
							//Render the bar y axis
							paretoPlot.append("g")
							.attr("class", "y axis")
							//.call(yAxis);
							
							//Render the pareto x axis
							paretoPlot.append("g")
							.call(xPercentAxis)
							.attr("transform", "translate(0," + (height-30) + ")")
							.attr("class", "x pareto axis")
							
							var line = d3.svg.line()
								.x(function (d) {
									return xPareto(d.x);
								}) //use the x axis from the PARETO plot
								.y(function (d) {
									return y(d.y);
								}); //use the y axis from the BAR plot (same as PARETO Y axis!)
							
							paretoPlot.selectAll(".dot")
							.data(cumulativePercent)
							.enter().append("circle")
							.attr("class", "dot")
							.attr("cx", line.x())
							.attr("cy", line.y())
							.attr("r", 3.5)
							.attr("transform", "translate("+0+","+30+")");
							
							var thresholdIndex = [];
											
							for (i in cumulativePercent) {
								if(cumulativePercent[i].x > 80){
									thresholdIndex = i;
									break;
								}
							}
							
							//Horizontal line for the Pareto threshold
							var horiLineX1 = 0;
							var horiLineX2 = width;
							var horiLineY = y(cumulativePercent[thresholdIndex].y) + y.rangeBand();
							
							paretoPlot.append("g")
							.attr("class", "pareto threshold")
							.append("line")
							.attr("x1", horiLineX1)
							.attr("y1", horiLineY)
							.attr("x2", horiLineX2)
							.attr("y2", horiLineY)
							
							paretoPlot.append("g")
							.attr("class","label")
							.attr("id","xlabel")
							.append("text")
								.text("Percentage")
								.attr("x",width/2)
								.attr("y",height-5)
								.attr("text-anchor","middle")
								.style("font-family","Verdana")
								.style("font-size",10);
						
							paretoPlot.append("g")
							.attr("class","label")
							.attr("id","xlabel")
							.append("text")
								.text("Metric")
								.attr("x",width/2)
								.attr("y",10)
								.attr("text-anchor","middle")
								.style("font-family","Verdana")
								.style("font-size",10);
								
							//paretoPlot.append("rect")
							//	.attr("width", cell_w)
							//	.attr("height", cell_h)
							//	.attr('fill-opacity', 0.0)
							//	.attr("x", -8)

						}
					}
				}
				/* ============================ END PARETO CODE ================================= */
				/* ========================== INPUT DISTRIBUTIONS ================================= */
				var inputdistData = this.data.inputData;
				if(inputdistData.length != 0) {
					// identify which input distributions to show
					var visibleInputs = [];
					var tempData = [];
					var allInputs = vm.listOfPETVariables();
					for(var q=0; q<allInputs.length; q++) {
						if(allInputs[q].display() == true) {
							visibleInputs.push(allInputs[q].name());
						}
					}
					
					// identifies the order of the input distributions so that they match
					for(var q=0; q<inputdistData.length; q++) {
						if(visibleInputs.indexOf(inputdistData[q].Name) != -1) {
							tempData[visibleInputs.indexOf(inputdistData[q].Name)] = inputdistData[q];
						}
					}
					inputdistData = tempData;

					for(var q=0; q<inputdistData.length; q++) {
						outputs = inputdistData[q];
						// matches the scale for the x axis
						var inputRange = [];
						for(var j=0; j<this.data.profilerData.Inputs.length; j++) {
							if(outputs.Name == this.data.profilerData.Inputs[j].name) {
								inputRange.min = this.data.profilerData.Inputs[j].min;
								inputRange.max = this.data.profilerData.Inputs[j].max;
							}
							
						}
						
						if(outputs != "na") {
							var fixedWidth = cell_w;
							var min; 
							var max;
						
							//if(outputs.Limits != undefined)
							if(outputs.Distribution == "NORM") { 	//mu = mean; sigma = std
								var mu = outputs.Param1;
								var sigma = outputs.Param2;
								var hiTemp = mu+3*sigma;
								var loTemp = mu;
								hiTemp = inputRange.max;
								loTemp = inputRange.min;
							
							}
							if(outputs.Distribution == "UNIF") { //set min/max to be slightly larger than the range
								var a=outputs.Param1;
								var b=outputs.Param2;
								var hiTemp = b+.01; //b+((b+a)/2);
								var loTemp = a-.01; //a-((b+a)/2);
							}
							if(outputs.Distribution == "BETA") {
								min = outputs.Limits.Min;
								max = outputs.Limits.Max;
								data = outputs.Plotting;
								var alph = data.alpha;
								var beta = data.beta;
								var hi = data.hi;
								var lo = data.lo;
								var hiTemp = data.hi;
								var loTemp = data.lo;
							}
							
							var dataset = []; //placeholder for computed dataset points
							var dataset2 = [];
							
							hi = 1;
							lo = 0;
							var points = 300;
							//var data = [];
							
							with(Math)	{	
								var betaFUN = (sqrt(2*PI)*pow(alph,alph-.5)*pow(beta,beta-.5))/(pow(alph+beta,alph+beta-.5));
							}
							for (i = lo; i < hi; i=i+((hi-lo)/points))	{
								with(Math)	{
									if(outputs.Distribution == "BETA") {
										var y = (1/(hi-lo))*(1/betaFUN)*(pow(i,alph-1))*(pow(1-i,beta-1));
									}
									
									if(outputs.Distribution == "NORM") {
										x = loTemp+i*(hiTemp-loTemp);
										var y = 1/(sigma*sqrt(2*PI))*exp(-pow(x-mu,2)/(2*pow(sigma,2)));
									}
									
									if(outputs.Distribution == "UNIF") {
										x = loTemp+i*(hiTemp-loTemp);
										if(a>x || x>b) {
											var y = 0;
										} else {						
											var y = 1/(b-a);
										}
									}	
								}
								dataset.push({'x' : loTemp+i*(hiTemp-loTemp), 'y' : y});
							}
							var	width = cell_w;
							var	height = cell_h;
							
							var x = d3.scale.linear()
								.domain([dataset[0].x, dataset[dataset.length - 1].x])
								.range([0, width]);
							
							var maxY = dataset[0].y;
							var minY = dataset[0].y;
							for (j= 1; j < dataset.length; j++) {
								if (dataset[j].y > maxY) {
									maxY = dataset[j].y;
								}
								if (dataset[j].y < minY) {
									minY = dataset[j].y;
								}
							}
							
							var y = d3.scale.linear()
								.domain([minY, maxY])
								.range([height, 0]);
							
							var xAxis = d3.svg.axis()
								.scale(x)
								.orient("top")
								.ticks(3); //width/55 dynamical change ticks based on width
							
							var yAxis = d3.svg.axis()
								.scale(y)
								.orient("left");

							var line = d3.svg.line()
								.x(function(d) { 
									return x(d.x); 
								})
								.y(function(d) { 
									return y(d.y); 
								});

							var area = d3.svg.area()
								.x(line.x())
								.y1(line.y())
								.y0(function() {
									if(y(0)>height) {
										return height;
									} else {
										return y(0);
									}
								});
							
							var inputdistGraph = vis.append("g")
								.attr("class", "inputdistGraph")
								.attr("transform", "translate("+(q*cell_w+1)+","+(-cell_h)+")")
								.attr("width", width)
								.attr("height", height)
							inputdistGraph.append("path")
								.attr("class", "distLine")
								.attr("width", cell_w)
								.attr("height", cell_h)
								.datum(dataset)
								.attr("d", area)
							inputdistGraph.append("rect")
								.attr("width", cell_w)
								.attr("height", cell_h)
								.attr('fill-opacity', 0.0)
								.attr("x", -1)
							
							// adds in dashed lines for the limits
							var minline;
							var maxline;
							if(min > loTemp && min < hiTemp) {
								minline = ((min-loTemp)/(hiTemp-loTemp))*width;
							}
							else {
								minline = 0;
							}
							if(max > loTemp && max < hiTemp) {
								maxline = ((max-loTemp)/(hiTemp-loTemp))*width;	
							}
							else {
								maxline = width;
							}			
							
							//Generate shaded area under the curve bounded by obj and thresh
							var subset = [];
							var subsetBegin = null;
							var subsetEnds = null;
							
							for (var i = 0; i < dataset.length; i++) {
								if ((dataset[i].x <= max && dataset[i].x >= min) || (dataset[i].x >= max && dataset[i].x <= min)) {
									subset.push(dataset[i]);
									if (subsetBegin == null) {
										subsetBegin = i;
									}
									subsetEnds = i;
									dataset[i]["shaded"] = 1;
								}
								else {
									dataset[i]["shaded"] = 0;
								}
							}
							
							var counter = 0;				
							line = d3.svg.line()
								.x(function(d, i) { 
									if (d.shaded == 1 && i != subsetBegin && i != subsetEnds){
										return x(d.x);
									}
									else {
										if (i == subsetBegin) {
											if (min < max) {
												return x(min);
											}
											else {
												return x(max);
											}
										}
										else {  //if (i == subsetEnd)
											if (min < max) {
												return x(max);
											}
											else {
												return x(min);
											}
										}
									}
								})
								.y(function(d, i) { 
									if (d.shaded == 1 && i != subsetBegin && i != subsetEnds) {
										return y(d.y);
									}
									else {
										return y(0);
									}
								});
						
							inputdistGraph.append("g")
								.attr("class", "x axis")
								.attr("transform", "translate(-1," + (height) + ")")
								//.attr("width", width)
								.call(xAxis);

							
							// create lines in the input distributions
							var value = cell_h/2;
							for(var i=0; i<vm.listOfPETVariables().length; i++) {
								if(vm.listOfPETVariables()[i].name() == outputs.Name) {
									value = vm.listOfPETVariables()[i].value(); 
								}
							}
							value = x(value);
						
							inputdistGraph.append("line")
								.attr("class", outputs.Name+"-dynamicInputLine")
								.attr("y1", 2)
								.attr("x1", value-1)   // change to be mean (where it begins initially)
								.attr("y2", cell_h)
								.attr("x2", value-1)
								.style("stroke","red")
								.style("stroke-dasharray",1)
								.style("stroke-width", 2)
								.style("opacity", .5)
						}
					
					}
				}	
				/* ============================ END INPUT DiST CODE ================================ */
			}
				
			/* Mark <vf-table> as rendered */
			$(element).attr('rendered', 'true');
		}
		catch(err) 
		{
			$(element).attr('rendered','true');
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