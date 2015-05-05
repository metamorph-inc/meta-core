/*///////////// vf-dist /////////////*/
function vf_dist(element){
		var self = this;
		this.data = eval($(element).attr('data'));
		this.element = element;
		var fixedWidth = $(element).parent().width();
		var fixedHeight = $(element).parent().height();
		
	this.render = function () 
	{
		try
		{
			if (!($(element).attr('rendered') == undefined || $(element).attr('rendered') == "false")) 
			{
				return 'vf-dist: already rendered';
			}
			
			if(outputs != "na")
			{
				var fixedWidth = 300;
				var outputs = clone(eval($(this.element).attr('data')));
				var data;
				
				var min; 
				var max;
				if(outputs == "na")
				{
					outputs = {alpha: 9.6, beta: 9.6, hi: 1, lo: 0};
					min = 0;
					max = 1;
				}
				else
				{
					if(outputs.Limits != undefined)
					{
						min = outputs.Limits.Min;
						max = outputs.Limits.Max;
						data = outputs.Plotting;
					}
				}
				var dataset = []; //placeholder for computed dataset points
				var alph = data.alpha;
				var beta = data.beta;
				var hi = 1;
				var lo = 0;
				
				//These are the "actual" ranges, but we need to plot on the range of
				// 0 to 1 in order for the beta distribution to look proper
				var hiTemp = data.hi;
				var loTemp = data.lo;
				
				var points = 250;
				with(Math)
				{
					var betaFUN = (sqrt(2*PI)*pow(alph,alph-.5)*pow(beta,beta-.5))/(pow(alph+beta,alph+beta-.5));
				}
				
				var numOfPoints = points;
				for (i = lo; i < hi; i=i+((hi-lo)/points))	
				{
					with(Math)
					{
						var y = (1/(hi-lo))*(1/betaFUN)*(pow(i,alph-1))*(pow(1-i,beta-1));
					}
					// pushing 'x':i will result works if plotting from 0 to 1
					// to plot against the apparent actual scale, need to account that the intervals
					// of 200 points from 0-1=.005 while the interval from 2-5 of 200 points is .015
					dataset.push({'x' : loTemp+i*(hiTemp-loTemp), 'y' : y});
				}
				
				var margin = {top: 15, right: 15, bottom: 40, left: 20};
				var	width = fixedWidth - margin.left - margin.right;
				var	height = fixedHeight - margin.top - margin.bottom + 150;
				
				var x = d3.scale.linear()
					.domain([dataset[0].x, dataset[dataset.length - 1].x])
					.range([0, width]);
					
				var maxY = dataset[0].y;
				var minY = dataset[0].y;
				for (j= 1; j < dataset.length; j++) {
					if (dataset[j].y > maxY) {
						maxY = dataset[j].y;
					}
					if (dataset[j].y < minY) {
						minY = dataset[j].y;
					}
				}
				
				var y = d3.scale.linear()
					.domain([minY, maxY])
					.range([height, 0]);

				var xAxis = d3.svg.axis()
					.scale(x)
					.orient("bottom")
					.ticks(width/55); //dynamical changes number of ticks based on width of svg
				
				var yAxis = d3.svg.axis()
					.scale(y)
					.orient("left");

				var line = d3.svg.line()
					.x(function(d) { 
						return x(d.x); 
					})
					.y(function(d) { 
						return y(d.y); 
					});

				var area = d3.svg.area()
					.x(line.x())
					.y1(line.y())
					.y0(y(0));
					
				var svg = d3.select("#"+$(element).attr("id")).append("svg") 
					.datum(dataset)
					.attr("width", width + margin.left + margin.right)
					.attr("height", height + margin.top + margin.bottom)
					.append("g")
					.attr("transform", "translate(" + margin.left + "," + margin.top + ")");

				svg.append("path")
					.attr("class", "line")
					.attr("d", area);
				
				// adds in dashed lines for the limits
				var minline;
				var maxline;
				if(min > loTemp && min < hiTemp) {
					minline = ((min-loTemp)/(hiTemp-loTemp))*width;
				} else {
					minline = 0;
				}
				if(max > loTemp && max < hiTemp) {

					maxline = ((max-loTemp)/(hiTemp-loTemp))*width;	
				} else {
					maxline = width;
				}			

				//Generate shaded area under the curve bounded by obj and thresh
				var subsetBegin = null;
				var subsetEnds = null;
				var plotMax = outputs.Limits.Max;
				var plotMin = outputs.Limits.Min;
				
				if(outputs.Limits.Max > outputs.Plotting.hi) {
					plotMax = outputs.Plotting.hi;
				} 
				if(outputs.Limits.Min < outputs.Plotting.lo) {
					plotMin = outputs.Plotting.lo;
				} 
				for (var i = 0; i < dataset.length; i++) {
					if ((dataset[i].x < plotMax && dataset[i].x > plotMin)) {
						if (subsetBegin == null) {
							subsetBegin = i;
						}
						subsetEnds = i;
						dataset[i]["shaded"] = 1;
					} else {
						dataset[i]["shaded"] = 0;
					}
				}
					
				var counter = 0;				
				line = d3.svg.line()
					.x(function(d, i) { 
						if (d.shaded == 1 && i != subsetBegin && i != subsetEnds) {
							return x(d.x);
						} else {
							if (i == subsetBegin) {
								if (plotMin < plotMax) {
									return x(plotMin);
								} else {
									return x(plotMax);
								}
							} else {
								if (plotMin < plotMax) {
									return x(plotMax);
								} else {
									return x(plotMin);
								}
							}
						}
					})
					.y(function(d, i) { 						
						if (d.shaded == 1 && i != subsetBegin && i != subsetEnds) {
							return y(d.y);
						} else {
							return y(0);
						}
					});

				area = d3.svg.area()
					.x(line.x())
					.y(line.y())
					.y0(y(0))
				svg.append("path")
					.attr("d", area)
					.attr("fill", "#ff7100")
					.attr("stroke", "#ff7100");
								
				svg.append("g")
					.attr("class", "x axis")
				.attr("transform", "translate(0," + (height+4) + ")")
					.call(xAxis);

				// creates the title on the x axis
				var meanValue = (alph/(alph+beta)).toFixed(3);

				var xTitle = "Mean ("+(loTemp+meanValue*(hiTemp-loTemp)).toFixed(1)+")";
				svg.append("g")
					.attr("class", "label")
					.attr("id", "xlabel")
					.append("text")
						.text(xTitle)
						.attr("x", (meanValue/(hi-lo)*width)) //x coordinate of the title label
						.attr("y", -5) //y coordinate of the title label
						.attr("text-anchor", "middle")
						.style("font-family", "Verdana")
						.style("font-size", 10);
				
				// creates the line for the mean value
				var vertLineY1 = 0;
				var vertLineY2 = height;

				//creates a ratio between the mean value and the range then translates into pixel value
				var vertLineX = (meanValue/(hi-lo))*width;
				
				//create the max and min lines (need to take in limits as inputs)
				svg.append("g")
					.attr("class", "pareto threshold")
					.append("line")
					.attr("x1", vertLineX)
					.attr("y1", vertLineY1)
					.attr("x2", vertLineX)
					.attr("y2", vertLineY2)
				
				// min limit line
				svg.append("g")
					.attr("class","paretomin")
					.append("line")
					.attr("x1",minline)
					.attr("y1",vertLineY1) 
					.attr("x2",minline)
					.attr("y2",vertLineY2)
				var minTitle = "Min("+(min)+")";
				svg.append("g")
					.attr("class", "distlabel")
					.attr("id", "xlabelmin")
					.append("text")
						.text(minTitle)
						// labels located below the lines: labels can overlap
						//.attr("x", minline) //x coordinate of the title label
						.attr("x", 10)
						.attr("y", height+35) //y coordinate of the title label
						.attr("text-anchor", "middle")
						.style("font-family", "Verdana")
						.style("font-size", 10);

				//max limit line
				svg.append("g")
					.attr("class","paretomax")
					.append("line")
					.attr("x1",maxline)
					.attr("y1",vertLineY1) 
					.attr("x2",maxline)
					.attr("y2",vertLineY2)				
				var maxTitle = "Max("+(max)+")";
				svg.append("g")
					.attr("class", "label")
					.attr("id", "xlabelmax")
					.append("text")
						.text(maxTitle)
						//.attr("x", maxline-10) //x coordinate of the title label
						.attr("x",width-20)
						.attr("y", height+35) //y coordinate of the title label
						.attr("text-anchor", "middle")
						.style("font-family", "Verdana")
						.style("font-size", 10);
				
				//append PCC index label
				svg.append("g")
					.append("text")
					.text("PCC:")
					.attr("x", 200)
					.attr("y", 35)
					.attr("text-anchor", "middle")
					.style("font-family", "Verdana")
					.style("font-size", 10)
					.attr("fill", "white");	
					
				svg.append("g")
					.append("text")
					.text(function(){
						if (data == "na"){
							return 1;
						} else {
							return outputs.PCC.toFixed(5);
						}
					})
					.attr("x", 200)
					.attr("y", 45)
					.attr("text-anchor", "middle")
					.style("font-family", "Verdana")
					.style("font-size", 10)
					.attr("fill", "white");	
				
				//append Complexity index label
				svg.append("g")
					.append("text")
					.text("Complexity:")
					.attr("x", 62)
					.attr("y", 35)
					.attr("text-anchor", "middle")
					.style("font-family", "Verdana")
					.style("font-size", 10)
					.attr("fill", "white");
				
				svg.append("g")
					.append("text")
					.text(function(){
						if (data == "na"){
							return "N/A";
						} else {
							return outputs.Distribution.Complexity.toFixed(5);
						}
					})
					.attr("x", 62)
					.attr("y", 45)
					.attr("text-anchor", "middle")
					.style("font-family", "Verdana")
					.style("font-size", 10)
					.attr("fill", "white");	
				
				/* Mark <vf-dist> as rendered */
				$(element).attr('rendered', 'true');
			}
			/* Mark <vf-dist> as rendered */
			$(element).attr('rendered', 'true');
		}
		catch(err)
		{
			/* Mark <vf-dist> as rendered */
			$(element).attr('rendered', 'true');
		}
	}

	/* Rerender Method */
	this.rerender = function () 
	{
		$(this.element).empty();
		$(element).attr('rendered', 'false');
		this.render();
	}
	
	/* Update Method */
	this.update = function () 
	{
		this.rerender();
	}
}


