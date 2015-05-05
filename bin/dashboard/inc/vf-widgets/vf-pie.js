/*///////////// vf-datatable /////////////*/
function vf_pie(element) {
	var self = this;
	this.data = clone(eval($(element).attr('data')));
	this.element = element;
	
	/* Render Method */
	this.render = function () 
	{
		var width = $(element).parent().width(),
			height = $(element).attr("ratio") * width,
			outerRadius = Math.min(width, height) / 2,
			innerRadius = $(element).attr("ir"),
			data = eval($(this.element).attr('data')),
			color = data.color,
			donut = d3.layout.pie(),
			arc = d3.svg.arc().innerRadius(innerRadius).outerRadius(outerRadius);
		
		var vis = d3.select("#" + $(element).attr("id"))
		  .append("svg")
			.data([data.data])
			.attr("width", width)
			.attr("height", height);
		
		var arcs = vis.selectAll("g.arc")
			.data(donut)
		  .enter().append("g")
			.attr("class", "arc")
			.attr("transform", "translate(" + outerRadius + "," + outerRadius + ")");
		
		arcs.append("path")
			.attr("fill", function(d, i) { 
				return color(i); 
			})
			.attr("d", arc);
		
		arcs.append("text")
			.attr("transform", function(d) { 
				return "translate(" + arc.centroid(d.label) + ")"; 
			})
			.attr("dy", ".35em")
			.attr("text-anchor", "middle")
			.attr("display", function(d) { 
				return d.value.value > .15 ? null : "none"; 
			})
			.text(function(d, i) { 
				return d.value.value.toFixed(2); 
			});
		
		arcs.append("text")
			.attr("transform", function(d) { 
				return "translate(" + arc.centroid(d.value) + ")"; 
			})
			.attr("dy", ".35em")
			.attr("text-anchor", "middle")
			.attr("display", function(d) { 
				return d.value > .15 ? null : "none"; 
			})
			.text(function(d, i) { 
				return d.value.toFixed(2); 
			});
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