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
	});
	
	$('#helpBtn').click(function(){
		$('#help').modal('show');
	}); */
	
	$('.modal-body').disableSelection();
});

$('#settings').on('show shown', function () {
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
	vf_viz.update();
});

ko.applyBindings(vm, $('#tabpage')[0]);

if(vm.topsisVariables().length < 1){
	$('#settings').modal('show');
}