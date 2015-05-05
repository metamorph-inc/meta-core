$(function() {	
/* 	$('ul#pageView li a').click(function (e) {
		e.preventDefault();
		$(this).tab('show');
	});
	
	$('#settingsBtn').click(function(){
		$('#settings').modal('show');
	}); */
	
	$('#downloadBtn').fadeOut();
	$('#settingsBtn').fadeOut();
	$('#helpBtn').fadeOut();
	$('#colorBtn').fadeIn();
	
	ko.applyBindings(vm, $('#tabpage')[0])
});