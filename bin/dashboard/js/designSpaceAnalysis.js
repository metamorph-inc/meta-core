$(function() {	
	$('#downloadBtn').fadeOut();
	$('#settingsBtn').fadeIn();
	$('#helpBtn').fadeIn();
	$('#colorBtn').fadeIn();
	
/* 	$('ul#pageView li a').click(function (e) {
		e.preventDefault();
		$(this).tab('show');
	});
	
	$('#settingsBtn').click(function(){
		$('#settings').modal('show');
	}); */
	
	$('.modal-body').disableSelection();
	
	$("#contextmenu.dropdown ul li").on("click",function(e){
		//$(this).attr("config");
		//console.log("MJD", $(e.target).attr("id"));
		//debugger
		var self = $(e.target);
		var config = self.parent().parent().parent().attr("config");
		var id = self.attr("id");
		
		if (id == "add")
		{
			$("vf-mvplot [config=" + config + "]").css("fill", "#ff4444");
			$("vf-pplot [config=" + config + "]").css("stroke", "#ff4444");
			
			vm.listOfCompareConfigurations.push(config);
			vm.changeCompareSelectedStatus(config, true);
			//$("#compareBtn").fadeIn();
		}
		else if(id == "blacklist")
		{
			debugger;
			
			var allConfigs = clone(vm.allConfigurations());
			var indexesToRemove = [];
			
			for (var i = 0; i < allConfigs.length; i++)
			{
				if(allConfigs[i].design == config)
				{
					indexesToRemove.push(i);
				}
			}
			
			for(var i = 0; i < indexesToRemove.length; i++)
			{
				allConfigs.splice(indexesToRemove[i], 1);
			}
			
			vm.allConfigurations(allConfigs);
		}
		
		ko.applyBindings(vm);
	});
	
/* 	$("#compTableGeneral a li").on(function(){
		alert("hi!");
	}); */
	
});

Unique =
  function(that) {
    var a = [];
    var l = that.length;
    for(var i=0; i<l; i++) {
      for(var j=i+1; j<l; j++) {
        // If that[i] is found later in the array
        if (that[i] === that[j])
          j = ++i;
      }
      a.push(that[i]);
    }
    return a;
  };
 
//render the Selected Design table
function RenderTable(header, data, element, dataObj) 
{
	//the dataObj parameter is only required if classes on each row is req'd
	
	$("#" + element).empty();
	
	var table = d3.select("#"+element)
		.append("table")
		.attr("class", "table-bordered");
	
	//append the header row
	table.append("thead")
		.selectAll("th")
		.data(header)
		.enter().append("th")		
		.text(function(d){
			return d;
		});
	
	//append rows
	table.append("tbody")
		.selectAll("tr")
		.data(data)
		.enter().append("tr")
		.attr("class", function(d){
			//add the class for each metricId as part of its corresponding row
			if(!(dataObj == undefined))
			{
				for(var j = 0; j < dataObj.design.length; j++)
				{
					if(dataObj.design[j] == d[0])
					{
						return dataObj.cid[j];
					}
				}
			}
		})
		.selectAll("td")       
		.data(function(d){
				return d;
		})       
		.enter().append("td")
		.text(function(d){
				return d;
		});
}


$('#settings').on('show shown', function () {
	// block updates
	vf_viz.blockAll();
	// Dynamically resize modal 
	$('#settings')
		.width($(window).width()-100)
		.css('margin-left',(-1*$(window).width()+100)/2)
		.height($(window).height()-100)
		.css('margin-top',-1*($(window).height()-100)/2);
	$('#settings .modal-body')
		.css('max-height',$('#settings').innerHeight()-$('#settings .modal-header').outerHeight()-$('#settings .modal-footer').outerHeight()-30)
		.css('min-height',$('#settings').innerHeight()-$('#settings .modal-header').outerHeight()-$('#settings .modal-footer').outerHeight()-30);
});

$('#settings').on('hidden', function () {
	vf_viz.unblockAll();
	vf_viz.whiteList.removeElement('d_v_comparelist')
	vf_viz.update();
});

$('#settings').on('shown', function () {
	vf_viz.whiteList.push('d_v_comparelist');
	vf_viz.updateById('d_v_comparelist');
});

ko.applyBindings(vm, $('#tabpage')[0]);

if(vm.currentVariables().length < 2){
	vf_viz.rerender();
	$('#settings').modal('show');
}

//MJD
$("#pplot_helpBtn").click(function(){
	$("#pplot_help").modal('show');
});

$("#mvplot_helpBtn").click(function(){
	$("#mvplot_help").modal('show');
});

$("#dsatable_helpBtn").click(function(){
	$("#dsatable_help").modal('show');
});

/* $("#helpBtn").click(function(){
	$("#dsatable_help").modal('show');
}); */
$(document).off('d_v_comparelist_click_on_rect').on('d_v_comparelist_click_on_rect', function(event, click, d, newSelection){
	
	//debugger;
	if ( d.testBench instanceof Array ){ //design is selected
		for ( var i = 0; i < vm.configurations().length; i++ ){
			if (vm.configurations()[i].design == d.displayName){
				//console.debug( "Design: find matching name... " + d.displayName );
				//console.debug("before: " + vm.configurations()[i].isActive());
				vm.configurations()[i].isActive(newSelection);
				//console.debug("after: " + vm.configurations()[i].isActive());
				//debugger;
				break;
			}
		}
	}
	else { //variable is selected
		for ( var i = 0; i < vm.variables().length; i++ )
		{
			console.debug( "1: "+ d.displayName);
			console.debug( "2: " + vm.variables()[i].Name + " [" + vm.variables()[i].testBench +"]");
			if ( (vm.variables()[i].Name + " [" + vm.variables()[i].testBench +"]") == d.displayName){
				//console.debug( "Variable: find matching name... " + d.displayName );
				//console.debug("before: " + vm.variables()[i].isActive());
				vm.variables()[i].isActive(newSelection);
				//console.debug("after: " + vm.variables()[i].isActive());
				//debugger;
				break;
			}
		}
	}
	return false;
})
$(document).off('pplot_clicked').on('pplot_clicked', function(event, mouseEvent, selectedDesignName){
	vm.selectedConfiguration(selectedDesignName);
	$('#designDetailsModal').modal('show');
	return false;
})
$(document).off('mvplot_clicked').on('mvplot_clicked', function(event, mouseEvent, selectedDesignName){
	vm.selectedConfiguration(selectedDesignName);
	$('#designDetailsModal').modal('show');
	return false;
})
