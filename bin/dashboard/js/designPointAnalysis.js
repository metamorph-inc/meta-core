	$('#downloadBtn').fadeOut();
	$('#settingsBtn').fadeOut();
	$('#helpBtn').fadeIn();
	$('#colorBtn').fadeOut();

/* $(function() {	
	$('ul#pageView li a').click(function (e) {

		e.preventDefault();
		$(this).tab('show');
	});
	
	$('#settingsBtn').click(function(){
		$('#settings').modal('show');
	});
}); */


ko.applyBindings(vm, $('#tabpage')[0]);
$('#settings').off('show');
$('.settings').on('show', function () {
	// Dynamically resize modal 
	$('#settings')
		.width($(window).width()-100)
		.css('margin-left',(-1*$(window).width()+100)/2)
		.height($(window).height()-100)
		.css('margin-top',-1*($(window).height()-100)/2);
	$('#settings .modal-body')
		.css('max-height',$('#settings').innerHeight()-$('#settings .modal-header').outerHeight()-$('#settings .modal-footer').outerHeight()-30)
		.css('min-height',$('#settings').innerHeight()-$('#settings .modal-header').outerHeight()-$('#settings .modal-footer').outerHeight()-30);
		
	$("#settings input[type='range']").each(function(){
		$(this).css("width", $(this).parent().width() - 10);
	});
});

$('#settings').off('shown');
$('.settings').on('shown', function () {
	// Dynamically resize modal 
	$('#settings')
		.width($(window).width()-100)
		.css('margin-left',(-1*$(window).width()+100)/2)
		.height($(window).height()-100)
		.css('margin-top',-1*($(window).height()-100)/2);
	$('#settings .modal-body')
		.css('max-height',$('#settings').innerHeight()-$('#settings .modal-header').outerHeight()-$('#settings .modal-footer').outerHeight()-30)
		.css('min-height',$('#settings').innerHeight()-$('#settings .modal-header').outerHeight()-$('#settings .modal-footer').outerHeight()-30);
		
	$("#settings input[type='range']").each(function(){
		$(this).css("width", $(this).parent().width() - 10);
	});
});

$('#settings').on('hidden', function () {
	vf_viz.update();
});

function printValue(type, num) 
{
	$("#value_" + type + "_" + num).text(document.getElementById("slider_" + type + "_" + num).value);
}

 function miniView(self)
 {
	if($(self).prop('checked') === true){
		$('#settings').modal('hide');
		$('#settings').prop('id','settingsBig');
		$('#settings2').prop('id','settings');
		$('#settings').modal('show');
	}
	else{
		$('#settings').modal('hide');
		$('#settings').prop('id','settings2');
		$('#settingsBig').prop('id','settings');
		$('#settings').modal('show');
	}
 }
 

$("div.colorSelector").each(function(i){
	 console.log("=====================MJD======================================");
	 console.log(this);
	
	var self = this;
	
	$(this).ColorPicker({
		color: $(self).attr('color'),
		onShow: function(colpkr){
			$(colpkr).fadeIn(500);
			return false;
		},
		onHide: function(colpkr){
			$(colpkr).fadeOut(500);
			return false;
		},
		onChange: function(hbs, hex, rgb){
			vm.listOfPETResponses()[+$(self).attr("index")].color("#" + hex);
		}
	});
});

$(function(){
	if ($(window).width() < 1200)
	{
		$("#respSettingsBtns button").eq(0).text("All Thrsh");
		$("#respSettingsBtns button").eq(1).text("All Objs");
	}
})

$(function(){
	$('#colorBtn').fadeOut();
	$('#downloadBtn').fadeOut();
	$('#settingsBtn').fadeOut();
	$('#helpBtn').fadeIn();

	// Dynamically resize modal 
	$('#ccpdetail')
		.width($(window).width()-100)
		.css('margin-left',(-1*$(window).width()+100)/2)
		.height($(window).height()-100)
		.css('margin-top',-1*($(window).height()-100)/2);
	$('#ccpdetail .modal-body')
		.css('max-height',$('#ccpdetail').innerHeight()-$('#ccpdetail .modal-header').outerHeight()-$('#ccpdetail .modal-footer').outerHeight()-85)
		.css('min-height',$('#ccpdetail').innerHeight()-$('#ccpdetail .modal-header').outerHeight()-$('#ccpdetail .modal-footer').outerHeight()-85);
		
	vf_viz.rerender(); //force rerendering due to ccpdetail
});

$(document).off('co_plot_dragend').on('co_plot_dragend', function(event, click, d){
	for(var i = 0; i < vm.listOfPETVariables().length; i++)
	{
		if ( vm.listOfPETVariables()[i].name() == d.x.label){
			//console.debug(vm.listOfPETVariables()[i].name());
			//console.debug("before: " + vm.listOfPETVariables()[i].value());
			vm.listOfPETVariables()[i].value(d.xdomain);
			//console.debug("after: " + vm.listOfPETVariables()[i].value());
			break;
		}
	}
	return false;
});

/*
	The below code in vf-constraintplot.js will triger this event.
 *$(document).trigger($(element).attr('id') + '_clicked', [event, d, xdomain, ydomain]);
*/
$(document).off('co_plot_clicked').on('co_plot_clicked', function(event, click, d, xdomain, ydomain){
	for(var i = 0; i < vm.listOfPETVariables().length; i++)
	{
		if ( vm.listOfPETVariables()[i].name() == d.x.label){
			vm.listOfPETVariables()[i].value(xdomain);
			break;
		}
	}
	for(var i = 0; i < vm.listOfPETVariables().length; i++)
	{
		if ( vm.listOfPETVariables()[i].name() == d.y.label){
			vm.listOfPETVariables()[i].value(ydomain);
			break;
		}
	}
	return false;
});