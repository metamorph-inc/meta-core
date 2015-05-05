/*
$(function() {	
	$('ul#pageView li a').click(function (e) {
		e.preventDefault();
		$(this).tab('show');
	});
});
*/

/*
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

$('#settings').on('hidden', function () {
	vf_viz.update();
});
*/

$(function() {
	$('#downloadBtn').fadeOut();
	$('#settingsBtn').fadeOut();
	$('#helpBtn').fadeIn();
	$('#colorBtn').fadeOut();

    ko.applyBindings(vm);
});
