/*///////////// vf_boxplot /////////////*/
function vf_boxplot(element) {
	
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
				return 'vf-boxplot: already rendered';
			}
			
			var data = clone(eval($(this.element).attr('data')));
			//output = clone(dataHeatMAP); //save "outputs" locally.  Do not use the pass by reference (from the dataprovider)
			var min = d3.min(data),
				max = d3.max(data);
				
			var margin = {top: 10, right: 10, bottom: 10, left: 10},
				width = $(element).width() - margin.left - margin.right,
				height = $(element).attr("h") - margin.top - margin.bottom;
				
			var x = d3.scale.linear()
				.range([0, height])
				.domain([min, max]);
				
			var y = d3.scale.ordinal()
				.rangeRoundBands([0, width], .1)
				.domain([0, height]);
			  
			var svg = d3.select("#" + $(element).attr("id"))
				  .data(data)
				.append("svg")
				  .attr("class", "box")
				  .attr("height", width + margin.left + margin.right)
				  .attr("width", height + margin.bottom + margin.top)
				.append("g")
				  .attr("transform", "translate(" + margin.left + "," + margin.top + ")");

			svg.append("line")
				.attr("x1",function(){ 
					return x(data[0]);
				})
				.attr("y1",0)
				.attr("x2",function(){ 
					return x(data[0]);
				})
				.attr("y2",height);
				
			svg.append("line")
				.attr("x1",function(){ 
					return x(data[1]);
				})
				.attr("y1",0)
				.attr("x2",function(){ 
					return x(data[1]);
				})
				.attr("y2",height);
				
			svg.append("line")
				.attr("x1",function(){ 
					return x(data[2]);
				})
				.attr("y1",0)
				.attr("x2",function(){ 
					return x(data[2]);
				})
				.attr("y2",height);
				
			svg.append("line")
				.attr("x1",function(){ 
					return x(data[3]);
				})
				.attr("y1",0)
				.attr("x2",function(){ 
					return x(data[3]);
				})
				.attr("y2",height);
				
			svg.append("line")
				.attr("x1",function(){ 
					return x(data[4]);
				})
				.attr("y1",0)
				.attr("x2",function(){ 
					return x(data[4]);
				})
				.attr("y2",height);
				
			svg.append("line")
				.attr("x1",function(){ 
					return x(data[1]);
				})
				.attr("y1",0)
				.attr("x2",function(){ 
					return x(data[3]);
				})
				.attr("y2",0);
				
			svg.append("line")
				.attr("x1",function(){ 
					return x(data[1]);
				})
				.attr("y1",height)
				.attr("x2",function(){ 
					return x(data[3]);
				})
				.attr("y2",height);
				
			svg.append("line")
				.attr("x1",function(){ 
					return x(data[0]);
				})
				.attr("y1",height/2)
				.attr("x2",function(){ 
					return x(data[1]);
				})
				.attr("y2",height/2)
				.attr("stroke-dasharray","5,5");
				
			svg.append("line")
				.attr("x1",function(){ 
					return x(data[3]);
				})
				.attr("y1",height/2)
				.attr("x2",function(){ 
					return x(data[4]);
				})
				.attr("y2",height/2)
				.attr("stroke-dasharray","5,5");
			
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
		this.render();
	}
	
	/* Update Method */
	this.update = function () {
		this.rerender();
	}
}