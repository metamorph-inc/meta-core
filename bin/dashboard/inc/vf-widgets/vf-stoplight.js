/*///////////// vf-stoplight /////////////*/
function vf_stoplight(element) {
	var self = this;
	this.data = eval($(element).attr('data'));
	this.element = element;
	this.renderCount = 0;
	
	/* Render Method */
	this.render = function () {
		
		/* 
		Check if element has already been rendered 
		if it has then skip it
		if it has not render it
		*/
		
		if (!($(element).attr('rendered') == undefined || $(element).attr('rendered') == "false")) {
			return 'vf-stoplight: already rendered';
		}
		
			this.data = eval($(this.element).attr('data'));
			
			/* INTERNAL CODE */
			
			//for (var i = 1; i <= 10000000; i += 1) {};
			
			var w = $(element).parent().width(),
				h = 860,
				x = d3.scale.linear().range([0, w]),
				y = d3.scale.linear().range([0, h]),
				plot_x = 60,
				plot_x_margin = 15,
				plot_y = 60;
					
			var vis = d3.select('#' + $(element).attr('id')).append("div")
				.attr("class", "chart")
				.style("width", w + "px")
				.style("height", h + "px")
				.append("svg:svg")
				.attr("width", w)
				.attr("height", h);
			
			var partition = d3.layout.partition()
				.value(function (d) {
					return d.size;
				});
			
			//if(typeof data.children != "undefined"){
			//	root = data}
			//else{
			//	var root = this.data};
			
			root = this.data;
			
			//console.log('vf-stoplight: data',root);
			
		
			if((typeof root.children == "undefined" && this.renderCount < 24)|| this.renderCount < 2){
				//console.log('vf-stoplight: bad data', root);
				window.setTimeout(function(){self.rerender();},250);
				this.renderCount++;
				if(typeof root.children == "undefined" && this.renderCount == 24){
					//console.log('vf-stoplight: bad data and giving up after one last try!!!', root);
				}
			}
			
			//d3.json("requirements.json", function (root) {
				
				//Basic setup of window and creation of data
				
				var g = vis.selectAll("g")
					.data(setupData())
					.enter().append("svg:g");
				g.attr("transform", function (d) {
					return "translate(" + x(d.y) + "," + y(d.x) + ")";
				})
				.attr("class", function (d) {
					// added if statement to overide objective, threshold, function if clickboc set to true -df
					if(!d.function || d.clickbox){
						d.objective = 1;
						d.threshold = 1;
						return "InactivePlotRect";
					}
					else{
						return "PlotRect";
					}
			});
			
			vis.selectAll(".InactivePlotRect").attr("class", function (d) {
				return d.clickbox ? "ClickBoxRect" : "InactiveRect"
			})
			
			var kx = w / root.dx, //root.dx = 1, so this is just w.
			ky = h / 1;
			
			//Add all elements
	
			vis.selectAll(".InactiveRect").append("svg:rect")
			.attr("width", function (d) {return d.dy * kx;})
			.attr("height", function (d) {return d.dx * ky;})
			.attr("class", function (d) {return d.children ? "parent" : "child";})
			.style("fill", function (d) {return d.color})
			.on("click", click);
			
			vis.selectAll(".PlotRect").append("svg:rect")
			.attr("width", function (d) {return d.dy * kx;})
			.attr("height", function (d) {return d.dx * ky;})
			.attr("class", function (d) {return d.children ? "parent" : "child";})
			.style("fill", function (d) {return d.color})
			.on("click", click);
			
			vis.selectAll(".InactiveRect").append("svg:text")
			.attr("class", function (d) {return d.unit ? "TextWithVals" : "TextNoVals"})
			.attr("transform", "translate(8,16)")
			.attr("dy", ".35em")
			.each(resize)
			.append("tspan")
			.attr("font-weight", "bold")
			.attr("x", 0)
			.text(function (d) {return d.name})
			
			vis.selectAll(".PlotRect").append("svg:text")
			.attr("class", function (d) {return d.unit ? "TextWithVals" : "TextNoVals"})
			.attr("transform", "translate(8,16)")
			.attr("dy", ".35em")
			.each(resize)
			.append("tspan")
			.attr("font-weight", "bold")
			.attr("x", 0)
			.text(function (d) {
				return d.name;
			})
			
			var g2 = vis.selectAll(".ClickBoxRect").append("svg:g");
				//.on("click", click);
				
			g2.append("svg:rect")
				.attr("width", function(d) {return d.dy * kx; })
				.attr("height", function(d) { return d.dx * ky; })
				.attr("class", function(d) { return d.children ? "parent" : "child"; })
				.style("fill", function(d) { return d.color})
				.on("click", click);
			  
			g2.append("svg:text")
				.attr("class", function(d) {return d.unit ? "TextWithVals" : "TextNoVals"})
				.attr("transform", "translate(8,16)")
				.attr("dy", ".35em")
				.each(resize)
				.append("tspan")
				.attr("font-weight","bold")
				.attr("x",0)
				.text(function(d) { return d.name; })
			
			g.selectAll(".TextWithVals")
				.each(addMainText)
				.each(checkLength)
			
			g.select(".InactivePlotRect")
				.style("opacity", 0);
			
			g.selectAll(".InactiveMvals")
				.style("opacity", 0);
			
			vis.selectAll(".PlotRect").append("svg:g")
				.attr("transform", function (d) {return "translate(" + (d.dy * kx - d.dx * ky * 0.95) + "," + (d.dx * ky * 0.05) + ")";})
				.attr("class", "Plot")
				.each(makePlot);
			
			vis.selectAll(".ClickBoxRect").append("svg:g")
			.attr("transform", function (d) {return "translate(" + (d.dy * kx - d.dx * ky * 0.95) + "," + (d.dx * ky * 0.25) + ")";})
			.attr("class", "ClickBox")
			.attr("opacity", function (d) {return (d.dx * ky > 100 ? 1 : 0)})
			.each(makeClickBox);
			
			//Set up corner box to rewind to top level
			
			var cornerBox = vis.append("g").attr("class", "cornerBox")
				.attr("transform", function () {return ("translate(0," + (h - 38) + ")")})
				.on("click", function () {click(root);});
			
			var c_scale = d3.scale.linear().range([0, 38])
				
				cornerBox.append("rect")
				.attr("width", c_scale(1))
				.attr("height", c_scale(1));
			
			var arrowData = [[0.1, 0.5], [0.4, 0.9], [0.4, 0.1]];
			
			var cornerLine = d3.svg.line()
				.x(function (d) {return c_scale(d[0])})
				.y(function (d) {return c_scale(d[1])})
				.interpolate("linear");
			
			cornerBox.append("svg:path")
			.attr("d", cornerLine(arrowData))
			.attr("class", "xline");
			
			var arrowData = [[0.55, 0.5], [0.85, 0.9], [0.85, 0.1]];
			
			cornerBox.append("svg:path")
			.attr("d", cornerLine(arrowData))
			.attr("class", "xline");
			
			function click(d) {
				
				//Main animation controller
				
				if (this.className) {
					if (this.className.baseVal == "ClickBoxRect") {
						return
					}
				}
				
				if (d3.event.altKey) {
					return
				}
				
				kx = (d.y ? w - 40 : w) / (1 - d.y);
				ky = h / d.dx;
				x.domain([d.y, 1]).range([d.y ? 40 : 0, w]);
				y.domain([d.x, d.x + d.dx]);
				
				var t = g.transition()
					.duration(d3.event.altKey ? 7500 : 750)
					.attr("transform", function (d) {
						return "translate(" + x(d.y) + "," + y(d.x) + ")";
					});
				
				t.select("rect")
				.attr("width", function (d) {
					return d.dy * kx;
				})
				.attr("height", function (d) {
					return d.dx * ky;
				});
				
				t.select(".Plot")
				.attr("transform", function (d) {
					return "translate(" + (d.dy * kx - d.dx * ky * 0.95) + "," + (d.dx * ky * 0.05) + ")";
				})
				.each(movePlot);
				
				t.selectAll(".ClickBox")
				.attr("transform", function (d) {
					return "translate(" + (d.dy * kx - d.dx * ky * 0.95) + "," + (d.dx * ky * 0.25) + ")";
				})
				.attr("opacity", function (d) {
					return (d.dx * ky > 40 ? 1 : 0)
				})
				.each(moveClickBox);
				
				t.select(".PlotRect")
				.attr("x", function (d) {
					return root.dy * kx - (d.dx * ky - 10) - (plot_x_margin)
				})
				.attr("y", function (d) {
					return (d.dx * ky) / 2 - ((d.dx * ky - 10) / 2)
				})
				.attr("width", function (d) {
					return d.dx * ky - 10
				})
				.attr("height", function (d) {
					return d.dx * ky - 10
				})
				.style("opacity", function (d) {
					return d.dx * ky > (plot_y + 2) ? 1 : 0;
				});
				
				t.select(".TextWithVals")
				.each(moveMainText);
				
				t.select(".TextNoVals")
				.each(moveMainText);
				
				t.selectAll(".Mvals")
				.style("opacity", function (d) {
					return d.dx * ky > 70 ? 1 : 0;
				});
				
				d3.event.stopPropagation();
			}
			
			function resize(d) {
			
				//Vertical text compression if needed
				
				var Text = d3.select(this)
				if (d.dx* ky < 28) {
					Text.attr("transform", "translate(8,12)")
						.style("font-size",10);
				}
				
				if (d.dx* ky < 16) {
					Text.attr("transform", "translate(8,8)")
						.style("font-size",8)
						.style("opacity", function(d) { return d.dx * ky > 10 ? 1 : 0})
				}
				
				d.Yshort = true
				
				return Text
			}
			
			function addMainText(d){
			
				//Compute Utility Value
				var threshold = d.threshold,
					thresVal = d.thresVal,
					objVal = d.objVal,
					coeff = d.coeff,
					objective = d.objective,
					fString = htmlEnDeCode.htmlDecode(d.function),
					i = d.currentVal,
					yMetric = 1;
					
				
				
				////////
				//Generates non-title text on each cell that has it (threshold, metric value, etc.)
				
				var Text = d3.select(this),
					Line1 = Text.append("tspan").attr("dx",15).attr("id","TextLine1").attr("class","secondaryText"),
					Line2 = Text.append("tspan").attr("dx",15).attr("id","TextLine2").attr("class","secondaryText"),
					Line3 = Text.append("tspan").attr("x",0).attr("dy",18).attr("id","TextLine3").attr("class","tertiaryText").style("opacity",0),
					Line4 = Text.append("tspan").attr("x",0).attr("dy",18).attr("id","TextLine4").attr("class","tertiaryText").style("opacity",0),
					Line5 = Text.append("tspan").attr("x",0).attr("dy",18).attr("id","TextLine5").attr("class","tertiaryText").style("opacity",0),
					Line6 = Text.append("tspan").attr("x",0).attr("dy",18).attr("id","TextLine6").attr("class","tertiaryText").style("opacity",0);
				
				Line1.text(function(d) {return "Threshold: " + d.threshold + " " + d.unit});

				if (d.currentVal !== null) {
					Line2.text(function(d){return "Your Metric Value: " + d.currentVal + " " + d.unit})}
				else {
					Line2.text("No metric data")}
					
				if ((typeof(i) == "number") && (typeof(fString) == "string")) {
				
					if (threshold > objective) {
						if (i <= objective) {
							yMetric = 1
						} else if (i >= threshold) {
							yMetric = 0
						} else {
							yMetric = eval(fString)
						}
					} else if (threshold < objective) {
						if (i >= objective) {
							yMetric = 1
						} else if (i <= threshold) {
							yMetric = 0
						} else {
							yMetric = eval(fString)
						}
					} else {
						if (d.function == "up") {
							yMetric = ((i < threshold) ? 0 : 1)
						} else {
							yMetric = ((i > threshold) ? 0 : 1)
						}
					}
						
					Line3.text(function(){return "Utility Value: " + yMetric.toString().slice(0,5)})
				}
				
				if (typeof(d.testBench) == "string") {
					Line4.text(function(d){return "Test Bench: " + d.testBench})
					}
				
				if (typeof(d.objective) == "number") {
					Line5.text(function(d){return "Objective: " + objective + " " + d.unit})
					}
					
				if (typeof(d.Priority) == "string") {
					Line6.text(function(d){return "Priority: " + d.Priority})
					}
					
				d.expanded = 0
					
				if (d.dx*ky > 55) {
					d.expanded = 1
					Line1.attr("x",0).attr("dy",18).attr("dx",null)
					Line2.attr("x",0).attr("dy",18).attr("dx",null)
				}
				
				if (d.dx*ky > 140) {
					Line3.style("opacity",1)
					Line4.style("opacity",1)
					d.expanded = 2
				}
				
				return Text
			}
			
			function checkLength(d){
			
				//Shortens text if it extends off the screen
				
				var TextObj = this
				var Text = d3.select(TextObj).selectAll(".secondaryText");
				if (d.dy*kx < TextObj.getBBox().width + 20){
					Text.style("opacity",0)
				}
				
				d.Xshort = true
			}
			  
			function moveMainText(d){
			
				//Text animation
				
				var TextObj = this
				var Text = d3.select(TextObj).style("opacity",1)
				
				if (d.dy*kx < TextObj.getBBox().width + 20) {
					Text.selectAll(".secondaryText").style("opacity",1)}
				else {Text.selectAll(".secondaryText").style("opacity",1)}
				
				
				if ( (d.dx*ky >= 28) && (d.Yshort) ) {
					Text.attr("transform", "translate(8,12)")
						.style("font-size",null)
						
					d.Yshort = false
					}
				//else if ( (d.dx*ky < 28) && (!d.Yshort) ) {
				//	Text.attr("transform", "translate(8,12)")
				//		.style("font-size",10)
				//	d.Yshort = true
				//	}
				
				if (d.dx* ky < 28) {
					Text.attr("transform", "translate(8,12)")
						.style("font-size",10);
						
					d.Yshort = true
				}
				
				if (d.dx* ky < 16) {
					Text.attr("transform", "translate(8,8)")
						.style("font-size",8)
						.style("opacity", function(d) { return d.dx * ky > 10 ? 1 : 0})
						
					d.Yshort = true
				}
				
				if ( (d.dx*ky >= 55) && (!d.expanded) && (d.dx*ky < 100) ){
				
					var T1 = Text.transition().duration(750)
						T1.select("#TextLine1").style("opacity",0)
						T1.select("#TextLine2").style("opacity",0)
						
					var T2 = Text.transition().delay(750).duration(0)
						T2.select("#TextLine1").attr("x",0).attr("dy",18).attr("dx",null)
						T2.select("#TextLine2").attr("x",0).attr("dy",18).attr("dx",null)
						
					var T3 = Text.transition().delay(750).duration(300)
						T3.select("#TextLine1").style("opacity",1)
						T3.select("#TextLine2").style("opacity",1)
					d.expanded = 1}
					
				else if ( (d.dx*ky >= 55) && (d.expanded == 2) && (d.dx*ky < 140) ){
				
					var T1 = Text.transition().duration(750)
						T1.select("#TextLine3").style("opacity",0)
						T1.select("#TextLine4").style("opacity",0)
						T1.select("#TextLine5").style("opacity",0)
						T1.select("#TextLine6").style("opacity",0)
					d.expanded = 1}
				
				else if ( (!d.expanded) && (d.dx*ky >= 140) ){
				
					var T1 = Text.transition().duration(750)
						T1.select("#TextLine1").style("opacity",0)
						T1.select("#TextLine2").style("opacity",0)
						
					var T2 = Text.transition().delay(750).duration(0)
						T2.select("#TextLine1").attr("x",0).attr("dy",18).attr("dx",null)
						T2.select("#TextLine2").attr("x",0).attr("dy",18).attr("dx",null)
						
					var T3 = Text.transition().delay(750).duration(300)
						T3.select("#TextLine1").style("opacity",1)
						T3.select("#TextLine2").style("opacity",1)
						T3.select("#TextLine3").style("opacity",1)
						T3.select("#TextLine4").style("opacity",1)
						T3.select("#TextLine5").style("opacity",1)
						T3.select("#TextLine6").style("opacity",1)
				
					d.expanded = 2}
					
				else if ( (d.expanded == 1) && (d.dx*ky >= 140) ){
				
					var T1 = Text.transition().duration(750)
						T1.select("#TextLine3").style("opacity",1)
						T1.select("#TextLine4").style("opacity",1)
						T1.select("#TextLine5").style("opacity",1)
						T1.select("#TextLine6").style("opacity",1)
					d.expanded = 2}
				
				else if ((d.dx*ky < 55) && !(d.expanded == 0)){
				
					Text.select("#TextLine1").style("opacity",0)
					Text.select("#TextLine2").style("opacity",0)
					Text.select("#TextLine3").style("opacity",0)
					Text.select("#TextLine4").style("opacity",0)
					Text.select("#TextLine5").style("opacity",0)
					Text.select("#TextLine6").style("opacity",0)
					
					Text.select("#TextLine1").attr("x",null).attr("dy",null).attr("dx",15)
					Text.select("#TextLine2").attr("x",null).attr("dy",null).attr("dx",15)
						
					if (d.dy*kx > TextObj.getBBox().width + 20) {
						var T3 = Text.transition().delay(750).duration(300)
						T3.select("#TextLine1").style("opacity",1)
						T3.select("#TextLine2").style("opacity",1)}
					
					d.expanded = 0}
			}
				
			function makePlot(d) {
				
				//Create metric plots
				
				var svg = d3.select(this)
					
					var cellw = root.dy * kx,
				cellh = d.dx * ky,
				cellPad = 0.05 * cellh,
				ploth = cellh - 2 * cellPad,
				plotw = ploth,
				plotxPad = 28,
				plotyPad = 20,
				leftBox = 20;
				
				var threshold = d.threshold,
				thresVal = d.thresVal,
				objVal = d.objVal,
				coeff = d.coeff,
				objective = d.objective,
				xPad = 0.1 * (objective - threshold),
				fString = htmlEnDeCode.htmlDecode(d.function),
				dString = d.designVal,	
				dataset = [],
				metric_r = 5;
				
				if (objective == threshold) {
					var iStep = 1
				}
				else {
					var iStep = (objective - threshold) / 10;
				}
				
				if (objective > threshold) {
					if (d.currentVal !== null) {
						var currentVal = d.currentVal,
						i = currentVal;
						
						if (currentVal < threshold) {
							var xMax = objective + xPad,
							xMin = currentVal - xPad,
							yMetric = 0
						} else if (currentVal < objective) {
							var xMax = objective + xPad,
							xMin = threshold - xPad,
							yMetric = eval(fString)
						} else {
							var xMax = currentVal + xPad,
							xMin = threshold - xPad,
							yMetric = 1
						}
					} else {
						var xMax = objective + xPad,
						xMin = threshold - xPad,
						yMetric = 1
					}
					
					dataset.push([xMin, 0])
					dataset.push([threshold, 0])
					
					for (i = threshold; i <= objective; i = i + iStep) {
						designVal = eval(dString)
							dataset.push([i, eval(fString)])
					}
					
					dataset.push([objective, 1])
					dataset.push([xMax, 1])
				}
				
				else if (objective < threshold) {
					xPad = -xPad
					if (d.currentVal !== null) {
						var currentVal = d.currentVal,
						i = currentVal;
						
						if (currentVal < objective) {
							var xMax = threshold + xPad,
							xMin = currentVal - xPad,
							yMetric = 1
						} else if (currentVal < threshold) {
							var xMax = threshold + xPad,
							xMin = objective - xPad,
							yMetric = eval(fString)
						} else {
							var xMax = currentVal + xPad,
							xMin = objective - xPad,
							yMetric = 0
						}
					} else {
						var xMax = threshold + xPad,
						xMin = objective - xPad,
						yMetric = 1
					}
					
					dataset.push([xMax, 0])
					dataset.push([threshold, 0])
					
					for (i = threshold; i > objective; i = i + iStep) {
						designVal = eval(dString)
							dataset.push([i, eval(fString)])
					}
					
					dataset.push([objective, 1])
					dataset.push([xMin, 1])
				} else {
					if (d.currentVal !== null) {
						var currentVal = d.currentVal,
						i = currentVal;
						
						xPad = ((currentVal > objective) ? 0.1 * (currentVal - objective) : 0.1 * (objective - currentVal))
						
						if (objective > currentVal) {
							var xMax = threshold + xPad,
							xMin = currentVal - xPad,
							yMetric = ((fString == "up") ? 0 : 1)
						} else {
							var xMax = currentVal + xPad,
							xMin = objective - xPad,
							yMetric = ((fString == "up") ? 1 : 0)
						}
					} else {
						xPad = 0.1 * threshold
						
						var xMax = threshold + xPad,
						xMin = objective - xPad,
						yMetric = 1
					}
					
					dataset.push([xMin, (fString == "up" ? 0 : 1)])
					dataset.push([threshold, (fString == "up" ? 0 : 1)])
					dataset.push([threshold, (fString == "up" ? 1 : 0)])
					dataset.push([xMax, (fString == "up" ? 1 : 0)])
				}
				
				var xScale = d3.scale.linear()
					.domain([xMin, xMax])
					.range([plotxPad, plotw - plotxPad / 2]);
				
				var yScale = d3.scale.linear()
					.domain([0, d3.max(dataset, function (d) {
								return d[1];
							})])
					.range([ploth - plotyPad, plotyPad / 2]);
				
				//var rScale = d3.scale.linear()
				//			   .domain([0, d3.max(dataset, function(d) {return d[1];})])
				//			   .range([2, 15]);
				
				var xAxis = d3.svg.axis()
					.scale(xScale)
					.orient("bottom")
					.ticks(5);
				
				var yAxis = d3.svg.axis()
					.scale(yScale)
					.orient("left")
					.ticks(5);
				
				var line = d3.svg.line()
					.x(function (d) {
						return xScale(d[0])
					})
					.y(function (d) {
						return yScale(d[1])
					})
					//.interpolate("linear")
					
					svg.append("rect")
					.attr("id", "backRect")
					.attr("height", ploth)
					.attr("width", plotw + leftBox)
					.attr("transform", "translate(" + (-leftBox) + ",0)")
					.style("fill", "white")
					
					if ((objective > threshold) || (fString == "up")) {
					
						svg.append("svg:rect")
						.attr("x", plotxPad)
						.attr("y", plotyPad / 2)
						.attr("class", "sideRect")
						.attr("id", "bottomRect")
						.attr("height", (ploth - plotyPad / 2 - plotyPad > 0 ? ploth - plotyPad / 2 - plotyPad : 0))
						.attr("width", (xScale(threshold) - xScale(xMin) > 0 ? xScale(threshold) - xScale(xMin) : 0))
						.attr("opacity", 0.2)
						.style("fill", "red")
						
						svg.append("svg:rect")
						.attr("x", xScale(objective))
						.attr("y", plotyPad / 2)
						.attr("class", "sideRect")
						.attr("id", "topRect")
						.attr("height", (ploth - plotyPad / 2 - plotyPad > 0 ? ploth - plotyPad / 2 - plotyPad : 0))
						.attr("width", (xScale(xMax) - xScale(objective) > 0 ? xScale(xMax) - xScale(objective) : 0))
						.attr("opacity", 0.2)
						.style("fill", "green")
					}
					else {
						svg.append("svg:rect")
						.attr("x", xScale(threshold))
						.attr("y", plotyPad / 2)
						.attr("class", "sideRect")
						.attr("id", "bottomRect")
						.attr("height", (ploth - plotyPad / 2 - plotyPad > 0 ? ploth - plotyPad / 2 - plotyPad : 0))
						.attr("width", (xScale(xMax) - xScale(threshold) > 0 ? xScale(xMax) - xScale(threshold) : 0))
						.attr("opacity", 0.2)
						.style("fill", "red")
						
						svg.append("svg:rect")
						.attr("x", plotxPad)
						.attr("y", plotyPad / 2)
						.attr("class", "sideRect")
						.attr("id", "topRect")
						.attr("height", (ploth - plotyPad / 2 - plotyPad > 0 ? ploth - plotyPad / 2 - plotyPad : 0))
						.attr("width", (xScale(objective) - xScale(xMin) > 0 ? xScale(objective) - xScale(xMin) : 0))
						.attr("opacity", 0.2)
						.style("fill", "green")
					}
					
					svg.append("svg:path")
					.attr("d", line(dataset))
					.attr("class", "line")
					
					svg.append("g")
					.attr("class", "axis")
					.attr("id", "xAxis")
					.attr("transform", "translate(0," + (ploth - plotyPad) + ")")
					.call(xAxis)
					
					svg.append("g")
					.attr("class", "axis")
					.attr("id", "yAxis")
					.attr("transform", "translate(" + plotxPad + ",0)")
					.call(yAxis)
					
					svg.append("text")
					.text(function (d) {
						return d.unit + " vs. Utility"
					})
					.attr("id", "plotText")
					.attr("text-anchor", "middle")
					.attr("transform", "translate(0," + ploth / 2 + ")" + "rotate(-90,0,0)")
					.style("letter-spacing", 1)
					
					if (currentVal !== null && currentVal !== undefined) {
						svg.append("svg:circle")
						.attr("cx", xScale(currentVal))
						.attr("cy", yScale(yMetric))
						.attr("r", metric_r)
						.attr("class", "currentPoint")
					}
					
					svg.attr("opacity", ploth > 120 ? 1 : 0);
				}
			
			function movePlot(d) {
				
				//Plot animation
				
				var cellw = root.dy * kx,
				cellh = d.dx * ky,
				cellPad = 0.05 * cellh,
				ploth = cellh - 2 * cellPad,
				plotw = ploth,
				plotxPad = 28,
				plotyPad = 20,
				leftBox = 20;
				
				var threshold = d.threshold,
				thresVal = d.thresVal,
				objVal = d.objVal,
				coeff = d.coeff,
				objective = d.objective,
				xPad = 0.1 * (objective - threshold),
				fString = htmlEnDeCode.htmlDecode(d.function),
				dString = d.designVal,	
				dataset = [],
				metric_r = 5;
				
				if (objective == threshold) {
					var iStep = 1
				}
				else {
					var iStep = (objective - threshold) / 10;
				}
				
				if (objective > threshold) {
					if (d.currentVal !== null) {
						var currentVal = d.currentVal,
						i = currentVal;
						
						if (currentVal < threshold) {
							var xMax = objective + xPad,
							xMin = currentVal - xPad,
							yMetric = 0
						} else if (currentVal < objective) {
							var xMax = objective + xPad,
							xMin = threshold - xPad,
							yMetric = eval(fString)
						} else {
							var xMax = currentVal + xPad,
							xMin = threshold - xPad,
							yMetric = 1
						}
					} else {
						var xMax = objective + xPad,
						xMin = threshold - xPad,
						yMetric = 1
					}
					
					dataset.push([xMin, 0])
					dataset.push([threshold, 0])
					
					for (i = threshold; i <= objective; i = i + iStep) {
						designVal = eval(dString)
							dataset.push([i, eval(fString)])
					}
					
					dataset.push([objective, 1])
					dataset.push([xMax, 1])
				}
				
				else if (objective < threshold) {
					xPad = -xPad
					if (d.currentVal !== null) {
						var currentVal = d.currentVal,
						i = currentVal;
						
						if (currentVal < objective) {
							var xMax = threshold + xPad,
							xMin = currentVal - xPad,
							yMetric = 1
						} else if (currentVal < threshold) {
							var xMax = threshold + xPad,
							xMin = objective - xPad,
							yMetric = eval(fString)
						} else {
							var xMax = currentVal + xPad,
							xMin = objective - xPad,
							yMetric = 0
						}
					} else {
						var xMax = threshold + xPad,
						xMin = objective - xPad,
						yMetric = 1
					}
					
					dataset.push([xMax, 0])
					dataset.push([threshold, 0])
					
					for (i = threshold; i > objective; i = i + iStep) {
						designVal = eval(dString)
							dataset.push([i, eval(fString)])
					}
					
					dataset.push([objective, 1])
					dataset.push([xMin, 1])
				} else {
					
					
					if (d.currentVal !== null) {
						var currentVal = d.currentVal,
						i = currentVal;
						
						xPad = ((currentVal > objective) ? 0.1 * (currentVal - objective) : 0.1 * (objective - currentVal))
						
						if (objective > currentVal) {
							var xMax = threshold + xPad,
							xMin = currentVal - xPad,
							yMetric = ((fString == "up") ? 0 : 1)
						} else {
							var xMax = currentVal + xPad,
							xMin = objective - xPad,
							yMetric = ((fString == "up") ? 1 : 0)
						}
					} else {
						xPad = 0.1 * threshold
						
						var xMax = threshold + xPad,
						xMin = objective - xPad,
						yMetric = 1
					}
					
					dataset.push([xMin, (fString == "up" ? 0 : 1)])
					dataset.push([threshold, (fString == "up" ? 0 : 1)])
					dataset.push([threshold, (fString == "up" ? 1 : 0)])
					dataset.push([xMax, (fString == "up" ? 1 : 0)])
				}
				
				var xScale = d3.scale.linear()
					.domain([xMin, xMax])
					.range([plotxPad, plotw - plotxPad / 2]);
				
				var yScale = d3.scale.linear()
					.domain([0, d3.max(dataset, function (d) {
								return d[1];
							})])
					.range([ploth - plotyPad, plotyPad / 2]);
				
				var xAxis = d3.svg.axis()
					.scale(xScale)
					.orient("bottom")
					.ticks(5);
				
				var yAxis = d3.svg.axis()
					.scale(yScale)
					.orient("left")
					.ticks(5);
				
				var line = d3.svg.line()
					.x(function (d) {
						return xScale(d[0])
					})
					.y(function (d) {
						return yScale(d[1])
					})
					//.interpolate("linear")
					
					var plot = d3.select(this).transition()
					.duration(d3.event.altKey ? 7500 : 750);
				
				plot.attr("opacity", ploth > 120 ? 1 : 0);
				
				plot.select("#backRect")
				.attr("height", ploth)
				.attr("width", plotw + leftBox)
				.attr("dx", -leftBox)
				
				plot.select("#xAxis")
				.attr("transform", "translate(0," + (ploth - plotyPad) + ")")
				.call(xAxis)
				
				plot.select("#yAxis")
				.attr("transform", "translate(" + plotxPad + ",0)")
				.call(yAxis)
				
				plot.select("path")
				.attr("d", line(dataset))
				
				if (currentVal !== null) {
					plot.select("circle")
					.attr("cx", xScale(currentVal))
					.attr("cy", yScale(yMetric))
				}
				
				if ((objective > threshold) || (fString == "up")) {
				
					plot.select("#bottomRect")
					.attr("x", plotxPad)
					.attr("y", plotyPad / 2)
					.attr("height", (ploth - plotyPad / 2 - plotyPad > 0 ? ploth - plotyPad / 2 - plotyPad : 0))
					.attr("width", (xScale(threshold) - xScale(xMin) > 0 ? xScale(threshold) - xScale(xMin) : 0))
					
					plot.select("#topRect")
					.attr("x", xScale(objective))
					.attr("y", plotyPad / 2)
					.attr("height", (ploth - plotyPad / 2 - plotyPad > 0 ? ploth - plotyPad / 2 - plotyPad : 0))
					.attr("width", (xScale(xMax) - xScale(objective) > 0 ? xScale(xMax) - xScale(objective) : 0))
				}
				else {
					plot.select("#topRect")
					.attr("x", plotxPad)
					.attr("y", plotyPad / 2)
					.attr("height", (ploth - plotyPad / 2 - plotyPad > 0 ? ploth - plotyPad / 2 - plotyPad : 0))
					.attr("width", (xScale(objective) - xScale(xMin) > 0 ? xScale(objective) - xScale(xMin) : 0))
					
					plot.select("#bottomRect")
					.attr("x", xScale(threshold))
					.attr("y", plotyPad / 2)
					.attr("height", (ploth - plotyPad / 2 - plotyPad > 0 ? ploth - plotyPad / 2 - plotyPad : 0))
					.attr("width", (xScale(xMax) - xScale(threshold) > 0 ? xScale(xMax) - xScale(threshold) : 0))

				}
				
				plot.select("#plotText")
				.attr("transform", "translate(0," + ploth / 2 + ")" + "rotate(-90,0,0)")
			}
			
			function setupData(d) {
				
				//Generates necessary fields for data partitioning
				
				var rawData = partition.nodes(root)
					
					var finalData = [],
						maxDepth = 5,
						nodeWidth = .15
					
					
					getColor(root)
					
					
					for (var i = 0; i < rawData.length; i++) {
						var currentNode = rawData[i]
						if (currentNode.depth > maxDepth) {
							maxDepth = currentNode.depth
							nodeWidth = .75 / maxDepth
						}
					}
					
					for (i = 0; i < rawData.length; i++) {
						currentNode = rawData[i]
							currentNode.dy = (currentNode.children ? nodeWidth : (1 - currentNode.depth * nodeWidth)) //Assumes max depth ~ 4, add depth check before for loop if necessary.)
							currentNode.y = currentNode.depth * nodeWidth
							currentNode.size = 1000
							
							finalData.push(currentNode)
					}
					return finalData;
			}
			
			function getColor(node) {
				
				//Generates color map based on metric values
				
				var blue = "#65A0DF",
				green = "#83BE6C",
				red = "#BB1F1F",
				gray = "#aaa";
				
				var modifier = 1
				
				if (node.threshold > node.objective) {modifier = -1}
				else if ((node.threshold == node.objective) && (node.function == "down")) {modifier = -1}
				
				if (node.children) {
					node.color = blue
						
						for (node.i = 0; node.i < node.children.length; node.i++) {
							childColor = getColor(node.children[node.i])
								if (childColor == red) {;
									node.color = red;
								} 
								else if (childColor == gray && node.color != red) {
									node.color = gray
								} 
								else if (childColor == green && node.color != gray && node.color != red) {
									node.color = green
								}
						}
				} else {
					node.color = gray
						if (typeof(node.currentVal) == "number") {
							( ( modifier * (node.currentVal - node.threshold) >= 0) ? 
							( ( modifier * (node.currentVal - node.objective) >= 0) ? node.color = blue : node.color = green) 
							: node.color = red)
						}
				}
				
				return node.color
			}
			
			function makeClickBox(d) {
				
				//Generates interactive check box for on/off requirements
				
				var svg = d3.select(this)
					
					var cellw = root.dy * kx,
				cellh = d.dx * ky,
				cellPad = 0.05 * cellh,
				ploth = cellh * 0.5,
				plotw = cellh - 2 * cellPad;
				
				var cx = d3.scale.linear().domain([0, 2]).range([0, plotw]),
				cy = d3.scale.linear().range([0, ploth]),
				
				cx2 = d3.scale.linear().range([cx(0.05), cx(0.95)]),
				cy2 = d3.scale.linear().range([cy(0.05), cy(0.95)]);
				
				var xdim = 0.1,
				xdata = [[0, 0], [0, xdim], [.5 - xdim, .5], [0, 1 - xdim],
					[0, 1], [xdim, 1], [.5, .5 + xdim], [1 - xdim, 1],
					[1, 1], [1, 1 - xdim], [.5 + xdim, .5], [1, xdim],
					[1, 0], [1 - xdim, 0], [.5, .5 - xdim], [xdim, 0]];
				
				var xline = d3.svg.line()
					.x(function (d) {
						return cx2(d[0] / 2)
					})
					.y(function (d) {
						return cy2(d[1] / 2)
					})
					.interpolate("linear")
					
					svg.append("svg:rect")
					.attr("id", "clickBackRect")
					.attr("width", cx(2))
					.attr("height", cy(1))
					.style("fill", "steelblue")
					
					///// True Group
					
					var g1 = svg.append("svg:g")
					.attr("id", "TrueGroup")
					//.on("click", TrueClick)
					
					g1.append("svg:rect")
					.attr("id", "TrueBox")
					.attr("width", cx(.5))
					.attr("height", cy(.5))
					.attr("transform", ("translate(" + cx(.25) + "," + cy(0.1) + ")"))
					
					g1.append("svg:path")
					.attr("d", xline(xdata))
					.attr("class", "xline")
					.attr("id", "TrueX")
					.attr("transform", ("translate(" + cx(.225) + "," + cy(0.075) + ")"))
					.attr("opacity", function (d) {
						return d.currentVal == 1 ? 1 : 0
					})
					
					///// False Group
					
					var g2 = svg.append("svg:g")
					.attr("id", "FalseGroup")
					//.on("click", FalseClick)
					
					g2.append("svg:rect")
					.attr("id", "FalseBox")
					.attr("width", cx(.5))
					.attr("height", cy(.5))
					.attr("transform", ("translate(" + cx(1.25) + "," + cy(0.1) + ")"))
					
					g2.append("svg:path")
					.attr("d", xline(xdata))
					.attr("class", "xline")
					.attr("id", "FalseX")
					.attr("transform", ("translate(" + cx(1.225) + "," + cy(0.075) + ")"))
					.attr("opacity", function (d) {
						return d.currentVal == 0.0 ? 1 : 0
					})
					
					///// Text
					
					var TrueText = svg.append("svg:text")
					.attr("id", "TrueText")
					.attr("text-anchor", "middle")
					.attr("transform", ("translate(" + (cx(0.5)) + "," + (cy(0.6) + 14) + ")"))
					.style("opacity", function (d) {
						return (d.dx * ky > 100 ? 1 : 0)
					})
					TrueText.append("svg:tspan")
					.text("True")
					.attr("x", 0)
					TrueText.append("svg:tspan")
					.text("")
					.attr("dy", 14)
					.attr("x", 0)
					
					var FalseText = svg.append("svg:text")
					.attr("id", "FalseText")
					.attr("text-anchor", "middle")
					.attr("transform", ("translate(" + (cx(1.5)) + "," + (cy(0.6) + 14) + ")"))
					.style("opacity", function (d) {
						return (d.dx * ky > 100 ? 1 : 0)
					})
					FalseText.append("svg:tspan")
					.text("False")
					.attr("x", 0)
					FalseText.append("svg:tspan")
					.text("")
					.attr("dy", 14)
					.attr("x", 0)
			}
			
			function moveClickBox(d) {
				
				//click box animation
				
				var svg = d3.select(this).transition()
					.duration(d3.event.altKey ? 7500 : 750);
				
				var cellw = root.dy * kx,
				cellh = d.dx * ky,
				cellPad = 0.05 * cellh,
				ploth = cellh * 0.5,
				plotw = cellh - 2 * cellPad;
				
				var cx = d3.scale.linear().domain([0, 2]).range([0, plotw]),
				cy = d3.scale.linear().range([0, ploth]),
				
				cx2 = d3.scale.linear().range([cx(0.05), cx(0.95)]),
				cy2 = d3.scale.linear().range([cy(0.05), cy(0.95)]);
				
				var xdim = 0.1,
				xdata = [[0, 0], [0, xdim], [.5 - xdim, .5], [0, 1 - xdim],
					[0, 1], [xdim, 1], [.5, .5 + xdim], [1 - xdim, 1],
					[1, 1], [1, 1 - xdim], [.5 + xdim, .5], [1, xdim],
					[1, 0], [1 - xdim, 0], [.5, .5 - xdim], [xdim, 0]];
				
				var xline = d3.svg.line()
					.x(function (d) {
						return cx2(d[0] / 2)
					})
					.y(function (d) {
						return cy2(d[1] / 2)
					})
					.interpolate("linear")
					
					svg.select("#clickBackRect")
					.attr("width", cx(2))
					.attr("height", cy(1))
					
					///// True Group
					
					var g1 = svg.select("#TrueGroup")
					
					g1.select("#TrueBox")
					.attr("width", cx(.5))
					.attr("height", cy(.5))
					.attr("transform", ("translate(" + cx(.25) + "," + cy(0.1) + ")"))
					
					g1.select("#TrueX")
					.attr("d", xline(xdata))
					.attr("transform", ("translate(" + cx(.225) + "," + cy(0.075) + ")"))
					
					///// False Group
					
					var g2 = svg.select("#FalseGroup")
					
					g2.select("#FalseBox")
					.attr("width", cx(.5))
					.attr("height", cy(.5))
					.attr("transform", ("translate(" + cx(1.25) + "," + cy(0.1) + ")"))
					
					g2.select("#FalseX")
					.attr("d", xline(xdata))
					.attr("transform", ("translate(" + cx(1.225) + "," + cy(0.075) + ")"))
					
					///// Text
					
					var TrueText = svg.select("#TrueText")
					.attr("transform", ("translate(" + (cx(0.5)) + "," + (cy(0.6) + 14) + ")"))
					.style("opacity", function (d) {
						return (d.dx * ky > 100 ? 1 : 0)
					})
					
					var FalseText = svg.select("#FalseText")
					.attr("transform", ("translate(" + (cx(1.5)) + "," + (cy(0.6) + 14) + ")"))
					.style("opacity", function (d) {
						return (d.dx * ky > 100 ? 1 : 0)
					})
					
			}
			
			function TrueClick(d) {
				
				//click box animation if true box is clicked
				
				var TrueGroup = this
					var TrueX = d3.select($(this).children("#TrueX")[0])
					
					var FalseGroup = $(this).next()[0]
					var FalseX = d3.select($(FalseGroup).children("#FalseX")[0])
					
					if (d.currentVal == 1.0) {
						d.currentVal = undefined
							TrueX.attr("opacity", 0)
					} else {
						d.currentVal = 1.0
							TrueX.attr("opacity", 1)
							FalseX.attr("opacity", 0)
					}
					
					getColor(root)
					g.selectAll(".parent").style("fill", function (d) {
						return d.color
					});
				g.selectAll(".child").style("fill", function (d) {
					return d.color
				});
			}
			
			function FalseClick(d) {
				
				//click box animation if false box is clicked
				
				var FalseGroup = this
					var FalseX = d3.select($(this).children("#FalseX")[0])
					
					var TrueGroup = $(this).prev()[0]
					var TrueX = d3.select($(TrueGroup).children("#TrueX")[0])
					
					if (d.currentVal == 0.0) {
						d.currentVal = undefined
							FalseX.attr("opacity", 0)
					} else {
						d.currentVal = 0.0
							FalseX.attr("opacity", 1)
							TrueX.attr("opacity", 0)
					}
					
					getColor(root)
					g.selectAll(".parent").style("fill", function (d) {
						return d.color
					});
				g.selectAll(".child").style("fill", function (d) {
					return d.color
				});
			}
					//////END CODE
		
			/* Mark <vf-table> as rendered */
			$(element).attr('rendered', 'true');
		
		
		// End set timeout
		
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