$(document).on('selectionChange', function(event, data){
	for (var i = 0; i < data.eventForWidget.length; i++)
	{
		//Reset the brush on the other widget
		if (data.eventForWidget[i] == "mvplot" || data.eventForWidget[i] == "pplot")
		{
			//need to test that the eventForWidget is not the same as the originating widget.
			//If not, BOTH extents will be minimized (h = 0)
			if(!(data.widget == data.eventForWidget[i]))
			{
				$("#" + data.eventForWidget[i] + " rect.extent")
					.attr("height", 0);
					//.attr("width", 0);
			}
		}
	}
		
	//No designs selected, which means none selected, and the brush is 0x0, then select every configuration
	if (!Boolean(data.selected.length) && $('#' + data.widget + ' rect.extent[x!="0"]').length == 0 &&  $('#' + data.widget + ' rect.extent[y!="0"]').length == 0) 
	{
		switch (data.widget)
		{
		case "pplot":
			d3.selectAll("#mvplot g circle").classed("selected", true);
			break;
		case "mvplot":
			d3.selectAll("#pplot path.design").classed("selected", true);
			break;
		}
		
		//RenderTable(tableHeader, tableData, "dsatable", data.data); //HOW WILL I CALL THIS TABLE?
	}
	
	$(document).trigger('selectionChangeMain', data);  //event for handeling all the selection change events
});

$(document).on('selectAll', function(event, data){
	d3.selectAll(" .design").classed("selected", true);
	
	//make all the brushes go away
	$('#pplot .extent[height!="0"]').each(function(){
		$(this).attr("height", 0);
	});
	
	$('#pplot .extent[y!="0"]').each(function(){
		$(this).attr("y", 0);
	});
	
	$("#pplot .resize").each(function(){
		if($(this).attr("style").split(";").length < 3)
		{
			$(this).attr("style", "cursor: ns-resize; display: none;");
		}
		
		$(this).attr("transform", "translate(0,0)");
	});
	
	$('#mvplot .extent[height!="0"]').each(function(){
		$(this).attr("height", 0)
	})
	$('#mvplot .extent[width!="0"]').each(function(){
		$(this).attr("width", 0)
	})
	
	selectorOfSelected = "#pplot g .selected"; //rely on pplot
	
	var tableData = [];
	var tableRow = [];
	
	$(selectorOfSelected).each(function(index, element){
		//global
		designID = $(element).attr("class").split(" ")[1];
		
		for(var i = 0; i < data.values.length; i++)
		{
			if(data.values[i].cid == designID)
			{
				tableRow.push(data.values[i].design);
				
				for(var j = 0; j < data.labels.length; j++)
				{
					if( !(data.labels[j] == "design") || !(data.labels[j] == "cid"))
					{
						tableRow.push(data.values[i][data.variables[j]]);
					}
				}
				tableData.push(tableRow);
				tableRow = [];							
			}
		}		
	});
	
	var headerData = [];
	for(var i = 0; i < data.labels.length; i++)
	{
		if (data.Unit[i] !== "")
		{
			headerData.push(data.labels[i] + " [" + data.Unit[i] + "]");
		}
		else
		{
			headerData.push(data.labels[i]);
		}
	}
	
	RenderTable(["Design ID"].concat(headerData), tableData, "dsatable", data); //HOW WILL I CALL THIS TABLE?
});

$(document).on('selectionChangeMain', function(event, data){
	//console.log(event);
	$('[rendered="true"]').each(function(){ 						
		if(!(data.widget == $(this).attr("id")))
		{
			//console.log($(this).attr("id"));
			var selection = d3.selectAll("#" + $(this).attr("id") + " circle").classed("selected", false);
			
			for(i = 0; i < data.selected.length; i++)
			{
				selection = d3.selectAll("#" + $(this).attr("id") + " ." + data.selected[i]).classed("selected", true);
			}
		}
	});
	
	
/* 	if(data.widget == "pplot")
	{
		if (Boolean($('#' + data.widget + ' .extent[height!="0"]').length) //If nothing is selected
		{
			//select ALL in pplot & mvplot
			d3.selectAll(" .design").classed("selected", true);
		}
	} */
	
	//Rerender the Selection table
	var tableHeader = ["Design ID"];
	
	for(var i = 0; i < data.data.labels.length; i++)
	{
		if (data.data.Unit[i] !== "")
		{
			tableHeader.push(data.data.labels[i] + " [" + data.data.Unit[i] + "]");
		}
		else
		{
			tableHeader.push(data.data.labels[i]);
		}
	}		
	
	var tableData = [];
	var tableRow = [];
	
	var selectorOfSelected;
	if(data.widget == "mvplot")
	{
		selectorOfSelected = "#" + data.widget + " g.cell:first .selected";
	}
	else
	{
		selectorOfSelected = "#pplot g .selected";
	}
	
	$(selectorOfSelected).each(function(index, element){
		//global
		designID = $(element).attr("class").split(" ")[1];
		
		for(var i = 0; i < data.data.values.length; i++)
		{
			if(data.data.values[i].cid == designID)
			{
				tableRow.push(data.data.values[i].design);
				
				for(var j = 0; j < data.data.labels.length; j++)
				{
					if( !(data.data.labels[j] == "design") || !(data.data.labels[j] == "cid"))
					{
						tableRow.push(data.data.values[i][data.data.variables[j]]);
					}
				}
				tableData.push(tableRow);
				tableRow = [];							
			}
		}
	});
		
	RenderTable(tableHeader, tableData, "dsatable", data.data); //HOW WILL I CALL THIS TABLE?
});

$(document).on("LBselectionChange", function(event, data){
	if(Boolean($("#" + data.widget + " g .selected").length))
	{
		if(!($("#" + data.widget + " g .selected").attr("class").split(" ")[1] == data.selected))
		{
			//deselectAll from the originating widget to only allow 1 selection at a time
			var deselectAll = d3.selectAll("#" + data.widget + " g .leader").classed("selected", false);
			//select clicked design on originating widget
			var selection = d3.selectAll("#" + data.widget + " ." + data.selected).classed("selected", true);
			
			for (var i = 0; i < data.eventForWidget.length; i++)
			{
				//deselectAll to only allow 1 selection at a time to all other widgets
				deselectAll = d3.selectAll("#" + data.eventForWidget[i] + " g .leader").classed("selected", false);
				//select clicked design on all other widgets
				selection = d3.selectAll("#" + data.eventForWidget[i] + " ." + data.selected).classed("selected", true);
			}
		}
	}
	else
	{
		//deselectAll from the originating widget to only allow 1 selection at a time
		var deselectAll = d3.selectAll("#" + data.widget + " g .leader").classed("selected", false);
		//select clicked design on originating widget
		var selection = d3.selectAll("#" + data.widget + " ." + data.selected.split(' ').join('')).classed("selected", true);
		
		for (var i = 0; i < data.eventForWidget.length; i++)
		{
			//deselectAll to only allow 1 selection at a time to all other widgets
			deselectAll = d3.selectAll("#" + data.eventForWidget[i] + " g .leader").classed("selected", false);
			//select clicked design on all other widgets
			selection = d3.selectAll("#" + data.eventForWidget[i] + " ." + data.selected.split(' ').join('')).classed("selected", true);
		}
	}
});