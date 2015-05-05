////////////// vf-widgetname /////////////
function vf_widgetname(element)
{
	var self = this;
	//this.data = eval($(element).attr('data'));
	this.element = element;
	
	//Render Method
	this.render = function () 
	{
		try
		{
			//Check if element has already been rendered if it has then skip it. if it has not render it
			if (!($(element).attr('rendered') == undefined || $(element).attr('rendered') == "false")) 
			{
				return 'vf-widgetname: already rendered';
			}
			
			var outputs = eval($(this.element).attr('data'));
				
			//begin code 
			
			
			
			//Mark <vf-table> as rendered
			$(element).attr('rendered', 'true');
		}
		catch(err)
		{
			//Mark <vf-table> as rendered
			$(element).attr('rendered', 'true');
		}
	}
	
	//Rerender Method
	this.rerender = function () 
	{
		updateTabSize();
		$(this.element).empty();
		$(element).attr('rendered', 'false');
		this.render();
	}
	
	//Update Method
	this.update = function () 
	{
		this.rerender();
	}
}