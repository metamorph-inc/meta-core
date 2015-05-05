/*///////////// vf-profiler /////////////*/
function vf_profiler(element) {
	var self = this;
	//this.data = eval($(element).attr('data'));
	this.data = eval($(element).attr('data'));
	this.element = element;
	this.renderCount = 0;
	
	/* Render Method */
	this.render = function () {
			
		/* 
		Check if element has already been rendered 
		if it has then skip it
		if it has not render it
		*/
		
		if (($(element).attr('rendered') == undefined || $(element).attr('rendered') == "false")) {
			this.data = eval($(this.element).attr('data'));
			
			/* INTERNAL CODE */
			
			
			var w = $(element).parent().width(),
				plot_x = 60,
				plot_x_margin = 15,
				plot_y = 60;
				
			//d3.json("profiler_data.json", function(root) {
			
				root = this.data

				var lPad = 50,
					rPad = 100,
					tPad = 50,
					bPad = 50,
					plotPad = 0;

				var outputs = root.Outputs,
					inputs = root.Inputs,
					num_out = outputs.length;
				
				var num_in = 0;
				
				for (i = 0; i < inputs.length; i++)
				{
					if(inputs[i].display)
					{
						num_in++;
					}
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
					.style("width", w + "px")
					.style("height", h + "px")
					.append("svg:svg")
					.attr("width", w)
					.attr("height", h)
					.append("svg:g")
					.attr("transform","translate(" + rPad + "," + tPad + ")");
					
				xAxes = [];
					
				for (var i = 0; i < inputs.length; i++)
				{
					//Create output variables, set to mean
					var Input = inputs[i];
					eval("var " + Input.name + "=" + Input.value);
				}
				
				var dataset = [];
				//var count = 0;
				var x_ini = 0;
				

				for (var j = 0; j < num_out; j++) //every Y
				{				
					for (i = 0; i < inputs.length; i++) //every X
					{
						if(inputs[i].display)
						{
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
				
				function createScales(d)
				{
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
					
				function createXAxis(d)
				{
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
				
				function dragstart(d) 
					{
					d.call(d3.behavior.drag()
						.origin(function(d) { 
						return {x: x(eval(d.Input.name))}})); 
					}

				function Ydrag(d) 
					{
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
					
				function updateYLine(d)
					{
						YLine = d3.select(this)
						
						var x = d.xScale,
							y = d.yScale
							
						var YDataSet = [[eval(d.Input.name), d.Output.min], [eval(d.Input.name), d.Output.max]] 
						
						var line = d3.svg.line()
							.x(function(d) {return x(d[0])})
							.y(function(d) {return y(d[1])})
							.interpolate("linear")
							
						YLine.attr("d", line(YDataSet))
					}
					
				function updateXLine(d)
					{
						XLine = d3.select(this)
						
						var XDataSet = [[d.Input.min, eval(d.Output.function)], [d.Input.max, eval(d.Output.function)]] 
					
						var x = d.xScale,
							y = d.yScale
							
						var line = d3.svg.line()
							.x(function(d) {return x(d[0])})
							.y(function(d) {return y(d[1])})
							.interpolate("linear")
							
						XLine.attr("d", line(XDataSet))
					}
				
				function updatePlot(d)
					{
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
					
				function updateOutputVal(d)
					{
						Text = d3.select(this);
						
						newVal = roundNumber(eval(d.Output.function), 2)
						Text.text(newVal);
					}
					
				function updateInputVal(d)
					{
						Text = d3.select(this);
						
						newVal = roundNumber(eval(d.Input.name), 2)
						Text.text(newVal)
					}
				
				function createXLines(d){
					var PlotObj = this,
						Plot = d3.select(PlotObj)
						
					var XDataSet = [[d.Input.min, eval(d.Output.function)], [d.Input.max, eval(d.Output.function)]] 
					
					var x = d.xScale,
						y = d.yScale
						
					var line = d3.svg.line()
						.x(function(d) {return x(d[0])})
						.y(function(d) {return y(d[1])})
						.interpolate("linear")
						
					Plot.append("svg:path")
						.attr("d", line(XDataSet))
						.attr("class","line xline")
						
					return Plot
				}
					
				function createPlot(d){
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

			//});

			
			/* END INTERNAL CODE */
		}
			
		/* Mark <vf-table> as rendered */
		$(element).attr('rendered', 'true');
		
		// End set timeout
		
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