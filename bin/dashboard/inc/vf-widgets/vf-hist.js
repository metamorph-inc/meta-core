// /*///////////// vf-hist /////////////*/
// function vf_hist(element) {
	// var self = this;
	// this.data = dataHeatMAP; //eval($(element).attr('data'));
	// this.element = element;
	
	// /* Render Method */
	// this.render = function () {
		// try
		// {
			// /* 
			// Check if element has already been rendered 
			// if it has then skip it
			// if it has not render it
			// */
			// /* if (!($(element).attr('rendered') == undefined || $(element).attr('rendered') == "false")) 
			// {
				// return 'vf-hist: already rendered';
			// }
			
			// var outputs = eval($(this.element).attr('data'));
			
			// if(outputs.length != 0)
			// {
				// //A formatter for counts.
				// var formatCount = d3.format(",.0f");

				// var margin = {top: 10, right: 30, bottom: 30, left: 30},
					// width = $(element).parent().height() - margin.left - margin.right,
					// height = width * $(element).attr("AR") - margin.top - margin.bottom;

				// var x = d3.scale.linear()
					// .domain([d3.min(outputs)*0.9, d3.max(outputs)*1.1])
					// .range([0, width]);

				// //Generate a histogram using twenty uniformly-spaced bins.
				// var data = d3.layout.histogram()
					// .bins(x.ticks(20))
					// (outputs);

				// var y = d3.scale.linear()
					// .domain([0, d3.max(data, function(d) { 
						// return d.y; 
					// })])
					// .range([height, 0]);

				// var xAxis = d3.svg.axis()
					// .scale(x)
					// .orient("bottom");

				// var svg = d3.select('#' + $(element).attr('id')).append("svg")
					// .attr("width", width + margin.left + margin.right)
					// .attr("height", height + margin.top + margin.bottom)
				  // .append("g")
					// .attr("transform", "translate(" + margin.left + "," + margin.top + ")");

				// var bar = svg.selectAll(".bar")
					// .data(data)
				  // .enter().append("g")
					// .attr("class", "bar")
					// .attr("transform", function(d) { 
						// return "translate(" + x(d.x) + "," + y(d.y) + ")"; 
					// });

				// bar.append("rect")
					// .attr("x", 1)
					// .attr("width", x(data[0].dx) - 1)
					// .attr("height", function(d) { 
						// return height - y(d.y); 
					// });

				// bar.append("text")
					// .attr("dy", ".75em")
					// .attr("y", 6)
					// .attr("x", x(data[0].dx) / 2)
					// .attr("text-anchor", "middle")
					// .text(function(d) { 
						// return formatCount(d.y); 
					// });

				// svg.append("g")
					// .attr("class", "x axis")
					// .attr("transform", "translate(0," + height + ")")
					// .call(xAxis);
			// } */
						
			// /* Mark <vf-table> as rendered */
			// $(element).attr('rendered', 'true');
		// }
		// catch(err)
		// {
			// $(element).attr('rendered', 'true');
		// }
	// }
	
	// /* Rerender Method */
	// this.rerender = function () {
		// updateTabSize();
		// $(this.element).empty();
		// $(element).attr('rendered', 'false');
		// this.render();
	// }
	
	// /* Update Method */
	// this.update = function () {
		// this.rerender();
	// }
// }

////////////// vf-widgetname /////////////
function vf_hist(element)
{
	var self = this;
	//this.data = eval($(element).attr('data'));
	this.element = element;
	
	//Render Method
	this.render = function () 
	{
		try
		{
			/* 
			Check if element has already been rendered 
			if it has then skip it
			if it has not render it
			*/
			if (!($(element).attr('rendered') == undefined || $(element).attr('rendered') == "false")) 
			{
				return 'vf-hist: already rendered';
			}
			
			var outputs = eval($(this.element).attr('data'));
			
			if(outputs.length != 0)
			{
				//A formatter for counts.
				var formatCount = d3.format(",.0f");

				var margin = {top: 10, right: 30, bottom: 75, left: 30},
					width = $(element).parent().width() - margin.left - margin.right,
					height = width * (+$(element).attr("AR")) - margin.top - margin.bottom;

				var x = d3.scale.linear()
					.domain([d3.min(outputs)*0.5, d3.max(outputs)*1.5])
					.range([0, width]);
				
				var numberOfBins = 10;
				//Generate a histogram using uniformly-spaced bins.
				var data = d3.layout.histogram()
					.bins(x.ticks(numberOfBins))
					(outputs);

				var y = d3.scale.linear()
					.domain([0, d3.max(data, function(d) { 
						return d.y; 
					})])
					.range([height, 0]);

				var xAxis = d3.svg.axis()
					.scale(x)
					.orient("bottom");

				var svg = d3.select('#' + $(element).attr('id')).append("svg")
					.attr("width", width + margin.left + margin.right)
					.attr("height", height + margin.top + margin.bottom)
				  .append("g")
					.attr("transform", "translate(" + margin.left + "," + margin.top + ")");

				var bar = svg.selectAll(".bar")
					.data(data)
				  .enter().append("g")
					.attr("class", "bar")
					.attr("transform", function(d) { 
						return "translate(" + x(d.x) + "," + y(d.y) + ")"; 
					});

				bar.append("rect")
					.attr("x", 1)
					//.attr("width", x(data[0].dx) - 1)
					.attr("width", width / numberOfBins)
					.attr("height", function(d) { 
						return height - y(d.y); 
					});

				bar.append("text")
					.attr("dy", ".75em")
					.attr("y", 6)
					.attr("x", width / (2* numberOfBins))
					.attr("text-anchor", "middle")
					.text(function(d) { 
						if (formatCount(d.y) != 0)
						{
							return formatCount(d.y); 
						}
						else
						{
							return "";
						}
					});

				svg.append("g")
					.attr("class", "x axis")
					.attr("transform", "translate(0," + height + ")")
					.call(xAxis);
					
				$("#" + $(element).attr("id") + " .x text, #" + $(element).attr("id") + " .y text").each(function(d){
					if (+$(this).text() < 0.001 || +$(this).text() > 999)
					{
						var a = +$(this).text();
						
						$(this).text(a.toExponential(2));
					}
				});
				
				d3.selectAll("#" + $(element).attr("id") + " .x text").attr("text-anchor", "end");
				d3.selectAll("#" + $(element).attr("id") + " .x text").attr("transform", "rotate(-45 0 0) translate(-15, 0)");
			}
					
			/* Mark <vf-table> as rendered */
			$(element).attr('rendered', 'true');
		}
		catch(err)
		{
			$(element).attr('rendered', 'true');
		}
		
		//Mark <vf-table> as rendered
		$(element).attr('rendered', 'true');
	}
	
	//Rerender Method
	this.rerender = function () 
	{
		updateTabSize();
		$(this.element).empty();
		$(element).attr('rendered', 'false');
		this.render();
	}
	
	//Update Method
	this.update = function () 
	{
		this.rerender();
	}
}