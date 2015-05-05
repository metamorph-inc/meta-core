////////////// vf-artifactsviewer /////////////
function vf_artifactsviewer(element)
{
	var self = this;
	//this.data = eval($(element).attr('data'));
	this.element = element;
	
	
	
	//Render Method
	this.render = function () 
	{
		console.debug("vf-artifactsviewer render");
		try
		{
			//Check if element has already been rendered if it has then skip it. if it has not render it
			if (!($(element).attr('rendered') == undefined || $(element).attr('rendered') == "false")) 
			{
				return 'vf-artifactsviewere: already rendered';
			}
			var outputs = eval($(this.element).attr('data'));
			//console.debug(outputs);	
			//widget is called when design configruation is changed, thus required to unload and then reload again.
			unloadVisualizationArtifacts()
			loadVisualizationArtifacts(outputs);
			
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
		console.debug("vf-artifactsviewer rerender");
		updateTabSize();
		$(this.element).empty();
		$(element).attr('rendered', 'false');
		this.render();
	}
	
	//Update Method
	this.update = function () 
	{
		console.debug("vf-artifactsviewer update");
		this.rerender();
	}
}