/*///////////// vf-datatable /////////////*/
function vf_datatable(element) {
	var self = this;
	this.data = clone(eval($(element).attr('data')));
	this.element = element;
	
	
	/* Render Method */
	this.render = function () 
	{
		try
		{
			if (!($(element).attr('rendered') == undefined || $(element).attr('rendered') == "false")) {
				return 'vf-datatable: already rendered';
			}
				
				var did = $(element).attr("id") + ""; //rankings
				
				
				
				var w = $(element).parent().width();
				var height = $(element).attr("height");
/* 				console.log("===========================arguments " + did + " =======");
				console.log("height: " + height );
				console.log("pagination: " + $(element).attr("pag"));
				console.log("aspect_ratio: " + $(element).attr("ratio"));
				
				console.log("width: " + w ); */
				
				var pag =  $(element).attr("pag");
				var aspect_ratio = $(element).attr("ratio");
				
				
				if ( height==""){
					//console.log("height not defined so calculated" );
					height = w /aspect_ratio + "px";
				}
				//console.log(did + ": height: " + height );
				
				var outputs = clone(eval($(this.element).attr('data')));
				
				// clean data (remove: undefined, null, etc.)
				for(var i = 0; i < outputs.data.length; i++){
					for(var j = 0; j < outputs.data[i].length; j++){
						if(outputs.data[i][j] == undefined || outputs.data[i][j] == null){
							outputs.data[i][j] = "";
						}
					}
				}
				
				
				var aColumData = outputs.headers;
				var aoCol = [];
				//var aoFoot=  '<tfoot><tr>'
				//var thtr="<tr>";
				for (var i=0; i < aColumData.length; i += 1) {
					
						
					//if (pag =="false" && i ==0) 
						//aoCol.push({'sTitle' : aColumData[i], 'sClass': 'center' , 'sWidth': '40%'});
					//else
						aoCol.push({'sTitle' : aColumData[i], 'sClass': 'center'});					
					//thtr+="<th/>";
					//aoFoot += '<th><input type="text" name="search_column" value="Search ' + aColumData[i] + '" class="search_init" /></th>';
				}
				//thtr+="</tr>";
				//aoFoot+="</tr></tfoot>";
				//var aoFootTable = '<div class= "dataTables_individual_search"><table cellpadding="0" cellspacing="0" border="0" class="display" id="' + did + "tablefooter" + '"><thead>' + thtr + '</thead><tbody>' + thtr + '</tbody>' +aoFoot + '</table></div>';
			
			
			//default
			var aColumConfig;
			if (pag =="true") {

				aColumConfig = 
				 {
					"bJQueryUI": true,
					"aaData": outputs.data,
					"aoColumns": aoCol
					,"bAutoWidth": false
				};
			}
			else { //pagination = false
				aColumConfig = 
				 {
					"bJQueryUI": true,
					"aaData": outputs.data,
					"aoColumns": aoCol,

					"sScrollY": height,
					"bPaginate": false,
					"bScrollInfinite": true,
					"bScrollCollapse": true,
					"bStateSave": false
					,"bAutoWidth": false
				};
				
			}
			
			var asInitVals = new Array();
			
			$(document).ready(function() {
				
				$("#" + $(element).attr("id")).html( '<table cellpadding="0" cellspacing="0" border="0" class="display" id="' + did + "table" + '"></table>' );
				var oTable = $("#" + did+"table").dataTable(aColumConfig);
				
				/*
				$("#" + did + "table_wrapper .dataTables_scroll").after(aoFootTable);
				$("#" +  did + "tablefooter").dataTable({
				
					"bPaginate": false,
					"bInfo": false ,
					"bFilter": false 
				});
			
				
				 $("#" +  did + "tablefooter tfoot input").keyup( function () {
					//Filter on the column (the index) of this element 
					oTable.fnFilter( this.value, $("#" +  did + "tablefooter tfoot input").index(this) );
					console.log("value: " + this.value);
				
				} );
				
				//Support functions to provide a little bit of 'user friendlyness' to the textboxes in
				$("#" +  did + "tablefooter tfoot input").each( function (i) {
					asInitVals[i] = this.value;
				} );
				 
				$( "#" +  did + "tablefooter tfoot input").focus( function () {
					if ( this.className == "search_init" )
					{
						this.className = "";
						this.value = "";
					}
				} );
				 
				$("#" +  did + "tablefooter tfoot input").blur( function (i) {
					if ( this.value == "" )
					{
						this.className = "search_init";
						this.value = asInitVals[$("#" +  did + "tablefooter tfoot input").index(this)];
					}
				} );
				*/
				
			} );
			
				/*
				$("#" + $(element).attr("id")).html( '<table cellpadding="0" cellspacing="0" border="0" class="display" id="' + $(element).attr("id") + "table" + '"></table>' );
				$("#" + $(element).attr("id")+"table").dataTable( {
					 "aaData": outputs.data,
					 "aoColumns": aoCol,
					 "bScrollInfinite": true,
					 "bScrollCollapse": true,
					 "sScrollY": "100%",
					 "sScrollX": "100%",
					 "bStateSave": true
					
				} );
				*/
				$(document).trigger($(element).attr('id') + '_clicked', [d]);	
				
				
				//ark <vf-table> as rendered 
				$(element).attr('rendered', 'true');
		}
		catch(err)
		{
			//ark <vf-table> as rendered 
			$(element).attr('rendered', 'true');
		}
	} 

	/* Rerender Method */
	this.rerender = function () {
		updateTabSize();
		$(this.element).empty();
		$(element).attr('rendered', 'false');
		this.render();
	}
	
	/* Update Method */
	this.update = function () {
		this.rerender();
	}
}
