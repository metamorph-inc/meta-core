/*///////////// vf-datatable2 /////////////*/
function vf_datatable2(element) {
	var self = this;
	
	this.element = element;
	
	/* Render Method */
	this.render = function () 
	{

			if (!($(element).attr('rendered') == undefined || $(element).attr('rendered') == "false")) {
				return 'vf-datatable: already rendered';
			}
				this.data = clone(eval($(element).attr('data')));
				var did = $(element).attr("id") + ""; //rankings
				//$("#" + did).html('<div> hello</div>');
				
				var rows="";
				var keys = Object.keys(this.data);
				for ( var i = 0; i < keys.length; i++ )
				{
					if (!$.isArray( this.data[keys[i]]))
					rows+= "<tr><td><div> "+ keys[i] + ":" + "</div></td><td><div>" + this.data[keys[i]] + "</div></td><tr>";
				}
				
				var prefix = '<table>'+ rows + '</table>'
				
				//Details has one column and possible to have multiple rows
				var tables =[];
				var details =this.data['Details'];
				console.debug(details);
				for ( var i = 0; i < details.length; i++ )
				{
					var headers=[];
					headers.push( details[i].GroupTitle);
					var tableData =[];
					var tableDataRow = [];
					for ( var j = 0; j < details[i].GroupBody.length; j++ )
					{
						tableDataRow = [];
						tableDataRow.push(details[i].GroupBody[j]);
						tableData.push(tableDataRow);
					}
					tables.push(createTable(headers, tableData));
				}
				var htmlText = prefix;	
				for ( var i = 0; i < tables.length; i++ )
				{
					htmlText += '<p><table width ="100%" cellpadding="0" cellspacing="0" border="0" class="display" id="' + did + "table" + i + '"></table>'
				}
				$("#" + did).html( htmlText );
				for ( var i = 0; i < tables.length; i++ )
				{
					$("#" + did+"table" + i).dataTable(tables[i]);
				}	
				//ark <vf-table> as rendered 
				$(element).attr('rendered', 'true');
				
			function createTable(headers, tableData)
			{
					var aoCol = [];
					for (var i=0; i < headers.length; i++) {
						aoCol.push({'sTitle' : headers[i], 'sClass': 'center'});	
					}
				
					//default
					var aColumConfig=
						 {
							"bJQueryUI": true,
							"aaData": tableData,
							"aoColumns": aoCol,
							"bPaginate": false,
							"bAutoWidth": true,
							"bFilter": false,
							"bInfo": false

						};
					
					return aColumConfig;
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
