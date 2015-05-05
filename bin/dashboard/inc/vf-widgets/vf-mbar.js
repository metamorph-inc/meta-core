/*///////////// vf-mbar /////////////*/
function vf_mbar(element){

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
			return 'vf-mbar: already rendered';
		}
		
			var outputs = eval($(this.element).attr('data'));
				//define padding and plot dimentions
				var mainPadding = {left: 150, right: 75, top: 10, bottom: 50};
				var width = $(element).parent().width() - mainPadding.left - mainPadding.right;
				
				if (width < 0)
				{
					width = 200; //realistically, no one will make the window so small, that at 200px, it  looks bad.... MJD
				}
				var height = Math.max(100,30*outputs.designs.length);
				//var height = ($(element).parent().width() * 0.3) - mainPadding.top - mainPadding.bottom;

				var plotPadding = 30; //padding between each plot

				var numberOfPlots = outputs.labels.length;

				//If multiple plots exist, then calculate a summation 
				if(false)//if (numberOfPlots > 1)
				{
					var sum = 0;
					var sumArray = [];

					//go through each data set and add each element to form the added 
					for (var j = 0; j < outputs.data[0].length; j++)
					{
						for (i = 0; i < outputs.data.length; i++)
						{
							sum += outputs.data[i][j].y;
						}
						
						sumArray.push(sum);
						sum = 0;
					}
					
					for (var i in sumArray)
					{
						sumArray[i] = {x: outputs.data[0][i].x , y: sumArray[i], i:outputs.data[0][i].i};
					}
					
					numberOfPlots++; //add +1 because of the addition of the Total plot
					
					//Append the summation array into the outputs.data array
					outputs.data.push(sumArray); 
				}

				//the actual width of each plot
				var plotWidth = 0;

				if (numberOfPlots > 1) //if more than one plot
				{
					plotWidth = (width + (1 - numberOfPlots) * plotPadding) / numberOfPlots;
				}
				else //if its only one plot
				{
					plotWidth = width;
				}

				//where each plot starts and ends
				var firstPadding = 1;

				var dimensions = [];
				dimensions.push([firstPadding, plotPadding + plotWidth]);

				for (var i = 0; i < numberOfPlots; i++)
				{
					dimensions.push([dimensions[i][1] + plotPadding, dimensions[i][1] + plotPadding + plotWidth]);
				}

				//----------begin plotting----------
				//select desired tag and append svg tag to it
				var svg = d3.select("#" + $(element).attr("id")).append("svg")
							.attr("width", width + mainPadding.right + mainPadding.left)
							.attr("height", height + mainPadding.top + mainPadding.bottom)
						.append("g")
							.attr("transform", "translate(" + mainPadding.left + "," + mainPadding.top + ")");

				//all needed variables to plot
				var x;
				var y;
				var xAxis;
				var yAxis;
				var bar;

				var colorPaletts = d3.scale.category20c();
				var colorRange = [];

				for (var i in colorPaletts){
					colorRange.push(colorPaletts(i));
				}

				var counter = 0;

			//loop through every plot
			for (var i = 0; i < numberOfPlots; i++){
					x = d3.scale.linear()
							.domain([0, d3.max(outputs.data[i], function(d) { 
								return d.y; 
							})])
							.range([dimensions[i][0], dimensions[i][1]]);
					
					y = d3.scale.ordinal()
							.domain(outputs.data[i].map(function(d) { 
								return d.i; 
							}))
							.rangeRoundBands([0, height], .1);
					
					xAxis = d3.svg.axis()
						.scale(x)
						.orient("bottom")
						.ticks(3)
						.tickSize(5);
					
					yAxis = d3.svg.axis()
						.scale(y)
						.orient("left")
						.tickSize(5);
					
					if (i == 0)
					{
						//Render the bar for the specificed plot
						bar = svg.selectAll("g.bar" + i)
									.data(outputs.data[i])
								.enter().append("g")
									.attr("class", function(d) { 
										return "leader " + d.x; 
									})
									.attr("fill", colorRange[i])
									.attr("transform", function(d) { 
										return "translate(" + dimensions[i][0] + "," + y(d.i) + ")"; 
									})
									.on("click", function(d){
										//toggle own class
										//console.log(d);
										/* var selection = d3.selectAll("#" + $(element).attr("id") + " ." + d.x);
										
										if ($("#" + $(element).attr("id") + " ." + d.x).attr("class").split(" ")[2] == undefined)
										{
											selection.classed("selected", true);
										}
										else
										{
											selection.classed("selected", false);
										} */
										
										//event for other widgets
										$(document).trigger('LBselectionChange', {
											"widget" : "history",
											"eventForWidget" : ["scoresSplot"],
											"selected" : d.x
										});									
									});
						
						bar.append("rect")
							.attr("width", function(d) { 
								return x(d.y) - firstPadding; 
							})
							.attr("height", y.rangeBand());
					}
					else
					{
						//Render the bar for the specificed plot
						bar = svg.selectAll("g.bar"+i)
									.data(outputs.data[i])
								.enter().append("g")
									.attr("class", function(d) { 
										return "leader " + d.x; 
									})
									.attr("fill", colorRange[i])
									.attr("transform", function(d) { 
										return "translate(" + dimensions[i][0] + "," + y(d.i) + ")"; 
									});
						
						bar.append("rect")
							.attr("width", function(d) { 
								return x(d.y) - dimensions[i][0]; 
							})
							.attr("height", y.rangeBand());
					}
					
					//add data label
					bar.append("text")
						.attr("class", "value")
						.attr("x", function(d) { 
							return x(d.y) - dimensions[i][0] + plotPadding; 
						})
						.attr("y", y.rangeBand() / 2)
						.attr("dx", -plotPadding)
						.attr("dy", ".35em")
						.attr("text-anchor", "start")
						.text(function(d) { 
							return d.y; 
						});
					
					//Render the bar x axis
					svg.append("g")
						.attr("class", "x axis" + i)
						.attr("transform", "translate(0," + height + ")")
						.call(xAxis);
					
					//rotate all x axis labels by -45deg
					svg.selectAll("g.x.axis" + i + " > g > text")
						.attr("text-anchor", "end")
						.attr("transform", "translate(-3, 0), rotate(-45 0 0)");
					
					//add labels
					svg.selectAll("g.x.axis" + i).append("text")
						//.text("Plot " + (i + 1))
						.data(outputs.designs)
						.text(function(d) {
							return "TEST"; 
							})
						.text(function(d){
							if(false)//if (i == numberOfPlots - 1)
							{
								return "Total";
							}
							else
							{
								return outputs.labels[i];
							}
						})
						.attr("text-anchor", "middle")
						.attr("x", (dimensions[i][0] + dimensions[i][1])/2)  //center the label
						.attr("y", 45);
						
						//adds the x axis label
						/*svg.append("g")
						.attr("class","label")
						.attr("id","xlabel")
						.append("text")
							.text(outputs.labels[i])
							.attr("x",width/2+10)
							.attr("y",height+45)
							.attr("text-anchor","middle");
							*/			
												
				if (i == 0)
				{
					//Render the bar y axis
					svg.append("g")
						.attr("class", "y axis")
						.call(yAxis);
						
					$("#" + $(element).attr("id") + " g.y.axis g text").each(function(i,element){
						for (var counter = 0; counter < outputs.data[0].length; counter++)
						{
							if($(this).text() == outputs.data[0][counter].i)
							{
								$(this).text(outputs.data[0][counter].x);
							}
						}
					}); 
					
					//add the y axis' data label
					svg.selectAll("g.y.axis").append("text")
						.attr("class", "ylabel")
						.text(outputs.xLabel)
						.attr("text-anchor", "middle")
						.attr("transform", "rotate(-90, 0, 0)")
						.attr("x", -height / 2) //center the label
						.attr("y", -mainPadding.left + 10); 
				}
			}
			
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
	this.update = function () {
		this.rerender();
	}
}