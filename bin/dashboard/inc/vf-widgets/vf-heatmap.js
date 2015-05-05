/*///////////// vf-heatmap /////////////*/
function vf_heatmap(element) {
	
	var self = this;
	//this.data = dataHeatMAP; //eval($(element).attr('data'));
	this.element = element;
	
	this.icolor = function(n){
		var a = "#ff3300";
		var b = "#1f77b4";

		var c = d3.hsl(a)
		var d = d3.hsl(b);

		var e = d3.interpolateNumber(c.h,d.h);
		var f = d3.interpolateNumber(c.s,d.s);
		var g = d3.interpolateNumber(c.l,d.l);

		c.h = e(n);
		c.s = f(n);
		c.l = g(n);

		return c.rgb().toString();
	 }
	 
	/* Render Method */
	this.render = function () {
		try
		{
			/* 
				Check if element has already been rendered 
				if it has then skip it
				if it has not render it
			*/
			
			if (!($(element).attr('rendered') == undefined || $(element).attr('rendered') == "false")) 
			{
				return 'vf-heatmap: already rendered';
			}
			
			var output = clone(eval($(this.element).attr('data')));
			//output = clone(dataHeatMAP); //save "outputs" locally.  Do not use the pass by reference (from the dataprovider)
			
			function sorter(data) {	
				var temp = [];
				var temp2 = [];
				
				for(var i=0; i<data.values.length; i++) {
					if(data.values[i].benchesX == 0) {
						temp.push(data.values[i]);
					} else {
						temp2.push(data.values[i]);
					}					
				}
				temp.sort(function(a,b){return b.values-a.values});
				temp2.sort(function(a,b){return b.values-a.values});

				var newBenches = [];
				var newConfigs = [];
				temp = temp.concat(temp2);
				for(var i=0; i<temp.length; i++) {
					if(newBenches.indexOf(temp[i].benches) == -1) {
						newBenches.push(temp[i].benches);
					}
					if(newConfigs.indexOf(temp[i].configs) == -1) {
						newConfigs.push(temp[i].configs);
					}
				}
				
				var newValues = [];
				for(var i=0; i<newConfigs.length; i++) {
					for(var j=0; j<newBenches.length; j++) {
						for(var k=0; k<data.values.length; k++) {
							if(data.values[k].configs == newConfigs[i] && data.values[k].benches == newBenches[j]) {
								data.values[k].benchesX = j;
								data.values[k].configsY = i;
								newValues.push(data.values[k]);
							}
						}
					}
				}
				var newOut = [];
				data.benches = newBenches;
				data.configs = newConfigs;
				data.values = newValues;
				
				d3.selectAll(".yLabel")
						.transition()
						.duration(500)
						.attr("y", function(d, i) { 
							return data.configs.indexOf(d) * gridSize + gridSize/2; 
						});
					// moves the boxes to a new location or to the initial location
				d3.selectAll("rect")
					.transition()
					.duration(500)
					.attr("y", function(d, i) { 
						return d.configsY * gridSize; 
					})
					.attr("x", function(d, i) { 
						return d.benchesX * gridSize; 
					});
				d3.selectAll(".xLabel")
					.transition()
					.duration(500)
					.attr("transform", function (d, i) {
						return "translate(" + 0 + ", 0)rotate(-45 "+ String(output.benches.indexOf(d)*gridSize+gridSize/2) +" 0)";
					})
					.attr("x", function(d, i) { 
						return output.benches.indexOf(d) * gridSize + gridSize/2; 
					});
			}
			
			heatmapSorter(output)
			function heatmapSorter(data) {
				var newBenches = [];
				var newConfigs = [];
				var columnTotalPCC = Array.apply(null, new Array(data.benches.length)).map(Number.prototype.valueOf,0);
				for(var i=0; i<data.values.length; i++) {
					var loc = data.benches.indexOf(data.values[i].benches)
					columnTotalPCC[loc] = columnTotalPCC[loc] + data.values[i].values
				}
				// sort the benches into descending order of total PCC
				var temp = [];
				for(var j=0; j<data.benches.length; j++) {
					temp.push({"bench": data.benches[j], "value":columnTotalPCC[j]})
				}
				temp.sort(function(a, b) {
					return ((a.value > b.value) ? -1 : ((a.value == b.value) ? 0 : 1));
				});
				for (var k=0; k<temp.length; k++) {
					newBenches[k] = temp[k].bench;
				}
				// sort the configs into descending order of total PCC
				var temp2 = data.values;
				temp2.sort(function(a, b) {
					return ((a.values > b.values) ? -1 : ((a.values == b.values) ? 0 : 1));
				});	
				for(var i=0; i<temp2.length; i++) {
					if(temp2[i].benches == newBenches[0]) {
						newConfigs.push(temp2[i].configs)
					} 
				}
				var tempExtra = [];
				for(var i=0; i<temp2.length; i++) {
					if(temp2[i].benches != newBenches[0]) {
						if(newConfigs.indexOf(temp2[i].configs) == -1 && tempExtra.indexOf(temp2[i].configs) == -1) {
							tempExtra.push(temp2[i].configs)
						}
					}
				}
				newConfigs = newConfigs.concat(tempExtra)
				
				// update the values x and y position
				var newValues = [];
				for(var i=0; i<data.values.length; i++) {
					var temp = data.values[i];
					newValues.push(data.values[i]);
					newValues[newValues.length-1].benchesX = newBenches.indexOf(data.values[i].benches)
					newValues[newValues.length-1].configsY = newConfigs.indexOf(data.values[i].configs)
				}
				
				
				output.benches = newBenches;
				output.configs = newConfigs;
				output.values = newValues;
			}
			
			//output = heatmaphelper(output);	
			function heatmaphelper(data) {
				var temp = data.values;
				temp.sort(function(a,b){return b.values-a.values});
			
				var newBenches = [];
				var newConfigs = [];
				for(var i=0; i<temp.length; i++) {
					if(newBenches.indexOf(temp[i].benches) == -1) {
						newBenches.push(temp[i].benches);
					}
					if(newConfigs.indexOf(temp[i].configs) == -1) {
						newConfigs.push(temp[i].configs);
					}
				}
				var newValues = [];
				for(var i=0; i<newConfigs.length; i++) {
					for(var j=0; j<newBenches.length; j++) {
						for(var k=0; k<data.values.length; k++) {
							if(data.values[k].configs == newConfigs[i] && data.values[k].benches == newBenches[j]) {
								data.values[k].benchesX = j;
								data.values[k].configsY = i;
								newValues.push(data.values[k]);
							}
						}
					}
				}
				var newOut = [];
				data.benches = newBenches;
				data.configs = newConfigs;
				data.values = newValues;
				return data;				
			}
			
			var dragy = d3.behavior.drag()
				.on("drag", function(d,i) {
					var position = d3.event.dy;
					var row = d3.select(this)[0][0].__data__.configsY;  // when dragging rects
					if(row == undefined) {
						row = output.configs.indexOf(d3.select(this)[0][0].__data__);
					}
					d3.selectAll("rect")
						.attr("y", function (d) {
							if(d.configsY == row) {
								return parseInt(d3.select(this).attr("y"))+position;
							} else {
								return parseInt(d3.select(this).attr("y"));
							}
						})
					d3.selectAll(".yLabel")
					.attr("y", function(d, i) {
						if(d == output.configs[row]) {
							return parseInt(d3.select(this).attr("y"))+position;
						} else {
							return parseInt(d3.select(this).attr("y"));
						}
					})
				})
				.on("dragend", function (d,i) {
					var row = d.configsY;
					// if the rows have been reordered, changes the data structure to reflect that
					var order = [];
					var labelorder = [];
					d3.selectAll("rect").each(function (d) {
						if(order.indexOf(d.configsY) == -1) {
							order[d.configsY] = d3.select(this).attr('y');
						} 							
					})
					order.sort(function(a,b){return a-b});
					// asigns new row position
					d3.selectAll("rect").each(function(d) {
						d.configsY = order.indexOf(d3.select(this).attr('y'));
						labelorder[d.configsY] = d.configs;
					})					
					output.configs = labelorder;
					
					d3.selectAll(".yLabel")
						.transition()
						.duration(500)
						.attr("y", function(d, i) { 
							return output.configs.indexOf(d) * gridSize + gridSize/2; 
						})
					// moves the boxes to a new location or to the initial location
					d3.selectAll("rect")
						.transition()
						.duration(500)
						.attr("y", function(d, i) { 
							return d.configsY * gridSize; 
						})
				});
				
				
			var dragx = d3.behavior.drag()
				.origin(function(d,i) {
					if(d.benchesX != undefined) {
						return {x: d.benchesX*gridSize, y:0}; 
					} else {
						return {x: output.benches.indexOf(d)*gridSize, y: 0};
					}
				})
				.on("drag", function(d,i) {
					var position = d3.event.dx;
					var column = d3.select(this)[0][0].__data__.benchesX;
					if(column == undefined) {
						column = output.benches.indexOf(d3.select(this)[0][0].__data__);
					}
					d3.selectAll("rect")
						.attr("x", function (d) {
							if(d.benchesX == column) {
								return parseInt(d3.select(this).attr("x"))+position;
							} else {
								return parseInt(d3.select(this).attr("x"));
							}
						})
					d3.selectAll(".xLabel")
					// moving the xlabels with the columns
						.attr("transform", function(d, i) {
							if(d == output.benches[column]) {
								return "translate(" + (d3.event.x - output.benches.indexOf(d)*gridSize) + ", 0)rotate(-45 "+ String(output.benches.indexOf(d)*gridSize+gridSize/2) +" 0)";
							} else {
								return "translate(" + 0 + ")rotate(-45 "+ String(output.benches.indexOf(d)*gridSize+gridSize/2) +" 0)"; 
							}
						})
				})
				.on("dragend", function (d,i) {
					var column = d.benchesX;
					var currentorder = output.benches;
					var order = [];
					var labelorder = [];
					d3.selectAll("rect").each(function (d) {
							if(order.indexOf(d.benchesX) == -1) {
								order[d.benchesX] = d3.select(this).attr('x');				
							} 							
						})
					order.sort(function(a,b){return a-b});
					// asigns new column position
					d3.selectAll("rect").each(function(d) {
						d.benchesX = order.indexOf(d3.select(this).attr('x'));
						labelorder[d.benchesX] = d.benches;
					})					
					output.benches = labelorder;
				
					d3.selectAll(".xLabel")
						.transition()
						.duration(500)
						.attr("transform", function (d, i) {
							return "translate(" + 0 + ", 0)rotate(-45 "+ String(output.benches.indexOf(d)*gridSize+gridSize/2) +" 0)";
						})
						.attr("x", function(d, i) { 
							return output.benches.indexOf(d) * gridSize + gridSize/2; 
						})
					// moves the boxes to a new location or to the initial location
					d3.selectAll("rect")
						.transition()
						.duration(500)
						.attr("x", function(d, i) { 
							return d.benchesX * gridSize; 
						})
					if(currentorder[0] != labelorder[0]) {
						sorter(output);
					}
				});
					
			
			
			var	gridSize = 45,
				buckets = 9,
				colors = colorbrewer.Blues[buckets];
			
			var margin = {bottom: 15};
			var maxSize = 0;
			var size = 0;
			
			//define left margin based on TB string length
			for (var i = 0; i < output.configs.length; i++)
			{
				size = output.configs[i].length; //length of the string
				if (maxSize < size)	{
					maxSize = size;
				}
			}
			margin.left = maxSize * 8;
			
			//define top and right margin based on TB string length
			maxSize = 0;
			size = 0;
			for (var i = 0; i < output.benches.length; i++)
			{
				size = output.benches[i].length; //length of the string
				if (maxSize < size)	{
					maxSize = size;
				}
			}
			margin.top = maxSize * 8 * 0.707 + 5;
			margin.right = maxSize * 8 * 0.707 - gridSize / 2;
			
			var height = gridSize * output.configs.length;// - margin.top - margin.bottom;
			var width = gridSize * output.benches.length;
			//var height = Math.pow(output.configs.length, 1.5) * gridSize - margin.top - margin.bottom;
			
			var colorScale = d3.scale.quantile()
				.domain([0, buckets - 1, 1]) //between 0 and 1, in buckets - 1 intervals
				.range(colors);
			
			var svg = d3.select('#'+$(element).attr('id')).append("svg")
				.attr("width", width + margin.left + margin.right)
				.attr("height", height + margin.top + margin.bottom)
				.append("g")
				.attr("transform", "translate(" + margin.left + "," + margin.top + ")");
			
			var yLabels = svg.selectAll(".yLabel")
				.data(output.configs)
				.enter().append("text")
				.text(function (d) { 
					return d; 
				})
				.attr("x", -5)
				.attr("y", function (d, i) { 
					return i * gridSize + gridSize/2; 
				})
				.attr("class", "yLabel")
				.style("text-anchor", "end")
				.on("mouseover", function (d) {
					d3.select(this).style("cursor","move");
				})
				.on("mouseout", function (d) {
					d3.select(this).style("cursor","auto");
				})
				.call(dragy);

			var xLabels = svg.selectAll(".xLabel")
				.data(output.benches)
				.enter().append("text")
				.text(function(d) { 
					return d; 
				})
				.attr("y", -5)
				.attr("x", function(d, i) { 
					//console.log(i * gridSize + gridSize/2); 
					return i * gridSize + gridSize/2; 
				})
				.attr("transform", function(d, i) { 
					//console.log(i * gridSize + gridSize/2); 
					return "rotate(-45 " + String(i * gridSize + gridSize/2) + " 0)"
				})
				.attr("class", "xLabel")
				.on("mouseover", function (d) {
					d3.select(this).style("cursor","move");
				})
				.on("mouseout", function (d) {
					d3.select(this).style("cursor","auto");
				})
				.call(dragx);

			var heatMap = svg.selectAll(".config")
				.data(output.values)
				.enter().append("rect")
				.attr("x", function(d, i) { 
					return d.benchesX * gridSize; 
				})
				.attr("y", function(d, i) { 
					return d.configsY * gridSize;
				})
				.attr("rx", 4)
				.attr("ry", 4)
				.attr("class", "config bordered")
				.attr("width", gridSize)
				.attr("height", gridSize)
				.style("fill", function(d) { 
					
					if (!isSet(d.values))
					{
						return null;
					}
					//else if (d.values == "" || d.values == -1)
					else if (d.values == -1)
					{
						return "#555555";
					}
					else
					{
						if(d.values == "") {
							return "#555555";
						} else {
							return self.icolor(d.values);
						}
					}
				})
				.on("mouseover", function (d) {
					//if (d.values != "" && 
					if (d.values != -1 && isSet(d.values))
					{
						d3.select(this).style("cursor","pointer");
					}
				})
				.on("mouseout", function (d) {
					//if (d.values != "" 
					if (d.values != -1 && isSet(d.values))
					{
						d3.select(this).style("cursor","auto");
					}
				})
				.on("click", function (d) {
					event.cancelBubble = true;
					event.stopPropagation();
					event.preventDefault();

					$(document).trigger($(element).attr('id') + '_click_on_rect', [event ,d]);
					//debugger;
					return false;
				}); 
				//.call(dragx); // this allows the rectangles to be dragged along the x axis
			  
			//Tooltips with the heat values
			heatMap.append("title").text(function(d) { 
				//if (d.values != "" && d.values != -1)
				if (d.values != -1)
				{
					return d.values; 
				}
				else if (d.values == -1)
				{
					return "No PCC Index data";
				}
			});
			
			
			
			/* Mark <vf-table> as rendered */
			$(element).attr('rendered', 'true');
		}
		catch(err)
		{
			$(element).attr('rendered', 'true');
		}
	}
	
	/* Rerender Method */
	this.rerender = function () {
		updateTabSize();
		$(this.element).empty();
		$(element).attr('rendered', 'false');
	}
	
	/* Update Method */
	this.update = function () {
		this.rerender();
	}
}