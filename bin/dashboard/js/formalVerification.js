$(function(){
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
	$('#qrModal')
		.width($(window).width()-100)
		.css('margin-left',(-1*$(window).width()+100)/2)
		.height($(window).height()-100)
		.css('margin-top',-1*($(window).height()-100)/2);
	$('#qrModal .modal-body')
		.css('max-height',$('#qrModal').innerHeight()-$('#qrModal .modal-header').outerHeight()-$('#qrModal .modal-footer').outerHeight()-85)
		.css('min-height',$('#qrModal').innerHeight()-$('#qrModal .modal-header').outerHeight()-$('#qrModal .modal-footer').outerHeight()-85);
	

	vf_viz.rerender(); //force rerendering due to qrModal
});


$('#qrModal').on('show shown', function () {
	// Dynamically resize modal 
	//debugger;
	$(this)
		.width($(window).width()-100)
		.css('margin-left',(-1*$(window).width()+100)/2)
		.height($(window).height()-100)
		.css('margin-top',-1*($(window).height()-100)/2);
	$(this).children(".modal-body")
		.css('max-height',$('#qrModal').innerHeight()-$('#qrModal .modal-header').outerHeight()-$('#qrModal .modal-footer').outerHeight()-85)
		.css('min-height',$('#qrModal').innerHeight()-$('#qrModal .modal-header').outerHeight()-$('#qrModal .modal-footer').outerHeight()-85);
});

$('#qrModal').on('shown', function () {
	ko.applyBindings(vm, $(this)[0]);
	vf_viz.rerender();
});


$(document).off('heatmap_qualitative_click_on_rect').on('heatmap_qualitative_click_on_rect', function(event, click, d){

	if (click.button === 0 && !click.ctrlKey)	{
		console.debug( d.values );
		console.debug(isSet(d.values));
		if (d.values != "" && d.values != -1)
		{
			ClickEvent(d.benches, d.configs);
		}
	}
	/*if (click.button === 2 || click.ctrlKey){
		CtrlClickEvent(d.benches, d.configs, click);
	}
	*/
	return false;
})

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

function ClickEvent(bench, config)
{
	
	vm.qrSelectedX(bench);
	vm.qrSelectedY(config);
	$('#qrModal').modal('show');
}
ko.applyBindings(vm);

