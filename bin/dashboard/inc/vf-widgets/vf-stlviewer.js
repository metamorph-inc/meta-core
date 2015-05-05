/*///////////// vf_stlviewer /////////////*/
function vf_stlviewer(element) {
	
	var self = this;
	this.element = element;
	
	/* Render Method */
	this.render = function () {
		try
		{
			/* 
			Check if element has already been rendered 
			if it has then skip it
			if it has not render it
			*/
			
			if (!($(element).attr('rendered') == undefined || $(element).attr('rendered') == "false")) 
			{
				return 'vf-stlviewer: already rendered';
			}
			
			var data = clone(eval($(this.element).attr('data')));
			//output = clone(dataHeatMAP); //save "outputs" locally.  Do not use the pass by reference (from the dataprovider)
			var min = d3.min(data),
				max = d3.max(data);
				
			var margin = {top: 10, right: 10, bottom: 10, left: 10},
				width = $(element).width() - margin.left - margin.right,
				height = $(element).attr("h") - margin.top - margin.bottom;



			
			/* Mark <vf-table> as rendered */
			$(element).attr('rendered', 'true');
		}
		catch(err)
		{
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