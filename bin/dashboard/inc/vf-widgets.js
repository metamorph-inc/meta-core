/*///////////// vf_widget class definition /////////////*/
/* NEED to add error handling for missing info within configuration file */
function vf_widget(configurationFilePath, dataproviderLoadingStatus) {
    console.log("vf_widgets: new");
	var self = this;
	/* Set location of configuration file */
	if(typeof configurationFilePath == 'undefined'){
		this.configurationFilePath = "inc/vf-widgets/vf-widgets.json";
	}
	else{
		this.configurationFilePath = configurationFilePath;
	}
	if(typeof dataproviderLoadingStatus == 'undefined'){
		this.dataproviderLoadingStatus = "vm.loadPending()";
	}
	else{
		this.dataproviderLoadingStatus = dataproviderLoadingStatus;
	}
	this.debugBlock = false;
	
    /**
     *  handlers
     *
     *  This array holds the instances of each widget
     */
	this.handlers = [];
	this.test = function () {
		if (window.$ == undefined) {
			console.log('vf_widgets:jquery was not detected');
			return false;
		}
		if (window.ko == undefined) {
			console.log('vf_widgets:KnockoutJS was not detected');
			return false;
		}
		if (window.d3 == undefined) {
			console.log('vf_widgets:d3 was not detected');
			return false;
		}
		return true;
	}
    

    /**
     *  event blocks
     *
     *  Widget classes are stored here
     */
    // white list
    this.whiteList = [];
    
    // black lisk
    this.blackList = [];

    // blockStatus
    this.blockStatus = function(id){
    	if(self.whiteList.indexOf(id) !== -1 || self.whiteList.indexOf('*') !== -1){
    		return false;
    	}
    	if(self.blackList.indexOf(id) !== -1 || self.blackList.indexOf('*') !== -1){
    		return true;
    	}
    	return false;
    }

	// render
	this._blockRenderStatus = false;
	this.blockRender = function(){
		self.blockRenderStatus = true;
	}
	this.unblockRender = function(){
		self.blockRenderStatus = false;
		self.render();
	}

	// rerender
	this.blockRerenderStatus = false;
	this.blockRerender = function(){
		self.blockRerenderStatus = true;
	}
	this.unblockRerender = function(){
		self.blockRerenderStatus = false;
		self.rerender();
	}

	// update
	this.blockUpdatesStatus = false;
	this.blockUpdates = function(){
		self.blockUpdatesStatus = true;
	}
	this.unblockUpdates = function(){
		self.blockUpdatesStatus = false;
		self.update();
	}

	// all
	this.blockAll = function(){
		self.whiteList.removeElement('*');
		self.blackList.push('*');
	}
	this.unblockAll = function(){
		self.blackList.removeElement('*');
		self.whiteList.push('*');
	}

	$(document).unbind('vf-widget-blockAll').bind('vf-widget-blockAll', {"self":this}, function (event) {
		console.log('vf_widgets: Received vf-widget-blockAll');
		self.blockAll();
	});

	$(document).unbind('vf-widget-unblockAll').bind('vf-widget-unblockAll', {"self":this}, function (event) {
		console.log('vf_widgets: Received vf-widget-unblockAll');
		self.unblockAll();
	});

    /**
     *  widgetClasses
     *
     *  Widget classes are stored here
     */
	this.widgetClasses = [];
	
	/**
     *  widgetLoadingTimers
     *
     *  track loading progress of widgets
     */
	this.widgetLoadingTimers = [];

	/**
     *  widgetLoadingTimeoutFunction
     *
     *  test if widget class has been loaded into memory if the class has 
	 *	been loaded into memory it evaluates the class and then tests if 
	 *	all the widget classes have been noted  all the widget causes have 
	 *	been loaded it will fire the vf_widgetLoadComplete event
     */	
	this.widgetLoadingTimeoutFunction = function(widgetIndex){
		if(typeof window[self.widgetClasses[widgetIndex].className] == 'function'){
			//eval('self.widgetClasses[widgetIndex].widgetClass = ' + window[self.widgetClasses[widgetIndex].className]);
			self.widgetClasses[widgetIndex].loadingStatus = 'complete';
			clearTimeout(self.widgetLoadingTimers[widgetIndex]);
			if (self.widgetClassesLoaded()) {
				console.log('vf_widgets: loadConfigurationFile: Sent vf_widgetLoadComplete');
				$(document).trigger('vf_widgetLoadComplete', self);
			}
		}
		else{
			self.widgetLoadingTimers[widgetIndex] = setTimeout(function(){
				self.widgetLoadingTimeoutFunction(widgetIndex);
			},100);
		}
	}
    
    /**
     *  loadConfigurationFile
     *
     *  Retrieve the widget classes (JavaScript) from the server and
     *  instantiate them
     */
	this.loadConfigurationFile = function () {
        console.log("vf_widgets: loadConfigurationFile:  enter");
		this.widgetClasses = [];
        console.log("vf_widgets: loadConfigurationFile: ajax: "+this.configurationFilePath);
		$.ajax({
			url : this.configurationFilePath,
			dataType : 'json',
			self : self,
			success : function (config) {
                console.log("vf_widgets: loadConfigurationFile: success: (widgetFile)");
				for (var i = 0; i < config.length; i++) {
					self.widgetClasses.push(config[i]);
					
					/* Set initail widget class status */
					self.widgetClasses[i].loadingStatus = 'pending';
                    
                    /* Retrieve widget classes from the web server */
					var loaderNode = document.createElement("script");
					loaderNode.setAttribute("type", "text/javascript"); 
					loaderNode.setAttribute("src", config[i].widgetClass); 
					document.getElementsByTagName("head")[0].appendChild(loaderNode);
				
					/* once the widget classes are loaded into the Dom load them into the widget framework */
					self.widgetLoadingTimeoutFunction(i);
                    
                    /* Retrieve the widget help files from web server */
					(function (config, i, self) {
                        console.log("vf_widgets: loadConfigurationFile: ajax: "+config[i].help);
						$.ajax({
							url : config[i].help,
							dataType : 'text',
							self : self,
							success : function (help) {
                                console.log("vf_widgets: loadConfigurationFile: success: "+config[i].help);
								self.widgetClasses[i].help = help;
								console.log('vf_widgets: loadConfigurationFile: loaded help '+ i + ' of ' + (config.length-1))
							}
						});
					})(config, i, self);
				}
			}
		});
        console.log("vf_widgets: loadConfigurationFile:  exit");
	}
	
	/**
     *  render
     *
     *  Render the widgets
     */
	this.render = function () {
		if(!self.blockRenderStatus && !self.blockStatus()){
	        console.log("vf_widgets: render: enter");
			/* 
			If all of the widget widgetClasses are loaded 
			and all of the required libraries can be found 
			and the dataprovider has completed loading all the data files
			*/
			if (this.widgetClassesLoaded() && this.test() && eval('!'+this.dataproviderLoadingStatus)) {
				console.log('vf_widgets: render: rendering...');
				
				/* Renew all current widget bindings from document */
				$(document).unbind('vf-widget-update').bind('vf-widget-update', {"self":this}, function (event) {
					console.log('vf_widgets: render: Received vf-widget-update');
					self.update();
				});
				$(document).unbind('vf-widget-render').bind('vf-widget-render', {"self":this}, function (event) {
					console.log('vf_widgets: render: Received vf-widget-render');
					self.render();
				});
				$(document).unbind('vf-widget-rerender').bind('vf-widget-rerender', {"self":this}, function (event) {
					console.log('vf_widgets: render: Received vf-widget-rerender');
					self.rerender();
				});
				
				/* Remove all deprecated widegt Bindings from document */
				$(document).unbind('vf-selectionUpdate');
				$(document).unbind('vf-selectionChange');
				
				/* Empty all widget handlers */
				this.handlers = [];
				
				/* Create instances of each widget on the page and store their handlers */
				while(this.listUnrenderedWidgets().length > 0){
	            
	                /* Loop over each widget class */
					for (var i = 0; i < this.widgetClasses.length; i++) {
	                
	                    /* For each occurance of the widget tag "vf-*" execute the function */
						$(this.widgetClasses[i].tag).each(function (index, element) {
							/* 
							Check if data attribute is set 
							and that the class has not been applied
							if so apply class
							*/
							
							if (!(typeof eval($(element).attr('data')) == 'undefined') && ($(element).attr('classApplied') == undefined || $(element).attr('classApplied') == "false")) {

	                            /* Instantiate the widget class */
								self.handlers.push(new window[self.widgetClasses[i].className](element));
								
								/* Set the 'classApplied' attribute to true */
								$(element).attr('classApplied', 'true');
								
								/* Render the widget - if the render function is defined */
								if(typeof self.handlers[self.handlers.length - 1].render == "function"){
									self.handlers[self.handlers.length - 1].render();
								}
							}
						});
					}
				}
				
				/* Trigger each widgets event to render */
				//$(document).trigger('vf-widget-render');
				//$(document).trigger('vf-selectionChange');
				
				/* Bugfix: Fix all negative signs on page after render */
				$('text').each(function (index, item) {
					item.textContent = item.textContent.replace("−", "-");
				});
				
				$(document).trigger('vf-widget-rendering-complete');
				console.log('vf-widgets: rendering complete');
			} 
			else {
				console.log('vf_widgets: render: NOT rendering...');
				console.log('vf_widgets: render: Render attempted but either widgetClasses not ready or data not ready');
				$(document).unbind('vf_widgetLoadComplete').bind('vf_widgetLoadComplete', function (event, self) {
					console.log('vf_widgets: render: Received vf_widgetLoadComplete');
					self.render();
				});
				$(document).unbind('vf-dataLoadComplete').bind('vf-dataLoadComplete', {"self": self}, function (event) {
					console.log('vf_widgets: render: Received vf-dataLoadComplete');
					self.render();
				});
			}
		}
		else{
	   		console.log("vf_widgets: render: blocked");
	   	}
	}

	this.renderById = function(id){
		console.log("vf_widgets: render: enter");
		for(var i = 0; i < this.handlers.length; i++){
			if(typeof this.handlers[i].rerender == "function" && $(this.handlers[i].element).attr('id') === id){
				this.handlers[i].render();
			}
		}
		$(document).trigger('vf-widget-render-complete');
	}
	
	
	this.rerender = function() {
       if(!self.blockRerenderStatus && !self.blockStatus()){
	       	console.log("vf_widgets: rerender: enter");
	   		var allWidgets = this.listAllWidgets();
	   		
	   		/* Empty all widget handlers */
	   		this.handlers = [];
	   		
	   		for(var i = 0; i < allWidgets.length; i++){
	   			$(allWidgets[i]).empty();
	   			$(allWidgets[i]).attr('rendered', 'false');
	   			$(allWidgets[i]).attr('classApplied', 'false');
	   		}
	   		this.render();
	   	}
	   	else{
	   		console.log("vf_widgets: rerender: blocked");
	   	}
	}

	this.rerenderById = function(id){
		console.log("vf_widgets: rerender: enter");
		for(var i = 0; i < this.handlers.length; i++){
			if(typeof this.handlers[i].rerender == "function" && $(this.handlers[i].element).attr('id') === id){
				this.handlers[i].rerender();
			}
		}
		$(document).trigger('vf-widget-rerender-complete');
	}
	
	this.update = function() {
	        console.log("vf_widgets: update: enter");
			for(var i = 0; i < this.handlers.length; i++){
				if(!self.blockUpdatesStatus && !self.blockStatus(vf_viz.handlers[i].element.id)){
					if(typeof this.handlers[i].update == "function"){
						this.handlers[i].update();
					}
				}
				else{
					console.log("vf_widgets: update: blocked");
				}
			}
			$(document).trigger('vf-widget-update-complete');
	}

	this.updateById = function(id){
		console.log("vf_widgets: update: enter");
		for(var i = 0; i < this.handlers.length; i++){
			if(typeof this.handlers[i].update == "function" && $(this.handlers[i].element).attr('id') === id){
				this.handlers[i].update();
			}
		}
		$(document).trigger('vf-widget-update-complete');
	}

	$(document).unbind('vf-widget-updateById').bind('vf-widget-updateById', {"self":this}, function (event, click, id) {
		console.log('vf_widgets: Received vf-widget-updateById');
		self.updateById(id);
	});
	
	/* provide a list of all availible widgets tags defined in vf-widgets.json */
	this.listAllTags = function(){
        console.log("vf_widgets: listAllTags: enter");
		/* prepare a list of all the tags of interest */
		var listOfTags = [];
		for(var i = 0; i < this.widgetClasses.length; i++){
			listOfTags.push(this.widgetClasses[i].tag);
		}
		listOfTags = listOfTags.join(', ');
		return listOfTags;
	}
	
    /**
     *  listUnrenderedWidgets
     *
     *  This function returns a list of the widgets that have
     *  not been rendered yet.
     */
	this.listUnrenderedWidgets = function(){
        console.log("vf_widgets: listUnrenderedWidgets: enter");
		var listOfTags = this.listAllTags();
		
		/* prepare an array of all the element with the tags listed above and not rendered */
		var listOfWidgets = [];
		$(listOfTags).each(function(index, element){
			if (($(element).attr('rendered') == undefined || $(element).attr('rendered') == "false")) {
				listOfWidgets.push(element);
			}		
		});
        console.log(listOfWidgets);
		return listOfWidgets;
	}
	
	/* provide a list of all of the widgets */
	this.listAllWidgets = function(){
        console.log("vf_widgets: listAllWidgets: enter");
		var listOfTags = this.listAllTags();
		
		/* prepare an array of all the element with the tags listed above*/
		var listOfWidgets = [];
		$(listOfTags).each(function(index, element){
				listOfWidgets.push(element);
		});
		return listOfWidgets;
	}
	
    /**
     *  widgetClassesLoaded
     *
     *  return true when all widget classes have been loaded
     */
	this.widgetClassesLoaded = function(){
        console.log("vf_widgets: widgetClassesLoaded: enter");
		for(var i = 0; i < this.widgetClasses.length; i++){
			if(typeof window[this.widgetClasses[i].className] != 'function'){
				return false;
			}
		}
		return true;
	}
	
	this.equivalentInObject = function(obj){
		var out = [];
		var keys = Object.keys(obj);
		for(var i = 0; i< keys.length; i++){
			if(obj[keys[i]] == this){
				out.push(keys[i]);
			}
		}
		return out;
	}
}
