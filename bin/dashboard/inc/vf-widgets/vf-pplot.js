/*///////////// vf-pplot /////////////*/
function vf_pplot(element) {
	
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
			return 'vf-pplot: already rendered';
		}
		
			var outputs = eval($(this.element).attr('data'));
			outputs = clone(outputs); //save "outputs" locally.  Do not use the pass by reference (from the dataprovider)
			
			var m = [0, 0, 25, 60]; //top, right, bottom, left
			
			var maxSize = 0;
			var size = 0;
						
			//redefine top and right margin based on TB string length
			maxSize = 0;
			size = 0;
			for (var i = 0; i < outputs.variables.length; i++)
			{
				size = outputs.variables[i].length; //length of the string
				
				if (maxSize < size)
				{
					maxSize = size;
				}
			}
			
			for(var i = 0; i < outputs.testBenchs.length; i++)
			{
				size = outputs.testBenchs[i].length; //length of the string
				
				if (maxSize < size)
				{
					maxSize = size;
				}
			}
			m[0] = maxSize * 4.5 * 0.707 + 5;
			m[1] = maxSize * 4.5 * 0.707 + 10;
			
			
			//define width and height
			var w = $(element).parent().width() - m[1] - m[3],
				h = ($(element).parent().width() * .37) - m[0] - m[2];
			
			var x = d3.scale.ordinal().domain(outputs.variables).rangePoints([0, w]),
			y = {};
			
			var line = d3.svg.line(),
			axis = d3.svg.axis().orient("left"),
			foreground;
			
			var svg = d3.select('#' + $(element).attr('id')).append("svg:svg")
				.attr("width", w + m[1] + m[3])
				.attr("height", h + m[0] + m[2])
				.append("svg:g")
				.attr("transform", "translate(" + m[3] + "," + m[0] + ")");
			
			//Need to create a temporary object with the data, objectives, and threshold values
			//to scale the axes properly
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
					
					//test to see if neither thresh or objs are defined
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

			var percentScaling = 0.07;
			
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
			
			//Create a scale and brush for each variables.
			outputs.variables.forEach(function (d) {
				y[d] = d3.scale.linear()
					.domain([minimum[d], maximum[d]])
					.range([h, 0]);
				
				y[d].brush = d3.svg.brush()
					.y(y[d])
					.on("brush", brush);
			});

			// Add foreground lines. background brush
			foreground = svg.append("svg:g")
			  .attr("class", "foreground")
			.selectAll("path")
			  .data(outputs.values)
			.enter().append("svg:path")
				.attr("d", path)
				.on("mouseover", function(d) {
					svg.append("svg:text")
						.text(d.design)
						.attr("class", "designID")
						.attr("x", event.pageX - m[3] - 10) //mouse location (x) minus margin
						.attr("y", event.pageY - m[0] - 230); //mouse location (y) minus margin and some padding for better text display
						//.attr("fill","white");
				})
				.on("mouseout", function() {
					//remove the text element added on the mouseover event when the mouseout event is triggered
					$("text.designID").remove(); 
				})
				.on("click", function(d){
					$(document).trigger($(element).attr('id') + '_clicked', [event, d.design]);	
				})
			.attr("stroke", function(d,i) {
				var color = outputs.color[i];
				
				if (!color || 0 === color.length)
				{
					return "#1f77b4";
				}
				else
				{
					return color;
				}
			})
			.attr("cid", function(d,i){
				return d.cid;
			})
			.attr("config", function(d,i){
				return d.design;
			})
			   //.attr("title", "TEST")
			.attr("class", function (d,i) {
					var string = "design " + d.cid + " selected";
					
					/* if(outputs.violations[i])
					{
						return string + " violation";
					}
					else
					{ */
						return string;
					//}
				}); 

			// Add a group element for each variables.
			var g = svg.selectAll(".variables")
			  .data(outputs.variables)
			.enter().append("svg:g")
			  .attr("class", "variables")
			  //.attr("stroke", "white")
			  .attr("transform", function(d,i) { 
						return "translate(" + x(d) + ")"; 
					})
			  .call(d3.behavior.drag()
					  .origin(function(d) { 
							return {x: x(d)}; 
						})
					  .on("dragstart", dragstart)
					  .on("drag", drag)
					  .on("dragend", dragend));

			// Add an axis and title.
			g.append("svg:g")
			  .attr("class", "axis")
			  .each(function(d) { 
					d3.select(this).call(axis.scale(y[d])); 
				})
			.append("svg:text")
			  .data(outputs.labels)
			  .attr("y", -12)
			  .attr("transform", "rotate(-45 0 0)") //deg y x
			  .attr("x", 0)
			  .attr("class", "axislabel")
			  .text(function(d){
					//console.log(d);
					var unit = "";
					if(outputs.Unit[$.inArray(d, outputs.labels)] != "")
					{
						unit = '  [' + outputs.Unit[$.inArray(d, outputs.labels)] + ']';
					}
					return d + unit;
			  });
			  
			g.append("svg:g")
			  .append("svg:text")
			  .data(outputs.testBenchs)
			  .attr("y", 4)
			  .attr("transform", "rotate(-45 0 0)") //deg y x
			  .attr("x", 14)
			  .attr("class", "axislabel")
			  .text(function(d){
					//console.log(d);
					return d;
			  });
			  
			g.append("svg:text")
			  .data(outputs.Unit)
			  .attr("y", function(){
					return $('#' + $(element).attr('id') + ' svg').attr('height') - m[0] - m[2] + 15;
			  })
			  //.attr("transform", "rotate(-45 0 0)") //deg y x
			  .attr("x", -5)
			  .attr("class", "axislabel")
			  .text(function(d){
					//console.log(d);
					return d;
			  });
			  
			//Add objective ticks
			g.select("g.axis").append("g")
			.attr("transform", function (d) {
				return "translate(0, " + y[d](outputs.objectives[d]) + ")";
			})
			.attr("class", "objectivetick")
			.append("line")
			.attr("class", "tick")
			//.attr("fill", "green")
			.attr("x2", 6)
			.attr("y2", 0);
			
			g.select("g.objectivetick").append("text")
			.attr("x", 8)
			.attr("y", 3)
			.attr("text-anchor", "start")
			//.attr("stroke", "green")
			.text(function (d) {
					return outputs.objectives[d];
				})
			
			//Add threshold ticks
			g.select("g.axis").append("g")
			.attr("transform", function (d) {
					return "translate(0, " + y[d](outputs.thresholds[d]) + ")";
				})
			.attr("class", "thresholdtick")
			.append("line")
			.attr("class", "tick")
			//.attr("fill", "red")
			.attr("x2", 6)
			.attr("y2", 0);
			
			g.select("g.thresholdtick").append("text")
			.attr("x", 8)
			.attr("y", 3)
			.attr("text-anchor", "start")
			//.attr("stroke", "red")
			.text(function (d) {
					return outputs.thresholds[d];
				})
			// Add a brush for each axis.
			g.append("svg:g")
			  .attr("class", "brush")
			  .attr("fill", "white")
			  .each(function(d) { 
					d3.select(this).call(y[d].brush);
				})
			.selectAll("rect")
			  .attr("x", -8)
			  .attr("fill", "white")
			  .attr("width", 16);

			function dragstart(d) 
			{
				i = outputs.variables.indexOf(d);
			}

			function drag(d) 
			{
				x.range()[i] = d3.event.x;
				outputs.variables.sort(function(a, b) { return x(a) - x(b); });
				g.attr("transform", function(d) { return "translate(" + x(d) + ")"; });
				foreground.attr("d", path);
			}

			function dragend(d) 
			{
				x.domain(outputs.variables).rangePoints([0, w]);
				var t = d3.transition().duration(500);
				t.selectAll(".variables").attr("transform", function(d) { return "translate(" + x(d) + ")"; });
				t.selectAll(".foreground path").attr("d", path);
			}

			// Returns the path for a given data point.
			function path(d) {
				return line(outputs.variables.map(function (p) {
						return [x(p), y[p](d[p])];
					}));
			}

			// Handles a brush event, toggling the display of foreground lines.
			function brush() 
			{
				var actives = outputs.variables.filter(function(p) { 						
					return !y[p].brush.empty(); 
				}),
				extents = actives.map(function(p) { 
					return y[p].brush.extent(); 
				});
				
				foreground.classed("selected", function(d) {
					return actives.every(function(p, i) {
							return extents[i][0] <= d[p] && d[p] <= extents[i][1];
						});
				  });
				
				if($('#pplot .extent[height!="0"]').length == 0)
				{
					$(document).trigger('selectAll', outputs);
					return;
				}
				
				SelectedList($(element).attr("id"));
			}
			SelectedList($(element).attr("id"));
			
			function SelectedList(id){
				var classSelectedList = [];
				var classNotSelectedList = [];
							
				$("#" + id + " g.foreground path.selected").each(function(index){
					classSelectedList.push($(this).attr("class").split(" ")[1]);
				});
				
				$("#" + id + " g.foreground path:not(.selected)").each(function(){
					classNotSelectedList.push($(this).attr("class").split(" ")[1]);
				});
				
				//trigger the selectionChange event defined in vf-events.js
				$(document).trigger('selectionChange', {
					"widget" : id,
					"eventForWidget" : ["mvplot", "dsatable"],
					"data" : outputs,
					"selected" : classSelectedList,
					"notSelected" : classNotSelectedList
				});
			}
			
			// fix negative signs in axis
			$('text').each(function (index, item) {
				item.textContent = item.textContent.replace("-", "-");
			});
			
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