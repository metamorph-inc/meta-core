
    var resetState = false;

    //Dump VM
    function dumpVM(){
        console.log("main.js: dumpVM: enter");
        /*
        var state = {};
        state.variables = ko.toJS(vm.variables);
        state.allVariables = ko.toJS(vm.allVariables);
        state.commonVariables = ko.toJS(vm.commonVariables);
        state.currentVariables = ko.toJS(vm.currentVariables);
        state.activeVariables = ko.toJS(vm.activeVariables);
        state.allConfigurations = ko.toJS(vm.allConfigurations);
        state.selectedConfigurations = ko.toJS(vm.selectedConfigurations);
        */
        var state = clone(vm);
        var blob;
        blob = new Blob([JSON.stringify(state,null,4)],{type: "application/json;charset="+document.characterSet});
		var s=saveAs(blob, "VMdump.json");
    }

$(function() {

	$('#copyright').html('&copy;' + new Date().getFullYear() + ' Sponsored by DARPA. Developed at ASDL');

    console.log("ready: enter");
	$('#source').html(window.location.origin);
    
    console.log("main.js: bind to vf-dataLoadComplete");
    $(document).unbind('vf-dataLoadComplete').bind('vf-dataLoadComplete', function (event, self) {
		console.log('main.js: Received vf-dataLoadComplete');
		vf_viz.render();
	});
	
	$('#sourceBtn').tooltip({
		placement:'top',
		title:'Click to change data source'
	});
	
	$('#changeSource').modal({
		backdrop:true,
		keyboard:true,
		show:false
	}).modal('hide');
	
	$('#sourceBtn').click(function(){
		$('#changeSource').modal('show');
	});
	
	
	$('ul#pageView li a').click(function (e) {
		e.preventDefault();
		$(this).tab('show');
	});
	
	$('#settingsBtn').click(function(){
		$('#settings').modal('show');
	});
	
	$('#helpBtn').click(function(){
		$('#help').modal('show');
	});
	
	
	
	$('#lockedBtn').click(function(){
        console.log("main.js: lockedBtn: enter");
        console.log("this="+this);
        
		// remove resizable and draggable properties from '.widget'
		$( ".widget" ).resizable('destroy').draggable('destroy').removeClass('unlocked');

		//
		$('.widget').each(function(i,e){
			$(e).css('overflow', 'visible');
		});
		
		// remove overlay with class '.moveResizeMsg'
		$( ".moveResizeMsg" ).remove();
		
		// restore rerender on window resize event
		$(window).unbind('resize').resize(function() {
		  $('#refresh_button').click();
		});
		
		// hide and and reset buttons
		$('#addBtn').fadeOut();
		$('#resetBtn').fadeOut();
		
        //if (!resetState) {
            // serialize html for localstorage
            var html = $('#tabpage').clone();
            html.find('*').filter(function(i){return this.nodeName.slice(0,3) === "VF-" ? true : false}).empty();
            html = html.html();
		
            //save html to local storage
            var tab = $($("ul#mainTabs li[style!='display: none;'][class*=active] a")[0]).attr('vf-page').substring(2).slice(0,-5);
            console.log("lockedBtn: tab="+tab);
            localStorage.setItem(vm.designHash+"-"+tab,html);
        //}
        //resetState = false;
	});
	
	$('#unlockedBtn').click(function(){
        console.log("main.js: unlockedBtn: enter");
        
		// remove resize binding to prevent rerenders during resizing
		$(window).unbind('resize');
		
		// show add and reset buttons
		$('#addBtn').fadeIn();
		$('#resetBtn').fadeIn();
		
		// loop over each '.widget' and convert them to global absolute position
		var positions = [];
		$('.widget').each(function(i,e){
			positions.push($(e).offset());
		});
		$('.widget').each(function(i,e){
			$(e).css('top', positions[i].top).css('left', positions[i].left).css('position', 'absolute').css('overflow', 'hidden');
		});
		
		// make '.widget' resizable and draggable and insert overlay with class '.moveResizeMsg'
		$( ".widget" ).resizable({
			aspectRatio: true,
			ghost: false,
			handles: "all",
			//containment: "#tabpage",
			start: function( event, ui ) {
				ui.element.children().each(function(i,e){
					if(!$(e).hasClass('moveResizeMsg')){
						$(e).fadeOut();
					}
				});
			},
			stop: function( event, ui ) {
				$('#refresh_button').click();
				ui.element.children().each(function(i,e){
					if(!$(e).hasClass('moveResizeMsg')){
						$(e).fadeIn();
					}
				});
			},
			resize: function( event, ui ) {
				$(this).find('.moveResizeMsgCloseBtn').css('top',-14).css('left',$(this).width()-18);
			}
		}).draggable({
			//containment: "#tabpage"
			start: function( event, ui ) {
				//ui.element.css('position', 'absolute');
			}
		}).addClass('unlocked').append('<div class="moveResizeMsg" style="color:white;position: absolute;text-shadow: 5px 5px 5px #000000;font-size: 16pt;width: 100%;height: 100%;top: 0px;display:-moz-box;-moz-box-pack:center;-moz-box-align:center;display:-webkit-box;-webkit-box-pack:center;-webkit-box-align:center;display:box;box-pack:center;box-align:center;"><table><tr><td>Move and Resize</td></tr><tr><td><center><button onclick="$(this).closest(\'.widget\').remove();" style="margin: 5px;" class="btn btn-inverse removeWidgetBtn">Remove</button></center></td></tr></table></div>');
		$('.widget').each(function(i,e){
			$(e).find('.moveResizeMsgCloseBtn').css('top',-14).css('left',$(e).width()-18);
		});
	});
	
	$('#resetBtn').click(function(){
        console.log("main.js: reset button: enter");
        
		// reset lock buttons
        //resetState = true;

        var tab = $($("ul#mainTabs li[style!='display: none;'][class*=active] a")[0]).attr('vf-page').substring(2).slice(0,-5);
        var key = vm.designHash+"-"+tab;
        console.log("delete "+key);
        var result = delete localStorage[vm.designHash+"-"+tab];
        console.log("delete "+result);
            
		loadTab($($("ul#mainTabs li[style!='display: none;'][class*=active] a")[0]).attr('vf-page'));
		//$('#unlockedBtn').click();
	});
	
	$('.removeWidgetBtn').click(function(){
		$(this).closest('.widget').remove();
	});
	
	/* 	
	$("body").delegate("g.cell", "click", function() {
		//alert("hi");
		debugger;
	}); */

	// resize iframe function
	sizeIframe = function(){
		//parent.$('.visualizerWrapper iframe').css('height',Math.max($('.container-fluid').outerHeight()+30, 740));
		var tag = parent.$('.visualizerWrapper iframe');
		if(tag.offset() != null){
		tag.css('height',parent.$('html').innerHeight() - tag.offset().top - 35);
		}
	}
	
	//resize iframe on vf-render-complete and vf-widget-update
	$(document).on('vf-widget-rendering-complete',function(){
		sizeIframe();
	});
	
	$(document).on('vf-widget-update-complete',function(){
		sizeIframe();
	});
	
	// resize iframe on scroll (I would like a better event for this)
	$(window).scroll(function(){
		sizeIframe();
    });
    
    configureTab = function(tab){
        console.log("main.js: configureTab: enter");
		// Load the HTML for the specified tab
        var key = vm.designHash+"-"+tab.substring(2).slice(0,-5);
        console.log("main.js: loading tab: key="+key);
		if (localStorage.hasOwnProperty(key)) {
            $("#tabpage").html(localStorage.getItem(key));
			vf_viz.rerender();
                    
			sizeIframe();
			
			$('ul#mainTabs li a').unbind();
			$('ul#mainTabs li a').click(function (e) {
				e.preventDefault();
				
				//this should change to configureTab()
				//--//loadTab($(this).attr('vf-page'));
				configureTab($(this).attr('vf-page'));
				$(this).tab('show');
			});
			
			$(window).unbind('resize').resize(function() {
			  $('#refresh_button').click();
			});
        } else {
            loadTab(tab);
        }
	}
	
	loadTab = function(tab){
        console.log("main.js: loadTab: enter: tab="+tab);
        
		if(typeof tab != 'undefined'){
        //$('#lockedBtn').click()
			// reset lock buttons
			$('li:has(#lockedBtn)').addClass("active");
            $('li:has(#unlockedBtn)').removeClass("active");
            $('#addBtn').fadeOut();
            $('#resetBtn').fadeOut();
			
			var d = new Date;
            // GET ./$tab.html?time=$d
            console.log("main.js: loadTab: GET: "+tab+ "?time=" +d.getTime());
			$.get(tab + "?time=" + d.getTime(),function(data){
                    console.log("main.js: loadTab: success: "+tab+ "?time=" +d.getTime());
					$("#tabpage").html(data).css('height','100%').css('width','100%');
					console.log("main.js: call vf_viz.render");
					vf_viz.rerender();
                    
					sizeIframe();
					
					$('ul#mainTabs li a').unbind();
					$('ul#mainTabs li a').click(function (e) {
						e.preventDefault();
						
						//this should change to configureTab()
						//--//loadTab($(this).attr('vf-page'));
                        configureTab($(this).attr('vf-page'));
						$(this).tab('show');
					});
					
					$(window).unbind('resize').resize(function() {
					  $('#refresh_button').click();
					});
			});
		}
        console.log("main.js: loadTab: exit");
	}
	
	// init load of first visible tab after progress bar is hidden
	$('#dataLoadInProgress').on('hide', function () {
	  // select the first tab
      $('ul#mainTabs li a:visible')[0].click();
	  //vf_viz.rerender();
	})
	
	// init dataprovider
    console.log("main.js: create new vf_dataprovider object: vm");
	vm = new vf_dataprovider();
	
	//// The Bindings should be applied after each pageload within the page's document ready event
	ko.applyBindings(vm);
	
    // Bind dumpVM button
    //$("#dumpVM").bind("click",dumpVM);
    
	// init dataprovider gui
	//vmui.setup('vm','table.dpSettings thead','table.dpSettings tbody');
	
    console.log("main.js: create new vf_widget object: vf_viz");
	vf_viz = new vf_widget("inc/vf-widgets/vf-widgets.json");
    
    console.log("main.js: call vf_viz.loadConfigurationFile");
	vf_viz.loadConfigurationFile();
	
	firstTabLoad = function(){
		// this should change to configureTab()
        resetState = true;
		loadTab($($("ul#mainTabs li[style!='display: none;'] a")[0]).attr('vf-page'));
	}
	console.log("ready: exit");
	
	//show secret tabs by holding the ctrl key down and clicking on the header
	$('#header').on('click', function(){
		if(event.ctrlKey){
			vm.showExperimental(true);
		}
	});
	
	$('#designDetailsModal').on('show shown', function () {
		var visualizationArtifacts = vm.configurations()[vm.listOfConfigurations().indexOf(vm.selectedConfiguration())].visualizationArtifacts;
		loadVisualizationArtifacts(visualizationArtifacts);
		// Dynamically resize modal 
		$('#designDetailsModal')
			.width($(window).width()-100)
			.css('margin-left',(-1*$(window).width()+100)/2)
			.height($(window).height()-100)
			.css('margin-top',-1*($(window).height()-100)/2);
		$('#designDetailsModal .modal-body')
			.css('max-height',$('#designDetailsModal').innerHeight()-$('#designDetailsModal .modal-header').outerHeight()-$('#designDetailsModal .modal-footer').outerHeight()-30)
			.css('min-height',$('#designDetailsModal').innerHeight()-$('#designDetailsModal .modal-header').outerHeight()-$('#designDetailsModal .modal-footer').outerHeight()-30);
	});
	
	$("#compareBtn").click(function(){			
		//vm.listOfCompareConfigurations(vm.listOfCompareConfigurations());
		$("#comparerModal").modal('show');
		
		//fired when the modal has been made visible to the user (will wait for css transitions to complete).
		$('#comparerModal').on('shown', function () {
			//invoke the datatable to rerender...
			vm.listOfCompareConfigurations(vm.listOfCompareConfigurations());
		})		
	});
	
	$('#comparerModal').on('show shown', function () {
		// Dynamically resize modal 
		$('#comparerModal')
			.width($(window).width()-100)
			.css('margin-left',(-1*$(window).width()+100)/2)
			.height($(window).height()-100)
			.css('margin-top',-1*($(window).height()-100)/2);
		$('#comparerModal .modal-body')
			.css('max-height',$('#comparerModal').innerHeight()-$('#comparerModal .modal-header').outerHeight()-$('#comparerModal .modal-footer').outerHeight()-30)
			.css('min-height',$('#comparerModal').innerHeight()-$('#comparerModal .modal-header').outerHeight()-$('#comparerModal .modal-footer').outerHeight()-30);
	});
	
	$("body").delegate('#annotate', 'click', function(){	
		$("#annotateModal").modal('show');
		
		var config = $(this).parent().parent().parent().attr("config");
 		vm.selectedConfiguration(config);
 	});
	
	$('#annotateModal').on('show shown', function () {
		// Dynamically resize modal 
		$(this)
			.width($(window).width()-100)
			.css('margin-left',(-1*$(window).width()+100)/2)
			.height($(window).height()-100)
			.css('margin-top',-1*($(window).height()-100)/2);
		$(this).children('.modal-body')
			.css('max-height',$(this).innerHeight()-$(this).children('.modal-header').outerHeight()-$(this).children('.modal-footer').outerHeight()-30)
			.css('min-height',$(this).innerHeight()-$(this).children('.modal-header').outerHeight()-$(this).children('.modal-footer').outerHeight()-30);
		
		$(this).find("h4 div").text($(this).attr("config"));
	});
	
	$('#annotateBtn').on('click', function() {
		//debugger;
		var text = $("#annotateText").val();
		var currDate = new Date();
		
		var obj = [{
			'user' : 'User001',
			'addedDate' : currDate,
			'lastModDate' : currDate,
			'text' : text
		}];
		
		var currentAnnotateObj = vm.contextAnnotateCurrrent();
		var newObj = jQuery.merge(obj, currentAnnotateObj); //preapend newObj to old array
		
		var config = vm.selectedConfiguration();
		for (var i = 0; i < vm.configurations().length; i++)
		{
			if(vm.configurations()[i].design == config)
			{
				vm.configurations()[i].annotate(newObj);
				break;
			}
		}
		
		vm.selectedConfiguration.notifySubscribers();
		
		$("#annotateText").val("");
	});

	$('body').on("onContextMenu", function (event) {
		return false;
	})

	$('body').delegate('.csv', 'click', function(){
		//debugger
		
		//alert($(this).attr("data"));
		var data = eval($(this).attr("data"));
		var fileName = $(this).attr("table");
		var str = "";
		var line = "";
		
		if (data && data.headers && data.headers.length > 0)
		{
			for (var i = 0; i < data.headers.length; i++)
			{
				line += "\'" + data.headers[i].toString() + "',";
			}		
			
			line = line.slice(0, -1);
			str += line + '\r\n';
			line = "";
		}
		
		if (data && data.data && data.data.length > 0)
		{
			for (var i = 0; i < data.data.length; i++)
			{
				for (var j = 0; j < data.data[i].length; j++)
				{
					if (data.data[i][j] != null)
					{
						line += "'" + data.data[i][j].toString().replace(/"/g, "\"")  + "',";
					}
					else
					{
						line += "'',";
					}
				}
				line = line.slice(0, -1);
				str += line + '\r\n';
				line = "";
			}
		}
		
		var blob;
		blob = new Blob([str],{type: "data:text/csv;charset=utf-8"});
		var s=saveAs(blob, fileName + ".csv");
	});
});

function GetSMPdata()
{
	$.ajax({
		dataType: "json",
		url: "./inc/model_perf-2.json",
		success: function(data){
			vm.dataproviders.smpData = data;
			//ko.applyBindings(vm, $('#tabpage')[0]);
			console.log("Downloaded Surrogate Model Performance data from file." , data);
		},
		async: false
	});
}