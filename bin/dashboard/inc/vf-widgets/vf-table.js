/*///////////// vf-table /////////////*/
function vf_table(element) {
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
		
		if (!($(element).attr('rendered') == undefined || $(element).attr('rendered') == "false")) 
		{
			return 'vf-table: already rendered';
		}
		var outputs = eval($(this.element).attr('data'));
		//console.debug(outputs);
		var table = d3.select("#" + $(element).attr("id"))
		.append("table")
		.attr("class", "table-bordered");
		
		table
		.append("thead")
		.selectAll("th")
		.data(outputs.headers)
		.enter()
		.append("th")
		.text(function (d) {
			return d;
		});
		
		table
		.append("tbody")
		.selectAll("tr")
		.data(outputs.data)
		.enter()
		.append("tr")
		.selectAll("td")
		.data(function (d) {
			return d;
		})
		.enter()
		.append("td")			
		.text(function (d) {
			return d;
		});
		
		
		/* taylor
		adds class names to the tables so they can be modified using css, but doesn't work
		since modifying the css files doesn't change the table no matter what I do.  Modifications
		to bootstrap.css allowed for overall changes to table size, but not to cell size
		table
		.selectAll("td")
		.attr("class",function (d,i) {
			return "width"+i;
			});
		*/
		
		/* The following code allows the content of <td> to rendered HTML */
		$('#' + $(element).attr("id") + ' td').each(function (subIndex, subElement) {
			$(subElement).html($(subElement).text());
		});
		
		/* Mark <vf-table> as rendered */
		$(element).attr('rendered', 'true');
		
		/* Trigger render methods for embeded widgets */
		//$(document).trigger('vf-widget-applyClasses');
		//$(document).trigger('vf-widget-render');
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
