$(function() {	
	$('#downloadBtn').fadeOut();
	$('#settingsBtn').fadeOut();
	$('#helpBtn').fadeOut();
	$('#colorBtn').fadeOut();
/* 	$.ajax({
		dataType: "json",
		url: "./inc/model_perf.json",
		success: function(data){
			vm.dataproviders.smpData = data;
			ko.applyBindings(vm, $('#tabpage')[0]);
		}
	}); */
});

ko.applyBindings(vm, $('#tabpage')[0]);

$(".mfediv").each(function(i){
	$(this).append('<vf-hist id="mfe' + i + '" AR="0.7" data="vm.dataproviders.smpData[' + i +'].MFE.data"></vf-hist>');
});

//debugger;

$(".variables").each(function(i){
	debugger;
	
	var variableList = "";
	
	for(var j = 0; j < vm.dataproviders.smpData[i].variables.length; j++)
	{
		variableList += vm.dataproviders.smpData[i].variables[j] + " [" + vm.dataproviders.smpData[i].ranges[vm.dataproviders.smpData[i].variables[j]][0] + ", " + vm.dataproviders.smpData[i].ranges[vm.dataproviders.smpData[i].variables[j]][1] + "] <br />";
	}
	
	$(this).append(variableList);
});

$(".mrediv").each(function(i){
	$(this).append('<vf-hist id="mre' + i + '" AR="0.7" data="vm.dataproviders.smpData[' + i +'].MRE.data"></vf-hist>');
});

$(".actByPredTraindiv").each(function(i){
	$(this).append('<vf-splot2 id="actByPredTrain' + i + '" AR="1" r="2" 45degline="1" xlabel="Predicted" ylabel="Actual" square="1" data="vm.dataproviders.smpData[' + i +'].actualByPredicted.training"></vf-splot2>');
});

$(".actByPredValdiv").each(function(i){
	$(this).append('<vf-splot2 id="actByPredVal' + i + '" AR="1" r="2" 45degline="1" xlabel="Predicted" ylabel="Actual" square="1" data="vm.dataproviders.smpData[' + i +'].actualByPredicted.validation"></vf-splot2>');
});
			
$(".resByPredTraindiv").each(function(i){
	$(this).append('<vf-splot2 id="resByPredTrain' + i + '" AR="1" r="2" zeroline="1" xlabel="Predicted" ylabel="Residual" square="1" data="vm.dataproviders.smpData[' + i +'].residualByPredicted.training"></vf-splot2>');
});

$(".resByPredValdiv").each(function(i){
	$(this).append('<vf-splot2 id="resByPredVal' + i + '" AR="1" r="2" zeroline="1" xlabel="Predicted" ylabel="Residual" square="1" data="vm.dataproviders.smpData[' + i +'].residualByPredicted.validation"></vf-splot2>');
});

$("a.accordion-toggle").prepend(function(i){
	if (i == 0)
	{
		return "<i class='icon-minus icon-white' /><i class='icon-plus icon-white' style='display: none;' />";
	}
	else
	{
		return "<i class='icon-minus icon-white' style='display: none;' /><i class='icon-plus icon-white' />";
	}
});

$("a.accordion-toggle").click(function(e){
	//debugger;

	if ($(this).find("i.icon-minus").css("display") == "inline-block")
	{
		$(this).find("i.icon-minus").css("display", "none");
		$(this).find("i.icon-plus").css("display", "inline-block");
	}
	else
	{
		$("#smpAccordion i.icon-minus").css("display", "none");
		$("#smpAccordion i.icon-plus").css("display", "inline-block");
		$(this).find("i").toggle();
	}
});

vf_viz.rerender();