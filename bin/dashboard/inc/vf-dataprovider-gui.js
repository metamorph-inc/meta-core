$(function(){
	//// Data Provider GUI
	
	vmui = {
		vm:"",
		
		headerRoot:"",
		
		bodyRoot:"",
		
		renderHeaders: function(){
			
			$(this.headerRoot).empty().append('<tr><th></th></tr>');
			for(var i = 0; i < window[this.vm].variables().length; i++){
				$(this.headerRoot + ' tr').append('<th><div>' + window[this.vm].variables()[i].Name + ' [' + window[this.vm].variables()[i].testBench + ']' + '</div></th>');
			}
		},
		renderBody: function(){
			$(this.bodyRoot).empty();
			for(var i = 0; i < window[this.vm].configurations().length; i++){
				$(this.bodyRoot).append('<tr><td><div>' + window[this.vm].configurations()[i].design + '</div></td></tr>');
				var variables = [];
				for(var k = 0; k < window[this.vm].configurations()[i].variables.length; k++){
					variables.push({
						Name: window[this.vm].configurations()[i].variables[k].Name,
						testBench: window[this.vm].configurations()[i].variables[k].testBench
					});
				}
				for(var j = 0; j < window[this.vm].variables().length; j++){
					if(variables.equivalentInArray({
							Name: window[this.vm].variables()[j].Name,
							testBench: window[this.vm].variables()[j].testBench
						})){
						$(this.bodyRoot + ' tr:last-of-type').append("<td><i class='icon-ok'></i></td>");
					}
					else{
						$(this.bodyRoot + ' tr:last-of-type').append('<td></td>');
					}
				}
			}
		},
		
		toggleActiveConfiguration: function (index){
			if(window[this.vm].configurations()[index].isActive()){
				window[this.vm].configurations()[index].isActive(false);
			}
			else{
				window[this.vm].configurations()[index].isActive(true);
			}
			this.updateColors();
		},
		
		toggleActiveVariables: function (index){
			if(window[this.vm].variables()[index].isActive()){
				window[this.vm].variables()[index].isActive(false);
			}
			else{
				window[this.vm].variables()[index].isActive(true);
			}
			this.updateColors();
		},
		
		updateColors: function(){
			var variables = $(this.headerRoot+ ' th').slice(1);
			var configurations = $(this.bodyRoot + ' tr td:first-of-type');
			
			if(window[this.vm].byVariable()){
				configurations.css('color','lightYellow');
				variables.css('color','lightYellow');
				for(var i = 0; i < variables.length; i++){
					if(window[this.vm].variables()[i].isActive()){
						$(variables[i]).css('color','lightYellow');
					}
					else{
						$(variables[i]).css('color','lightsteelblue');
					}
				}
			}
			else{
				configurations.css('color','lightYellow');
				variables.css('color','lightYellow');
				for(var i = 0; i < configurations.length; i++){
					if(window[this.vm].configurations()[i].isActive()){
						$(configurations[i]).css('color','lightYellow');
					}
					else{
						$(configurations[i]).css('color','lightsteelblue');
					}
				}		
			}		
		},
		
		setup: function(vmName, headerRoot, bodyRoot){
			var self = this;
			this.vm = vmName;
			this.headerRoot = headerRoot;
			this.bodyRoot = bodyRoot;
			this.renderHeaders();
			this.renderBody();
			this.updateColors();
			$(this.bodyRoot + ' tr td:first-of-type').click(function(event){
				var that = self;
				that.toggleActiveConfiguration(getIndexFromEventObj($(bodyRoot + ' tr td:first-of-type'),event));
				event.preventDefault();
				return false;
			});
			$(this.headerRoot + ' th+th').click(function(event){
				var that = self;
				that.toggleActiveVariables(getIndexFromEventObj($(headerRoot + ' th').slice(1),event));
				event.preventDefault();
				return false;
			});
		}
	};
	
	$(document).on('dp-ActiveToggled', function(){
		vmui.setup('vm','table.dpSettings thead','table.dpSettings tbody');
	});
	$('#settings').on('hidden', function () {
		loadTab($($("ul#mainTabs li.active a")).attr('vf-page'));
	});
	
	$('#settings').on('show', function () {
		// Dynamically resize modal 
		$('#settings')
			.width($('#settings').parent().width()-100)
			.css('margin-left',(-1*$('#settings').parent().width()+100)/2)
			.height($('#settings').parent().height()-100)
			.css('margin-top',-1*($('#settings').parent().height()-100)/2);
		$('#settings .modal-body')
			.css('max-height',$('#settings').innerHeight()-$('#settings .modal-header').outerHeight()-$('#settings .modal-footer').outerHeight()-30)
			.css('min-height',$('#settings').innerHeight()-$('#settings .modal-header').outerHeight()-$('#settings .modal-footer').outerHeight()-30);
	});
	
	$('#settings').on('shown', function () {
		// Dynamically resize modal 
		$('#settings')
			.width($('#settings').parent().width()-100)
			.css('margin-left',(-1*$('#settings').parent().width()+100)/2)
			.height($('#settings').parent().height()-100)
			.css('margin-top',-1*($('#settings').parent().height()-100)/2);
		$('#settings .modal-body')
			.css('max-height',$('#settings').innerHeight()-$('#settings .modal-header').outerHeight()-$('#settings .modal-footer').outerHeight()-30)
			.css('min-height',$('#settings').innerHeight()-$('#settings .modal-header').outerHeight()-$('#settings .modal-footer').outerHeight()-30);
	});
	
	
	
	
});