/*/
//////////// vf-bubbles /////////////*/
function vf_bubbles(element) {
    var self = this;
    this.data = eval($(element).attr('data'));
    this.element = element;
    var fixedWidth = $(element).parent().width();
    var fixedHeight = $(element).parent().height();

    /* Render Method */
    this.render = function () {
        if (($(element).attr('rendered') == undefined || $(element).attr('rendered') == "false")) {
						var radius = 20; //determine how to size the circles based on # components
						var headers = clone(this.data.headers);
						var data = clone(this.data.data);
						var groupVar = clone(this.data.groupVar);
						
						var groups = [];
						for (var i = 0; i < data.length; i++) {
							groups.push({
								x: fixedWidth / 2,
								y: 200
							});
						}
						var value = 0;
						var colorValue = 0;
						var colors = d3.scale.category10();
						var width = fixedWidth;
						var height = 400;
						
						// new color scheme
						function colors2(colorIndex) {
						// need to know the number of colors
						
						var diffs = [];
						for (var i = 0; i < data.length; i++) {
							var count = 0;
							for (var j = 0; j < diffs.length; j++) {

								if (diffs[j] == data[i].comps[temp - 1]) {
									count = count + 1;
								}
							}
							if (count == 0) {
								diffs.push(data[i].comps[temp - 1]);
							}
						}
						// Creates the multiple groupings
						for (i = 0; i < data.length; i++) {
							ind = diffs.indexOf(data[i].comps[temp - 1]);
							data[i].color = 20 + ind;
						}
						//console.log("-----");
						//console.log(diffs);
						
						}

						var force = d3.layout.force()
							.charge(-150)
							.size([width, height])
							.nodes(data)
							.gravity(.02)

						//	var chart = d3.select("body").append("svg")
						var chart = d3.select('#' + $(element).attr('id')).append("svg")
							.attr("width", width)
							.attr("height", height)

						var tooltip = d3.select("body")
							.append("div")
							.style("position", "absolute")
							.style("z-index", "10")
							.style("visibility", "hidden")
							.style("border", "black 1px solid")
							.style("fill", "black")
							.style("color", "black")
							.style("background-color", "rgba(255,255,255,1)")
							//.style("background-color", "rgba(255,255,255,.5)") //transparent background
							//.style("opacity", .8)  // makes the entire div transparent (including text)

						var bubs = chart.selectAll("circle.node")
							.data(data)
							.enter().append("circle")
							.attr("class", "node")
							.attr("r", function (d) {return d.rad})
							.style("fill", function (d) {return "hsl("+d.color.hue+","+d.color.saturation+"%,"+d.color.lightness+"%)"})
							.attr("cx", function (d, i) {return groups[i].x})
							.attr("cy", function (d, i) {return groups[i].y})
							.style("stroke", "black")
							.style("stroke-width", "2px")
							.on("mouseover", function (d) {
								d3.select(this).style("stroke-width", 3);
								if(value !=0 && colorValue !=0){
									return tooltip
										.style("visibility", "visible")
										.html('<div>Group Component: ' + d.comps[value - 1] + '</br>Color Component: ' + d.comps[colorValue -1] + '</br>Design: ' + d.config + '</div>')}
								else if(value == 0 && colorValue == 0) {
									return tooltip
										.style("visibility", "visible")
										.text("Design: " + d.config)}
								else if(value == 0 && colorValue != 0) {
								 return tooltip
										.style("visibility", "visible")
										.html('<div>Color Component: ' + d.comps[colorValue - 1] + '</br> Design: ' + d.config + '</div>')}
								else if(value != 0 && colorValue == 0) {
									 return tooltip
										.style("visibility", "visible")
										.html('<div>Group Component: ' + d.comps[value - 1] + '</br> Design: ' + d.config + '</div>')}
							})
							.on("mousemove", function () {
								return tooltip
								.style("top", (event.pageY - 10) + "px")
								.style("left", (event.pageX + 10) + "px");})
							.on("mouseout", function () {
								d3.select(this).style("stroke-width", 2);
								return tooltip
								.style("visibility", "hidden");})
							.on("click", function(d){
								$(document).trigger($(element).attr('id') + '_clicked', [event, d.config]);	
							})
							.call(force.drag); //alows for svgs to be dragged

						force.on("tick", function (e) {
							bubs
								.attr("cx", function (d, i) {return d.x})
								.attr("cy", function (d, i) {return d.y})
							})
							.start();
				
						function update(diffs) {
							force.on("tick", function(e){
								data.forEach(function(d, i) {
									d.x += (groups[i].x - d.x) * .1 * e.alpha;
									d.y += (groups[i].y - d.y) * .1 * e.alpha});
								bubs.attr("cx", function(d) { return d.x; })
									.attr("cy", function(d) { return d.y; })
									.attr("r", function(d) {return d.rad; })
									.style("fill", function (d) {return "hsl("+d.color.hue+","+d.color.saturation+"%,"+d.color.lightness+"%)"})
									
									if(diffs != undefined && e.alpha<.1)
									{
										groupBoxes(diffs);
									}else{
										//chart.selectAll("rect").remove();
										chart.selectAll("text").remove();
									}
							});	
							force.resume();
						}

				
						function minmaxBox(diffs){
							var out = [];
							for(i = 0; i<diffs.length; i++){
								out.push({name:"", xmin: 0, xmax: 0, ymin: 0, ymax: 0});	}
							for(i=0; i<data.length; i++){
								if(out[data[i].group].xmin == 0) {
									out[data[i].group].xmin = data[i].x-data[i].rad; }
								else if (out[data[i].group].xmin > data[i].x-data[i].rad) {
									out[data[i].group].xmin = data[i].x-data[i].rad; }
								
								if(out[data[i].group].xmax == 0) {
									out[data[i].group].xmax = data[i].x+data[i].rad; }
								else if (out[data[i].group].xmax < data[i].x+data[i].rad) {
									out[data[i].group].xmax = data[i].x+data[i].rad; }
								
								if(out[data[i].group].ymin == 0) {
									out[data[i].group].ymin = data[i].y-data[i].rad; }
								else if (out[data[i].group].ymin > data[i].y-data[i].rad) {
									out[data[i].group].ymin = data[i].y-data[i].rad; }
								
								if(out[data[i].group].ymax == 0) {
									out[data[i].group].ymax = data[i].y+data[i].rad; }
								else if (out[data[i].group].ymax < data[i].y+data[i].rad) {
									out[data[i].group].ymax = data[i].y+data[i].rad; }
							}
							return out;
						}

						function groupBoxes(diffs) {
							var boxes = minmaxBox(diffs);

							chart.selectAll("text").remove();
							var labels = chart.selectAll("text")
								.data(boxes)
							.enter().append("text")
								.attr("x", function(d,i) {return d.xmin+(d.xmax-d.xmin)*.5-(measure(diffs[i])/2)})
								.attr("y", function(d) {return d.ymax+16})
								.text(function(d,i) {return diffs[i]})
						}	

						function measure(txt) {
							$('.ruler').text(txt);
							var out = $('.ruler').width();
							$('.ruler').text('');
							return out;
						}
									
						function colorUpdate() {
							//bubs.style("fill", function (d) {return colors(d.color)});
							//console.log("hsl("+d.color.hue+","+d.color.saturation+"%,"+d.color.lightness+"%)"); 
							
							bubs.style("fill", function (d) {
								return "hsl("+d.color.hue+","+d.color.saturation+"%,"+d.color.lightness+"%)"
							});
							
						}

						transition = function (v) {
							value = v;
						}

						colorTransition = function (v) {
							colorValue = v;
						}
				
				
						groupUpdates = function(x) {
							var temp = x;
							groups = [];
							if (temp == 0) {
								for (var i = 0; i < data.length; i++) {
									groups.push({x: fixedWidth / 2,y: 200});
								}
							} else {
								var diffs = [];
								for (var i = 0; i < data.length; i++) {
									var count = 0;
									for (var j = 0; j < diffs.length; j++) {
										if (diffs[j] == data[i].comps[temp - 1]) {
											count = count + 1;}
									}
									if (count == 0) {
										diffs.push(data[i].comps[temp - 1]);
									}
								}

								// Creates the multiple groupings
								var range = [];
								for (i = 0; i < data.length; i++) {
									range.push(i);
									ind = diffs.indexOf(data[i].comps[temp - 1]);
									wpg = fixedWidth / (diffs.length + 1);
									groups.push({
										x: wpg + ind * wpg,
										y: 200
									});
									data[i].rad = 20 - (diffs.length);
									data[i].group = ind;
								}
							}
							update(diffs);
							return false;
						}
						
						colorUpdates = function(y) {
							var temp = y;
							console.log("TESTING");
							
							//colorgroups = [];
								if (this.temp != 0) {
									var diffs = [];
									for (var i = 0; i < data.length; i++) {
										var count = 0;
										for (var j = 0; j < diffs.length; j++) {
											if (diffs[j] == data[i].comps[temp - 1]) {
												count = count + 1;
											}
										}
										if (count == 0) {
											diffs.push(data[i].comps[temp - 1]);
										}
									}
									// Creates the multiple groupings
									for (i = 0; i < data.length; i++) {
										ind = diffs.indexOf(data[i].comps[temp - 1]);
										data[i].color = 20 + ind;
										data[i].color = colorSelector(ind);
									}
								}
								
								
								function colorSelector(ind) {
									var interval = 360/diffs.length;
									if(interval < 36 ) {
										interval = 36;
									}
									
									var c = [];
									c.hue = interval*ind;
									c.hue.toFixed(2);
									c.saturation = 100;
									c.lightness = 50;
									
									if(ind > 9) {
										c.saturation = 70;
										c.lightness = 25;
									}
									if(ind > 19) {
										c.saturation = 30;
										c.lightness = 50;
									}
									if(ind > 29) {
										c.saturation = 50;
										c.lightness = 65;
									}
									if(ind > 39) {
										c.saturation = 25;
										c.lightness = 75;
									}
									if(ind > 49) {
										c.saturation = Math.random()*100;
										c.lightness = 20+Math.random()*60;
									}

									return c;
								}
								
								
								
								
								
								
								//$(document).ready(function () {
									$('.colorlegend').empty();
									
									if(diffs != undefined) {
										$('.colorlegend').append("<table><tr></tr><tr></tr><tr></tr></table>");
										$.each(diffs,function(i) {
											if(diffs[i] != undefined) {
												//$('.colorlegend tbody tr:nth-child('+(1+i%3)+')').append("<td><font color='#ffffff'>"+diffs[i]+"</font></td><td style='background-color:"+colors(i)+"'>&nbsp;&nbsp;</td><td width='15px'></td>");
												var tempColor = colorSelector(i);
												$('.colorlegend tbody tr:nth-child('+(1+i%3)+')').append("<td>"+diffs[i]+"</font></td><td style='background-color:hsl("+tempColor.hue+","+tempColor.saturation+"%,"+tempColor.lightness+"%)'>&nbsp;&nbsp;</td><td width='15px'></td>");
												console.log(data[i]);
												
												
												
											}
										});
									}
								//}
								colorUpdate();
								return false;
						}
						
						colorUpdates2 = function(y) {
							var temp = y;
							//colorgroups = [];
								if (this.temp != 0) {
									var diffs = [];
									for (var i = 0; i < data.length; i++) {
										var count = 0;
										for (var j = 0; j < diffs.length; j++) {

											if (diffs[j] == data[i].comps[temp - 1]) {
												count = count + 1;
											}
										}
										if (count == 0) {
											diffs.push(data[i].comps[temp - 1]);
										}
									}
									// Creates the multiple groupings
									for (i = 0; i < data.length; i++) {
										ind = diffs.indexOf(data[i].comps[temp - 1]);
										data[i].color = 20 + ind;
									}
								}
								
								//$(document).ready(function () {
									$('.colorlegend').empty();
									if(diffs != undefined) {
										$('.colorlegend').append("<table><tr></tr><tr></tr><tr></tr></table>");
										$.each(diffs,function(i) {
											if(diffs[i] != undefined) {
												$('.colorlegend tbody tr:nth-child('+i+')').append("<td><font color='#ffffff'>"+diffs[i]+"</font></td><td style='background-color:"+colors(20+i)+"'>&nbsp;&nbsp;</td><td width='15px'></td>");
											}
										});
									}
								//}
								
								console.log("=====");
								console.log(diffs);
								
								colorUpdate();
								return false;
						}
				

						/*$(document).ready(function () {
							groups = [];
							for (var i = 0; i < data.length; i++) {
								groups.push({
									x: fixedWidth / 2,
									y: 200
								});
							}
							update();

							$('.visoptions').children().remove();
							$('.visoptions').append('<option selected="selected" value="0">All Designs</option>');
							$.each(headers, function (index, text) {
								$('.visoptions').append($('<option></option').val(index + 1).html(text))
							});

							$('.viscoloroptions').children().remove();
							$('.viscoloroptions').append('<option selected="selected" value="0">All Designs</option>');
							$.each(headers, function (index, text) {
								$('.viscoloroptions').append($('<option></option').val(index + 1).html(text))
							});
							
							$(".visoptions").on('change', function (f) {
								groups = [];
								if (this.value == 0) {
									for (var i = 0; i < data.length; i++) {
										groups.push({x: fixedWidth / 2,y: 200});
									}
								} else {
									var diffs = [];
									for (var i = 0; i < data.length; i++) {
										var count = 0;
										for (var j = 0; j < diffs.length; j++) {
											if (diffs[j] == data[i].comps[value - 1]) {
												count = count + 1;}
										}
										if (count == 0) {
											diffs.push(data[i].comps[value - 1]);
										}
									}

									// Creates the multiple groupings
									var range = [];
									for (i = 0; i < data.length; i++) {
										range.push(i);
										ind = diffs.indexOf(data[i].comps[value - 1]);
										wpg = fixedWidth / (diffs.length + 1);
										groups.push({
											x: wpg + ind * wpg,
											y: 200
										});
										data[i].rad = 20 - (diffs.length);
										data[i].group = ind;
									}
								}
								update(diffs);
								return false;
							});

							$(".viscoloroptions").on('change', function (f) {
								colorgroups = [];
								if (this.colorValue != 0) {
								
									var diffs = [];
									for (var i = 0; i < data.length; i++) {
										var count = 0;
										for (var j = 0; j < diffs.length; j++) {

											if (diffs[j] == data[i].comps[colorValue - 1]) {
												count = count + 1;
											}
										}
										if (count == 0) {
											diffs.push(data[i].comps[colorValue - 1]);
										}
									}
									// Creates the multiple groupings
									for (i = 0; i < data.length; i++) {
										ind = diffs.indexOf(data[i].comps[colorValue - 1]);
										data[i].color = 20 + ind;
									}
								}
								
								// creates the color legend
								$('.colorlegend').empty();
								if(diffs != undefined) {
									$('.colorlegend').append("<table><tr></tr><tr></tr><tr></tr></table>");
									$.each(diffs,function(i) {
										if(diffs[i] != undefined) {
											$('.colorlegend tbody tr:nth-child('+i%4+')').append("<td><font color='#ffffff'>"+diffs[i]+"</font></td><td style='background-color:"+colors(20+i)+"'>&nbsp;&nbsp;</td><td width='15px'></td>");
										}
									});
								}

								colorUpdate();
								return false;
							});
						});
			*/			
						
			/* Mark <vf-bubbles> as rendered */
			$(element).attr('rendered', 'true');
	}
	
	if ($(element).attr('group') == "update" || $(element).attr('group') == undefined){
		var x = eval($(this.element).attr('data')).groupVar[0];
		var y = eval($(this.element).attr('data')).colorVar[0];
		groupUpdates(x);
		colorUpdates(y);
		
		$(element).attr('group') == "false";
	}

	
}
		this.rerender = function () {	
			$(element).attr('group','update');
			this.render();
		}

        /* Update Method */
        this.update = function () {
			this.rerender();
        }
		
    }