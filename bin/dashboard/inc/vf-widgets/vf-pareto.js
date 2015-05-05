/*///////////// vf-pareto /////////////*/
function vf_pareto(element) {
		var self = this;
		this.data = eval($(element).attr('data'));
		this.element = element;

		
	/* Render Method */
	this.render = function () {
			
		//var fixedWidth = 400;
		var fixedHeight = $(element).parent().height();
		
		/* 
		Check if element has already been rendered 
		if it has then skip it
		if it has not render it
		*/
		if (!($(element).attr('rendered') == undefined || $(element).attr('rendered') == "false")) {
			return 'vf-pareto: already rendered';
		}
		
			var outputs = eval($(this.element).attr('data'));
			var data = outputs;
			
			
		if(outputs.length != 0)
		{
		
			// modified to resize the pareto chart text
			var margin = {
				top : 30,
				right : 25,
				bottom : -20,
				left : 150
			};
			
			var fixedWidth = $(element).parent().width() - 20;
			var minWidth = +$(element).attr("minwidth");
			
			if (minWidth != undefined)
			{
				width = minWidth - margin.right - margin.left;
			}
			else
			{
				width = fixedWidth - margin.right - margin.left;
			}
		
			// changed $(element).parent().width() to 200 in order to fix 
			// the rendering upon configuration selection change
			
			height = fixedHeight - margin.top - margin.bottom;
			
			// resizes the Pareto height based on the number of parameters
				height = data.length*15+100;
			
			var format = d3.format(",.2f");  // formats to 2 decimal places
			
			var x = d3.scale.linear()
				.range([0, width]);
			
			var y = d3.scale.ordinal()
				.rangeRoundBands([0, height], .1);
						
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
			
			//select the div tag to append all SVG to
			var svg = d3.select('#' + $(element).attr('id')).append("svg")
				.attr("width", width + margin.right + margin.left)
				.attr("height", height + margin.top + margin.bottom + 50)
				.append("g")
				.attr("transform", "translate(" + margin.left + "," + margin.top + ")");
			
			var sum = 0;
			var cumulativePercent = [];
			var indexCounter = [];
			
			// Parse numbers, sort by value, sum all data, and find the relative cumulative percent
			data.forEach(function (d) {
				d.value = +d.value;
			});
			data.sort(function (a, b) {
				return b.value - a.value;
			});
			data.forEach(function (d) {
				return sum += +d.value;
			});
			cumulativePercent.push({
				"x" : data[0].value / sum * 100,
				"y" : data[0].name
			});
			//indexCounter.push({'index' : 0});
			for (var i = 1; i < data.length; i++) {
				
				cumulativePercent.push({
					"x" : cumulativePercent[cumulativePercent.length - 1].x + (data[i].value / sum) * 100,
					"y" : data[i].name
				});
				//indexCounter.push({'index' : i});
			}

			// Set the scale domain.
			x.domain([0, sum]);
			y.domain(data.map(function (d) {
					return d.name;
				}));
			
			//Make the bars
			var bar = svg.selectAll("g.bar")
				.data(data)
				.enter().append("g")
				.attr("class", "bar")
				.attr("transform", function (d) {
					return "translate(0," + y(d.name) + ")";
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
				return format(d.value)
			});
			
			//Render the bar x axis
			svg.append("g")
			.attr("class", "x axis")
			.call(xAxis);
			
			//Render the bar y axis
			svg.append("g")
			.attr("class", "y axis")
			.call(yAxis);
			
			//Render the pareto x axis
			svg.append("g")
			.attr("transform", "translate(0," + height + ")")
			.attr("class", "x pareto axis")
			.call(xPercentAxis);
			
			//svg.data(cumulativePercent);
			
			var line = d3.svg.line()
				.x(function (d) {
					return xPareto(d.x);
				}) //use the x axis from the PARETO plot
				.y(function (d) {
					return y(d.y);
				}); //use the y axis from the BAR plot (same as PARETO Y axis!)
			
			/*svg.append("path")
			.attr("class", "line")
			.attr("d", line);  */
			
			svg.selectAll(".dot")
			.data(cumulativePercent)
			.enter().append("circle")
			.attr("class", "dot")
			.attr("cx", line.x())
			.attr("cy", line.y())
			.attr("r", 3.5);
			
			var thresholdIndex = [];
			
			/*
			for (i in cumulativePercent) {
				console.log("TESTTEST");
				console.log(cumulativePercent[i].x)
				if (cumulativePercent[i].x <= 80) {
					thresholdIndex = i;
									console.log(thresholdIndex);
				}
				else 
				{		if(i==0){ //incase there is only 1 parameter for the metric
						thresholdIndex = 0; }
				}
			}*/
			
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
			
			svg.append("g")
			.attr("class", "pareto threshold")
			.append("line")
			.attr("x1", horiLineX1)
			.attr("y1", horiLineY)
			.attr("x2", horiLineX2)
			.attr("y2", horiLineY)
			
			svg.append("g")
			.attr("class","label")
			.attr("id","xlabel")
			.append("text")
				.text("Percentage")
				.attr("x",width/2)
				.attr("y",height+27)
				.attr("text-anchor","middle")
				.style("font-family","Verdana")
				.style("font-size",10);
		
			svg.append("g")
			.attr("class","label")
			.attr("id","xlabel")
			.append("text")
				.text("Metric")
				.attr("x",width/2)
				.attr("y",0-20)
				.attr("text-anchor","middle")
				.style("font-family","Verdana")
				.style("font-size",10);
		
		}
			/* Mark <vf-pareto> as rendered */
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
	this.update = function () {
		this.rerender();
	}
	
}