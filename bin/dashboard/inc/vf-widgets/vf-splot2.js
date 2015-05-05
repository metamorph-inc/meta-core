/*///////////// vf-splot2 /////////////*/
function vf_splot2(element){
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
		
		if (!($(element).attr('rendered') == undefined || $(element).attr('rendered') == "false")) {
			return 'vf-splot2: already rendered';
		}
		
			var data = eval($(this.element).attr('data'));
			
			var widthpx = $(element).parent().width();
			var heightpx = $(element).parent().width() * (+$(element).attr("AR"));
			
			var margin = {
				top : 10,
				right : 30,
				bottom : 75,
				left : 75
			},
			width = widthpx - margin.left - margin.right,
			height = heightpx - margin.top - margin.bottom;
			
			if (typeof $(element).attr("square") != "undefined" && $(element).attr("square") == "1")
			{
				var max = d3.max([d3.max(data, function(d){
						return d[0];
					}), d3.max(data, function(d){
						return d[1];
					})]);
				
				var min = d3.min([d3.min(data, function(d){
						return d[0];
					}), d3.min(data, function(d){
						return d[1];
					})]);
				
				if (min > max)
				{
					var x = d3.scale.linear()
						.domain([min, max])
						.range([0, width]);
					
					var y = d3.scale.linear()
						.domain([min, max])
						.range([height, 0]);
						//.range([0, height]);
				}
				else
				{
					var x = d3.scale.linear()
						.domain([min, max])
						.range([0, width]);
					
					var y = d3.scale.linear()
						.domain([max, min])
						//.range([height, 0]);
						.range([0, height]);
				}
			}
			else
			{
				var x = d3.scale.linear()
					.domain([0, d3.max(data, function (d) {
						return d[0];
					})])
					.range([0, width]);
				
				var y = d3.scale.linear()
					.domain([0, d3.max(data, function (d) {
						return d[1];
					})])
					.range([height, 0]);
			}

			
			var xAxis = d3.svg.axis()
				.scale(x)
				.orient("bottom");
			
			var yAxis = d3.svg.axis()
				.scale(y)
				.orient("left");
			
			var svg = d3.select('#' + $(element).attr('id')).append("svg")
				.datum(data)
				.attr("width", width + margin.left + margin.right)
				.attr("height", height + margin.top + margin.bottom)
				.append("g")
				.attr("transform", "translate(" + margin.left + "," + margin.top + ")");
			
			svg.append("g")
				.attr("class", "x axis")
				.attr("transform", "translate(0," + height + ")")
				.call(xAxis);
			
			//X-axis data label
			svg.append("g")
				.attr("class", "twodscatterplotaxislabel")
				.attr("id", "xlabel")
				.append("text")
				.text(function () {
					if (typeof $(element).attr("xlabel") != "undefined")
					{
						var text = $(element).attr("xlabel");
						
						if (typeof $(element).attr("xunit") != "undefined") 
						{
							return text += " (" + $(element).attr("xunit") + ")";
						} 
						else 
						{
							return text;
						}
					}
				})
				.attr("x", width / 2)
				.attr("y", height + 40 + 15)
				.attr("text-anchor", "middle");
			
			svg.append("g")
				.attr("class", "y axis")
				.call(yAxis);
			
			//Y-axis data label
			svg.append("g")
				.attr("class", "twodscatterplotaxislabel")
				.attr("id", "ylabel")
				.append("text")
				.text(function () {
					if (typeof $(element).attr("ylabel") != "undefined")
					{
						var text = $(element).attr("ylabel");
						
						if (typeof $(element).attr("yunit") != "undefined") 
						{
							return text += " (" + $(element).attr("yunit") + ")";
						} 
						else 
						{
							return text;
						}
					}
				})
				.attr("transform", "rotate(-90 0 0)")
				.attr("x", -height / 2)
				.attr("y", -40 - 20)
				.attr("text-anchor", "middle");				
			
			//ADD THE DATA POINTS (BLUE CIRECLES)
			svg.selectAll(".dot") //select the class "dot"
				.data(data)
				.enter().append("circle")
				.attr("class", function (d) {
					if(typeof d.x != 'undefined'){
						return "leader " + d.cid.split(' ').join('');
					}
				})
				.attr("cx", function (d) {
					return x(d[0]);
				})
				.attr("cy",function (d) {
					return y(d[1]);
				})
				.attr("r", function (d) {
					return $(element).attr("r");
				});
			
			if(typeof $(element).attr("45degline") != "undefined")
			{
				if ($(element).attr("45degline") == "1")
				{
					svg.append("line")
						.attr("x1",function(){ 
							return x(min);
						})
						.attr("y1",function(){
							return y(min);
						})
						.attr("x2",function(){ 
							return x(max);
						})
						.attr("y2", function(){
							return y(max);
						})
						.style("stroke", "red")
						.style("shape-rendering", "crispEdges");
				}
			}
			
			if(typeof $(element).attr("zeroline") != "undefined")
			{
				if ($(element).attr("zeroline") == "1")
				{
					svg.append("line")
						.attr("x1",function(){ 
							return x(min);
						})
						.attr("y1",function(){
							return y(0);
						})
						.attr("x2",function(){ 
							return x(max);
						})
						.attr("y2", function(){
							return y(0);
						})
						.style("stroke", "red")
						.style("shape-rendering", "crispEdges");
				}
			}
			
			$("#" + $(element).attr("id") + " .x text, #" + $(element).attr("id") + " .y text").each(function(d){
				if (+$(this).text() < 0.001 || +$(this).text() > 999)
				{
					var a = +$(this).text();
					
					$(this).text(a.toExponential(2));
				}
			});

			
			d3.selectAll("#" + $(element).attr("id") + " .x text").attr("text-anchor", "end")
			d3.selectAll("#" + $(element).attr("id") + " .x text").attr("transform", "rotate(-45 0 0) translate(-15, 0)")
			
			/* Mark <vf-table> as rendered */
			$(element).attr('rendered', 'true');
	}
	
	/* Rerender Method */
	this.rerender = function () {
		updateTabSize();
		$(this.element).empty();
		$(element).attr('rendered', 'false');
		this.render();
	}
	
	/* Update Method */				
	this.update = function() {
		this.rerender();
	}
}