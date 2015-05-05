$(function(){
	//vf_viz.blackList.push('heatmap');
	$('#colorBtn').fadeOut();
	$('#downloadBtn').fadeOut();
	$('#settingsBtn').fadeOut();
	$('#helpBtn').fadeIn();
	
	$('ul#pageView li a').click(function (e) {
		e.preventDefault();
		$(this).tab('show');
	});
	
	$('#helpBtn').click(function(){
		$('#help').modal('show');
	});
	
	$('.modal-body').disableSelection();
	
	// Dynamically resize modal 
	$('#pccModal')
		.width($(window).width()-100)
		.css('margin-left',(-1*$(window).width()+100)/2)
		.height($(window).height()-100)
		.css('margin-top',-1*($(window).height()-100)/2);
	$('#pccModal .modal-body')
		.css('max-height',$('#help').innerHeight()-$('#pccModal .modal-header').outerHeight()-$('#pccModal .modal-footer').outerHeight()-30)
		.css('min-height',$('#help').innerHeight()-$('#pccModal .modal-header').outerHeight()-$('#pccModal .modal-footer').outerHeight()-30);
	
	
	ko.applyBindings(vm);
	vf_viz.rerender(); //force rerendering due to pccModal
});

$('#help').on('show shown', function () {
	// Dynamically resize modal 
	$('#help')
		.width($(window).width()-100)
		.css('margin-left',(-1*$(window).width()+100)/2)
		.height($(window).height()-100)
		.css('margin-top',-1*($(window).height()-100)/2);
	$('#help .modal-body')
		.css('max-height',$('#help').innerHeight()-$('#help .modal-header').outerHeight()-$('#help .modal-footer').outerHeight()-30)
		.css('min-height',$('#help').innerHeight()-$('#help .modal-header').outerHeight()-$('#help .modal-footer').outerHeight()-30);
});

$('#pccModal').on('show shown', function () {
	// Dynamically resize modal 
	//debugger
	
	$(this)
		.width($(window).width()-100)
		.css('margin-left',(-1*$(window).width()+100)/2)
		.height($(window).height()-100)
		.css('margin-top',-1*($(window).height()-100)/2);
	$(this).children(".modal-body")
		.css('max-height',$('#pccModal').innerHeight()-$('#pccModal .modal-header').outerHeight()-$('#pccModal .modal-footer').outerHeight()-30)
		.css('min-height',$('#pccModal').innerHeight()-$('#pccModal .modal-header').outerHeight()-$('#pccModal .modal-footer').outerHeight()-30);
});

$('#pccModal').on('shown', function () {
	//small CSS hacks for formatting:
	//$("#pccTabletable th").eq(0).width("1px"); //this makes the first column fit to the text, i.e. only as big as it needs to be to display its contents
	
	ko.applyBindings(vm, $(this)[0]);
	//$("#pccTabletable th").width('33%');
	vf_viz.rerender();
	//$("#pccTabletable th").width('33%');
});


$(document).off('heatmap_click_on_rect').on('heatmap_click_on_rect', function(event, click, d){
	//debugger;
	if (click.button === 0 && !click.ctrlKey)	{
		//if (d.values != "" && d.values != -1 && isSet(d.values))
		if (d.values != -1 && isSet(d.values))
		{
			ClickEvent(d.benches, d.configs);
		}
	}
	if (click.button === 2 || click.ctrlKey){
		CtrlClickEvent(d.benches, d.configs, click);
	}
	return false;
})

function ClickEvent(bench, config)
{
	//debugger;
	vf_viz.blackList.push('heatmap');
	// set  vm.currentPccResultIndex for bench and design pair
	for (var i = 0; i < vm.activePccResults().length; i++)
	{
		if(vm.activePccResults()[i].testBench == bench && vm.activePccResults()[i].design == config)
		{
			vm.currentPccResultIndex(i);
			break;
		}
	}
	
	// set current bench and design
	vm.selectedBench(bench);
	vm.selectedConfiguration(config);
	
	// show modal
	$('#pccModal').modal('show');
	vf_viz.blackList.remove('heatmap');
}

function CtrlClickEvent(bench, config, event)
{
	//debugger;
	vf_viz.blackList.push('heatmap');
	vm.selectedBench(bench);
	vm.selectedConfiguration(config);
	for (var i = 0; i < vm.activePccResults().length; i++)
	{
		if(vm.activePccResults()[i].testBench == bench && vm.activePccResults()[i].design == config)
		{
			vm.currentPccResultIndex(i);
			break;
		}
	}
	console.log("heatmap testing");
	console.log(vm.dataproviders.pccHeatMap());
	$("#contextmenu").css({
		position:'absolute',
		top: event.pageY,
		left: event.pageX,
		zIndex:5000
	});
	$("#contextmenu").addClass("open");//.delay(1000);
	event.stopPropagation();
	vf_viz.blackList.remove('heatmap');
}

$("#contextmenu").delegate("ul li", 'click', function(){
	//debugger;
		
	if ($(this).children("a[type='up']").length > 0)
	{
		var upIndexArray = vm.activeUpPccResultsIds();
		var index = +$(this).children("a[type='up']").attr("index");
		var value = +$(this).children("a[type='up']").attr("value");
		upIndexArray[index] = value;
		vm.activeUpPccResultsIds(upIndexArray);
	}
	else if ($(this).children("a[type='sa']").length > 0)
	{
		var saIndexArray = vm.activeSaPccResultsIds();
		index = +$(this).children("a[type='sa']").attr("index");
		value = +$(this).children("a[type='sa']").attr("value");
		saIndexArray[index] = value;
		vm.activeSaPccResultsIds(saIndexArray);
	}
});
