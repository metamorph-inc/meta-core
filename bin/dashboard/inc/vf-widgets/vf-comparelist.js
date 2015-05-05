
function vf_comparelist(element) {
	var self = this;
	this.element = element;
			  
	
				function createList(comparelist, data, a_list_width, a_list_height) {
				
					var insersectionOfConnected = [];
					comparelist.selectAll("rect")
					.data(data)
					.enter().append("rect")
					.attr("y", function (d, i) { return i * (a_list_height + padding_btw_list); })
					.attr( "rx", "5")
					.attr( "ry", "5")
					.attr("width", a_list_width)
					.attr("height", a_list_height)
					.attr("id", function (d) { return d.id; })
					.attr("fill",  function(d){
						console.debug( d.color);
						return d.color;
					})
					.attr("selected1", function (d) {
						if ( d.selected1 == true) {
							if ( insersectionOfConnected.length == 0 )
								insersectionOfConnected = d.connectedTo;
							else
								insersectionOfConnected = insersectionOfConnected.filter( 
									function(n){ 
										//console.debug( n + " -----" + d.connectedTo );
										//console.debug( d.connectedTo.indexOf(n) > -1 );
										return d.connectedTo.indexOf(n) > -1;
									});
								
							//console.debug(insersectionOfConnected);
						}						
						return d.selected1; 
					});
					
					comparelist.selectAll("text")
					.data(data)
					.enter().append("text")
					.attr("x", 0)
					.attr("y", function (d, i) { return i * (a_list_height + padding_btw_list) + a_list_height/2})
					.attr("dx", 3) // padding-right
					.attr("dy", ".35em") // vertical-align: middle
					.attr("text-anchor", "start") // text-align: right
					.attr("id", function (d) { return d.id; })
					.attr("selected1", function (d) { return d.selected1; })
					.attr("mousein", false)
					.text(function (d) { return d.displayName }); 
					
					return insersectionOfConnected;
					
				}
				
			//addListEvent(comparelist1, data1, "#list1", "rect", "text", "id1");
				function addListEventLeft(comparelist, data, selector, listeningElement, otherlisteningElement,  lineId) {
					comparelist.selectAll(selector + " " + listeningElement) //i.e, "#list1 rect" or #list1 text
						.data(data)
					.on("click", function (d, i) {
						var $s_rect = $(listeningElement + "[id='" + d.id + "']");
						if ($s_rect.attr("selected1") == "true"){
							$s_rect.attr("selected1", false);  
							$(document).trigger($(element).attr('id') + '_click_on_rect', [event ,d, false]);
						}
						else {
							$s_rect.attr("selected1", true);          
							$(document).trigger($(element).attr('id') + '_click_on_rect', [event ,d, true]);
						}
						//just set attribute
						var $s_text = $(otherlisteningElement + "[id='" + d.id + "']");
						if ($s_text.attr("selected1") == "true"){
							$s_text.attr("selected1", false);  
						}
						else {
							$s_text.attr("selected1", true);  
						}
						//$(document).trigger('vf-widget-updateById', [event, $(element).attr('id')]);
						
					})
					.on("mouseover", function (d, i) {
						$("line[" + lineId + "='" + d.id + "']").attr("selected1", true);
						$("line").each(function () {
							if ($(this).attr(lineId) != d.id) {
								$(this).attr("selected1", false);
							}
						});
						
						$("text[id='" + d.id + "']").attr("mousein", true);
						$("rect[id='" + d.id + "']").attr("mousein", true);
						
						
					})
					.on("mouseout", function (d, i) {
						$("line").each(function () {
							$(this).attr("selected1", false);
						});
						
						$("text[id='" + d.id + "']").attr("mousein", false);
						$("rect[id='" + d.id + "']").attr("mousein", false);
					});   	
				}
				//rect and text selected1 (true/false) is changed when the click action cause vm to call vf-comparelist.js's update again
				function addListEventRight(comparelist, data, selector, listeningElement, otherlisteningElement,  lineId, insersectionOfConnected) {
					comparelist.selectAll(selector + " " + listeningElement) //i.e, "#list2 rect" or #list2 text
						.data(data)
					.on("click", function (d, i) {
						if (insersectionOfConnected.indexOf(d.id) > -1 ){
							//console.debug(d.id + "is IN - so updated by click");
							var $s_rect = $(listeningElement + "[id='" + d.id + "']");
							if ($s_rect.attr("selected1") == "true"){
								$(document).trigger($(element).attr('id') + '_click_on_rect', [event ,d, false]);
							}
							else {
								$(document).trigger($(element).attr('id') + '_click_on_rect', [event ,d, true]);
							}
						}
						//else
							//console.debug( d.id + "is NOT - so No action for click");
					})
					.on("mouseover", function (d, i) {
						$("line[" + lineId + "='" + d.id + "']").attr("selected1", true);
						$("line").each(function () {
							if ($(this).attr(lineId) != d.id) {
								$(this).attr("selected1", false);
							}
						});
						if (insersectionOfConnected.indexOf(d.id) > -1 ){
							$("text[id='" + d.id + "']").attr("mousein", true);
							$("rect[id='" + d.id + "']").attr("mousein", true);
							//console.debug(d.id + "is IN");
						}
						//else
							//console.debug(d.id + " is NOT");
					})
					.on("mouseout", function (d, i) {
						$("line").each(function () {
							$(this).attr("selected1", false);
						});
						
						$("text[id='" + d.id + "']").attr("mousein", false);
						$("rect[id='" + d.id + "']").attr("mousein", false);
					});   	
				}
			
				function createLines(comparelist, x1, x2, deltaY, data) {
					comparelist.selectAll("line")
						.data(data)
						.enter().append("line")
						.attr("id1", function (d) {return d.id1; })
						.attr("id2", function (d) {return d.id2; })
						.attr("x1", x1)
						.attr("x2", x2)
						.attr("y1", function (d, i) { return d.start * (deltaY + padding_btw_list)+ deltaY / 2; })
						.attr("y2", function (d, i) { return d.end * (deltaY +  padding_btw_list) + deltaY / 2; })
						.attr("selected1", false)
						.style("stroke", "#000");
				}

				 
				//this is data tha case data2 is not provided by json and order is determined by data1 only
				function createLineData(data1, data2) {
					var all = [];
					var alldata2ids = [];
					for (var i = 0; i < data2.length; i++)
						alldata2ids.push(data2[i].id);
					for (var i = 0; i < data1.length; i++) {
						for (var j = 0; j < data1[i].connectedTo.length; j++) {
							all.push({	"start": i, 
										"end": alldata2ids.indexOf(data1[i].connectedTo[j]),
										"id1": data1[i].id,
										"id2": data1[i].connectedTo[j]});
						}
					}
					return all; // [[0, 1, Design1, Variable1] etc...
				}
				
	var padding_btw_list = 5;
	/* Render Method */
	this.render = function(){
		if (($(element).attr('rendered') == undefined || $(element).attr('rendered') == "false")) {
		   
			
				var jsondata = eval($(element).attr('data'));
				console.debug( "render is called");
				console.debug(jsondata);
				
			
				var data1 = jsondata.dataLeft;
				var data2 = jsondata.dataRight;
				var linedata = createLineData(data1, data2);
				var maxdata = Math.max( data1.length, data2.length);
				var padding_top = 5; //distance from top
				var padding_x = 0; //distance from sides

				//var setting_style = $('#settings').attr('style');
				//console.debug( setting_style);
				//var setting_width = -1;
				//if ( setting_style != undefined){
				//	setting_width = new Number(setting_style.substring( setting_style.indexOf('width: ') + 7, setting_style.indexOf('px;')));
				//}
				//if ( setting_width < 0 )
				var setting_width = Math.max(0, $(element).parent().width() - padding_x);				
				var grid = Math.floor(setting_width/7);
				var a_list_width = grid * 2;
				var a_line_width = grid;	
				var a_list_height = 30;					
					
				
				var comparelist1 = d3.select('#'+$(element).attr('id')).append("svg")
				.attr("class", "comparelist")
				.attr("width", setting_width)
				.attr("height", ( a_list_height + padding_btw_list) * maxdata + padding_top)
				.append("g")
				.attr("id", "list1")
				.attr("transform", "translate("+grid+"," + padding_top + ")");
				//.attr("transform", "translate(" + grid + "," + padding_top + ")");

				
				//var comparelist2_x = grid + a_list_width + a_line_width;
				var comparelist2_x = grid+ a_list_width + a_line_width;
				var comparelist2 = d3.select('#'+$(element).attr('id')).select("svg")
				.append("g")
				.attr("id", "list2")
				.attr("transform", "translate(" + comparelist2_x + "," + padding_top + ")");
			
				
				var insersectionOfConnected = createList(comparelist1, data1, a_list_width, a_list_height);
				
				createList(comparelist2, data2, a_list_width, a_list_height);
				createLines(comparelist1, a_list_width, a_list_width + a_line_width, a_list_height, linedata);
							
				addListEventLeft(comparelist1, data1, "#list1", "rect", "text", "id1");
				addListEventRight(comparelist2, data2, "#list2", "rect", "text", "id2", insersectionOfConnected);
				addListEventLeft(comparelist1, data1, "#list1", "text", "rect", "id1");
				addListEventRight(comparelist2, data2, "#list2", "text", "rect", "id2", insersectionOfConnected);
				
				
		
		}
		/* Mark <vf-table> as rendered */
			$(element).attr('rendered', 'true');
			$(element).attr('updated', 'false');
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
