	$('#downloadBtn').fadeOut();
	$('#settingsBtn').fadeOut();
	$('#helpBtn').fadeOut();
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
/*
* The below code in vf-bubbles.js triggers this event.
* $(document).trigger($(element).attr('id') + '_clicked', [event, d, tooltip]);	
*/
$(document).off('Bubbles_clicked').on('Bubbles_clicked', function(event, mouseEvent, selectedDesignName){
	vm.selectedConfiguration(selectedDesignName);
	$('#designDetailsModal').modal('show');
	return false;
});
$(document).off('cfgTable_clicked').on('cfgTable_clicked', function(d){
	$("#cfgTable .sorting_1").each(function (subIndex, subElement) {
		$(subElement).html('<a onclick="$(\'#designDetailsModal\').modal(\'show\');vm.selectedConfiguration(\''+ this.innerText+'\')">'+ this.innerText+'</a>');
	});
	return false;
});
				
				
				