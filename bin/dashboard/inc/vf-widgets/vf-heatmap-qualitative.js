/*///////////// vf-heatmap-qualitative /////////////*/
function vf_heatmap_qualitative(element) {
	
	var self = this;
	//this.data = dataHeatMAP; //eval($(element).attr('data'));
	this.element = element;
	
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
			
			console.debug( output);
			var	gridSize = 45,
				buckets = 9;
				//colors = colorbrewer.Blues[buckets];
			
			var margin = {bottom: 15};
			var maxSize = 0;
			var size = 0;
			
			//define left margin based on TB string length
			for (var i = 0; i < output.configs.length; i++)
			{
				size = output.configs[i].length; //length of the string
				
				if (maxSize < size)
				{
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
				
				if (maxSize < size)
				{
					maxSize = size;
				}
			}
			margin.top = maxSize * 8 * 0.707 + 5;
			margin.right = maxSize * 8 * 0.707 - gridSize / 2;
			
			var height = gridSize * output.configs.length;// - margin.top - margin.bottom;
			var width = gridSize * output.benches.length;
			//var height = Math.pow(output.configs.length, 1.5) * gridSize - margin.top - margin.bottom;
			
			//var colorScale = d3.scale.quantile()
				//.domain([0, buckets - 1, 1]) //between 0 and 1, in buckets - 1 intervals
				//.range(colors);
		
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
				.style("text-anchor", "end");

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
				});
			var color = { "SUCCESS": "#83BE6C", "FAIL":  "#BB1F1F", "UNKNOWN" : "#DBD64B"};//SUCCESS, FAIL, UNKNOWN
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
					return color[d.values]; 
				})
				.on("mouseover", function (d) {
					if (d.values != "" )
					{
						d3.select(this).style("cursor","pointer");
					}
				})
				.on("mouseout", function (d) {
					if (d.values != "" )
					{
						d3.select(this).style("cursor","auto");
					}
				})
				/*.on("mousedown", function (d) {
					if (d.values != "")
					{
						if (!(event.ctrlKey))
						{
							ClickEvent(d.benches, d.configs);
						}
						else
						{
							CtrlClickEvent(d.benches, d.configs, event);
							//$("#contextmenu .dropdown-toggle").dropdown("toggle");
						}
					}
				})*/
				.on("click", function (d) {
					$(document).trigger($(element).attr('id') + '_click_on_rect', [event ,d]);
					//debugger;
					return false;
				});
			  
			//Tooltips with the heat values
			heatMap.append("title").text(function(d) { 
				if (d.values != "" && d.values != -1)
				{
					return d.values; 
				}
				
			});
			
			/* Mark <vf-table> as rendered */
			$(element).attr('rendered', 'true');
		}
		catch(err)
		{
			console.debug( err);
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