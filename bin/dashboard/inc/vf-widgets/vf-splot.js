/*///////////// vf-splot /////////////*/
function vf_splot(element){
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
			return 'vf-splot: already rendered';
		}
		
			var data = eval($(this.element).attr('data'));
			
			var widthpx = $(element).parent().width();
			var heightpx = $(element).parent().width() * 0.45;
			
			var margin = {
				top : 50,
				right : 30,
				bottom : 50,
				left : 50
			},
			width = widthpx - margin.left - margin.right,
			height = heightpx - margin.top - margin.bottom;
			
			var x = d3.scale.linear()
				.domain([0, d3.max(data.data, function (d) {
							return d.x;
				})])
				.range([0, width]);
			
			var y = d3.scale.linear()
				.domain([0, d3.max(data.data, function (d) {
							return d.y;
				})])
				.range([height, 0]);
			
			var xAxis = d3.svg.axis()
				.scale(x)
				.orient("bottom");
			
			var yAxis = d3.svg.axis()
				.scale(y)
				.orient("left");
			
			var svg = d3.select('#' + $(element).attr('id')).append("svg")
				.datum(data.data)
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
					var text = data.labels.x;
					
					if (!(data.units.x == "")) {
						return text += " (" + data.units.x + ")";
					} else {
						return text;
					}
				})
				.attr("x", width / 2)
				.attr("y", height + 40)
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
					var text = data.labels.y;
					
					if (!(data.units.y == "")) {
						return text += " (" + data.units.y + ")";
					} else {
						return text;
					}
				})
				.attr("transform", "rotate(-90 0 0)")
				.attr("x", -height / 2)
				.attr("y", -40)
				.attr("text-anchor", "middle");				
			
			var maxRadius = 15;
			var maxTotal = d3.max(data.data, function (d) {
					return d.total;
				});
			
			//ADD THE DATA POINTS (BLUE CIRECLES)
			svg.selectAll(".dot") //select the class "dot"
				.data(data.data)
				.enter().append("circle")
				.attr("class", function (d) {
					if(typeof d.x != 'undefined'){
						return "leader " + d.cid.split(' ').join('');
					}
				})
				.attr("cx", function (d) {
					return x(d.x);
				})
				.attr("cy",function (d) {
					return y(d.y);
				})
				.attr("r", function (d) {
					if ($(element).attr('relativeradius') == "0")
					{
						return 2;
					}
					else
					{
						return d.total / maxTotal * maxRadius;
					}
				})
				.on("mouseover", function (d) {
					svg.append("text")
					.attr("class", "designID")
					.attr("x", function () {
						return x(d.x) + 5;
					}) //mouse location (x) minus margin
					.attr("y", function () {
						return y(d.y) - 5;
					}) //mouse location (y) minus margin and some padding for better text display
					.attr("fill", "white")
					.text(d.designId);
				})
				.on("mouseout", function (d) {
					//remove the text element added on the mouseover event when the mouseout event is triggered
					$("text.designID").remove();
				})
				.on("click", function (d) {
					//event for other widgets
					$(document).trigger('LBselectionChange', {
						"widget" : "scoresSplot",
						"eventForWidget" : ["history"],
						"selected" : d.cid
					});
				});
			
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
		var data = eval($(this.element).attr('data'));
		var widthpx = $(element).parent().width();
		var heightpx = $(element).parent().width() * 0.45;
		
		var margin = {
			top : 50,
			right : 30,
			bottom : 50,
			left : 50
		},
		width = widthpx - margin.left - margin.right,
		height = heightpx - margin.top - margin.bottom;
		
		var transDuration = 2500;
		
		//select the whole element to be transitioned
		var svg = d3.select('#' + $(element).attr('id'));
		
		//Define the transition (in ms)
		var transition = svg.transition().duration(transDuration);
		
		//Redefine the domain of the x scale (range remains constant)
		var x = d3.scale.linear()
			.domain([0, d3.max(data.data, function (d) {
						return d.x;
					})])
			.range([0, width]);
		
		//Redefine the domain of the y scale (range remains constant)
		var y = d3.scale.linear()
			.domain([0, d3.max(data.data, function (d) {
						return d.y;
					})])
			.range([height, 0]);
		
		//Redefine the xAxis with the X scale
		var xAxis = d3.svg.axis()
			.scale(x)
			.orient("bottom");
		//Redefine the xAxis with the Y scale
		
		var yAxis = d3.svg.axis()
			.scale(y)
			.orient("left");
		
		//apply the transition to the NEW xAxis (xAxis is a function of x, which its domain was redefined above)
		transition.select('#' + $(element).attr('id') + " g.x.axis").call(xAxis);
		//apply the transition to the NEW yAxis (yAxis is a function of y, which its domain was redefined above)
		transition.select('#' + $(element).attr('id') + " g.y.axis").call(yAxis);
		
		//Select ALL circles and point it to the newly defined data variable
		var circles = d3.selectAll('#' + $(element).attr('id') + " g circle");
		
		circles.data(data.data);
		
		//Redefine the maxRadius and maxTotal
		var maxRadius = 15;
		var maxTotal = d3.max(data.data, function (d) {
				return d.total;
			});
			
		//Apply the new positions to the circles with a transition
		//and see if the radii changed at all
		circles.transition().duration(transDuration)
			//.data(data.data)
			.attr("cx", function (d) {
				return x(d.x);
			})
			.attr("cy", function (d) {
				return y(d.y);
			})
			.attr("r", function (d) {
				//console.log(d);
				
				return d.total / maxTotal * maxRadius;
			});
		
		//Select the text of the X axis label
		var xText = d3.select('#' + $(element).attr('id') + " #xlabel text");
		//Update the text on the X Axis
		xText.transition()
			.text(data.labels.x);
		
		//Select the text of the Y axis label
		var yText = d3.select('#' + $(element).attr('id') + " #ylabel text")
		//Update the text on the Y Axis
		yText.transition()
			.text(data.labels.y);
	};
}